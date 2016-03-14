#include <cxxtest/TestDrive.h>

#include <openma/base/subject.h>
#include <openma/base/date.h>
#include <openma/base/enums.h>

ma::Subject* _generate_dummy_subject(ma::Node* parent)
{
  auto subject = new ma::Subject("Anonymous", 
    {{"description", "Dummy subject"},
     {"sex", ma::Sex::Male},
     {"height", 1750.0},
     {"weight", 74.0},
     {"birthdate", ma::Date{1981,3,9}},
     {"leftLegLength",940.0},
     {"leftKneeWidth",110.0},
     {"leftAnkleWidth",70.0},
     {"leftFootFlat", true},
     {"rightLegLength",940.0},
     {"rightKneeWidth",120.0},
     {"rightAnkleWidth",70.0},
     {"rightFootFlat", true}},
    parent);
  return subject;
}

void _compare_dummy_subject(ma::Node* subject)
{
  TS_ASSERT_EQUALS(subject->name(), std::string{"Anonymous"});
  TS_ASSERT_EQUALS(subject->description(), std::string{"Dummy subject"});
  TS_ASSERT_EQUALS(subject->property("sex").cast<ma::Sex>(), ma::Sex::Male);
  TS_ASSERT_EQUALS(subject->property("height").cast<double>(), 1750.0);
  TS_ASSERT_EQUALS(subject->property("weight").cast<double>(), 74.0);
  TS_ASSERT_EQUALS(subject->property("birthdate").cast<ma::Date>(), ma::Date(1981,3,9));
  TS_ASSERT_EQUALS(subject->property("leftLegLength").cast<double>(), 940.0);
  TS_ASSERT_EQUALS(subject->property("leftKneeWidth").cast<double>(), 110.0);
  TS_ASSERT_EQUALS(subject->property("leftAnkleWidth").cast<double>(), 70.0);
  TS_ASSERT_EQUALS(subject->property("leftFootFlat").cast<bool>(), true);
  TS_ASSERT_EQUALS(subject->property("rightLegLength").cast<double>(), 940.0);
  TS_ASSERT_EQUALS(subject->property("rightKneeWidth").cast<double>(), 120.0);
  TS_ASSERT_EQUALS(subject->property("rightAnkleWidth").cast<double>(), 70.0);
  TS_ASSERT_EQUALS(subject->property("rightFootFlat").cast<bool>(), true);
}

CXXTEST_SUITE(SubjectTest)
{
  CXXTEST_TEST(properties)
  {
    ma::Node root("root");
    auto subject = _generate_dummy_subject(&root);
    _compare_dummy_subject(subject);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::Node root("root");
    auto subject = _generate_dummy_subject(&root);
    auto clone = subject->clone(&root);
    _compare_dummy_subject(clone);
    subject->setProperty("leftAnkleWidth", 80.0);
    subject->setProperty("rightFootFlat", false);
    subject->setDescription("");
    _compare_dummy_subject(clone);
  };
  
  CXXTEST_TEST(copy)
  {
    ma::Node root("root");
    auto subject = _generate_dummy_subject(&root);
    ma::Subject myCopy("myCopy");
    myCopy.copy(subject);
    _compare_dummy_subject(&myCopy);
    subject->setProperty("leftAnkleWidth", 80.0);
    subject->setProperty("rightFootFlat", false);
    subject->setDescription("");
    _compare_dummy_subject(&myCopy);
  }
};

CXXTEST_SUITE_REGISTRATION(SubjectTest)
CXXTEST_TEST_REGISTRATION(SubjectTest, properties)
CXXTEST_TEST_REGISTRATION(SubjectTest, clone)
CXXTEST_TEST_REGISTRATION(SubjectTest, copy)