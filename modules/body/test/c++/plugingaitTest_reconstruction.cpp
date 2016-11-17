#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "plugingaitTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(PluginGaitReconstructionTest)
{
  CXXTEST_TEST(reconstructBothLowerBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Upper, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
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
  
  CXXTEST_TEST(reconstruct2BothUpperBodyHeadOffsetDisabled)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftShoulderOffset(50.0); // mm
    helper.setRightShoulderOffset(50.0); // mm
    helper.setLeftElbowWidth(80.0); // mm
    helper.setRightElbowWidth(80.0); // mm
    helper.setLeftWristWidth(40.0); // mm
    helper.setRightWristWidth(40.0); // mm
    helper.setLeftHandThickness(30.0); // mm
    helper.setRightHandThickness(30.0); // mm
    helper.setLeftLegLength(780.0); // mm
    helper.setLeftKneeWidth(90.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightLegLength(780.0); // mm
    helper.setRightKneeWidth(95.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration4.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion4_noFF_noHO.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    // NOTE: Why the toleranace are so different? for the upper limb when the marker diameter is not 16 mm?
    compare_segment_motion(model, trial, "Head.SCS", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "Torso.SCS", {"TRXO","TRXA","TRXL","TRXP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Clavicle.SCS", {"LCLO","LCLA","LCLL","LCLP"}, {5.1e-4});
    compare_segment_motion(model, trial, "R.Clavicle.SCS", {"RCLO","RCLA","RCLL","RCLP"}, {3.1e-4});
    compare_segment_motion(model, trial, "L.Arm.SCS", {"LHUO","LHUA","LHUL","LHUP"}, {3.5e-4});
    compare_segment_motion(model, trial, "R.Arm.SCS", {"RHUO","RHUA","RHUL","RHUP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Forearm.SCS", {"LRAO","LRAA","LRAL","LRAP"}, {3e-4});
    compare_segment_motion(model, trial, "R.Forearm.SCS", {"RRAO","RRAA","RRAL","RRAP"}, {5.8e-4});
    compare_segment_motion(model, trial, "L.Hand.SCS", {"LHNO","LHNA","LHNL","LHNP"}, {3.1e-4});
    compare_segment_motion(model, trial, "R.Hand.SCS", {"RHNO","RHNA","RHNL","RHNP"}, {6.1e-4});
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {6.1e-4});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
    
    auto progression = model->findChild<ma::TimeSequence*>("Progression.SCS");
    TS_ASSERT_DIFFERS(progression, nullptr);
    if (progression == nullptr) return;
    for (size_t i = 0 ; i < progression->samples() ; ++i)
    {
      TS_ASSERT_DELTA(progression->data(i, 0), 1.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 1), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 2), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 3), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 4), 1.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 5), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 6), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 7), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 8), 1.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i, 9), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i,10), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i,11), 0.0, 1e-15);
      TS_ASSERT_DELTA(progression->data(i,12), 0.0, 1e-15);
    }
  };
  
  CXXTEST_TEST(reconstructFullBodyKAD)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KAD);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(860.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(75.3); // mm
    helper.setRightLegLength(865.0); // mm
    helper.setRightKneeWidth(103.4); // mm
    helper.setRightAnkleWidth(72.9); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKad_Calibration.c3d"));
    TS_ASSERT_EQUALS(rootCalibration.children().size(),1u);
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKad_Motion.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {8.1e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {1.1e-3,2e-5,2e-5});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {7e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {8.5e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); 
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4});
  };
  
  CXXTEST_TEST(reconstructFullBodyKADFF)
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
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKad_Calibration_FootFlat.c3d"));
    TS_ASSERT_EQUALS(rootCalibration.children().size(),1u);
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKad_Motion_FootFlat.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {8.1e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {1.1e-3,2e-5,2e-5});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {7e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {8.5e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); 
    compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4});
  };
  
  CXXTEST_TEST(reconstructFullBodyKADMed)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KADMed);
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(860.0); // mm
    helper.setLeftKneeWidth(102.0); // mm
    helper.setLeftAnkleWidth(75.3); // mm
    helper.setRightLegLength(865.0); // mm
    helper.setRightKneeWidth(103.4); // mm
    helper.setRightAnkleWidth(72.9); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKadMed_Calibration.c3d"));
    TS_ASSERT_EQUALS(rootCalibration.children().size(),1u);
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKadMed_Motion.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {1.1e-3,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {1.1e-3,2e-5,2e-5});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {6e-4,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {1.1e-3,2e-5,2e-5});
    // NOTE: The feet  cannot be compared as it seems there is an error in the definition of their SCS in the original PluginGait KADMed model.
    // compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4});
    // compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4});
  };
  
  CXXTEST_TEST(reconstructFullBodyKADMed2)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::KADMed);
    helper.setMarkerDiameter(25.0); // mm
    helper.setLeftLegLength(775.0); // mm
    helper.setLeftKneeWidth(105.1); // mm
    helper.setLeftAnkleWidth(68.4); // mm
    helper.setRightLegLength(770.0); // mm
    helper.setRightKneeWidth(107.0); // mm
    helper.setRightAnkleWidth(68.6); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKadMed_Calibration2.c3d"));
    TS_ASSERT_EQUALS(rootCalibration.children().size(),1u);
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiGKadMed_Motion2.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    
    auto trial = rootDynamic.findChild<ma::Trial*>();
    auto model = rootModel.findChild<ma::body::Model*>();
    compare_segment_motion(model, trial, "Pelvis.SCS", {"PELO","PELA","PELL","PELP"}, {5e-4});
    compare_segment_motion(model, trial, "R.Thigh.SCS", {"RFEO","RFEA","RFEL","RFEP"}, {1.2e-3,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Thigh.SCS", {"LFEO","LFEA","LFEL","LFEP"}, {1.8e-3,3e-5,3e-5});
    compare_segment_motion(model, trial, "R.Shank.SCS", {"RTIO","RTIA","RTIL","RTIP"}, {1.3e-3,2e-5,2e-5});
    compare_segment_motion(model, trial, "L.Shank.SCS", {"LTIO","LTIA","LTIL","LTIP"}, {1.5e-3,3e-5,3e-5});
    // NOTE: The feet  cannot be compared as it seems there is an error in the definition of their SCS in the original PluginGait KADMed model.
    // compare_segment_motion(model, trial, "R.Foot.SCS", {"RFOO","RFOA","RFOL","RFOP"}, {1e4});
    // compare_segment_motion(model, trial, "L.Foot.SCS", {"LFOO","LFOA","LFOL","LFOP"}, {1e4});
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
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstruct2BothUpperBodyHeadOffsetDisabled)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructFullBodyKAD)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructFullBodyKADFF)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructFullBodyKADMed)
CXXTEST_TEST_REGISTRATION(PluginGaitReconstructionTest, reconstructFullBodyKADMed2)