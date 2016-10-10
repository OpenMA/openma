#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "internationalsocietybiomechanicsTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(InternationalSocietyBiomechanicsKinematicsTest)
{
  CXXTEST_TEST(kinematicsUpperLimbBothWithCustomTranslator)
  {
    ma::Subject subject("Anonymous", {{"sex", ma::Sex::Male}});
    ma::Node statictrials("statictrials"), models("models"), kinematicsanalyses("kinematicsanalyses");
    // Static trial
    TS_ASSERT_EQUALS(ma::io::read(&statictrials, OPENMA_TDD_PATH_IN("c3d/internationalsocietybiomechanics/UpperLimbStatic.c3d")), true);
    TS_ASSERT_EQUALS(statictrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::InternationalSocietyBiomechanics skeletonhelper(ma::body::Region::Upper, ma::body::Side::Both);
    set_isb_custom_landmarks_translator(&skeletonhelper);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &statictrials, &subject), true);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&models, &skeletonhelper, &statictrials), true);
    TS_ASSERT_EQUALS(models.children().size(), 1u);
    models.findChild<ma::body::EulerDescriptor*>("R.Wrist.Angle")->configure(ma::body::EulerDescriptor::ZYX);
    models.findChild<ma::body::EulerDescriptor*>("L.Wrist.Angle")->configure(ma::body::EulerDescriptor::ZYX);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&kinematicsanalyses, &models, true), true);
    TS_ASSERT_EQUALS(kinematicsanalyses.children().size(), 1u);

    auto kinematics = kinematicsanalyses.child(0);    
    // The tests on shoulder and elbow joint angles were removed due to a difference in the computation of the SJC in the original data.
    // Left wrist angles #126
    const double lwristangle126[3] = {20.486435656144469, 23.050571399986694, -12.364210415360624};
    compare_joint_kinematics(kinematics, "L.Wrist.Angle", 126, lwristangle126);
    // Right wrist angles #145
    const double rwristangle145[3] = {6.995882978659257, -11.388780067630307, 9.746227693435785};
    compare_joint_kinematics(kinematics, "R.Wrist.Angle", 145, rwristangle145);
    // Thorax angles #0
    // const double thoraxangle0[3] = {-109.9300720909064, 84.1764610476819, 112.1812594245926};
    // compare_joint_kinematics(kinematics, "Torso.Progression.Angle", 0, thoraxangle0);
    TS_WARN("IMPLEMENT THE RECONSTRUCTION OF THE PROGRESSION FRAME AND THE ASSOCIATED JOINTS");
  };
  
  CXXTEST_TEST(kinematicsFullBodyBothWithCustomTranslator)
  {
    ma::Subject subject("Anonymous", {{"sex", ma::Sex::Male}});
    ma::Node rootStaticTrials("rootStaticTrials"), rootDynamicTrials("rootDynamicTrials"), rootModels("rootModels"), rootKinematicsanalyses("rootKinematicsanalyses");
    // Static trial
    generate_trial_from_c3d_file(&rootStaticTrials, OPENMA_TDD_PATH_IN("c3d/internationalsocietybiomechanics/SprintCalib.c3d"));
    TS_ASSERT_EQUALS(rootStaticTrials.children().size(), 1u);
    // Skeleton helper creation
    ma::body::InternationalSocietyBiomechanics skeletonhelper(ma::body::Region::Lower, ma::body::Side::Both);
    set_isb_custom_landmarks_translator2(&skeletonhelper);
    // Skeleton helper calibration
    TS_ASSERT_EQUALS(ma::body::calibrate(&skeletonhelper, &rootStaticTrials, &subject), true);
    // Dynamic trial
    generate_trial_from_c3d_file(&rootDynamicTrials, OPENMA_TDD_PATH_IN("c3d/internationalsocietybiomechanics/SprintMotion.c3d"));
    TS_ASSERT_EQUALS(rootDynamicTrials.children().size(), 1u);
    // Model reconstruction
    TS_ASSERT_EQUALS(ma::body::reconstruct(&rootModels, &skeletonhelper, &rootDynamicTrials), true);
    TS_ASSERT_EQUALS(rootModels.children().size(), 1u);
    // Joint kinematics
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinematics(&rootKinematicsanalyses, &rootModels, true), true);
    TS_ASSERT_EQUALS(rootKinematicsanalyses.children().size(), 1u);

    auto kinematics = rootKinematicsanalyses.child(0); 
    TS_WARN("MODIFY THE IMPLEMENTATION TO GIVE THE POSSIBILITY TO REMOVE SEGMENT (OR INVALID POSE) IN CASE OF MISSING MARKERS");
    // // Right elbow angles #456
    // const double relbowangle456[3] = {19.0821335358026,-1.26096492611055,73.8676573074368};
    // compare_joint_kinematics(kinematics, "R.Elbow.Angle", 456, relbowangle456);
    // // Left elbow angles #286
    // const double lelbowangle286[3] = {18.5495587800068,-5.87869535442481,-77.4630861838434};
    // compare_joint_kinematics(kinematics, "L.Elbow.Angle", 286, lelbowangle286);
    // Right hip angles #111
    const double rhipangle111[3] = {38.0645407877351,-9.83813683420906,14.8288199946509};
    compare_joint_kinematics(kinematics, "R.Hip.Angle", 111, rhipangle111);
    // Left hip angles #158
    const double lhipangle158[3] = {70.8358554013765,3.47229848546519,-4.74765897535033};
    compare_joint_kinematics(kinematics, "L.Hip.Angle", 158, lhipangle158);
  };
};

CXXTEST_SUITE_REGISTRATION(InternationalSocietyBiomechanicsKinematicsTest)  
CXXTEST_TEST_REGISTRATION(InternationalSocietyBiomechanicsKinematicsTest, kinematicsUpperLimbBothWithCustomTranslator)
CXXTEST_TEST_REGISTRATION(InternationalSocietyBiomechanicsKinematicsTest, kinematicsFullBodyBothWithCustomTranslator)