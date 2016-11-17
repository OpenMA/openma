#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "plugingaitTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(PluginGaitCalibrationTest)
{
  CXXTEST_TEST(calibrateLeftFullBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Left, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(16.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightLegLength(940.0); // mm (Because both leg length are needed)
    
    ma::Node root("root");
    generate_static_trial_oneframe(&root);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 228.693, 5e-4);
    TS_ASSERT_DELTA(helper.property("interAsisDistance").cast<double>(), 228.693, 5e-4);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.property("leftAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.headOffset(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(helper.property("headOffset").cast<double>(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(helper.property("leftStaticPlantarFlexionOffset").cast<double>(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.0164273, 1e-4);
    TS_ASSERT_DELTA(helper.property("leftStaticRotationOffset").cast<double>(), 0.0164273, 1e-4);
  };
  
  CXXTEST_TEST(calibrateRightFullBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Right, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(16.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(940.0); // mm
    helper.setRightKneeWidth(120.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    helper.setLeftLegLength(940.0); // mm (Because both leg length are needed)
    
    ma::Node root("root");
    generate_static_trial_oneframe(&root);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 228.693, 5e-4);
    TS_ASSERT_DELTA(helper.property("interAsisDistance").cast<double>(), 228.693, 5e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.property("rightAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.headOffset(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(helper.property("headOffset").cast<double>(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(helper.property("rightStaticPlantarFlexionOffset").cast<double>(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0179406, 1e-4);
    TS_ASSERT_DELTA(helper.property("rightStaticRotationOffset").cast<double>(), -0.0179406, 1e-4);
  };
  
  CXXTEST_TEST(calibrateBothFullBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(16.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(940.0); // mm
    helper.setRightKneeWidth(120.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node root("root");
    generate_static_trial_oneframe(&root);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 228.693, 5e-4);
    TS_ASSERT_DELTA(helper.property("interAsisDistance").cast<double>(), 228.693, 5e-4);
    TS_ASSERT_DELTA(helper.headOffset(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(helper.property("headOffset").cast<double>(), 0.0945073, 1e-4);
    
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.property("leftAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(helper.property("leftStaticPlantarFlexionOffset").cast<double>(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.0164273, 1e-4);
    TS_ASSERT_DELTA(helper.property("leftStaticRotationOffset").cast<double>(), 0.0164273, 1e-4);
    
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.property("rightAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(helper.property("rightStaticPlantarFlexionOffset").cast<double>(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0179406, 1e-4);
    TS_ASSERT_DELTA(helper.property("rightStaticRotationOffset").cast<double>(), -0.0179406, 1e-4);
  };
  
  CXXTEST_TEST(calibrateBothUpperBodyFullFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setHeadOffsetEnabled(true);
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Full.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_EQUALS(helper.interAsisDistance(), 0.0);
    TS_ASSERT_DELTA(helper.headOffset(), 0.353677, 1e-4);
    TS_ASSERT_EQUALS(helper.leftAsisTrochanterAPDistance(), 0.0);
    TS_ASSERT_EQUALS(helper.leftStaticPlantarFlexionOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.leftStaticRotationOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.rightAsisTrochanterAPDistance(), 0.0);
    TS_ASSERT_EQUALS(helper.rightStaticPlantarFlexionOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.rightStaticRotationOffset(), 0.0);
  };

  CXXTEST_TEST(calibrateBothLowerBodyFullFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setHeadOffsetEnabled(true);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(800.0); // mm
    helper.setLeftKneeWidth(105.0); // mm
    helper.setLeftAnkleWidth(55.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(800.0); // mm
    helper.setRightKneeWidth(105.0); // mm
    helper.setRightAnkleWidth(55.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Full.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 199.902, 5e-4);
    TS_ASSERT_EQUALS(helper.headOffset(), 0.0);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.115969, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.00446829, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.162755, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0163829, 1e-4);
  };

  CXXTEST_TEST(calibrateBothFullBodyFullFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setHeadOffsetEnabled(true);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(800.0); // mm
    helper.setLeftKneeWidth(105.0); // mm
    helper.setLeftAnkleWidth(55.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(800.0); // mm
    helper.setRightKneeWidth(105.0); // mm
    helper.setRightAnkleWidth(55.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Full.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 199.902, 5e-4);
    TS_ASSERT_DELTA(helper.headOffset(), 0.353677, 1e-4);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.115969, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.00446829, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.162755, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0163829, 1e-4);
  };
  
  CXXTEST_TEST(calibrateBothFullBodyHoleFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setHeadOffsetEnabled(true);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(940.0); // mm
    helper.setRightKneeWidth(120.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Hole.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 227.043, 5e-4);
    TS_ASSERT_DELTA(helper.headOffset(), 0.123762, 1e-4);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.197419, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.0178294, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.209483, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0183813, 1e-4);
  };
  
  CXXTEST_TEST(calibrate2BothFullBodyNoOption)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(14.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setInterAsisDistance(255.0); // mm
    helper.setLeftLegLength(905.0); // mm
    helper.setLeftKneeWidth(105.0); // mm
    helper.setLeftAnkleWidth(75.0); // mm
    helper.setRightLegLength(905.0); // mm
    helper.setRightKneeWidth(105.0); // mm
    helper.setRightAnkleWidth(75.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration2-NoOption.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);

    TS_ASSERT_DELTA(helper.interAsisDistance(), 255.0, 1e-15);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 68.004, 5e-3);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.202148, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.0270116, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 68.004, 5e-3);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.233063, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), 0.00807751, 1e-4);
  };
  
  CXXTEST_TEST(calibrate3BothLowerBodyFF)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(14.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(920.0); // mm
    helper.setRightKneeWidth(102.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_FF.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);

    TS_ASSERT_DELTA(helper.interAsisDistance(), 245.244, 5e-3);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 69.936, 5e-3);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.15719, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), -0.0888886, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 69.936, 5e-3);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.186005, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0166437, 1e-4);
  };
  
  CXXTEST_TEST(calibrate3BothLowerBodyFF_N18)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(14.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(920.0); // mm
    helper.setRightKneeWidth(102.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_FF_N18.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);

    TS_ASSERT_DELTA(helper.interAsisDistance(), 245.32, 5e-3);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.157482, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), -0.0890265, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.18605, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.0165357, 1e-4);
  };
  
  CXXTEST_TEST(calibrate3BothLowerBodynoFF)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightLegLength(920.0); // mm
    helper.setRightKneeWidth(102.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_noFF.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);

    // TS_ASSERT_DELTA(helper.interAsisDistance(), 245.236, 5e-3); // Should be 245.244 like Calibrate3BothLowerBodyFF but Nexus computed a length of 245.236...
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 69.936, 5e-3);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.146773, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), -0.0883149, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 69.936, 5e-3);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.128716, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), -0.00978336, 1e-4);
  };
  
  CXXTEST_TEST(calibrate2BothUpperBodyHeadOffsetDisabled)
  {
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both, ma::body::PluginGait::Basic);
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration4.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_EQUALS(helper.interAsisDistance(), 0.0);
    TS_ASSERT_EQUALS(helper.headOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.leftAsisTrochanterAPDistance(), 0.0);
    TS_ASSERT_EQUALS(helper.leftStaticPlantarFlexionOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.leftStaticRotationOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.rightAsisTrochanterAPDistance(), 0.0);
    TS_ASSERT_EQUALS(helper.rightStaticPlantarFlexionOffset(), 0.0);
    TS_ASSERT_EQUALS(helper.rightStaticRotationOffset(), 0.0);
  };
  
  CXXTEST_TEST(calibrateFullBodyFrameKAD)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KAD);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(860.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(75.3); // mm
    helper.setRightLegLength(865.0); // mm
    helper.setRightKneeWidth(103.4); // mm
    helper.setRightAnkleWidth(72.9); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKad_Calibration.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 211.162, 1e-3);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 62.208, 1e-3);
    TS_ASSERT_DELTA(helper.leftThighRotationOffset(), 8.9585 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.leftShankRotationOffset(), 13.8726 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 4.72487 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.233667 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 62.852, 1e-3);
    TS_ASSERT_DELTA(helper.rightThighRotationOffset(), -10.0483 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.rightShankRotationOffset(), 15.3639 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 3.88629 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), 2.22081 * M_PI / 180.0, 1e-5);
  };
  
  CXXTEST_TEST(calibrateFullBodyFrameKADFF)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KAD);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(860.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(75.3); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setRightLegLength(865.0); // mm
    helper.setRightKneeWidth(103.4); // mm
    helper.setRightAnkleWidth(72.9); // mm
    helper.setRightFootFlatEnabled(true);
    
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKad_Calibration_FootFlat.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 211.162, 1e-3);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 62.208, 1e-3);
    TS_ASSERT_DELTA(helper.leftThighRotationOffset(), 0.156355, 1e-5);
    TS_ASSERT_DELTA(helper.leftShankRotationOffset(), 0.242123, 1e-5);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.0629524, 1e-5);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.00504156, 1e-5);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 62.852, 1e-5);
    TS_ASSERT_DELTA(helper.rightThighRotationOffset(), -0.175377, 1e-5);
    TS_ASSERT_DELTA(helper.rightShankRotationOffset(), 0.268151, 1e-5);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.12429, 1e-5);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), 0.0345916, 1e-5);
  };
  
  CXXTEST_TEST(calibrateFullBodyFrameKADMed)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KADMed);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(860.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(75.3); // mm
    helper.setRightLegLength(865.0); // mm
    helper.setRightKneeWidth(103.4); // mm
    helper.setRightAnkleWidth(72.9); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKadMed_Calibration.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 211.162, 1e-3);
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 62.208, 1e-3);
    TS_ASSERT_DELTA(helper.leftThighRotationOffset(), 8.9585 * M_PI / 180.0, 1e-4);
    TS_ASSERT_DELTA(helper.leftShankRotationOffset(), 3.89674 * M_PI / 180.0, 1.1e-4);
    TS_ASSERT_DELTA(helper.leftTibialTorsionOffset(), -12.0031 * M_PI / 180.0, 1e-4);
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 9.75336 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), -0.406183 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.leftAnkleAbAddOffset(), 11.5186 * M_PI / 180.0, 1e-4);
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 62.852, 1e-3);
    TS_ASSERT_DELTA(helper.rightThighRotationOffset(), -10.0483 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.rightShankRotationOffset(), 0.5526 * M_PI / 180.0 , 1.1e-4);
    TS_ASSERT_DELTA(helper.rightTibialTorsionOffset(), -17.7351 * M_PI / 180.0, 1e-4);
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 7.86442 * M_PI / 180.0, 1e-5);
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), 3.1846 * M_PI / 180.0, 1e-4);
    TS_ASSERT_DELTA(helper.rightAnkleAbAddOffset(), 8.70843 * M_PI / 180.0, 1e-5);
  };
  
  CXXTEST_TEST(calibrateFullBodyFrameKADMed2)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KADMed);
    helper.setMarkerDiameter(25.0); // mm
    helper.setLeftLegLength(775.0); // mm
    helper.setLeftKneeWidth(105.1); // mm
    helper.setLeftAnkleWidth(68.4); // mm
    helper.setRightLegLength(770.0); // mm
    helper.setRightKneeWidth(107.0); // mm
    helper.setRightAnkleWidth(68.6); // mm
    
    ma::Node root("root");
    generate_trial_from_file(&root, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKadMed_Calibration2.c3d"));
    TS_ASSERT_EQUALS(root.children().size(),1u);
    helper.calibrate(&root, nullptr);
    
    TS_ASSERT_DELTA(helper.interAsisDistance(), 213.74600219726562, 1e-3)
    TS_ASSERT_DELTA(helper.leftAsisTrochanterAPDistance(), 51.259998321533203, 1e-3)
    TS_ASSERT_DELTA(helper.leftThighRotationOffset(), -0.19650661945343018, 1e-4)
    TS_ASSERT_DELTA(helper.leftShankRotationOffset(), 0.036574419587850571, 1e-4)
    TS_ASSERT_DELTA(helper.leftTibialTorsionOffset(), 0.18572920560836792, 1e-4)
    TS_ASSERT_DELTA(helper.leftStaticPlantarFlexionOffset(), 0.082310251891613007, 1e-4)
    TS_ASSERT_DELTA(helper.leftStaticRotationOffset(), 0.08043140172958374, 1e-4)
    TS_ASSERT_DELTA(helper.leftAnkleAbAddOffset(), 0.13921844959259033, 1e-4)
    TS_ASSERT_DELTA(helper.rightAsisTrochanterAPDistance(), 50.616001129150391, 1e-3)
    TS_ASSERT_DELTA(helper.rightThighRotationOffset(), 0.230163544416427610, 1e-4)
    TS_ASSERT_DELTA(helper.rightShankRotationOffset(), -0.11728926748037338, 1e-4)
    TS_ASSERT_DELTA(helper.rightTibialTorsionOffset(), -0.34318757057189941, 1e-4)
    TS_ASSERT_DELTA(helper.rightStaticPlantarFlexionOffset(), 0.032737139612436295, 1e-4)
    TS_ASSERT_DELTA(helper.rightStaticRotationOffset(), 0.28456372022628784, 1e-4)
    TS_ASSERT_DELTA(helper.rightAnkleAbAddOffset(), 0.13163727521896362, 1e-4)
  };
};

CXXTEST_SUITE_REGISTRATION(PluginGaitCalibrationTest)  
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateLeftFullBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateRightFullBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateBothFullBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateBothUpperBodyFullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateBothLowerBodyFullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateBothFullBodyFullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateBothFullBodyHoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrate2BothFullBodyNoOption)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrate3BothLowerBodyFF)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrate3BothLowerBodyFF_N18)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrate3BothLowerBodynoFF)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrate2BothUpperBodyHeadOffsetDisabled)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateFullBodyFrameKAD)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateFullBodyFrameKADFF)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateFullBodyFrameKADMed)
CXXTEST_TEST_REGISTRATION(PluginGaitCalibrationTest, calibrateFullBodyFrameKADMed2)