#include <cxxtest/TestDrive.h>

#include <openma/body/chain.h>
#include <openma/body/segment.h>
#include <openma/body/joint.h>

CXXTEST_SUITE(AnchorTest)
{
  CXXTEST_TEST(constructor)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(setDefinition)
  {
    TS_WARN("Implement the method ma::body::Chain::setDefinition()");
  }
  
  CXXTEST_TEST(clone)
  {
    TS_WARN("Implement the method ma::body::Chain::clone()");
  };
  
  CXXTEST_TEST(copy)
  {
    TS_WARN("Implement the method ma::body::Chain::copy()");
  };
};

CXXTEST_SUITE_REGISTRATION(AnchorTest)
CXXTEST_TEST_REGISTRATION(AnchorTest, constructor)
CXXTEST_TEST_REGISTRATION(AnchorTest, setDefinition)
CXXTEST_TEST_REGISTRATION(AnchorTest, clone)
CXXTEST_TEST_REGISTRATION(AnchorTest, copy)
