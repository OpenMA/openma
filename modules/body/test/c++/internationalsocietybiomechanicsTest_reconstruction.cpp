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
    // NOTE: There is no test on the arms due to a difference in the computation of the SJC in the original data.
    const double reftorso44[12] = {0.9947868968, 0.0307040361, -0.0972434680, 0.0981749206, -0.0304426669, 0.9947034377, 0.0275810598, -0.9990648158, -0.0332983325, 14.0739380377, -112.1111774964, 1073.1328298614};
    compare_segment_motion(model, "Torso.SCS", 44, reftorso44);
    const double reflforearm5[12] = {0.1823621254, 0.3037176942, -0.9351468427, -0.5103727504, -0.7836871488, -0.3540538214, -0.8403949731, 0.5418394735, 0.0120943808, -45.2054200000, 317.2064350000, 966.7606200000};
    compare_segment_motion(model, "L.Forearm.SCS", 5, reflforearm5);
    const double refrforearm90[12] = {0.1719042574, -0.1061332154, -0.9793797358, -0.6484765074, 0.7362036602, -0.1936036934, 0.7415707287, 0.6683860496, 0.0577316467, -74.2896050000, -535.5918300000, 919.1575050000};
    compare_segment_motion(model, "R.Forearm.SCS", 90, refrforearm90);
    const double reflhand32[12] = {0.3370520349, -0.2541281133, -0.9065400310, -0.3805269349, -0.9175009438, 0.1157206548, -0.8611592057, 0.3059590172, -0.4059481522, 83.9074950000, 519.8002450000,1053.8471700000};
    compare_segment_motion(model, "L.Hand.SCS", 32, reflhand32);
    const double refrhand145[12] = {0.2356765711, 0.1088597395, -0.9657153364, -0.5420506897, 0.8395018818, -0.0376515629, 0.8066211029, 0.5323402554, 0.2568584218, 96.5360600000, -724.7153950000, 974.8815600000};
    compare_segment_motion(model, "R.Hand.SCS", 145, refrhand145);
  };
};

CXXTEST_SUITE_REGISTRATION(InternationalSocietyBiomechanicsCalibrationTest)  
CXXTEST_TEST_REGISTRATION(InternationalSocietyBiomechanicsCalibrationTest, reconstructUpperLimbBothWithCustomTranslator)