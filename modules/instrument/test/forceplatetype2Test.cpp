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
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType2Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType2Test, wrench)