#include <cxxtest/TestDrive.h>

#include "forceplateTest_def.h"

#include <openma/instrument/forceplatetype4.h>

CXXTEST_SUITE(ForcePlateType4Test)
{
  CXXTEST_TEST(wrench)
  {
    ma::instrument::ForcePlateType4 fp("FP");
    forceplatetest_fill_sample10_type4(&fp);
    forceplatetest_compare_sample10_wrench_at_origin(&fp, fp4dataout);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::instrument::ForcePlateType4 fp("FP");
    forceplatetest_fill_sample10_type4(&fp);
    auto fp4_ = static_cast<ma::instrument::ForcePlateType4*>(fp.clone());
    TS_ASSERT_EQUALS(fp4_->type(), 4);
    forceplatetest_compare_fp4_clone(&fp, fp4_);
    forceplatetest_compare_sample10_wrench_at_origin(fp4_, fp4dataout);
    delete fp4_;
  };
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType4Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType4Test, wrench)
CXXTEST_TEST_REGISTRATION(ForcePlateType4Test, clone)