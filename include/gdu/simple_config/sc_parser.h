#ifndef SC_PARSER_H_URAIWD6Y
#define SC_PARSER_H_URAIWD6Y

#include <iostream>
#include <time.h>
#include "gdu/simple_config/sc_object.h"
#include "gdu/simple_config/parse_error.h"
namespace gdu {
   class SCParser {
   public:
      static SCObject parse_string(const std::string& config);
#ifndef SC_TESTING
   private:
#endif
      const std::string& str_;
      size_t line_;
      size_t offset_;
      size_t processed_;
      SCObject result_;

      explicit SCParser(const std::string& str): str_(str), line_(1), offset_(0), processed_(0)
      {}
      void parse();
      bool parse_statement();
      std::string parse_key();
      gdu::SCValue parse_value();
      std::string expect_word_token();
      bool expect_equals_token();
      bool expect_semi_token();
      bool expect_int_token(int64_t& val);
      bool expect_double_token(double& val);
      bool expect_bool_token(bool& val);
      bool expect_date_token(int64_t& val);
      bool expect_string_token(std::string& val);
      void skip_whitespace();
   };
}

#include <regex>
inline gdu::SCObject gdu::SCParser::parse_string(const std::string& str) {
   SCParser t(str);
   t.parse();
   return t.result_;
}

// EBNF:
// file: statement* EOF
// statement: key EQUALS value SEMI
// key: WORD
// value: NUMBER
inline void gdu::SCParser::parse() {
   bool more = true;
   while (more) {
      more = parse_statement();
   }
}

// EBNF:
// statement: key EQUALS value SEMI
inline bool gdu::SCParser::parse_statement() {
   skip_whitespace();
   if (processed_ == str_.size()) return false;
   std::string key = parse_key();
   skip_whitespace();
   bool has_equals = expect_equals_token();
   if (!has_equals) {
      throw gdu::parse_error(line_, offset_, "'='", "");
   }
   skip_whitespace();
   gdu::SCValue val = parse_value();
   skip_whitespace();
   bool has_semi = expect_semi_token();
   if (!has_semi) {
      throw gdu::parse_error(line_, offset_, "';'", "");
   }
   result_.add( key, val);
   return processed_ != str_.size();
}

// EBNF:
// key: WORD | STRING
inline std::string gdu::SCParser::parse_key() {
   std::string key = expect_word_token();
   if (key.empty()) {
      throw gdu::parse_error(line_, offset_, "key", "");
   }
   return key;
}

// EBNF:
// value: STRING | INT | DOUBLE | DATE | BOOL | array | object
inline gdu::SCValue gdu::SCParser::parse_value() {
   if (processed_ == str_.size()) {
      throw gdu::parse_error(line_, offset_, "value", "end of file");
   }
   if (isdigit(str_[processed_]) || str_[processed_] == '+' || str_[processed_] == '-') {
      double d;
      int64_t i;
      if (expect_double_token(d)) {
         return gdu::SCValue(d);
      } else if (expect_date_token(i)) {
         return gdu::SCValue(i, true);
      } else if (expect_int_token(i)) {
         return gdu::SCValue(i);
      } else {
         throw gdu::parse_error(line_, offset_, "number", "");
      }
   } else {
      bool b;
      std::string str;
      if (expect_bool_token(b)) {
         return gdu::SCValue(b);
      } else if (expect_string_token(str)) {
         return gdu::SCValue(str);
      }
   }
   throw gdu::parse_error(line_, offset_, "value", "");
}

inline std::string gdu::SCParser::expect_word_token() {
   static const std::regex word_re("(\\w+)");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, word_re, std::regex_constants::match_continuous) &&
      m.size() >= 2) {
      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      return m[1].str();
   }
   return "";
}

inline bool gdu::SCParser::expect_int_token(int64_t&val) {
   static const std::regex int_re("([+-]?(\\d+))([KMGsmhd])?");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, int_re, 
         std::regex_constants::match_continuous) &&
      m.size() >= 2) {

      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      val = std::strtol(m[1].str().c_str(), nullptr, 10);
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_double_token(double& val) {
   static const std::regex float_re("^([-+]?[0-9]+\\.[0-9]*([eE][-+]?[0-9]+)?)([KMGsmhd])?");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, float_re, 
         std::regex_constants::match_continuous) &&
      m.size() >= 2) {

      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      val = std::strtod(m[1].str().c_str(), nullptr);
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_bool_token(bool& val) {
   static const std::regex bool_re("^(true|false)", std::regex_constants::icase);
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, bool_re, 
         std::regex_constants::match_continuous) &&
      m.size() == 2) {

      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      val = std::tolower(m[1].str()[0]) == 't';
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_date_token(int64_t& val) {
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
      val = epoch;

      processed_ += m[0].length();
      offset_ += m[0].length();
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_string_token(std::string& val) {
   static const std::regex 
      str_re("\"((([^\"\\\\\n\r]*)(\\\\(\"))?)*)\"");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, str_re, 
         std::regex_constants::match_continuous)) {

      val = m[1].str();
      for (size_t ii = 0; ii < val.size(); ++ii) {
         if (val[ii] == '\\' && ii+1 != val.size()) {
            switch (val[ii+1]) {
            }
         }

      }
      processed_ += m[0].length();
      offset_ += m[0].length();
      return true;
   }
   return false;
}

inline bool gdu::SCParser::expect_equals_token() {
   if (processed_ == str_.size() || str_[processed_] != '=') {
      return false;
   }
   processed_++;
   offset_++;
   return true;
}

inline bool gdu::SCParser::expect_semi_token() {
   if (processed_ == str_.size() || str_[processed_] != ';') {
      return false;
   }
   processed_++;
   offset_++;
   return true;
}

inline void gdu::SCParser::skip_whitespace() {
   bool in_comment = false;
   while (processed_ < str_.size()) {
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

#endif /* end of include guard: SC_PARSER_H_URAIWD6Y */
