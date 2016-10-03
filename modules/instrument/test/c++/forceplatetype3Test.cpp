#include <cxxtest/TestDrive.h>

#include "forceplateTest_def.h"

#include <openma/instrument/forceplatetype3.h>



CXXTEST_SUITE(ForcePlateType3Test)
{
  CXXTEST_TEST(wrench_local)
  {
    ma::instrument::ForcePlateType3 fp("FP");
    forceplatetest_fill_gaitfb1_type3(&fp);
    forceplatetest_compare_gaitfb1_type3_wrench_local(&fp);
  };
  
  CXXTEST_TEST(wrench_global)
  {
    ma::instrument::ForcePlateType3 fp("FP");
    forceplatetest_fill_gaitfb1_type3(&fp);
    forceplatetest_compare_gaitfb1_type3_wrench_global(&fp);
  };
  
  CXXTEST_TEST(clone)
  {
    ma::instrument::ForcePlateType3 fp("FP");
    forceplatetest_fill_gaitfb1_type3(&fp);
    auto fp3_ = static_cast<ma::instrument::ForcePlateType3*>(fp.clone());
    TS_ASSERT_EQUALS(fp3_->type(), 3);
    forceplatetest_compare_fp3_clone(&fp, fp3_);
    forceplatetest_compare_gaitfb1_type3_wrench_local(fp3_);
    forceplatetest_compare_gaitfb1_type3_wrench_global(fp3_);
    delete fp3_;
  };
  
  CXXTEST_TEST(nodeid)
  {
    ma::Node root("root");
    ma::instrument::ForcePlateType3 fp("FP", &root);
    TS_ASSERT_EQUALS(root.findChild<ma::instrument::ForcePlateType3*>(), &fp);
  };
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType3Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType3Test, wrench_local)
CXXTEST_TEST_REGISTRATION(ForcePlateType3Test, wrench_global)
CXXTEST_TEST_REGISTRATION(ForcePlateType3Test, clone)
CXXTEST_TEST_REGISTRATION(ForcePlateType3Test, nodeid)