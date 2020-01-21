#define SC_TESTING
#include "gdu/simple_config/sc_parser.h"

#include "gtest/gtest.h"
using namespace gdu;

TEST(SCValue, simple_key) {
   std::string t1 = "key";
   SCParser::parse_string(t1);
   std::string t2 = "   key2";
   SCParser::parse_string(t2);
   std::string t3 = "   \n   key3";
   SCParser::parse_string(t3);
   std::string t4 = "   \n   key4   ";
   SCParser::parse_string(t4);
   std::string t5 = "   keya\n keyb\n  keyc\n";
   SCParser::parse_string(t5);
   std::string t6 = "   %foo";
   EXPECT_THROW(SCParser::parse_string(t6), std::runtime_error);
}

