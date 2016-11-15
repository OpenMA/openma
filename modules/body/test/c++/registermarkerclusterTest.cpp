#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "registermarkerclusterTest_def.h"
#include "test_file_path.h"

#include <openma/io.h>

CXXTEST_SUITE(RegisterMarkerClusterTest)
{
  CXXTEST_TEST(cube)
  {
    ma::Node root("root");
    ma::Trial trial("trial",&root);
    auto tss = ma::make_nodes<ma::TimeSequence*>(8,4,1,100.0,0.0,ma::TimeSequence::Marker,"mm",trial.timeSequences());
    set_pt_data(tss[0], 10.0, 10.0, 10.0);
    set_pt_data(tss[1], 10.0, 20.0, 10.0);
    set_pt_data(tss[2], 20.0, 20.0, 10.0);
    set_pt_data(tss[3], 20.0, 10.0, 10.0);
    set_pt_data(tss[4], 10.0, 10.0,  0.0);
    set_pt_data(tss[5], 10.0, 20.0,  0.0);
    set_pt_data(tss[6], 20.0, 20.0,  0.0);
    set_pt_data(tss[7], 20.0, 10.0,  0.0);
    CubeHelper helper;
    TS_ASSERT_EQUALS(ma::body::register_marker_cluster(&helper,&root), true);
    auto mcr = helper.findChild("MarkerClusterRegistration",{},false);
    TS_ASSERT_DIFFERS(mcr, nullptr);
    compare_pt_data(mcr, "Cube.uname*1._TCS", -5., -5., 5.);
    compare_pt_data(mcr, "Cube.uname*2._TCS", -5.,  5., 5.);
    compare_pt_data(mcr, "Cube.uname*3._TCS",  5.,  5., 5.);
    compare_pt_data(mcr, "Cube.uname*4._TCS",  5., -5., 5.);
    compare_pt_data(mcr, "Cube.uname*5._TCS", -5., -5.,-5.);
    compare_pt_data(mcr, "Cube.uname*6._TCS", -5.,  5.,-5.);
    compare_pt_data(mcr, "Cube.uname*7._TCS",  5.,  5.,-5.);
    compare_pt_data(mcr, "Cube.uname*8._TCS",  5., -5.,-5.);
    compare_pose_data(mcr, "Cube.SCS._TCS", {{1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, -15.0, -15.0, -5.0}});
  };
  
  CXXTEST_TEST(lowerlimbs)
  {
    ma::Node trials("trials");
    TS_ASSERT_EQUALS(ma::io::read(&trials, OPENMA_TDD_PATH_IN("c3d/other/FunctionalStatic.c3d")), true);
    TS_ASSERT_EQUALS(trials.children().size(), 1u);
    LowerLimbsHelper helper;
    TS_ASSERT_EQUALS(ma::body::register_marker_cluster(&helper,&trials), true);
    auto mcr = helper.findChild("MarkerClusterRegistration",{},false);
    TS_ASSERT_DIFFERS(mcr, nullptr);
    compare_pt_data(mcr, "L.Thigh.LF01._TCS", -7.025244478951322, -37.370944620032674, 87.141969281512957);
    compare_pt_data(mcr, "L.Thigh.LF02._TCS", -0.165385196229863, 38.234319805021528, 105.922967136904840);
    compare_pt_data(mcr, "L.Thigh.LF03._TCS", 17.159762411867746, -4.788614959904180, -15.747916452835625);
    compare_pt_data(mcr, "L.Thigh.LF04._TCS", -6.529633962273351, 51.953804520632048, -93.866625118232832);
    compare_pt_data(mcr, "L.Thigh.LF05._TCS", -3.439498774412527, -48.028564745716835, -83.450394847349457);
    compare_pose_data(mcr, "L.Thigh.SCS._TCS", {{0.943699706253521, -0.031004933152791, 0.329347170228026, 0.051558704860279, 0.997216746468030, -0.053855923693202, -0.326760714239255, 0.067804532915642, 0.942671724910610, -345.384674707770614, -663.138239162530112, -749.377189117997091}});
    compare_pose_data(mcr, "R.Thigh.SCS._TCS", {{0.961054683310447, 0.139613641918686, 0.238499322173580, -0.110629588032540, 0.985199939898702, -0.130928120262010, -0.253248869570639, 0.099444001396735, 0.962276415926006, -312.806138621892785, -597.059384172363707, -678.807654291632957}});
    compare_pose_data(mcr, "L.Shank.SCS._TCS", {{0.977303599344294, 0.202073590223233, 0.063591971529287, -0.199436258571977, 0.978855597364385, -0.045463152939771, -0.071434259816288, 0.031748758128562, 0.996939899332850, -301.606060104188657, -753.530821459073422, -254.121676808772634}});
    compare_pose_data(mcr, "R.Shank.SCS._TCS", {{0.961944748941980, 0.251872967266288, 0.105935397027745, -0.258636959800861, 0.964370032994486, 0.055653953023869, -0.088143196038711, -0.080934836891729, 0.992814347785824, -302.967142857202191, -514.956067007683828, -328.673822323042543}});
  };
};

CXXTEST_SUITE_REGISTRATION(RegisterMarkerClusterTest)
CXXTEST_TEST_REGISTRATION(RegisterMarkerClusterTest, cube)
CXXTEST_TEST_REGISTRATION(RegisterMarkerClusterTest, lowerlimbs)
