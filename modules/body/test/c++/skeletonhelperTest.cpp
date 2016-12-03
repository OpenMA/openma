#include <cxxtest/TestDrive.h>

#include <openma/body/skeletonhelper.h>

CXXTEST_SUITE(SkeletonHelperTest)
{
  CXXTEST_TEST(process)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(clone)
  {
    TS_WARN("Implement the method ma::body::SkeletonHelper::clone()");
  };
  
  CXXTEST_TEST(copy)
  {
    TS_WARN("Implement the method ma::body::SkeletonHelper::copy()");
  };
};

CXXTEST_SUITE_REGISTRATION(SkeletonHelperTest)
CXXTEST_TEST_REGISTRATION(SkeletonHelperTest, process)
CXXTEST_TEST_REGISTRATION(SkeletonHelperTest, clone)
CXXTEST_TEST_REGISTRATION(SkeletonHelperTest, copy)
