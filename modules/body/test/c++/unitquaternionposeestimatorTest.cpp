#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "registermarkerclusterTest_def.h"
#include "test_file_path.h"

#include <openma/io.h>

CXXTEST_SUITE(UnitQuaternionPoseEstimatorTest)
{
  CXXTEST_TEST(cube)
  {
    ma::Node root("root"), models("models");
    ma::Trial trial("trial",&root);
    auto tss = ma::make_nodes<ma::TimeSequence*>(8,4,1,100.0,0.0,ma::TimeSequence::Marker,"mm",trial.timeSequences());
    set_pt_data(tss[0], 10.0, 10.0, 10.0);
    set_pt_data(tss[1], 10.0, 20.0, 10.0);
    set_pt_data(tss[2], 20.0, 20.0, 10.0);
    set_pt_data(tss[3], 20.0, 10.0, 10.0);
    set_pt_data(tss[4], 10.0, 10.0,  0.0);
    set_pt_data(tss[5], 10.0, 20.0,  0.0);
    set_pt_data(tss[6], 20.0, 20.0,  0.0);
    set_pt_data(tss[7], 20.0, 10.0,  0.0);
    CubeHelper helper;
    TS_ASSERT_EQUALS(ma::body::register_marker_cluster(&helper,&root), true);
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models,&helper,&root), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    auto cube_tcs = models.findChild<ma::TimeSequence*>("Cube.TCS");
    TS_ASSERT_DIFFERS(cube_tcs, nullptr);
    TS_ASSERT_DELTA(cube_tcs->data()[0],  1.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[1],  0.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2],  0.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[3],  0.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[4],  1.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[5],  0.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[6],  0.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[7],  0.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[8],  1.0,  1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[9],  15.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[10], 15.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[11], 5.0,  1e-15);
    auto cube_scs = models.findChild<ma::TimeSequence*>("Cube.SCS");
    TS_ASSERT_DIFFERS(cube_scs, nullptr);
    TS_ASSERT_DELTA(cube_scs->data()[0],  1.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[2],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[3],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[4],  1.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[5],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[6],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[7],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[8],  1.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[9],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[10], 0.0, 1e-15);
    TS_ASSERT_DELTA(cube_scs->data()[11], 0.0, 1e-15);
  };
  
  CXXTEST_TEST(cubeRotated)
  {
    ma::Node rootCalibration("rootCalibration");
    ma::Trial trialCalibration("trialCalibration",&rootCalibration);
    auto tsscal = ma::make_nodes<ma::TimeSequence*>(8,4,1,100.0,0.0,ma::TimeSequence::Marker,"mm",trialCalibration.timeSequences());
    set_pt_data(tsscal[0], 10.0, 10.0, 10.0);
    set_pt_data(tsscal[1], 10.0, 20.0, 10.0);
    set_pt_data(tsscal[2], 20.0, 20.0, 10.0);
    set_pt_data(tsscal[3], 20.0, 10.0, 10.0);
    set_pt_data(tsscal[4], 10.0, 10.0,  0.0);
    set_pt_data(tsscal[5], 10.0, 20.0,  0.0);
    set_pt_data(tsscal[6], 20.0, 20.0,  0.0);
    set_pt_data(tsscal[7], 20.0, 10.0,  0.0);
    ma::Node rootDynamic("rootDynamic");
    ma::Trial trialDynamic("trialDynamic",&rootDynamic);
    auto tssdyn = ma::make_nodes<ma::TimeSequence*>(8,4,2,100.0,0.0,ma::TimeSequence::Marker,"mm",trialDynamic.timeSequences());
    // Frame #0 - Rotated by 180 degrees around axis X
    set_pt_data(tssdyn[5], 10.0, 10.0, 10.0, 0);
    set_pt_data(tssdyn[4], 10.0, 20.0, 10.0, 0);
    set_pt_data(tssdyn[7], 20.0, 20.0, 10.0, 0);
    set_pt_data(tssdyn[6], 20.0, 10.0, 10.0, 0);
    set_pt_data(tssdyn[1], 10.0, 10.0,  0.0, 0);
    set_pt_data(tssdyn[0], 10.0, 20.0,  0.0, 0);
    set_pt_data(tssdyn[3], 20.0, 20.0,  0.0, 0);
    set_pt_data(tssdyn[2], 20.0, 10.0,  0.0, 0);
    // Frame #1 - Rotated by 90 degrees around axis Z
    set_pt_data(tssdyn[1], 10.0, 10.0, 10.0, 1);
    set_pt_data(tssdyn[2], 10.0, 20.0, 10.0, 1);
    set_pt_data(tssdyn[3], 20.0, 20.0, 10.0, 1);
    set_pt_data(tssdyn[0], 20.0, 10.0, 10.0, 1);
    set_pt_data(tssdyn[5], 10.0, 10.0,  0.0, 1);
    set_pt_data(tssdyn[6], 10.0, 20.0,  0.0, 1);
    set_pt_data(tssdyn[7], 20.0, 20.0,  0.0, 1);
    set_pt_data(tssdyn[4], 20.0, 10.0,  0.0, 1);
    CubeHelper helper;
    TS_ASSERT_EQUALS(ma::body::register_marker_cluster(&helper,&rootCalibration), true);
    ma::Node models("models");
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models,&helper,&rootDynamic), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    auto cube_tcs = models.findChild<ma::TimeSequence*>("Cube.TCS");
    TS_ASSERT_DIFFERS(cube_tcs, nullptr);
    TS_ASSERT_DELTA(cube_tcs->data()[2*0 +0],  1.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*1 +0],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*2 +0],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*3 +0],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*4 +0], -1.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*5 +0],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*6 +0],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*7 +0],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*8 +0], -1.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*9 +0], 15.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*10+0], 15.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*11+0],  5.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*0 +1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*1 +1],  1.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*2 +1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*3 +1], -1.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*4 +1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*5 +1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*6 +1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*7 +1],  0.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*8 +1],  1.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*9 +1], 15.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*10+1], 15.0, 1e-15);
    TS_ASSERT_DELTA(cube_tcs->data()[2*11+1],  5.0, 1e-15);
  };
  
  CXXTEST_TEST(lowerlimbs)
  {
    ma::Node staticTrials("staticTrials"), dynamicTrials("dynamicTrials");
    TS_ASSERT_EQUALS(ma::io::read(&staticTrials, OPENMA_TDD_PATH_IN("c3d/other/FunctionalStatic.c3d")), true);
    TS_ASSERT_EQUALS(staticTrials.children().size(), 1u);
    LowerLimbsHelper helper;
    TS_ASSERT_EQUALS(ma::body::register_marker_cluster(&helper,&staticTrials), true);
    TS_ASSERT_EQUALS(ma::io::read(&dynamicTrials, OPENMA_TDD_PATH_IN("c3d/other/FunctionalWalk.c3d")), true);
    TS_ASSERT_EQUALS(dynamicTrials.children().size(), 1u);
    ma::Node models("models");
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models,&helper,&dynamicTrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    auto L_Thigh_TCS = models.findChild<ma::TimeSequence*>("L.Thigh.TCS");
    TS_ASSERT_DIFFERS(L_Thigh_TCS, nullptr);
    // Sample #0
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*0 +0],     0.980095544188540, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*1 +0],    -0.129106037679063, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*2 +0],     0.150812318119510, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*3 +0],     0.128295956966800, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*4 +0],     0.991620496483664, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*5 +0],     0.015130709813622, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*6 +0],    -0.151502051760830, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*7 +0],     0.004519069406784, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*8 +0],     0.988446612783895, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*9 +0], -1639.759033203125200, 1e-12);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*10+0],   647.411523437500020, 1e-12);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*11+0],   637.549755859374950, 1e-12);
    // Sample #100
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*0 +100],    0.958685446761432, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*1 +100],    0.134359942772854, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*2 +100],    0.250738150160497, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*3 +100],   -0.134035253096809, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*4 +100],    0.990804746088446, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*5 +100],   -0.018452806179111, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*6 +100],   -0.250911867186682, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*7 +100],   -0.015917314681965, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*8 +100],    0.967879059592782, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*9 +100], -661.608996582031410, 1e-12);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*10+100],  647.678857421875020, 1e-12);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*11+100],  675.265576171874950, 1e-12);
    // Sample #200
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*0 +200],   0.911726183125601, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*1 +200],   0.228908499917774, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*2 +200],  -0.341110342365367, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*3 +200],  -0.231505256983725, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*4 +200],   0.972250627763754, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*5 +200],   0.033675403514822, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*6 +200],   0.339353330604141, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*7 +200],   0.048266090357318, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*8 +200],   0.939419874991740, 1e-14);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*9 +200], 179.500515747070210, 1e-12);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*10+200], 674.303796386718770, 1e-12);
    TS_ASSERT_DELTA(L_Thigh_TCS->data()[438*11+200], 617.405200195312660, 1e-12);
  };
};

CXXTEST_SUITE_REGISTRATION(UnitQuaternionPoseEstimatorTest)
CXXTEST_TEST_REGISTRATION(UnitQuaternionPoseEstimatorTest, cube)
CXXTEST_TEST_REGISTRATION(UnitQuaternionPoseEstimatorTest, cubeRotated)
CXXTEST_TEST_REGISTRATION(UnitQuaternionPoseEstimatorTest, lowerlimbs)
