#include <cxxtest/TestDrive.h>

#include "plugingaitTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(PluginGaitReconstructionTest)
{
  CXXTEST_TEST(reconstructBothLowerBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(940.0); // mm
    helper.setRightKneeWidth(120.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-One.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-One.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
  
  CXXTEST_TEST(reconstructBothUpperBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftShoulderOffset(50.0); // mm
    helper.setRightShoulderOffset(50.0); // mm
    helper.setLeftElbowWidth(80.0); // mm
    helper.setRightElbowWidth(80.0); // mm
    helper.setLeftWristWidth(40.0); // mm
    helper.setRightWristWidth(40.0); // mm
    helper.setLeftHandThickness(30.0); // mm
    helper.setRightHandThickness(30.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-One.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-One.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Head.SCS", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "Torso.SCS", {"TRXO","TRXA","TRXL","TRXP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Clavicle.SCS", {"LCLO","LCLA","LCLL","LCLP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Clavicle.SCS", {"RCLO","RCLA","RCLL","RCLP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Arm.SCS", {"LHUO","LHUA","LHUL","LHUP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Arm.SCS", {"RHUO","RHUA","RHUL","RHUP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Forearm.SCS", {"LRAO","LRAA","LRAL","LRAP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Forearm.SCS", {"RRAO","RRAA","RRAL","RRAP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Hand.SCS", {"LHNO","LHNA","LHNL","LHNP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Hand.SCS", {"RHNO","RHNA","RHNL","RHNP"}, {5e-4});
  };
  
  CXXTEST_TEST(reconstructBothLowerBodyHoleFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(940.0); // mm
    helper.setRightKneeWidth(120.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Hole.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-Hole.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {75e-5});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {7e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
  
  CXXTEST_TEST(reconstructBothUpperBodyHoleFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftShoulderOffset(50.0); // mm
    helper.setRightShoulderOffset(50.0); // mm
    helper.setLeftElbowWidth(80.0); // mm
    helper.setRightElbowWidth(80.0); // mm
    helper.setLeftWristWidth(40.0); // mm
    helper.setRightWristWidth(40.0); // mm
    helper.setLeftHandThickness(30.0); // mm
    helper.setRightHandThickness(30.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Hole.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-Hole.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    // NOTE: The accuracy is not the same than for the other unit tests. The reason is not known
    compare_segment_motion(model, trial, "Head.SCS", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "Torso.SCS", {"TRXO","TRXA","TRXL","TRXP"}, {1e-3});
    compare_segment_motion(model, trial, "L.Clavicle.SCS", {"LCLO","LCLA","LCLL","LCLP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Clavicle.SCS", {"RCLO","RCLA","RCLL","RCLP"}, {1e-3});
    compare_segment_motion(model, trial, "L.Arm.SCS", {"LHUO","LHUA","LHUL","LHUP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Arm.SCS", {"RHUO","RHUA","RHUL","RHUP"}, {1e-3});
    compare_segment_motion(model, trial, "L.Forearm.SCS", {"LRAO","LRAA","LRAL","LRAP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Forearm.SCS", {"RRAO","RRAA","RRAL","RRAP"}, {1e-3});
    compare_segment_motion(model, trial, "L.Hand.SCS", {"LHNO","LHNA","LHNL","LHNP"}, {1e-3});
    compare_segment_motion(model, trial, "R.Hand.SCS", {"RHNO","RHNA","RHNL","RHNP"}, {15e-4});
  };
  
  CXXTEST_TEST(reconstructBothFullBodyFullFrames)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
    helper.setHeadOffsetEnabled(true);
    helper.setLeftShoulderOffset(50.0); // mm
    helper.setRightShoulderOffset(50.0); // mm
    helper.setLeftElbowWidth(80.0); // mm
    helper.setRightElbowWidth(80.0); // mm
    helper.setLeftWristWidth(40.0); // mm
    helper.setRightWristWidth(40.0); // mm
    helper.setLeftHandThickness(30.0); // mm
    helper.setRightHandThickness(30.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(800.0); // mm
    helper.setLeftKneeWidth(105.0); // mm
    helper.setLeftAnkleWidth(55.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(800.0); // mm
    helper.setRightKneeWidth(105.0); // mm
    helper.setRightAnkleWidth(55.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Full.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-Full.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    // NOTE: The accuracy is not the same than for the other unit tests. The reason is not known
    compare_segment_motion(model, trial, "Head.SCS", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "Torso.SCS", {"TRXO","TRXA","TRXL","TRXP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Clavicle.SCS", {"LCLO","LCLA","LCLL","LCLP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Clavicle.SCS", {"RCLO","RCLA","RCLL","RCLP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Arm.SCS", {"LHUO","LHUA","LHUL","LHUP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Arm.SCS", {"RHUO","RHUA","RHUL","RHUP"}, {6e-4});
    compare_segment_motion(model, trial, "L.Forearm.SCS", {"LRAO","LRAA","LRAL","LRAP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Forearm.SCS", {"RRAO","RRAA","RRAL","RRAP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Hand.SCS", {"LHNO","LHNA","LHNL","LHNP"}, {6e-4});
    compare_segment_motion(model, trial, "R.Hand.SCS", {"RHNO","RHNA","RHNL","RHNP"}, {5e-4});
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {6e-4});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
  
  CXXTEST_TEST(reconstruct3BothLowerBodyFF)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(920.0); // mm
    helper.setRightKneeWidth(102.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_FF.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion3_FF.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));

    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
  
  CXXTEST_TEST(reconstruct3BothLowerBodyFF_N18)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(920.0); // mm
    helper.setRightKneeWidth(102.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_FF_N18.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion3_FF_N18.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));

    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
  
  CXXTEST_TEST(reconstruct3BothLowerBodyNoFF)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(920.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightLegLength(920.0); // mm
    helper.setRightKneeWidth(102.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_noFF.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion3_noFF.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));

    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
};

CXXTEST_SUITE_REGISTRATION(PluginGaitReconstructionTest)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructBothLowerBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructBothUpperBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructBothLowerBodyHoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructBothUpperBodyHoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructBothFullBodyFullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstruct3BothLowerBodyFF)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstruct3BothLowerBodyFF_N18)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstruct3BothLowerBodyNoFF)