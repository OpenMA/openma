#include <cxxtest/TestDrive.h>

#include "plugingaitTest_def.h"
#include "tdd_file_path.h"

CXXTEST_SUITE(PluginGaitCalibrationTest)
{
  CXXTEST_TEST(calibrateLeftFullBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Left);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Right);
    helper.setMarkerDiameter(16.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlat(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both);
    
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlat(true);
    helper.setLeftLegLength(800.0); // mm
    helper.setLeftKneeWidth(105.0); // mm
    helper.setLeftAnkleWidth(55.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlat(true);
    helper.setLeftLegLength(800.0); // mm
    helper.setLeftKneeWidth(105.0); // mm
    helper.setLeftAnkleWidth(55.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlat(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftFootFlat(true);
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftFootFlat(true);
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlat(true);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
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