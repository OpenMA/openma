#include <cxxtest/TestDrive.h>

#include <openma/body/landmarkstranslator.h>
#include <openma/body/referenceframe.h>
#include <openma/body/point.h>
#include <openma/body/segment.h>
#include <openma/body/utils.h>

CXXTEST_SUITE(UtilsTest)
{
  CXXTEST_TEST(transformRelative)
  {
    ma::Node root("root");
    ma::body::Segment seg("seg",0,0,&root);
    // Position from seg
    double coords[3] = {10.,10.,10.};
    ma::body::Point foo("foo",coords,&seg);
    // Frame from seg
    double transfo[12] = {-1.,  0.,  0.,   // u
                           0., -1.,  0.,   // v
                           0.,  0.,  1.,   // w
                           5.,  10., 15.}; // o
    ma::body::ReferenceFrame bar("bar",transfo,&seg);
    // Position from "bar"
    coords[0] = 0.; coords[1] = 0.; coords[2] = 0.;
    ma::body::Point toto("toto",coords,&bar);
    // Frame from "bar"
    transfo[0] = 0.; transfo[1] = 0.; transfo[2] = 1.;
    transfo[3] =-1.; transfo[4] = 0.; transfo[5] = 0.;
    transfo[6] = 0.; transfo[7] =-1.; transfo[8] = 0.;
    transfo[9] = 10.; transfo[10] =-43.2; transfo[11] = 100.19;
    ma::body::ReferenceFrame ice("ice",transfo,&bar);
    // Position from "ice"
    coords[0] = 1.; coords[1] = 2.; coords[2] = 3.;
    ma::body::Point milk("milk",coords,&ice);
    
    ma::math::Pose pose(3);
    pose.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                     0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                     1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    pose.residuals().setZero();
    
    auto _foo = ma::body::transform_relative_point(&foo, &seg, pose);
    TS_ASSERT_DELTA(_foo.values().coeff(0,0), 0., 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(0,1), -53.2, 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(0,2), 110.19, 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(1,0), 18.20466, 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(1,1), -36.90621, 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(1,2), 114.08503, 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(2,0), 10., 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(2,1), 10., 1e-15);
    TS_ASSERT_DELTA(_foo.values().coeff(2,2), 10., 1e-15);

    auto _toto = ma::body::transform_relative_point(&toto, &seg, pose);
    TS_ASSERT_DELTA(_toto.values().coeff(0,0), 0., 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(0,1), -58.2, 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(0,2), 105.19, 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(1,0),   21.12314, 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(1,1),  -43.24388, 5e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(1,2),  115.23241, 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(2,0),  5., 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(2,1), 10., 1e-15);
    TS_ASSERT_DELTA(_toto.values().coeff(2,2), 15., 1e-15);
    
    auto _milk = ma::body::transform_relative_point(&milk, &seg, pose);
    TS_ASSERT_DELTA(_milk.values().coeff(0,0), -46.200000000000003, 1e-15);
    TS_ASSERT_DELTA(_milk.values().coeff(0,1), -159.389999999999986, 1e-15);
    TS_ASSERT_DELTA(_milk.values().coeff(0,2), 97.189999999999998, 1e-15);
    TS_ASSERT_DELTA(_milk.values().coeff(1,0), 91.362521319999999, 1e-14);
    TS_ASSERT_DELTA(_milk.values().coeff(1,1), -83.119854409999988, 1e-14);
    TS_ASSERT_DELTA(_milk.values().coeff(1,2), 192.136294299999975, 5e-14);
    TS_ASSERT_DELTA(_milk.values().coeff(2,0), -3.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_milk.values().coeff(2,1), 56.200000000000003, 1e-15);
    TS_ASSERT_DELTA(_milk.values().coeff(2,2), 116.189999999999998, 1e-15);
    
    auto _bar = ma::body::transform_relative_frame(&bar, &seg, pose);
    TS_ASSERT_DELTA(_bar.values().coeff(0,0), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,1), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,2), -1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,3), 1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,4), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,5), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,6), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,7), -1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,8), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,9), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,10), -58.200000000000003, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(0,11), 105.189999999999998, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,0), -0.299252000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,1), -0.947355000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,2), -0.113874000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,3), 0.361734000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,4), -0.002203000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,5), -0.932279000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,6), 0.882948000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,7), -0.320179000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,8), 0.343350000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,9), 21.123139999999999, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,10), -43.243880000000004, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(1,11), 115.232410000000002, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,0), -1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,1), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,2), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,3), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,4), -1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,5), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,6), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,7), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,8), 1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,9), 5.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,10), 10.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_bar.values().coeff(2,11), 15.000000000000000, 1e-15);
    
    auto _ice = ma::body::transform_relative_frame(&ice, &seg, pose);
    TS_ASSERT_DELTA(_ice.values().coeff(0,0), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,1), -1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,2), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,3), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,4), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,5), 1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,6), -1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,7), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,8), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,9), -43.200000000000003, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,10), -158.389999999999986, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(0,11), 95.189999999999998, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,0), 0.882948000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,1), -0.320179000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,2), 0.343350000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,3), 0.299252000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,4), 0.947355000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,5), 0.113874000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,6), -0.361734000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,7), 0.002203000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,8), 0.932279000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,9), 90.966271320000004, 1e-14);
    TS_ASSERT_DELTA(_ice.values().coeff(1,10), -84.700994409999993, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(1,11), 188.768359299999986, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,0), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,1), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,2), 1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,3), 1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,4), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,5), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,6), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,7), 1.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,8), 0.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,9), -5.000000000000000, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,10), 53.200000000000003, 1e-15);
    TS_ASSERT_DELTA(_ice.values().coeff(2,11), 115.189999999999998, 1e-15);
  };
  
  CXXTEST_TEST(transformInvalid)
  {
    TS_WARN("What about the transformation of a relative frame using an invalid pose and then extract the position?");
  };
  
  CXXTEST_TEST(extractLandmarkPositions)
  {
    double rate = 0., start = 0.;
    bool ok = false;
    ma::body::LandmarksTranslator lt("lt",{
      {"uname*1", "pt1"},
      {"uname*2", "pt2"},
      {"uname*3", "pt3"},
      {"C7"     , "C7" },
      {"uname*4", "pt4"}
    });
    ma::Node root("root");
    auto tss = ma::make_nodes<ma::TimeSequence*>(4,4,1,100.0,0.0,ma::TimeSequence::Marker,"mm",&root);
    auto lmks = ma::body::extract_landmark_positions(&lt, tss, &rate, &start, &ok);
    TS_ASSERT_EQUALS(rate,100.0);
    TS_ASSERT_EQUALS(start,0.0);
    TS_ASSERT_EQUALS(ok,true);
    TS_ASSERT_EQUALS(lmks.size(),4u);
    lmks["pt1"].values().setRandom();
    lmks["pt2"].values().setRandom();
    lmks["pt3"].values().setRandom();
    lmks["pt4"].values().setRandom();
    TS_ASSERT_DELTA(lmks["pt1"].values().coeff(0),tss[0]->data()[0],1e-15);
    TS_ASSERT_DELTA(lmks["pt2"].values().coeff(0),tss[1]->data()[0],1e-15);
    TS_ASSERT_DELTA(lmks["pt3"].values().coeff(0),tss[2]->data()[0],1e-15);
    TS_ASSERT_DELTA(lmks["pt4"].values().coeff(0),tss[3]->data()[0],1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(UtilsTest)
CXXTEST_TEST_REGISTRATION(UtilsTest, transformRelative)
CXXTEST_TEST_REGISTRATION(UtilsTest, transformInvalid)
CXXTEST_TEST_REGISTRATION(UtilsTest, extractLandmarkPositions)