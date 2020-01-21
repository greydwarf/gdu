#define private public
#include "gdu/simple_config/sc_value.h"
#undef private

#include "gtest/gtest.h"
using namespace gdu;
TEST(SCValue, as_int) {
   SCValue val(12);
   EXPECT_EQ(12, val.as_int());

   SCValue unset;
   EXPECT_THROW(unset.as_int(), std::invalid_argument);
   EXPECT_EQ(12, unset.as_int(12));

   SCValue fval(3.1415);
   EXPECT_EQ(3, fval.as_int());
}
TEST(SCValue, as_double) {
   SCValue val(12);
   EXPECT_EQ(static_cast<double>(12), val.as_float());

   SCValue unset;
   EXPECT_THROW(unset.as_float(), std::invalid_argument);
   EXPECT_EQ(12, unset.as_float(12));

   SCValue fval(3.1415);
   EXPECT_NEAR(3.1415, fval.as_float(), 0.001);
}
