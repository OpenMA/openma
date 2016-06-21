#include <cxxtest/TestDrive.h>

#include <openma/body/dempstertable.h>
#include <openma/body/segment.h>
#include <openma/body/inertialparameters.h>

CXXTEST_SUITE(DempsterTableTest)
{
  CXXTEST_TEST(generate)
  {
    TS_WARN("TODO");
  }
  
  CXXTEST_TEST(clone)
  {
    TS_WARN("Implement the method ma::body::DempsterTable::clone()");
  };
  
  CXXTEST_TEST(copy)
  {
    TS_WARN("Implement the method ma::body::DempsterTable::copy()");
  };
};

CXXTEST_SUITE_REGISTRATION(DempsterTableTest)
CXXTEST_TEST_REGISTRATION(DempsterTableTest, generate)
CXXTEST_TEST_REGISTRATION(DempsterTableTest, clone)
CXXTEST_TEST_REGISTRATION(DempsterTableTest, copy)
