#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "lyonwholebodymodelTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(LyonWholeBodyModelKineticsTest)
{
  CXXTEST_TEST(verifyNoError)
  {
    ma::Subject subject("Anonymous", {
      {"sex", ma::Sex::Male},
      {"mass", 80.0},
      {"gravity", std::array<double,3>{{0,0,-9.81}}}
    });
    // Static trial
    ma::Node statictrials("statictrials"), models("models"), kinematicsanalyses("kinematicsanalyses"), kineticsanalyses("kineticsanalyses");
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/GaitCalib.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::LyonWholeBodyModel skeletonhelper(ma::body::Region::Full, ma::body::Side::Both);
    set_isb_custom_landmarks_translator3(&skeletonhelper);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    TS_ASSERT_EQUALS(skeletonhelper.property("L.Arm.length").isValid(), true);
    TS_ASSERT_EQUALS(skeletonhelper.property("Torso.length").isValid(), true);
    TS_ASSERT_EQUALS(skeletonhelper.property("R.Foot.length").isValid(), true);
    // Marker cluster registration
    TS_ASSERT_EQUALS(ma::body::register_marker_cluster(&skeletonhelper, &statictrials), true);
    TS_ASSERT_EQUALS(skeletonhelper.property("L.Arm.length").isValid(), true);
    TS_ASSERT_EQUALS(skeletonhelper.property("Torso.length").isValid(), true);
    TS_ASSERT_EQUALS(skeletonhelper.property("R.Foot.length").isValid(), true);
    // CALIBRATION
    // Skeleton helper reconstruction with a least sequare pose estimator
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &statictrials), true);
    TS_ASSERT_EQUALS(models.child(0)->property("L.Arm.length").isValid(), true);
    TS_ASSERT_EQUALS(models.child(0)->property("Torso.length").isValid(), true);
    TS_ASSERT_EQUALS(models.child(0)->property("R.Foot.length").isValid(), true);
    TS_ASSERT_EQUALS(models.findChild("L.Arm")->property("length").isValid(), true);
    TS_ASSERT_EQUALS(models.findChild("Torso")->property("length").isValid(), true);
    TS_ASSERT_EQUALS(models.findChild("R.Foot")->property("length").isValid(), true);
    // Extract joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);
    TS_ASSERT_EQUALS(kinematicsanalyses.findChildren<ma::TimeSequence*>().size(), 12u);
    // Extract joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinetics(&kineticsanalyses, &models), true);
    TS_ASSERT_EQUALS(kineticsanalyses.children().size(), 1u);
    TS_ASSERT_EQUALS(kineticsanalyses.findChildren<ma::TimeSequence*>().size(), 18u);
    // DYNAMIC
    // Dynamic trial
    ma::Node dynamictrials("dynamictrials"), models2("models"), kinematicsanalyses2("kinematicsanalyses"), kineticsanalyses2("kineticsanalyses");
    TS_ASSERT_EQUALS(ma::io::read(&dynamictrials, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/GaitMotion.c3d")), true);
    TS_ASSERT_EQUALS(dynamictrials.children().size(), 1u);
    // Add an external wrench assigner
    // IMPORTANT: We can add it now and not before the reconstruction because by default the LWBM does not have a default external wrench assigner.
    new ma::body::SimpleGaitForcePlateToFeetAssigner(&skeletonhelper);
    // Skeleton helper reconstruction with a least sequare pose estimator
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models2, &skeletonhelper, &dynamictrials), true);
    TS_ASSERT_EQUALS(models2.child(0)->property("L.Arm.length").isValid(), true);
    TS_ASSERT_EQUALS(models2.child(0)->property("Torso.length").isValid(), true);
    TS_ASSERT_EQUALS(models2.child(0)->property("R.Foot.length").isValid(), true);
    TS_ASSERT_EQUALS(models2.findChild("L.Arm")->property("length").isValid(), true);
    TS_ASSERT_EQUALS(models2.findChild("Torso")->property("length").isValid(), true);
    TS_ASSERT_EQUALS(models2.findChild("R.Foot")->property("length").isValid(), true);
    // Extract joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses2, &models2), true);
    TS_ASSERT_EQUALS(kinematicsanalyses2.children().size(), 1u);
    TS_ASSERT_EQUALS(kinematicsanalyses2.findChildren<ma::TimeSequence*>().size(), 12u);
    // Extract joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinetics(&kineticsanalyses2, &models2), true);
    TS_ASSERT_EQUALS(kineticsanalyses2.children().size(), 1u);
    TS_ASSERT_EQUALS(kineticsanalyses2.findChildren<ma::TimeSequence*>().size(), 18u);
    
    auto trial = dynamictrials.child<ma::Trial*>(0);
    auto kinetic = kineticsanalyses2.child(0);
    
    for (const auto& c : kinetic->findChildren<ma::TimeSequence*>())
    {
      // Specific to the C3D file format that accepts only one unit by type
      c->addParent(trial->timeSequences());
      if (c->type() == ma::TimeSequence::Force)
        c->setUnit("N");
      else if (c->type() == ma::TimeSequence::Moment)
        c->setUnit("Nmm");
      else if (c->type() == ma::TimeSequence::Power)
        c->setUnit("W");
    }
    ma::io::write(&dynamictrials, "test3.c3d");
  };
};

CXXTEST_SUITE_REGISTRATION(LyonWholeBodyModelKineticsTest)  
CXXTEST_TEST_REGISTRATION(LyonWholeBodyModelKineticsTest, verifyNoError)