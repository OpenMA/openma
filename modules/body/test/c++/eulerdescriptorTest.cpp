#include <cxxtest/TestDrive.h>

#include <openma/body/eulerdescriptor.h>

CXXTEST_SUITE(EulerDescriptorTest)
{
  CXXTEST_TEST(process)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(clone)
  {
    TS_WARN("Implement the method ma::body::EulerDescriptor::clone()");
  };
  
  CXXTEST_TEST(copy)
  {
    TS_WARN("Implement the method ma::body::EulerDescriptor::copy()");
  };
};

CXXTEST_SUITE_REGISTRATION(EulerDescriptorTest)
CXXTEST_TEST_REGISTRATION(EulerDescriptorTest, process)
CXXTEST_TEST_REGISTRATION(EulerDescriptorTest, clone)
CXXTEST_TEST_REGISTRATION(EulerDescriptorTest, copy)
