#include <cxxtest/TestDrive.h>

#include <openma/body/anchor.h>
#include <openma/body/segment.h>
#include <openma/body/point.h>
#include <openma/body/referenceframe.h>

CXXTEST_SUITE(AnchorTest)
{
  CXXTEST_TEST(point)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(origin1)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(origin2)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(clone)
  {
    TS_WARN("Implement the method ma::body::Anchor::clone()");
  };
  
  CXXTEST_TEST(copy)
  {
    TS_WARN("Implement the method ma::body::Anchor::copy()");
  };
};

CXXTEST_SUITE_REGISTRATION(AnchorTest)
CXXTEST_TEST_REGISTRATION(AnchorTest, point)
CXXTEST_TEST_REGISTRATION(AnchorTest, origin1)
CXXTEST_TEST_REGISTRATION(AnchorTest, origin2)
CXXTEST_TEST_REGISTRATION(AnchorTest, clone)
CXXTEST_TEST_REGISTRATION(AnchorTest, copy)
