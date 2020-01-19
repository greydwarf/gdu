#define private public
#include "gdu/simple_config/sc_value.h"
#undef private

#include "gtest/gtest.h"
using namespace gdu;
TEST(SVAlue, as_int8) {
   SCValue val;
   val.set_int(12);
   EXPECT_EQ(static_cast<int8_t>(12), val.as<int8_t>());
}
