#include <regex>

inline gdu::SCObject gdu::SCParser::parse_string(const std::string& str) {
   SCParser t(str);
   t.parse();
   return t.result_;
}

// EBNF:
// file: statement* EOF
inline void gdu::SCParser::parse() {
   while (true) {
      std::string key;
      SCValue val;
      bool valid = expect_statement(key, val);
      if (!valid) {
         break;
      }
      result_.add(key, val);
   }
   skip_whitespace();
   if (processed_ != ssize(str_)) {
      throw gdu::parse_error(line_, offset_, "End of File", "");
   }
}

// EBNF:
// statement: key EQUALS value SEMI
inline bool gdu::SCParser::expect_statement(std::string& key, SCValue& value) {
   skip_whitespace();
   if (processed_ == ssize(str_)) return false;
   if (!expect_key(key)) return false;

   skip_whitespace();
   bool has_equals = expect_equals_token();
   if (!has_equals) {
      throw gdu::parse_error(line_, offset_, "'='", "");
   }
   skip_whitespace();
   if (!expect_value(value)) {
      throw gdu::parse_error(line_, offset_, "value", "");
   }
   skip_whitespace();
   bool has_semi = expect_semi_token();
   if (!has_semi && !value.is_array() && !value.is_object()) {
      throw gdu::parse_error(line_, offset_, "';'", "");
   }
   return true;
}

// EBNF:
// key: WORD | STRING
inline bool gdu::SCParser::expect_key(std::string& key) {
   key = expect_word_token();
   if (key.empty()) {
      return false;
   }
   return true;
}

// EBNF:
// value: STRING | INT | DOUBLE | DATE | BOOL | array | object
inline bool gdu::SCParser::expect_value(SCValue& val) {
   if (processed_ == ssize(str_)) {
      throw gdu::parse_error(line_, offset_, "value", "end of file");
   }
   if (isdigit(str_[processed_]) || str_[processed_] == '+' || str_[processed_] == '-') {
      if (expect_double_token(val)) {
         return true;
      } else if (expect_date_token(val)) {
         return true;
      } else if (expect_int_token(val)) {
         return true;
      } else {
         throw gdu::parse_error(line_, offset_, "number", "");
      }
   } else {
      if (expect_bool_token(val)) {
         return true;
      } else if (expect_string_token(val)) {
         return true;
      } else if (expect_array(val)) {
         return true;
      } else if (expect_object(val)) {
         return true;
      }
   }
   return false;
}

inline std::string gdu::SCParser::expect_word_token() {
   static const std::regex word_re("(\\w+)");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, word_re, std::regex_constants::match_continuous) &&
      ssize(m) >= 2) {
      ssize_t sz = ssize(m[1].str());
      processed_ += sz;
      offset_ += sz;
      return m[1].str();
   }
   return "";
}

