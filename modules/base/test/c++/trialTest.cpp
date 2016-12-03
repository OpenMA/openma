#include <cxxtest/TestDrive.h>

#include <openma/base/trial.h>
#include <openma/base/event.h>
#include <openma/base/timesequence.h>

CXXTEST_SUITE(TrialTest)
{
  CXXTEST_TEST(events)
  {
    ma::Trial trial("trial");
    TS_ASSERT_EQUALS(trial.children().size(), 0ul);
    TS_ASSERT_DIFFERS(trial.events(), nullptr);
    TS_ASSERT_EQUALS(trial.children().size(), 1ul);
    TS_ASSERT_DIFFERS(trial.findChild("Events"), nullptr);
    TS_ASSERT_EQUALS(trial.events(), trial.findChild("Events"));
  };
  
  CXXTEST_TEST(event)
  {
    ma::Trial trial("trial");
    auto evt1 = new ma::Event("evt1"); evt1->addParent(trial.events());
    auto evt2 = new ma::Event("evt2"); evt2->addParent(trial.events());
    TS_ASSERT_EQUALS(trial.event(0), evt1);
    TS_ASSERT_EQUALS(trial.event(1), evt2);
    TS_ASSERT_EQUALS(trial.event(2), nullptr);
  };
  
  CXXTEST_TEST(timesequences)
  {
    ma::Trial trial("trial");
    TS_ASSERT_EQUALS(trial.children().size(), 0ul);
    TS_ASSERT_DIFFERS(trial.timeSequences(), nullptr);
    TS_ASSERT_EQUALS(trial.children().size(), 1ul);
    TS_ASSERT_DIFFERS(trial.findChild("TimeSequences"), nullptr);
    TS_ASSERT_EQUALS(trial.timeSequences(), trial.findChild("TimeSequences"));
  };
  
  CXXTEST_TEST(timesequence)
  {
    ma::Trial trial("trial");
    auto ts1 = new ma::TimeSequence("ts1", 1, 0, 100.0, 0.0, 0, "mm"); ts1->addParent(trial.timeSequences());
    auto ts2 = new ma::TimeSequence("ts2", 1, 0, 100.0, 0.0, 0, "mm"); ts2->addParent(trial.timeSequences());
    TS_ASSERT_EQUALS(trial.timeSequence(0), ts1);
    TS_ASSERT_EQUALS(trial.timeSequence(1), ts2);
    TS_ASSERT_EQUALS(trial.timeSequence(2), nullptr);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::Trial trial("trial");
    auto node1 = new ma::Node("leaf", trial.timeSequences());
    auto node2 = new ma::Node("leaf2", trial.events());
    auto clone = static_cast<ma::Trial*>(trial.clone());
    TS_ASSERT_EQUALS(clone->children().size(), 2ul);
    TS_ASSERT_DIFFERS(clone->timeSequences(), trial.timeSequences());
    TS_ASSERT_DIFFERS(clone->events(), trial.events());
    TS_ASSERT_DIFFERS(clone->findChild("TimeSequences"), nullptr);
    TS_ASSERT_EQUALS(clone->timeSequences(), clone->findChild("TimeSequences"));
    TS_ASSERT_DIFFERS(clone->findChild("Events"), nullptr);
    TS_ASSERT_EQUALS(clone->events(), clone->findChild("Events"));
    TS_ASSERT_DIFFERS(clone->timeSequences()->child(0), node1);
    TS_ASSERT_DIFFERS(clone->events()->child(0), node2);
    TS_ASSERT_EQUALS(clone->timeSequences()->child(0)->name(), node1->name());
    TS_ASSERT_EQUALS(clone->events()->child(0)->name(), node2->name());
    delete clone;
  };
  
  CXXTEST_TEST(copy)
  {
    ma::Trial trial1("trial1");
    ma::Trial trial2("trial2");
    new ma::Node("leaf", trial2.timeSequences());
    new ma::Node("leaf2", trial2.events());
    trial2.copy(&trial1);
    TS_ASSERT_EQUALS(trial2.children().size(), 0ul);
    TS_ASSERT_EQUALS(trial2.name(), trial1.name());
  }
};

CXXTEST_SUITE_REGISTRATION(TrialTest)
CXXTEST_TEST_REGISTRATION(TrialTest, events)
CXXTEST_TEST_REGISTRATION(TrialTest, event)
CXXTEST_TEST_REGISTRATION(TrialTest, timesequences)
CXXTEST_TEST_REGISTRATION(TrialTest, timesequence)
CXXTEST_TEST_REGISTRATION(TrialTest, clone)
CXXTEST_TEST_REGISTRATION(TrialTest, copy) 