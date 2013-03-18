
#include <stdbool.h>
#include <math.h>

#include "integral.h"

#include "gtest/gtest.h"

float fsin(float x)
{
  return sin(x);
}

TEST(IntegralTests, SineIntegralTest1) {
  EXPECT_NEAR(integrate(fsin, 0, 2 * M_PI, 4, 100000), 0.0, 1e-6);
}

TEST(IntegralTests, SineIntegralTest2) {
  EXPECT_NEAR(integrate(fsin, 0, M_PI, 4, 100000), 2.0, 1e-4);
}

TEST(IntegralTests, SineIntegralTest3) {
  EXPECT_NEAR(integrate(fsin, 0, 1.0, 4, 1000000), 0.45969769413186, 1e-4);
}

