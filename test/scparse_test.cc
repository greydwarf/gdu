#define SC_TESTING
#include "gdu/simple_config/sc_parser.h"

#include "gtest/gtest.h"
using namespace gdu;

TEST(SCValue, simple_key) {
   std::string t1 = "key=5;";
   SCParser::parse_string(t1);
   std::string t2 = "   key2 = 1234242;";
   SCParser::parse_string(t2);
   std::string t3 = "   \n   key3 \n   = \n  45674  ;";
   SCParser::parse_string(t3);
   std::string t4 = "   %foo";
   EXPECT_THROW(SCParser::parse_string(t4), gdu::parse_error);
   std::string t5 = "   foo";
   EXPECT_THROW(SCParser::parse_string(t5), gdu::parse_error);
   std::string t6 = "   foo:";
   EXPECT_THROW(SCParser::parse_string(t6), gdu::parse_error);
   std::string t7 = "   foo  =";
   EXPECT_THROW(SCParser::parse_string(t7), gdu::parse_error);
   std::string t8 = "   foo  = are";
   EXPECT_THROW(SCParser::parse_string(t8), gdu::parse_error);
   std::string t9 = "   foo  = 57";
   EXPECT_THROW(SCParser::parse_string(t9), gdu::parse_error);
   std::string t10 = "   foo  = 57 sdf";
   EXPECT_THROW(SCParser::parse_string(t10), gdu::parse_error);
}

