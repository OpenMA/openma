#include <cxxtest/TestDrive.h>

#include "forceplateTest_def.h"

#include <openma/instrument/forceplatetype2.h>

CXXTEST_SUITE(ForcePlateType2Test)
{
  CXXTEST_TEST(wrench)
  {
    ma::instrument::ForcePlateType2 fp("FP");
    forceplatetest_fill_sample10_type2(&fp);
    forceplatetest_compare_sample10_wrench_at_origin(&fp, fp2data);
  };
  
  CXXTEST_TEST(pointOfApplicationCrossVerification)
  {
    ma::instrument::ForcePlateType2 fp("FP");
    forceplatetest_fill_sample10_type2(&fp);
    forceplatetest_cross_verification_pwa(&fp, {1e-15,1e-10,1e-10});
  };
  
  CXXTEST_TEST(clone)
  {
    ma::instrument::ForcePlateType2 fp("FP");
    forceplatetest_fill_sample10_type2(&fp);
    auto fp2_ = static_cast<ma::instrument::ForcePlateType2*>(fp.clone());
    TS_ASSERT_EQUALS(fp2_->type(), 2);
    forceplatetest_compare_fp2_clone(&fp, fp2_);
    forceplatetest_compare_sample10_wrench_at_origin(fp2_, fp2data);
    delete fp2_;
  };
  
  CXXTEST_TEST(nodeid)
  {
    ma::Node root("root");
    ma::instrument::ForcePlateType2 fp("FP", &root);
    TS_ASSERT_EQUALS(root.findChild<ma::instrument::ForcePlateType2*>(), &fp);
  };
  
  CXXTEST_TEST(softReset)
  {
    ma::instrument::ForcePlateType2 fp("FP");
    forceplatetest_fill_sample10_type2(&fp);
    fp.setSoftResetEnabled(true);
    auto w = fp.wrench(ma::instrument::Location::Origin,false);
    for (unsigned i = 0 ; i < sample10_fpsamples ; ++i)
    {
      const std::string s = std::to_string(i);
      TSM_ASSERT_DELTA(s, w->data()[i],                      fp2data[i]                      - -35.56109, 1e-4);
      TSM_ASSERT_DELTA(s, w->data()[i+  sample10_fpsamples], fp2data[i+  sample10_fpsamples] - -53.75195, 1e-4);
      TSM_ASSERT_DELTA(s, w->data()[i+2*sample10_fpsamples], fp2data[i+2*sample10_fpsamples] - -209.09763, 1e-4);
      TSM_ASSERT_DELTA(s, w->data()[i+3*sample10_fpsamples], fp2data[i+3*sample10_fpsamples] - -17279.31169, 1e-4);
      TSM_ASSERT_DELTA(s, w->data()[i+4*sample10_fpsamples], fp2data[i+4*sample10_fpsamples] - -8614.09473, 1e-4);
      TSM_ASSERT_DELTA(s, w->data()[i+5*sample10_fpsamples], fp2data[i+5*sample10_fpsamples] - -4563.57997, 1e-4);
    }
  }
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType2Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType2Test, wrench)
CXXTEST_TEST_REGISTRATION(ForcePlateType2Test, pointOfApplicationCrossVerification)
CXXTEST_TEST_REGISTRATION(ForcePlateType2Test, clone)
CXXTEST_TEST_REGISTRATION(ForcePlateType2Test, nodeid)
CXXTEST_TEST_REGISTRATION(ForcePlateType2Test, softReset)