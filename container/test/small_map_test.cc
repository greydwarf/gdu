#include "gdu/container/small_map.h"
#include <string>
#include <iostream>

template<typename T> 
void expect_eq(const T& a, const T& b, const std::string& msg) {
   if (a != b) {
      std::cerr << "Error," << msg << "\n";
   }
}

void test_construction() {
   gdu::container::small_map<int, std::string> m { {7,"hi"}, {3,"thiere"}, {7, "no"}};
   expect_eq(m.v_.size(), 2UL, "size");
   expect_eq(m.v_[0], std::make_pair(3, std::string("thiere")), "element 1");
   expect_eq(m.v_[1], std::make_pair(7, std::string("hi")), "element 2");
}

void test_find() {
   auto el1 = std::make_pair<int, std::string>(3, std::string("test1"));
   auto el2 = std::make_pair<int, std::string>(7, std::string("hi"));
   gdu::container::small_map<int, std::string> m { el2, el1};
   expect_eq(*m.find(7), el2, "find second");
   expect_eq(*m.find(3), el1, "find first");
   expect_eq(m.find(6), m.cend(), "find unknown");
}

void test_subscript() {
   gdu::container::small_map<int, std::string> m;
   m[7] = "hi";
   m[3] = "test1";
   expect_eq(m[7], std::string("hi"), "find second");
   expect_eq(m[3], std::string("test1"), "find first");
   expect_eq(m[6], std::string(), "find unknown");
   m[6]=std::string("update");
   expect_eq(m[6], std::string("update"), "update");
   m[7]=std::string("another update");
   expect_eq(m[7], std::string("another update"), "find second after update");
}

void test_emplace() {
   gdu::container::small_map<int, std::string> m;
   m.emplace(7, "hi");
   m.emplace(3, "test1");
   expect_eq(m.size(), 2UL, "size (2)");
   expect_eq(m[3], std::string("test1"), "find first");
   expect_eq(m[7], std::string("hi"), "find second");
   m.emplace(7, "update");
   expect_eq(m[7], std::string("update"), "find second (updated)");
   expect_eq(m.size(), 2UL, "size (2)");
}

void test_small_insert() {
   gdu::container::small_map<int, std::string> m{{7,"hi"},{3,"there"}};
   std::vector<std::pair<int, std::string>> c {{3, "hey"}, {10,"add"}};
   m.insert(c.begin(), c.end());
   expect_eq(m[3], std::string("hey"), "replace");
   expect_eq(m[7], std::string("hi"), "original");
   expect_eq(m[10], std::string("add"), "added");
   expect_eq(m.size(), 3UL, "size");
}

void test_mass_insert() {
   gdu::container::small_map<int, std::string> m{{37,"hi"},{3,"there"}};
   std::vector<std::pair<int, std::string>> c;
   for (int ii=0; ii < 20; ii++) c.emplace_back(ii, std::to_string(ii));
   m.insert(c.begin(), c.end());
   expect_eq(m[3], std::string("3"), "replace");
   expect_eq(m[37], std::string("hi"), "original");
   expect_eq(m[10], std::string("10"), "added");
   expect_eq(m.size(), 21UL, "size");
}

int main() {
   gdu::container::small_map<int, std::string> m;
   test_construction();
   test_find();
   test_subscript();
   test_emplace();
   test_small_insert();
   test_mass_insert();
}

