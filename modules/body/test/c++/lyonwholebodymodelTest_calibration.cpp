#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "lyonwholebodymodelTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(LyonWholeBodyModelCalibrationTest)
{
  CXXTEST_TEST(calibrateUpperLimbBothWithCustomTranslator)
  {
    ma::Node root("root");
    generate_trial_from_c3d_file(&root, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/UpperLimbStatic.c3d"));
    ma::body::LyonWholeBodyModel helper(ma::body::Region::Upper, ma::body::Side::Both);
    helper.setProperty("sex",ma::Sex::Male);
    set_isb_custom_landmarks_translator(&helper);
    TS_ASSERT_EQUALS(helper.calibrate(&root, nullptr), true);
    const double reftorsorelscs[12] = {0.9980485036,-0.0624434499,0.0,0.0624434499,0.9980485036,0.0,0.0,0.0,1.0,86.2581302099,-21.0216470741,0.0};
    compare_segment_relscs(&helper, "Torso.SCS", reftorsorelscs);
  };
  
  CXXTEST_TEST(calibrateFullBodyWithCustomTranslator)
  {
    ma::Node root("root");
    generate_trial_from_c3d_file(&root, OPENMA_TDD_PATH_IN("c3d/lyonwholebodymodel/SprintCalib.c3d"));
    ma::body::LyonWholeBodyModel helper(ma::body::Region::Full, ma::body::Side::Both);
    helper.setProperty("sex",ma::Sex::Male);
    set_isb_custom_landmarks_translator2(&helper);
    TS_ASSERT_EQUALS(helper.calibrate(&root, nullptr), true);
    // Note: The original model doesn't use the same definitions (TF/SF) than in OpenMA for the torso
    const double refrarmlscs[12] = {0.99708,-0.06959,0.03150,0.07614,0.93869,-0.33626,-0.00617,0.33768,0.94124,23.49057,50.02477,-103.74997};
    compare_segment_relscs(&helper, "R.Arm.SCS", refrarmlscs);
    const double reflarmlscs[12] = {0.99748,-0.06440,-0.02992,0.07091,0.92560,0.37181,0.00375,-0.37299,0.92783,20.39282,46.00213,106.92687};
    compare_segment_relscs(&helper, "L.Arm.SCS", reflarmlscs);
    const double refpelvisscs[12] = {1.,0.,0.,0.,1.,0.,0.,0.,1.,68.40228,-4.69427,-0.0};
    compare_segment_relscs(&helper, "Pelvis.SCS", refpelvisscs);
    const double refrthighlscs[12] = {0.99635,-0.08311,0.01937,0.08471,0.99073,-0.10621,-0.01037,0.10747,0.99415,39.65977,449.51689,-111.26055};
    compare_segment_relscs(&helper, "R.Thigh.SCS", refrthighlscs);
    const double reflthighlscs[12] = {0.99732,-0.07278,-0.00743,0.07315,0.99308,0.09189,0.00069,-0.09219,0.99574,34.11390,456.74214,105.22313};
    compare_segment_relscs(&helper, "L.Thigh.SCS", reflthighlscs);
  }
};

CXXTEST_SUITE_REGISTRATION(LyonWholeBodyModelCalibrationTest)  
CXXTEST_TEST_REGISTRATION(LyonWholeBodyModelCalibrationTest, calibrateUpperLimbBothWithCustomTranslator)
CXXTEST_TEST_REGISTRATION(LyonWholeBodyModelCalibrationTest, calibrateFullBodyWithCustomTranslator)