#include <cxxtest/TestDrive.h>

#include "plugingaitTest_def.h"

CXXTEST_SUITE(PluginGaitTest)
{   
  CXXTEST_TEST(clone)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
    fill_parameters_with_fake_value(&helper);
    ma::Node root("root");
    auto clonedhelper = helper.clone(&root);
    helper.setLeftWristWidth(24.0);
    compare_parameters_with_fake_value(clonedhelper);
  };
  
  CXXTEST_TEST(copy)
  {
    ma::body::PluginGait helper(ma::body::Region::Full, ma::body::Side::Both, ma::body::PluginGait::Basic);
    ma::body::PluginGait copyhelper(ma::body::Region::Lower, ma::body::Side::Right, ma::body::PluginGait::Basic);
    fill_parameters_with_fake_value(&helper);
    copyhelper.setMarkerDiameter(16.0);
    copyhelper.setHeadOffsetEnabled(false);
    copyhelper.setLeftShoulderOffset(71.3);
    copyhelper.setRightElbowWidth(50.1);
    copyhelper.setLeftLegLength(942.0);
    copyhelper.setRightAsisTrochanterAPDistance(123.5); 
    copyhelper.setRightKneeWidth(93.0);
    copyhelper.copy(&helper);
    compare_parameters_with_fake_value(&helper);
    compare_parameters_with_fake_value(&copyhelper);
  };
};

CXXTEST_SUITE_REGISTRATION(PluginGaitTest)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, clone)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, copy)
