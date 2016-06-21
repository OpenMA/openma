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
};

CXXTEST_SUITE_REGISTRATION(ForcePlateType5Test)
CXXTEST_TEST_REGISTRATION(ForcePlateType5Test, wrench)