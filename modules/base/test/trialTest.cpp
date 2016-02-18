#include <cxxtest/TestDrive.h>

#include <openma/base/trial.h>

CXXTEST_SUITE(TrialTest)
{
  CXXTEST_TEST(events)
  {
    ma::Trial trial("trial");
    TS_ASSERT_DIFFERS(trial.events(), nullptr);
    TS_ASSERT_DIFFERS(trial.findChild("Events"), nullptr);
    TS_ASSERT_EQUALS(trial.events(), trial.findChild("Events"));
  };
  
  CXXTEST_TEST(timesequences)
  {
    ma::Trial trial("trial");
    TS_ASSERT_DIFFERS(trial.timeSequences(), nullptr);
    TS_ASSERT_DIFFERS(trial.findChild("TimeSequences"), nullptr);
    TS_ASSERT_EQUALS(trial.timeSequences(), trial.findChild("TimeSequences"));
  };
};

CXXTEST_SUITE_REGISTRATION(TrialTest)
CXXTEST_TEST_REGISTRATION(TrialTest, events)
CXXTEST_TEST_REGISTRATION(TrialTest, timesequences)