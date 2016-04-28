#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "plugingaitTest_def.h"
#include "test_file_path.h"

#include <openma/body.h>
#include <openma/io.h>

#include <memory> // std::shared_ptr

CXXTEST_SUITE(PluginGaitKinematicsTest)
{
  CXXTEST_TEST(kinematicsBothLowerBodyOneFrame)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 16.0},
       {"leftFootFlatEnabled", true},
       {"leftLegLength", 940.0},
       {"leftKneeWidth", 110.0},
       {"leftAnkleWidth", 70.0},
       {"rightFootFlatEnabled", true},
       {"rightLegLength", 940.0},
       {"rightKneeWidth", 120.0},
       {"rightAnkleWidth", 70.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-One.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-One.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Lower, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LHipAngles");
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LKneeAngles");
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LAnkleAngles");
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RHipAngles", {1e-4,1e-4,1.1e-4});
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RKneeAngles");
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RAnkleAngles");
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RFootProgressAngles");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LFootProgressAngles");
  };
  
  CXXTEST_TEST(kinematicsBothUpperBodyOneFrame)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 16.0},
       {"headOffsetEnabled", true},
       {"leftShoulderOffset", 50.0},
       {"rightShoulderOffset", 50.0},
       {"leftElbowWidth", 80.0},
       {"rightElbowWidth", 80.0},
       {"leftWristWidth", 40.0},
       {"rightWristWidth", 40.0},
       {"leftHandThickness", 30.0},
       {"rightHandThickness", 30.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-One.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-One.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Upper, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    
    compare_joint_kinematics(kinematics, trial, "L.Neck.Angle", "LNeckAngles");
    compare_joint_kinematics(kinematics, trial, "R.Neck.Angle", "RNeckAngles");
    compare_joint_kinematics(kinematics, trial, "L.Shoulder.Angle", "LShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "L.Elbow.Angle", "LElbowAngles");
    compare_joint_kinematics(kinematics, trial, "L.Wrist.Angle", "LWristAngles", {1.2e-4,1e-4,1.1e-4});
    compare_joint_kinematics(kinematics, trial, "R.Shoulder.Angle", "RShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "R.Elbow.Angle", "RElbowAngles");
    compare_joint_kinematics(kinematics, trial, "R.Wrist.Angle", "RWristAngles");
    compare_joint_kinematics(kinematics, trial, "R.Thorax.Progress.Angle", "RThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "L.Thorax.Progress.Angle", "LThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "R.Head.Progress.Angle", "RHeadAngles");
    compare_joint_kinematics(kinematics, trial, "L.Head.Progress.Angle", "LHeadAngles");
  };

  CXXTEST_TEST(kinematicsBothLowerBodyHoleFrames)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 16.0},
       {"leftFootFlatEnabled", true},
       {"leftLegLength", 940.0},
       {"leftKneeWidth", 110.0},
       {"leftAnkleWidth", 70.0},
       {"rightFootFlatEnabled", true},
       {"rightLegLength", 940.0},
       {"rightKneeWidth", 120.0},
       {"rightAnkleWidth", 70.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Hole.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-Hole.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Lower, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LHipAngles", {1e-4,1e-4,2e-4});
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LKneeAngles", {1e-4,1e-4,1.3e-4});
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LAnkleAngles", {1e-4,1e-4,1.3e-4});
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RHipAngles");
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RKneeAngles", {1e-4,1e-4,1.1e-4});
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RAnkleAngles", {1e-4,1e-4,1.3e-4});
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RFootProgressAngles");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LFootProgressAngles");
  };

  CXXTEST_TEST(kinematicsBothUpperBodyHoleFrames)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 16.0},
       {"headOffsetEnabled", true},
       {"leftShoulderOffset", 50.0},
       {"rightShoulderOffset", 50.0},
       {"leftElbowWidth", 80.0},
       {"rightElbowWidth", 80.0},
       {"leftWristWidth", 40.0},
       {"rightWristWidth", 40.0},
       {"leftHandThickness", 30.0},
       {"rightHandThickness", 30.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Hole.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-Hole.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Upper, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    
    compare_joint_kinematics(kinematics, trial, "L.Neck.Angle", "LNeckAngles");
    compare_joint_kinematics(kinematics, trial, "R.Neck.Angle", "RNeckAngles");
    compare_joint_kinematics(kinematics, trial, "L.Shoulder.Angle", "LShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "L.Elbow.Angle", "LElbowAngles");
    compare_joint_kinematics(kinematics, trial, "L.Wrist.Angle", "LWristAngles");
    compare_joint_kinematics(kinematics, trial, "R.Shoulder.Angle", "RShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "R.Elbow.Angle", "RElbowAngles");
    compare_joint_kinematics(kinematics, trial, "R.Wrist.Angle", "RWristAngles", {1.5e-4,1e-4,1.3e-4});
    compare_joint_kinematics(kinematics, trial, "R.Thorax.Progress.Angle", "RThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "L.Thorax.Progress.Angle", "LThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "R.Head.Progress.Angle", "RHeadAngles");
    compare_joint_kinematics(kinematics, trial, "L.Head.Progress.Angle", "LHeadAngles");
  };

  CXXTEST_TEST(kinematicsBothFullBodyFullFrames)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 16.0},
       {"headOffsetEnabled", true},
       {"leftShoulderOffset", 50.0},
       {"rightShoulderOffset", 50.0},
       {"leftElbowWidth", 80.0},
       {"rightElbowWidth", 80.0},
       {"leftWristWidth", 40.0},
       {"rightWristWidth", 40.0},
       {"leftHandThickness", 30.0},
       {"rightHandThickness", 30.0},
       {"leftFootFlatEnabled", true},
       {"leftLegLength", 800.0},
       {"leftKneeWidth", 105.0},
       {"leftAnkleWidth", 55.0},
       {"rightFootFlatEnabled", true},
       {"rightLegLength", 800.0},
       {"rightKneeWidth", 105.0},
       {"rightAnkleWidth", 55.0}});
    
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-Full.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-Full.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Full, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
  
    compare_joint_kinematics(kinematics, trial, "L.Neck.Angle", "LNeckAngles");
    compare_joint_kinematics(kinematics, trial, "R.Neck.Angle", "RNeckAngles");
    compare_joint_kinematics(kinematics, trial, "L.Shoulder.Angle", "LShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "L.Elbow.Angle", "LElbowAngles");
    compare_joint_kinematics(kinematics, trial, "L.Wrist.Angle", "LWristAngles", {1.2e-4});
    compare_joint_kinematics(kinematics, trial, "R.Shoulder.Angle", "RShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "R.Elbow.Angle", "RElbowAngles");
    compare_joint_kinematics(kinematics, trial, "R.Wrist.Angle", "RWristAngles");
    compare_joint_kinematics(kinematics, trial, "L.Spine.Angle", "LSpineAngles");
    compare_joint_kinematics(kinematics, trial, "R.Spine.Angle", "RSpineAngles");
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LHipAngles", {1e-4,1e-4,1.3e-4});
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LKneeAngles");
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LAnkleAngles");
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RHipAngles", {1e-4,1e-4,1.2e-4});
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RKneeAngles");
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RAnkleAngles");
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RFootProgressAngles");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LFootProgressAngles");
    compare_joint_kinematics(kinematics, trial, "R.Thorax.Progress.Angle", "RThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "L.Thorax.Progress.Angle", "LThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "R.Head.Progress.Angle", "RHeadAngles");
    compare_joint_kinematics(kinematics, trial, "L.Head.Progress.Angle", "LHeadAngles");
  };
  
  CXXTEST_TEST(kinematics3BothLowerBodyFF)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 14.0},
       {"leftFootFlatEnabled", true},
       {"leftLegLength", 920.0},
       {"leftKneeWidth", 102.0},
       {"leftAnkleWidth", 70.0},
       {"rightFootFlatEnabled", true},
       {"rightLegLength", 920.0},
       {"rightKneeWidth", 102.0},
       {"rightAnkleWidth", 70.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_FF.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion3_FF.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Lower, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LHipAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LKneeAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LAnkleAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RHipAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RKneeAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RAnkleAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RPelvisAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LPelvisAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RFootProgressAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LFootProgressAnglesV");
  };

  CXXTEST_TEST(kinematics3BothLowerBodyFF_N18)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 14.0},
       {"leftFootFlatEnabled", true},
       {"leftLegLength", 920.0},
       {"leftKneeWidth", 102.0},
       {"leftAnkleWidth", 70.0},
       {"rightFootFlatEnabled", true},
       {"rightLegLength", 920.0},
       {"rightKneeWidth", 102.0},
       {"rightAnkleWidth", 70.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_FF_N18.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion3_FF_N18.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Lower, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LHipAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LKneeAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LAnkleAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RHipAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RKneeAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RAnkleAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RPelvisAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LPelvisAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RFootProgressAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LFootProgressAnglesV");
  };

  CXXTEST_TEST(kinematics3BothLowerBodyNoFF)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 14.0},
       {"leftLegLength", 920.0},
       {"leftKneeWidth", 102.0},
       {"leftAnkleWidth", 70.0},
       {"rightLegLength", 920.0},
       {"rightKneeWidth", 102.0},
       {"rightAnkleWidth", 70.0}});
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration3_noFF.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion3_noFF.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Lower, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LHipAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LKneeAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LAnkleAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RHipAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RKneeAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RAnkleAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RPelvisAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LPelvisAnglesV");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RFootProgressAnglesV");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LFootProgressAnglesV");
  };
  
  CXXTEST_TEST(kinematicsBothFullBodyFullFramesHeadOffsetDisabled)
  {
    ma::Subject subject("Anonymous",
      {{"markerDiameter", 14.0},
       {"leftShoulderOffset", 50.0},
       {"rightShoulderOffset", 50.0},
       {"leftElbowWidth", 80.0},
       {"rightElbowWidth", 80.0},
       {"leftWristWidth", 40.0},
       {"rightWristWidth", 40.0},
       {"leftHandThickness", 30.0},
       {"rightHandThickness", 30.0},
       {"leftLegLength", 780.0},
       {"leftKneeWidth", 90.0},
       {"leftAnkleWidth", 70.0},
       {"rightLegLength", 780.0},
       {"rightKneeWidth", 95.0},
       {"rightAnkleWidth", 70.0}});
    
    ma::Node statictrials("statictrials"), dynamictrials("dynamictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration4.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Dynamic trial
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion4_noFF_noHO.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::PluginGait skeletonhelper(ma::body::Region::Full, ma::body::Side::Both);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinematics = kinematicsanalyses.child(0);
  
    compare_joint_kinematics(kinematics, trial, "L.Neck.Angle", "LViconNeckAngles");
    compare_joint_kinematics(kinematics, trial, "R.Neck.Angle", "RViconNeckAngles");
    compare_joint_kinematics(kinematics, trial, "L.Shoulder.Angle", "LViconShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "L.Elbow.Angle", "LViconElbowAngles");
    compare_joint_kinematics(kinematics, trial, "L.Wrist.Angle", "LViconWristAngles");
    compare_joint_kinematics(kinematics, trial, "R.Shoulder.Angle", "RViconShoulderAngles");
    compare_joint_kinematics(kinematics, trial, "R.Elbow.Angle", "RViconElbowAngles");
    compare_joint_kinematics(kinematics, trial, "R.Wrist.Angle", "RViconWristAngles");
    compare_joint_kinematics(kinematics, trial, "L.Spine.Angle", "LViconSpineAngles");
    compare_joint_kinematics(kinematics, trial, "R.Spine.Angle", "RViconSpineAngles");
    compare_joint_kinematics(kinematics, trial, "L.Hip.Angle", "LViconHipAngles");
    compare_joint_kinematics(kinematics, trial, "L.Knee.Angle", "LViconKneeAngles");
    compare_joint_kinematics(kinematics, trial, "L.Ankle.Angle", "LViconAnkleAngles");
    compare_joint_kinematics(kinematics, trial, "R.Hip.Angle", "RViconHipAngles");
    compare_joint_kinematics(kinematics, trial, "R.Knee.Angle", "RViconKneeAngles");
    compare_joint_kinematics(kinematics, trial, "R.Ankle.Angle", "RViconAnkleAngles");
    compare_joint_kinematics(kinematics, trial, "R.Pelvis.Progress.Angle", "RViconPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "L.Pelvis.Progress.Angle", "LViconPelvisAngles");
    compare_joint_kinematics(kinematics, trial, "R.Foot.Progress.Angle", "RViconFootProgressAngles");
    compare_joint_kinematics(kinematics, trial, "L.Foot.Progress.Angle", "LViconFootProgressAngles");
    compare_joint_kinematics(kinematics, trial, "R.Thorax.Progress.Angle", "RViconThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "L.Thorax.Progress.Angle", "LViconThoraxAngles");
    compare_joint_kinematics(kinematics, trial, "R.Head.Progress.Angle", "RViconHeadAngles");
    compare_joint_kinematics(kinematics, trial, "L.Head.Progress.Angle", "LViconHeadAngles");
  };
};

CXXTEST_SUITE_REGISTRATION(PluginGaitKinematicsTest)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematicsBothLowerBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematicsBothUpperBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematicsBothLowerBodyHoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematicsBothUpperBodyHoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematicsBothFullBodyFullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematics3BothLowerBodyFF)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematics3BothLowerBodyFF_N18)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematics3BothLowerBodyNoFF)
CXXTEST_TEST_REGISTRATION(PluginGaitKinematicsTest, kinematicsBothFullBodyFullFramesHeadOffsetDisabled)