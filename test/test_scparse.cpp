#define SC_TESTING
#include "gdu/simple_config/sc_parser.h"

#include "gtest/gtest.h"
using namespace gdu;

TEST(SCValue, parse_whith_whitespace) {
   std::string t1 = "key=5;";
   SCObject a = SCParser::parse_string(t1);
   EXPECT_TRUE(a["key"].is_int());
   EXPECT_EQ(5, a["key"].as_int());
   EXPECT_EQ(5.0, a["key"].as_float());
   std::string t2 = "   key2 = 1234242;";
   auto b = SCParser::parse_string(t2);
   EXPECT_TRUE(b["key2"].is_int());
   EXPECT_EQ(1234242, b["key2"].as_int());
   EXPECT_EQ(1234242.0, b["key2"].as_float());
   std::string t3 = "   \n   key3 \n   = \n  45674  ;";
   auto c = SCParser::parse_string(t3);
   EXPECT_TRUE(c["key3"].is_int());
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

TEST(SCValue, parse_errors) {
   EXPECT_THROW(SCParser::parse_string("%foo"), gdu::parse_error);
   EXPECT_THROW(SCParser::parse_string("   foo"), gdu::parse_error);
   EXPECT_THROW(SCParser::parse_string("  foo: 5;"), gdu::parse_error);
   EXPECT_THROW(SCParser::parse_string("foo="), gdu::parse_error);
   EXPECT_THROW(SCParser::parse_string("foo=are"), gdu::parse_error);
   EXPECT_THROW(SCParser::parse_string("foo=56"), gdu::parse_error);
   EXPECT_THROW(SCParser::parse_string("foo=56 bar;"), gdu::parse_error);
}

TEST(SCValue, parse_int) {

   SCObject a = SCParser::parse_string("key=5;");
   EXPECT_TRUE(a["key"].is_int());
   EXPECT_EQ(5, a["key"].as_int());
   EXPECT_EQ(5.0, a["key"].as_float());

   auto b = SCParser::parse_string("key=-5;");
   EXPECT_TRUE(b["key"].is_int());
   EXPECT_EQ(-5, b["key"].as_int());
   EXPECT_EQ(-5.0, b["key"].as_float());
}

TEST(SCValue, parse_bool) {

   SCObject a = SCParser::parse_string("key=true;");
   EXPECT_TRUE(a["key"].is_bool());
   EXPECT_TRUE(a["key"].as_bool());

   auto b = SCParser::parse_string("key=True;");
   EXPECT_TRUE(b["key"].is_bool());
   EXPECT_TRUE(b["key"].as_bool());

   auto c = SCParser::parse_string("key=false;");
   EXPECT_TRUE(c["key"].is_bool());
   EXPECT_FALSE(c["key"].as_bool());
}

TEST(SCValue, parse_float) {

   SCObject a = SCParser::parse_string("key=5.0;");
   EXPECT_TRUE(a["key"].is_float());
   EXPECT_EQ(5, a["key"].as_int());
   EXPECT_EQ(5.0, a["key"].as_float());

   auto b = SCParser::parse_string("key=-5.0;");
   EXPECT_TRUE(b["key"].is_float());
   EXPECT_EQ(-5, b["key"].as_int());
   EXPECT_EQ(-5.0, b["key"].as_float());
   
   auto c = SCParser::parse_string("key=+5.0;");
   EXPECT_TRUE(c["key"].is_float());
   EXPECT_EQ(5, c["key"].as_int());
   EXPECT_EQ(5.0, c["key"].as_float());
   
   auto d = SCParser::parse_string("key=5.0e2;");
   EXPECT_TRUE(d["key"].is_float());
   EXPECT_EQ(500, d["key"].as_int());
   EXPECT_EQ(500.0, d["key"].as_float());
   
   auto e = SCParser::parse_string("key=5.0e-2;");
   EXPECT_TRUE(e["key"].is_float());
   EXPECT_EQ(0, e["key"].as_int());
   EXPECT_EQ(0.05, e["key"].as_float());
   
   auto f = SCParser::parse_string("key=5.1e-2;");
   EXPECT_TRUE(f["key"].is_float());
   EXPECT_EQ(0, f["key"].as_int());
   EXPECT_EQ(0.051, f["key"].as_float());
}

