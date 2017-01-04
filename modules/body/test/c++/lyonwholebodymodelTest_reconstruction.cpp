#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "lyonwholebodymodelTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(LyonWholeBodyModelReconstructionTest)
{
  CXXTEST_TEST(reconstructUpperLimbBothWithCustomTranslator)
  {
    ma::Node rootTrials("rootTrials"), rootModels("rootModels");
    generate_trial_from_c3d_file(&rootTrials, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/UpperLimbStatic.c3d"));
    ma::body::LyonWholeBodyModel helper(ma::body::Region::Upper, ma::body::Side::Both);
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
  
  CXXTEST_TEST(reconstructFullBodyBothWithCustomTranslator)
  {
    ma::Node rootStaticTrials("rootStaticTrials"), rootDynamicTrials("rootDynamicTrials"), rootModels("rootModels");
    generate_trial_from_c3d_file(&rootStaticTrials, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/SprintCalib.c3d"));
    ma::body::LyonWholeBodyModel helper(ma::body::Region::Lower, ma::body::Side::Both);
    helper.setSex(ma::Sex::Male);
    set_isb_custom_landmarks_translator2(&helper);
    TS_ASSERT_EQUALS(helper.calibrate(&rootStaticTrials, nullptr), true);
    generate_trial_from_c3d_file(&rootDynamicTrials, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/SprintMotion.c3d"));
    TS_ASSERT_EQUALS(helper.reconstruct(&rootModels, &rootDynamicTrials), true);
    auto model = rootModels.findChild<ma::body::Model*>();
    // NOTE: There is no test on the shanks due to a difference in the computation of the U-axis in the original data.
    // NOTE: There is no test on the hands and the torso because makers are missing
    // const double refrarm65[12] = {0.65402, 0.62141, -0.43141, 0.31926, 0.29027, 0.90212, 0.68581, -0.72773, -0.00855, 120.45673, -57.81515, 698.06440};
//     compare_segment_motion(model, "R.Arm.SCS", 65, refrarm65);
//     const double refrforearm100[12] = {-0.61999, 0.78453, 0.01065, 0.16057, 0.11358, 0.98047, 0.76800, 0.60959, -0.19640, 19.29595, -150.22911, 409.27459};
//     compare_segment_motion(model, "R.Forearm.SCS", 100, refrforearm100);
//     const double reflarm0[12] = {0.66085, -0.61337, -0.43251, 0.26771, -0.34572, 0.89934, -0.70115, -0.71011, -0.06426, 86.02440, 208.41668, 679.71141};
//     compare_segment_motion(model, "L.Arm.SCS", 0, reflarm0);
//     const double reflforearm455[12] = {-0.60022, -0.79664, 0.07135, 0.22122, -0.07962, 0.97197, -0.76863, 0.59918, 0.22402, 29.11515, 313.40861, 410.57111};
//     compare_segment_motion(model, "L.Forearm.SCS", 455, reflforearm455);
    TS_WARN("MODIFY THE IMPLEMENTATION TO GIVE THE POSSIBILITY TO REMOVE SEGMENT (OR INVALID POSE) IN CASE OF MISSING MARKERS");
    const double refpelvis699[12] = {0.88559, -0.21975, -0.40920, 0.44259, 0.13201, 0.88696, -0.14089, -0.96658, 0.21416, 932.99631, 21.33425, 838.24317};
    compare_segment_motion(model, "Pelvis.SCS", 699, refpelvis699, true, false);
    const double refrthigh2[12] = {0.98657, 0.13929, -0.08536, 0.07414, 0.08383, 0.99372, 0.14557, -0.98670, 0.07238, -337.63073, 20.35287, 526.23056};
    compare_segment_motion(model, "R.Thigh.SCS", 2, refrthigh2);
    const double refrfoot678[12] = {0.88891, -0.32512, -0.32269, 0.29834, -0.12365, 0.94642, -0.34760, -0.93755, -0.01291, 524.93696, -106.27201, 113.06468};
    compare_segment_motion(model, "R.Foot.SCS", 678, refrfoot678);
    const double reflthigh385[12] = {0.96309, -0.25188, 0.09497, -0.09864, -0.00196, 0.99512, -0.25046, -0.96776, -0.02673, -272.01179, 151.92236, 881.52430};
    compare_segment_motion(model, "L.Thigh.SCS", 385, reflthigh385);
    const double reflfoot274[12] = {0.42335, 0.01933, -0.90576, 0.90454, -0.06517, 0.42139, -0.05088, -0.99769, -0.04507, -548.68966, 169.59689, 207.65759};
    compare_segment_motion(model, "L.Foot.SCS", 274, reflfoot274);
  };
};

CXXTEST_SUITE_REGISTRATION(LyonWholeBodyModelReconstructionTest)  
CXXTEST_TEST_REGISTRATION(LyonWholeBodyModelReconstructionTest, reconstructUpperLimbBothWithCustomTranslator)
CXXTEST_TEST_REGISTRATION(LyonWholeBodyModelReconstructionTest, reconstructFullBodyBothWithCustomTranslator)