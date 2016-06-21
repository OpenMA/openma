#include <cxxtest/TestDrive.h>

#include <openma/body/inertialparameters.h>

CXXTEST_SUITE(InertialParametersTest)
{
  CXXTEST_TEST(constructor)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(clone)
  {
    TS_WARN("Implement the method ma::body::InertialParameters::clone()");
  };
  
  CXXTEST_TEST(copy)
  {
    TS_WARN("Implement the method ma::body::InertialParameters::copy()");
  };
};

CXXTEST_SUITE_REGISTRATION(InertialParametersTest)
CXXTEST_TEST_REGISTRATION(InertialParametersTest, constructor)
CXXTEST_TEST_REGISTRATION(InertialParametersTest, clone)
CXXTEST_TEST_REGISTRATION(InertialParametersTest, copy)
