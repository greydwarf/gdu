#ifndef SC_PARSER_H_URAIWD6Y
#define SC_PARSER_H_URAIWD6Y

#include "gdu/simple_config.h"
#include "gdu/simple_config/parse_error.h"
namespace gdu {
   class SCParser {
   public:
      static SimpleConfig parse_string(const std::string& config);
#ifndef SC_TESTING
   private:
#endif
      const std::string& str_;
      size_t line_;
      size_t offset_;
      size_t processed_;
      SimpleConfig result_;

      SCParser(const std::string& str): str_(str), line_(1), offset_(0), processed_(0)
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
      void skip_whitespace();
   };
}

#include <regex>
gdu::SimpleConfig gdu::SCParser::parse_string(const std::string& str) {
   SCParser t(str);
   t.parse();
   return t.result_;
}

// EBNF:
// file: statement* EOF
// statement: key EQUALS value SEMI
// key: WORD
// value: NUMBER
void gdu::SCParser::parse() {
   bool more = true;
   while (more) {
      more = parse_statement();
   }
}

// EBNF:
// statement: key EQUALS value SEMI
bool gdu::SCParser::parse_statement() {
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
   return processed_ != str_.size();
}

// EBNF:
// key: WORD | STRING
std::string gdu::SCParser::parse_key() {
   std::string key = expect_word_token();
   if (key.empty()) {
      throw gdu::parse_error(line_, offset_, "key", "");
   }
   return key;
}

// EBNF:
// value: STRING | INT | DOUBLE | DATE | BOOL | array | object
gdu::SCValue gdu::SCParser::parse_value() {
   if (processed_ == str_.size()) {
      throw gdu::parse_error(line_, offset_, "value", "end of file");
   }
   if (isdigit(str_[processed_])) {
      double d;
      int64_t i;
      if (expect_double_token(d)) {
         return gdu::SCValue(d);
      } else if (expect_int_token(i)) {
         return gdu::SCValue(i);
      }
   } 
   throw gdu::parse_error(line_, offset_, "value", "");
}

std::string gdu::SCParser::expect_word_token() {
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

bool gdu::SCParser::expect_int_token(int64_t&val) {
   static const std::regex int_re("((\\d+)([KMGsmhd])?");
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

bool gdu::SCParser::expect_double_token(double& val) {
   static const std::regex int_re("(\\d+)([KMGsmhd])?");
   std::cmatch m;
   if (std::regex_search(str_.c_str()+processed_, m, int_re, 
         std::regex_constants::match_continuous) &&
      m.size() >= 2) {

      size_t sz = m[1].str().size();
      processed_ += sz;
      offset_ += sz;
      val = std::strtod(m[1].str().c_str(), nullptr);
   }
   return "";
}

bool gdu::SCParser::expect_equals_token() {
   if (processed_ == str_.size() || str_[processed_] != '=') {
      return false;
   }
   processed_++;
   offset_++;
   return true;
}

bool gdu::SCParser::expect_semi_token() {
   if (processed_ == str_.size() || str_[processed_] != ';') {
      return false;
   }
   processed_++;
   offset_++;
   return true;
}

void gdu::SCParser::skip_whitespace() {
   while (processed_ < str_.size() && isspace(str_[processed_] )) {
      if (str_[processed_] == '\n') {
         offset_ = 1;
         line_ ++;
      } else {
         offset_++;
      }
      processed_++;
   }
}

#endif /* end of include guard: SC_PARSER_H_URAIWD6Y */
