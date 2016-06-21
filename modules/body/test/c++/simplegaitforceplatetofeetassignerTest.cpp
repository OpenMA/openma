#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include <openma/body/simplegaitforceplatetofeetassigner.h>
#include <openma/body/segment.h>
#include <openma/instrument/forceplate.h>

#include "plugingaitTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(SimpleGaitForceplateToFeetAssignerTest)
{
  CXXTEST_TEST(TwoForplatesTwoFeet)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setMarkerDiameter(14.0); // mm
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
    ma::body::SimpleGaitForcePlateToFeetAssigner assigner;
    TS_ASSERT(assigner.run(&rootModel));
    auto fps = rootModel.findChildren<ma::instrument::ForcePlate*>();
    TS_ASSERT_EQUALS(fps.size(), 2u);
    auto LFoot = rootModel.findChild<ma::body::Segment*>("L.Foot");
    auto RFoot = rootModel.findChild<ma::body::Segment*>("R.Foot");
    TS_ASSERT_DIFFERS(LFoot, nullptr);
    TS_ASSERT_DIFFERS(RFoot, nullptr);
    auto LFPw = LFoot->findChild<ma::TimeSequence*>("FP1.Wrench.Global.COP",{},false);
    auto RFPw = RFoot->findChild<ma::TimeSequence*>("FP2.Wrench.Global.COP",{},false);
    TS_ASSERT_DIFFERS(LFPw, nullptr);
    TS_ASSERT_DIFFERS(RFPw, nullptr);
  };
  
  CXXTEST_TEST(TwoForplatesTwoFeetBis)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both);
    helper.setMarkerDiameter(16.0); // mm
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
    ma::body::SimpleGaitForcePlateToFeetAssigner assigner;
    TS_ASSERT(assigner.run(&rootModel));
    auto fps = rootModel.findChildren<ma::instrument::ForcePlate*>();
    TS_ASSERT_EQUALS(fps.size(), 2u);
    auto LFoot = rootModel.findChild<ma::body::Segment*>("L.Foot");
    auto RFoot = rootModel.findChild<ma::body::Segment*>("R.Foot");
    TS_ASSERT_DIFFERS(LFoot, nullptr);
    TS_ASSERT_DIFFERS(RFoot, nullptr);
    auto LFPw = LFoot->findChild<ma::TimeSequence*>("FP2.Wrench.Global.COP",{},false);
    auto RFPw = RFoot->findChild<ma::TimeSequence*>("FP1.Wrench.Global.COP",{},false);
    TS_ASSERT_DIFFERS(LFPw, nullptr);
    TS_ASSERT_DIFFERS(RFPw, nullptr);
  };
  
  CXXTEST_TEST(TwoForplatesTwoFeetTer)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both);
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
    ma::body::SimpleGaitForcePlateToFeetAssigner assigner;
    TS_ASSERT(assigner.run(&rootModel));
    auto fps = rootModel.findChildren<ma::instrument::ForcePlate*>();
    TS_ASSERT_EQUALS(fps.size(), 2u);
    auto LFoot = rootModel.findChild<ma::body::Segment*>("L.Foot");
    auto RFoot = rootModel.findChild<ma::body::Segment*>("R.Foot");
    TS_ASSERT_DIFFERS(LFoot, nullptr);
    TS_ASSERT_DIFFERS(RFoot, nullptr);
    auto LFPw = LFoot->findChild<ma::TimeSequence*>("FP2.Wrench.Global.COP",{},false);
    auto RFPw = RFoot->findChild<ma::TimeSequence*>("FP1.Wrench.Global.COP",{},false);
    TS_ASSERT_DIFFERS(LFPw, nullptr);
    TS_ASSERT_DIFFERS(RFPw, nullptr);
    // double subjectMass = 66.5; // kg
    // TS_ASSERT_EIGEN_DELTA(ma::math::to_vector(LFPw).values() / subjectMass, ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>("LGroundReactionForce",{{"type",ma::TimeSequence::Force}})).values(), 1e-5);
    // TS_ASSERT_EIGEN_DELTA(ma::math::to_vector(RFPw).values() / subjectMass, ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>("RGroundReactionForce",{{"type",ma::TimeSequence::Force}})).values(), 1e-5);
    // std::cout << "\n" << ma::math::to_vector(LFPw).values() / subjectMass << std::endl;
    // std::cout << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>("LGroundReactionForce",{{"type",ma::TimeSequence::Force}})).values() << std::endl;
    // std::cout << "\n" << ma::math::to_vector(RFPw).values() << std::endl;
  };
};

CXXTEST_SUITE_REGISTRATION(SimpleGaitForceplateToFeetAssignerTest)
CXXTEST_TEST_REGISTRATION(SimpleGaitForceplateToFeetAssignerTest, TwoForplatesTwoFeet)
CXXTEST_TEST_REGISTRATION(SimpleGaitForceplateToFeetAssignerTest, TwoForplatesTwoFeetBis)
CXXTEST_TEST_REGISTRATION(SimpleGaitForceplateToFeetAssignerTest, TwoForplatesTwoFeetTer)