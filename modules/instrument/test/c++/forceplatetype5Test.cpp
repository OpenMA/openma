#include <cxxtest/TestDrive.h>

#include "forceplateTest_def.h"

#include <openma/instrument/forceplatetype5.h>

CXXTEST_SUITE(ForcePlateType5Test)
{
  CXXTEST_TEST(wrench)
  {
    ma::instrument::ForcePlateType5 fp("FP");
    forceplatetest_fill_gait1_type5(&fp);
    forceplatetest_compare_gait1_wrench_at_origin(&fp);
    forceplatetest_compare_gait1_wrench_position_at_cop(&fp);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::instrument::ForcePlateType5 fp("FP");
    forceplatetest_fill_gait1_type5(&fp);
    auto fp5_ = static_cast<ma::instrument::ForcePlateType5*>(fp.clone());
    TS_ASSERT_EQUALS(fp5_->type(), 5);
    forceplatetest_compare_fp5_clone(&fp, fp5_);
    forceplatetest_compare_gait1_wrench_at_origin(fp5_);
    forceplatetest_compare_gait1_wrench_position_at_cop(fp5_);
    delete fp5_;
  };
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType5Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType5Test, wrench)
CXXTEST_TEST_REGISTRATION(ForcePlateType5Test, clone)