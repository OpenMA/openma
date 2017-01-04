#include <cxxtest/TestDrive.h>

#include <openma/body/landmarksregistrar.h>
#include <openma/body/landmarkstranslator.h>
#include <openma/base/timesequence.h>

CXXTEST_SUITE(LandmarksRegistrarTest)
{
  CXXTEST_TEST(getset)
  {
    std::vector<std::string> labels{"pt1","pt2","pt3","pt4"};
    ma::body::LandmarksRegistrar reg("reg",labels);
    TS_ASSERT_EQUALS(reg.labels(), labels);
    labels.push_back("pt5");
    auto ts = reg.timestamp();
    reg.setLabels(labels);
    TS_ASSERT_DIFFERS(reg.timestamp(), ts);
    TS_ASSERT_EQUALS(reg.labels(), labels);
  };
  
  CXXTEST_TEST(retrieveLandmarks)
  {
    ma::body::LandmarksRegistrar reg("reg",{"uname*1","uname*2","uname*3","pt4"});
    ma::Node root("root");
    auto tss = ma::make_nodes<ma::TimeSequence*>(4,4,0,100.0,0.0,ma::TimeSequence::Position,"mm",&root);
    auto lmks = reg.retrieveLandmarks(nullptr,&root);
    TS_ASSERT_EQUALS(lmks.size(),3u);
    TS_ASSERT_EQUALS(lmks[0],tss[0]);
    TS_ASSERT_EQUALS(lmks[1],tss[1]);
    TS_ASSERT_EQUALS(lmks[2],tss[2]);
  };
  
  CXXTEST_TEST(retrieveLandmarksBis)
  {
    ma::body::LandmarksRegistrar reg("reg",{"pt1","pt2","pt3","pt4"});
    ma::body::LandmarksTranslator lt("lt",{
      {"uname*1", "pt1"},
      {"uname*2", "pt2"},
      {"uname*3", "pt3"},
      {"C7"     , "C7" },
      {"uname*4", "pt4"}
    });
    ma::Node root("root");
    auto tss = ma::make_nodes<ma::TimeSequence*>(4,4,0,100.0,0.0,ma::TimeSequence::Position,"mm",&root);
    auto lmks = reg.retrieveLandmarks(&lt,&root);
    TS_ASSERT_EQUALS(lmks.size(),4u);
    TS_ASSERT_EQUALS(lmks[0],tss[0]);
    TS_ASSERT_EQUALS(lmks[1],tss[1]);
    TS_ASSERT_EQUALS(lmks[2],tss[2]);
    TS_ASSERT_EQUALS(lmks[3],tss[3]);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::Node root("root");
    std::vector<std::string> labels{"pt1","pt2","pt3","pt4"};
    ma::body::LandmarksRegistrar reg("reg",labels);
    auto another = static_cast<ma::body::LandmarksRegistrar*>(reg.clone(&root));
    TS_ASSERT_EQUALS(another->name(), std::string{"reg"});
    TS_ASSERT_EQUALS(another->labels(), labels);
    labels.push_back("pt5");
    another->setLabels(labels);
    TS_ASSERT_DIFFERS(reg.labels(), labels);
    TS_ASSERT_EQUALS(another->labels(), labels);
    TS_ASSERT_EQUALS(another->hasParents(), true);
    TS_ASSERT_EQUALS(root.hasChildren(), true);
  };
  
  CXXTEST_TEST(copy)
  {
    std::vector<std::string> labels{"pt1","pt2","pt3","pt4"};
    ma::body::LandmarksRegistrar reg("reg",labels);
    ma::body::LandmarksRegistrar another("foo",{});
    another.copy(&reg);
    TS_ASSERT_EQUALS(another.labels(), labels);
    labels.push_back("pt5");
    another.setLabels(labels);
    TS_ASSERT_DIFFERS(reg.labels(), labels);
    TS_ASSERT_EQUALS(another.labels(), labels);
    TS_ASSERT_EQUALS(another.hasParents(), false);
  };
};

CXXTEST_SUITE_REGISTRATION(LandmarksRegistrarTest)
CXXTEST_TEST_REGISTRATION(LandmarksRegistrarTest, getset)
CXXTEST_TEST_REGISTRATION(LandmarksRegistrarTest, retrieveLandmarks)
CXXTEST_TEST_REGISTRATION(LandmarksRegistrarTest, retrieveLandmarksBis)
CXXTEST_TEST_REGISTRATION(LandmarksRegistrarTest, clone)
CXXTEST_TEST_REGISTRATION(LandmarksRegistrarTest, copy)