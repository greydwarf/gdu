#define private public
#include "gdu/flat_map.h"
#undef private

#include <string>
#include <iostream>
#include "gtest/gtest.h"

TEST(flat_map, construction) {
   gdu::flat_map<int, std::string> m { {7,"hi"}, {3,"thiere"}, {7, "no"}};
   EXPECT_EQ( 2UL,m.v_.size());
   EXPECT_EQ( std::make_pair(3, std::string("thiere")), m.v_[0]);
   EXPECT_EQ( std::make_pair(7, std::string("hi")), m.v_[1]);
}

TEST(flat_map, find) {
   auto el1 = std::make_pair<int, std::string>(3, std::string("test1"));
   auto el2 = std::make_pair<int, std::string>(7, std::string("hi"));
   gdu::flat_map<int, std::string> m { el2, el1};
   EXPECT_EQ( el2, *m.find(7));
   EXPECT_EQ( el1, *m.find(3));
   EXPECT_EQ( m.cend(), m.find(6));
}

TEST(flat_map, subscript) {
   gdu::flat_map<int, std::string> m;
   m[7] = "hi";
   m[3] = "test1";
   EXPECT_EQ( std::string("hi"), m[7]);
   EXPECT_EQ( std::string("test1"), m[3]);
   EXPECT_EQ( std::string(), m[6]);
   m[6]=std::string("update");
   EXPECT_EQ( std::string("update"), m[6]);
   m[7]=std::string("another update");
   EXPECT_EQ( std::string("another update"), m[7]);
}

TEST(flat_map, emplace) {
   gdu::flat_map<int, std::string> m;
   m.emplace(7, "hi");
   m.emplace(3, "test1");
   EXPECT_EQ( 2UL, m.size());
   EXPECT_EQ( std::string("test1"), m[3]);
   EXPECT_EQ( std::string("hi"), m[7]);
   m.emplace(7, "update");
   EXPECT_EQ( std::string("update"), m[7]);
   EXPECT_EQ( 2UL, m.size());
}

TEST(flat_map, small_insert) {
   gdu::flat_map<int, std::string> m{{7,"hi"},{3,"there"}};
   std::vector<std::pair<int, std::string>> c {{3, "hey"}, {10,"add"}};
   m.insert(c.begin(), c.end());
   EXPECT_EQ( std::string("hey"), m[3]);
   EXPECT_EQ( std::string("hi"), m[7]);
   EXPECT_EQ( std::string("add"), m[10]);
   EXPECT_EQ( 3UL, m.size());
}

TEST(flat_map, mass_insert) {
   gdu::flat_map<int, std::string> m{{37,"hi"},{3,"there"}};
   std::vector<std::pair<int, std::string>> c;
   for (int ii=0; ii < 20; ii++) c.emplace_back(ii, std::to_string(ii));
   m.insert(c.begin(), c.end());
   EXPECT_EQ( std::string("3"), m[3]);
   EXPECT_EQ( std::string("hi"), m[37]);
   EXPECT_EQ( std::string("10"), m[10]);
   EXPECT_EQ( 21UL, m.size());
}
