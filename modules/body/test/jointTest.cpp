#include <cxxtest/TestDrive.h>

#include <openma/body/joint.h>
#include <openma/body/segment.h>

CXXTEST_SUITE(JointTest)
{
  CXXTEST_TEST(constructor)
  {
    ma::Node root("root");
    ma::body::Segment prox("prox", 0, 0, &root);
    ma::body::Segment dist("dist", 0, 0, &root);
    ma::body::Joint joint("test", &prox, &dist, &root);
    
    TS_ASSERT_EQUALS(joint.name(), std::string("test"));
    TS_ASSERT_EQUALS(prox.parents().size(),2ul);
    TS_ASSERT_EQUALS(dist.parents().size(),2ul);
    TS_ASSERT_EQUALS(joint.proximalSegment(),&prox);
    TS_ASSERT_EQUALS(joint.distalSegment(),&dist);
  }
  
  CXXTEST_TEST(mutator)
  {
    ma::Node root("root");
    ma::body::Segment prox("prox", 0, 0, &root);
    ma::body::Segment dist("dist", 0, 0, &root);
    ma::body::Joint joint("test", nullptr, nullptr, &root);
    
    TS_ASSERT_EQUALS(joint.proximalSegment(),nullptr);
    TS_ASSERT_EQUALS(joint.distalSegment(),nullptr);
    
    joint.setProximalSegment(&prox);
    joint.setDistalSegment(&dist);
    
    TS_ASSERT_EQUALS(prox.parents().size(),2ul);
    TS_ASSERT_EQUALS(dist.parents().size(),2ul);
    TS_ASSERT_EQUALS(joint.proximalSegment(),&prox);
    TS_ASSERT_EQUALS(joint.distalSegment(),&dist);
  }
  
  CXXTEST_TEST(setNullSegments)
  {
    TS_WARN("TO FINALIZE FIRST!"); return;
    ma::body::Segment* prox = new ma::body::Segment("prox", 0, 0);
    ma::body::Segment* dist = new ma::body::Segment("dist", 0, 0);
    ma::body::Joint joint("test", prox, dist);
    joint.setProximalSegment(nullptr);
    TS_ASSERT_EQUALS(joint.proximalSegment(), nullptr);
    joint.setDistalSegment(nullptr);
    TS_ASSERT_EQUALS(joint.distalSegment(), nullptr);
  };
  
  CXXTEST_TEST(deleteSegment)
  {
    TS_WARN("TO FINALIZE FIRST!"); return;
    ma::body::Segment* prox = new ma::body::Segment("prox", 0, 0);
    ma::body::Joint joint("test", prox, nullptr);
    delete prox;
    TS_ASSERT_EQUALS(joint.proximalSegment(), nullptr);
  }
  
  CXXTEST_TEST(clone)
  {
    ma::body::Segment* prox = new ma::body::Segment("prox", 0, 0);
    ma::body::Segment* dist = new ma::body::Segment("dist", 0, 0);
    ma::body::Joint joint("test", prox, dist);
    auto clone = joint.clone();
    TS_ASSERT_EQUALS(prox->parents().size(),1ul);
    TS_ASSERT_EQUALS(dist->parents().size(),1ul);
    TS_ASSERT_EQUALS(joint.proximalSegment(),prox);
    TS_ASSERT_EQUALS(joint.distalSegment(),dist);
    TS_ASSERT_EQUALS(clone->name(),"test");
    TS_ASSERT_EQUALS(clone->proximalSegment()->name(),prox->name());
    TS_ASSERT_EQUALS(clone->distalSegment()->name(),dist->name());
    TS_ASSERT_DIFFERS(clone->proximalSegment(),prox);
    TS_ASSERT_DIFFERS(clone->distalSegment(),dist);
  };
  
  CXXTEST_TEST(copy)
  {
    ma::body::Segment* prox = new ma::body::Segment("prox", 0, 0);
    ma::body::Segment* dist = new ma::body::Segment("dist", 0, 0);
    ma::body::Joint joint1("joint1", prox, dist);
    ma::body::Joint joint2("joint2");
    joint2.copy(&joint1);
    TS_ASSERT_EQUALS(prox->parents().size(),1ul);
    TS_ASSERT_EQUALS(dist->parents().size(),1ul);
    TS_ASSERT_EQUALS(joint1.proximalSegment(),prox);
    TS_ASSERT_EQUALS(joint1.distalSegment(),dist);
    TS_ASSERT_EQUALS(joint2.name(),"joint1");
    TS_ASSERT_EQUALS(joint2.proximalSegment()->name(),prox->name());
    TS_ASSERT_EQUALS(joint2.distalSegment()->name(),dist->name());
    TS_ASSERT_DIFFERS(joint2.proximalSegment(),prox);
    TS_ASSERT_DIFFERS(joint2.distalSegment(),dist);
  };
};

CXXTEST_SUITE_REGISTRATION(JointTest)
CXXTEST_TEST_REGISTRATION(JointTest, constructor)
CXXTEST_TEST_REGISTRATION(JointTest, mutator)
CXXTEST_TEST_REGISTRATION(JointTest, setNullSegments)
CXXTEST_TEST_REGISTRATION(JointTest, deleteSegment)
CXXTEST_TEST_REGISTRATION(JointTest, clone)
CXXTEST_TEST_REGISTRATION(JointTest, copy)
