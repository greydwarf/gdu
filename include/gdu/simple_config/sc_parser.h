#ifndef SC_PARSER_H_URAIWD6Y
#define SC_PARSER_H_URAIWD6Y

#include "gdu/simple_config.h"
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
      std::string expect_word_token();
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
   std::string key = parse_key();
   return processed_ != str_.size();
}

// EBNF:
// statement: key EQUALS value SEMI
std::string gdu::SCParser::parse_key() {
   skip_whitespace();
   if (processed_ == str_.size()) return "";
   std::string key = expect_word_token();
   if (key.empty()) {
      throw std::runtime_error("key expected");
   }
   return key;
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

void gdu::SCParser::skip_whitespace() {
   while (isspace(str_[processed_] )) {
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
