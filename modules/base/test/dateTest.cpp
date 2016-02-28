#include <cxxtest/TestDrive.h>

#include <openma/base/date.h>

CXXTEST_SUITE(DateTest)
{
  CXXTEST_TEST(accessors)
  {
    auto d = ma::Date(2016,02,27);
    TS_ASSERT_EQUALS(d.year(),2016u);
    TS_ASSERT_EQUALS(d.month(),02u);
    TS_ASSERT_EQUALS(d.day(),27u);
  };
  
  CXXTEST_TEST(mutators)
  {
    auto d = ma::Date(2016,02,27);
    d.setDay(01);
    d.setMonth(05);
    d.setYear(2009);
    TS_ASSERT_EQUALS(d.day(),01u);
    TS_ASSERT_EQUALS(d.month(),05u);
    TS_ASSERT_EQUALS(d.year(),2009u);
  };
  
  CXXTEST_TEST(equality)
  {
    auto d1 = ma::Date(2016,02,27);
    auto d2 = ma::Date(2016,02,27);
    auto d3 = ma::Date();
    TS_ASSERT_EQUALS(d1 == d2, true);
    TS_ASSERT_EQUALS(d1, d2);
    TS_ASSERT_EQUALS(d1 != d3, true);
    TS_ASSERT_DIFFERS(d1, d3);
  }
};

CXXTEST_SUITE_REGISTRATION(DateTest)
CXXTEST_TEST_REGISTRATION(DateTest, accessors)
CXXTEST_TEST_REGISTRATION(DateTest, mutators)
CXXTEST_TEST_REGISTRATION(DateTest, equality)