#ifndef SC_PARSER_H_URAIWD6Y
#define SC_PARSER_H_URAIWD6Y

#include <iostream>
#include <time.h>
#include "gdu/size.h"
#include "gdu/simple_config/sc_object.h"
#include "gdu/simple_config/parse_error.h"
namespace gdu {
   class SCParser {
   public:
      [[nodiscard]] static SCObject parse_string(const std::string& config);
#ifndef SC_TESTING
   private:
#endif
      const std::string& str_;
      ssize_t line_;
      ssize_t offset_;
      ssize_t processed_;
      SCObject result_;

      explicit SCParser(const std::string& str): str_(str), line_(1), offset_(0), processed_(0)
      {}
      void parse();
      [[nodiscard]] bool expect_statement(std::string& key, SCValue& value);
      [[nodiscard]] bool expect_key(std::string& key);
      [[nodiscard]] bool expect_value(SCValue& val);
      [[nodiscard]] std::string expect_word_token();
      [[nodiscard]] bool expect_equals_token();
      [[nodiscard]] bool expect_semi_token();
      [[nodiscard]] bool expect_int_token(SCValue& val);
      [[nodiscard]] bool expect_double_token(SCValue& val);
      [[nodiscard]] bool expect_bool_token(SCValue& val);
      [[nodiscard]] bool expect_date_token(SCValue& val);
      [[nodiscard]] bool expect_string_token(SCValue& val);
      [[nodiscard]] bool expect_array(SCValue& val);
      [[nodiscard]] bool expect_object(SCValue& val);
      void skip_whitespace();
   };
}

#include "gdu/simple_config/sc_parser.inl"

#endif /* end of include guard: SC_PARSER_H_URAIWD6Y */
