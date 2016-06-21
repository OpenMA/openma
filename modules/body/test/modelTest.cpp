#include <cxxtest/TestDrive.h>

#include <openma/body/model.h>
#include <openma/body/segment.h>
#include <openma/body/joint.h>

CXXTEST_SUITE(ModelTest)
{
  CXXTEST_TEST(joints)
  {
    ma::body::Model model("model");
    TS_ASSERT_EQUALS(model.children().size(), 0ul);
    TS_ASSERT_DIFFERS(model.joints(), nullptr);
    TS_ASSERT_EQUALS(model.children().size(), 1ul);
    TS_ASSERT_DIFFERS(model.findChild("Joints"), nullptr);
    TS_ASSERT_EQUALS(model.joints(), model.findChild("Joints"));
  };
  
  CXXTEST_TEST(joint)
  {
    ma::body::Model model("model");
    auto jnt1 = new ma::body::Joint("jnt1",nullptr,nullptr);
    jnt1->addParent(model.joints());
    auto jnt2 = new ma::body::Joint("jnt2",nullptr,nullptr);
    jnt2->addParent(model.joints());
    TS_ASSERT_EQUALS(model.joint(0), jnt1);
    TS_ASSERT_EQUALS(model.joint(1), jnt2);
    TS_ASSERT_EQUALS(model.joint(2), nullptr);
  };
  
  CXXTEST_TEST(segments)
  {
    ma::body::Model model("model");
    TS_ASSERT_EQUALS(model.children().size(), 0ul);
    TS_ASSERT_DIFFERS(model.segments(), nullptr);
    TS_ASSERT_EQUALS(model.children().size(), 1ul);
    TS_ASSERT_DIFFERS(model.findChild("Segments"), nullptr);
    TS_ASSERT_EQUALS(model.segments(), model.findChild("Segments"));
  };
  
  CXXTEST_TEST(segment)
  {
    ma::body::Model model("model");
    auto seg1 = new ma::body::Segment("seg1"); seg1->addParent(model.segments());
    auto seg2 = new ma::body::Segment("seg2"); seg2->addParent(model.segments());
    TS_ASSERT_EQUALS(model.segment(0), seg1);
    TS_ASSERT_EQUALS(model.segment(1), seg2);
    TS_ASSERT_EQUALS(model.segment(2), nullptr);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::body::Model model("model");
    auto node1 = new ma::Node("leaf", model.segments());
    auto node2 = new ma::Node("leaf2", model.joints());
    auto clone = model.clone();
    TS_ASSERT_EQUALS(clone->children().size(), 2ul);
    TS_ASSERT_DIFFERS(clone->segments(), model.segments());
    TS_ASSERT_DIFFERS(clone->joints(), model.joints());
    TS_ASSERT_DIFFERS(clone->findChild("Segments"), nullptr);
    TS_ASSERT_EQUALS(clone->segments(), clone->findChild("Segments"));
    TS_ASSERT_DIFFERS(clone->findChild("Joints"), nullptr);
    TS_ASSERT_EQUALS(clone->joints(), clone->findChild("Joints"));
    TS_ASSERT_DIFFERS(clone->segments()->child(0), node1);
    TS_ASSERT_DIFFERS(clone->joints()->child(0), node2);
    TS_ASSERT_EQUALS(clone->segments()->child(0)->name(), node1->name());
    TS_ASSERT_EQUALS(clone->joints()->child(0)->name(), node2->name());
    TS_ASSERT_EQUALS(clone->segment(0), nullptr);
    TS_ASSERT_EQUALS(clone->joint(0), nullptr);
    delete clone;
  };
  
  CXXTEST_TEST(copy)
  {
    ma::body::Model model1("model1");
    ma::body::Model model2("model2");
    new ma::Node("leaf", model2.segments());
    new ma::Node("leaf2", model2.joints());
    model2.copy(&model1);
    TS_ASSERT_EQUALS(model2.children().size(), 0ul);
    TS_ASSERT_EQUALS(model2.name(), model1.name());
  };
  
  CXXTEST_TEST(modelJointSegmentAndClone)
  {
    TS_WARN("VERIFY THAT CLONED SEGMENT IN A JOINT ARE THE SAME THAN IN THE MODEL");
  }
};

CXXTEST_SUITE_REGISTRATION(ModelTest)
CXXTEST_TEST_REGISTRATION(ModelTest, joints)
CXXTEST_TEST_REGISTRATION(ModelTest, joint)
CXXTEST_TEST_REGISTRATION(ModelTest, segments)
CXXTEST_TEST_REGISTRATION(ModelTest, segment)
CXXTEST_TEST_REGISTRATION(ModelTest, clone)
CXXTEST_TEST_REGISTRATION(ModelTest, copy)
CXXTEST_TEST_REGISTRATION(ModelTest, modelJointSegmentAndClone)
