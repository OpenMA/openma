#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "internationalsocietybiomechanicsTest_def.h"
#include "test_file_path.h"

#include <openma/body/referenceframe.h>
#include <openma/base/enums.h>

CXXTEST_SUITE(InternationalSocietyBiomechanicsCalibrationTest)
{
  CXXTEST_TEST(calibrateUpperLimbBothWithCustomTranslator)
  {
    ma::Node root("root");
    generate_trial_from_c3d_file(&root, OPENMA_TDD_PATH_IN("c3d/internationalsocietybiomechanics/UpperLimbStatic.c3d"));
    ma::body::InternationalSocietyBiomechanics helper(ma::body::Region::Upper, ma::body::Side::Both);
    helper.setSex(ma::Sex::Male);
    set_isb_custom_landmarks_translator(&helper);
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