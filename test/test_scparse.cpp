#define SC_TESTING
#include "gdu/simple_config/sc_parser.h"

#include "gtest/gtest.h"
using namespace gdu;

TEST(SCValue, parse_string) {
   std::string t1 = "key=5;";
   SCObject a = SCParser::parse_string(t1);
   EXPECT_TRUE(a["key"].is_float());
   EXPECT_EQ(5, a["key"].as_int());
   EXPECT_EQ(5.0, a["key"].as_float());
   std::string t2 = "   key2 = 1234242;";
   auto b = SCParser::parse_string(t2);
   EXPECT_TRUE(b["key2"].is_float());
   EXPECT_EQ(1234242, b["key2"].as_int());
   EXPECT_EQ(1234242.0, b["key2"].as_float());
   std::string t3 = "   \n   key3 \n   = \n  45674  ;";
   auto c = SCParser::parse_string(t3);
   EXPECT_TRUE(c["key3"].is_float());
   EXPECT_EQ(45674, c["key3"].as_int());
   EXPECT_EQ(45674.0, c["key3"].as_float());
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

