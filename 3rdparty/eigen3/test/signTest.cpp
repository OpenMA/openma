#include <cxxtest/TestDrive.h>

#include <Eigen_openma/Utils/sign.h>

CXXTEST_SUITE(SignTest)
{
  CXXTEST_TEST(positive)
  {
    TS_ASSERT_EQUALS(sign(1292) > 0, true);
    TS_ASSERT_EQUALS(sign(9999) > 0, true);
    TS_ASSERT_EQUALS(sign(1) > 0, true);
  };
  
  CXXTEST_TEST(negative)
  {
    TS_ASSERT_EQUALS(sign(-11) < 0, true);
    TS_ASSERT_EQUALS(sign(-5000) < 0, true);
    TS_ASSERT_EQUALS(sign(-1) < 0, true);
  };
  
  CXXTEST_TEST(null)
  {
    TS_ASSERT_EQUALS(sign(+0) == 0, true);
    TS_ASSERT_EQUALS(sign(-0) == 0, true);
    TS_ASSERT_EQUALS(sign(0) == 0, true);
  };
};

CXXTEST_SUITE_REGISTRATION(SignTest)
CXXTEST_TEST_REGISTRATION(SignTest, positive)
CXXTEST_TEST_REGISTRATION(SignTest, negative)
CXXTEST_TEST_REGISTRATION(SignTest, null)
