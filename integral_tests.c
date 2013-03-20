
#include <stdbool.h>
#include <math.h>

#include "integral.h"

#include "gtest/gtest.h"

double dsin(double x)
{
  return sin(x);
}

TEST(IntegralTests, SineIntegralTest1) {
  double ans;
  integrate(dsin, 0, 2 * M_PI, 4, 1000000, &ans);
  EXPECT_NEAR(ans, 0.0, 1e-5);
}

TEST(IntegralTests, SineIntegralTest2) {
  double ans;
  integrate(dsin, 0, M_PI, 4, 1000000, &ans);
  EXPECT_NEAR(ans, 2.0, 1e-4);
}

TEST(IntegralTests, SineIntegralTest3) {
  double ans;
  integrate(dsin, 0, 1.0, 4, 1000000, &ans);
  EXPECT_NEAR(ans, 0.45969769413186, 1e-5);
}

