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
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType4Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType4Test, wrench)