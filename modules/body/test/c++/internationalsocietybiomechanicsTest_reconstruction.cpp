#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "internationalsocietybiomechanicsTest_def.h"
#include "test_file_path.h"

#include <openma/body/referenceframe.h>
#include <openma/base/enums.h>

CXXTEST_SUITE(InternationalSocietyBiomechanicsCalibrationTest)
{
  CXXTEST_TEST(reconstructUpperLimbBothWithCustomTranslator)
  {
    ma::Node rootTrials("rootTrials"), rootModels("rootModels");
    generate_trial_from_c3d_file(&rootTrials, OPENMA_TDD_PATH_IN("c3d/internationalsocietybiomechanics/UpperLimbStatic.c3d"));
    ma::body::InternationalSocietyBiomechanics helper(ma::body::Region::Upper, ma::body::Side::Both);
    helper.setSex(ma::Sex::Male);
    set_isb_custom_landmarks_translator(&helper);
    TS_ASSERT_EQUALS(helper.calibrate(&rootTrials, nullptr), true);
    TS_ASSERT_EQUALS(helper.reconstruct(&rootModels, &rootTrials), true);
    auto model = rootModels.findChild<ma::body::Model*>();
    const double reftorso44[12] = {0.9947868968, 0.0307040361, -0.0972434680, 0.0981749206, -0.0304426669, 0.9947034377, 0.0275810598, -0.9990648158, -0.0332983325, 14.0739380377, -112.1111774964, 1073.1328298614};
    
    compare_segment_motion(model, "Torso.SCS", 44, reftorso44);
  };
};

CXXTEST_SUITE_REGISTRATION(InternationalSocietyBiomechanicsCalibrationTest)  
CXXTEST_TEST_REGISTRATION(InternationalSocietyBiomechanicsCalibrationTest, reconstructUpperLimbBothWithCustomTranslator)