inline bool gdu::SCParser::expect_int_token(SCValue& val) {
   static const std::regex int_re("([+-]?(\\d+))([KMGsmhd])?");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, int_re, 
         std::regex_constants::match_continuous) &&
      ssize(m) >= 2) {

      ssize_t sz = ssize(m[1].str());
      processed_ += sz;
      offset_ += sz;
      int64_t i = std::strtol(m[1].str().c_str(), nullptr, 10);
      val = SCValue(i);
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_double_token(SCValue& val) {
   static const std::regex float_re("^([-+]?[0-9]+\\.[0-9]*([eE][-+]?[0-9]+)?)([KMGsmhd])?");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, float_re, 
         std::regex_constants::match_continuous) &&
      m.size() >= 2) {

      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      double d = std::strtod(m[1].str().c_str(), nullptr);
      val = SCValue(d);
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_bool_token(SCValue& val) {
   static const std::regex bool_re("^(true|false)", std::regex_constants::icase);
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, bool_re, 
         std::regex_constants::match_continuous) &&
      m.size() == 2) {

      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      bool b = std::tolower(m[1].str()[0]) == 't';
      val = SCValue(b);
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_date_token(SCValue& val) {
   static const std::regex 
      date_re("^((\\d{4}-\\d{2}-\\d{2}+)( \\d{2}:\\d{2}:\\d{2})?)|(\\d{2}:\\d{2}:\\d{2})");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, date_re, 
         std::regex_constants::match_continuous)) {

      struct tm tm = {};
      if (m[1].length()!=0) {
         std::string dstr = m[1].str();
         if (m[3].length()==0) {
            dstr += " 00:00:00";
         }
         if(sscanf(dstr.c_str(), "%d-%d-%d %d:%d:%d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec)!=6) {
            std::cout << "Weird, failure parsing time\n";
         }
      } else {
         std::string tstr = m[4].str();
         sscanf(tstr.c_str(),  "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
         tm.tm_year = 1970; tm.tm_mon = 1; tm.tm_mday = 1;
      }
      tm.tm_year -= 1900;
      tm.tm_mon--;
      time_t epoch = mktime(&tm);
      val = SCValue(epoch, 1);

      processed_ += m[0].length();
      offset_ += m[0].length();
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_string_token(SCValue& val) {
   static const std::regex 
      str_re("\"((([^\"\\\\\n\r]*)(\\\\(\"|\n|\r|\\\\))?)*)\"");
   std::cmatch m;
   if (str_[processed_] != '"') return false;

   if (std::regex_search(str_.c_str()+processed_, m, str_re, 
         std::regex_constants::match_continuous)) {

      std::string tstr = m[1].str();
      for (ssize_t ii = 0; ii < ssize(tstr); ++ii) {
         if (tstr[ii] == '\\' && ii+1 != ssize(tstr)) {
            switch (tstr[ii+1]) {
               case '\n':
               case '\r':
               case '\\':
               case '"': tstr.erase(ii, 1); break;
               default:
                  throw gdu::parse_error(line_, offset_, "Illegal backslash in string", "");
            }
         }
      }
      processed_ += m[0].length();
      offset_ += m[0].length();
      val = SCValue(tstr);
      return true;
   }
   throw gdu::parse_error(line_, offset_, "string", "");
   return false;
}

inline bool gdu::SCParser::expect_array(SCValue& val) {

   if (str_[processed_] != '[') return false;
   processed_++;
   offset_++;
   std::vector<SCValue> vec;

   SCValue v;
   bool valid = expect_value(v);
   while (valid) {
      vec.push_back(v);
      valid = expect_semi_token();
      if (!valid) break;
      skip_whitespace();
      if (!expect_value(v)) {
         throw gdu::parse_error(line_, offset_, "value", "");
      }
   }
   if (str_[processed_] != ']')
      throw gdu::parse_error(line_, offset_, "closing bracket", "");
   processed_++;
   offset_++;
   val = SCValue(vec);
   return true;
}

inline bool gdu::SCParser::expect_object(SCValue& val) {

   if (str_[processed_] != '{') return false;
   processed_++;
   offset_++;

   std::string key;
   SCValue v;
   SCObject obj;
   while (expect_statement(key, v)) {
      obj.add(key, v);
   }
   if (str_[processed_] != '}')
      throw gdu::parse_error(line_, offset_, "closing brace", "");
   processed_++;
   offset_++;
   val = SCValue(obj);
   return true;
}

inline bool gdu::SCParser::expect_equals_token() {
   if (processed_ == ssize(str_) || str_[processed_] != '=') {
      return false;
   }
   processed_++;
   offset_++;
   return true;
}

inline bool gdu::SCParser::expect_semi_token() {
   if (processed_ == ssize(str_) || str_[processed_] != ';') {
      return false;
   }
   processed_++;
   offset_++;
   return true;
}

inline void gdu::SCParser::skip_whitespace() {
   bool in_comment = false;
   while (processed_ < ssize(str_)) {
      if (str_[processed_] == '#') {
         in_comment = true; 
         processed_++;
         offset_++;
      }

      if (!isspace(str_[processed_])  && !in_comment ) {
         break;
      }

      if (str_[processed_] == '\n') {
            offset_ = 1;
            line_ ++;
            in_comment = false;
      } else {
         offset_++;
      }
      processed_++;
   }
}
