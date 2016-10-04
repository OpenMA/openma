#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "internationalsocietybiomechanicsTest_def.h"
#include "test_file_path.h"

#include <openma/body/referenceframe.h>
#include <openma/body/landmarkstranslator.h>
#include <openma/base/enums.h>

CXXTEST_SUITE(InternationalSocietyBiomechanicsCalibrationTest)
{
  CXXTEST_TEST(calibrateUpperLimbBothWithCustomTranslator)
  {
    ma::Node root("root");
    generate_trial_from_c3d_file(&root, OPENMA_TDD_PATH_IN("c3d/internationalsocietybiomechanics/UpperLimbStatic.c3d"));
    ma::body::InternationalSocietyBiomechanics helper(ma::body::Region::Upper, ma::body::Side::Both);
    helper.setSex(ma::Sex::Male);
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"C7"              , "C7"},
      {"T8"              , "T8"},
      {"IJ"              , "SS"},
      {"PX"              , "XP"},
      {"L.Acromion"      , "L.AC"},
      {"L.AngulusAcromio", "L.AA"},
      {"L.Biceps"        , "L.GH"},
      {"L.Epi.Lat"       , "L.LHE"},
      {"L.Epi.Med"       , "L.MHE"},
      {"L.Wrist.Ulna"    , "L.US"},
      {"L.Wrist.Rad"     , "L.RS"},
      {"L.D5M1"          , "L.MH5"},
      {"L.D2M1"          , "L.MH2"},
      {"R.Acromion"      , "R.AC"},
      {"R.AngulusAcromio", "R.AA"},
      {"R.Biceps"        , "R.GH"},
      {"R.Epi.Lat"       , "R.LHE"},
      {"R.Epi.Med"       , "R.MHE"},
      {"R.Wrist.Ulna"    , "R.US"},
      {"R.Wrist.Rad"     , "R.RS"},
      {"R.D5M1"          , "R.MH5"},
      {"R.D2M1"          , "R.MH2"},
    }, &helper);
    helper.calibrate(&root, nullptr);
    auto torso_relscs_node = helper.findChild<const ma::body::ReferenceFrame*>("Torso.SCS");
    TS_ASSERT_DIFFERS(torso_relscs_node, nullptr);
    TS_ASSERT_DELTA(torso_relscs_node->u()[0], 0.9980485036, 1e-8);
    TS_ASSERT_DELTA(torso_relscs_node->u()[1], -0.0624434499, 1e-8);
    TS_ASSERT_DELTA(torso_relscs_node->u()[2], 0.0, 1e-15);
    TS_ASSERT_DELTA(torso_relscs_node->v()[0], 0.0624434499, 1e-8);
    TS_ASSERT_DELTA(torso_relscs_node->v()[1], 0.9980485036, 1e-8);
    TS_ASSERT_DELTA(torso_relscs_node->v()[2], 0.0, 1e-15);
    TS_ASSERT_DELTA(torso_relscs_node->w()[0], 0.0, 1e-15);
    TS_ASSERT_DELTA(torso_relscs_node->w()[1], 0.0, 1e-15);
    TS_ASSERT_DELTA(torso_relscs_node->w()[2], 1.0, 1e-15);
    TS_ASSERT_DELTA(torso_relscs_node->o()[0], 86.2581302099, 1e-7);
    TS_ASSERT_DELTA(torso_relscs_node->o()[1], -21.0216470741, 1e-7);
    TS_ASSERT_DELTA(torso_relscs_node->o()[2], 0.0, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(InternationalSocietyBiomechanicsCalibrationTest)  
CXXTEST_TEST_REGISTRATION(InternationalSocietyBiomechanicsCalibrationTest, calibrateUpperLimbBothWithCustomTranslator)