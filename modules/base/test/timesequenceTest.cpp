#include <cxxtest/TestDrive.h>

#include <openma/base/timesequence.h>

CXXTEST_SUITE(TimeSequenceTest)
{
  CXXTEST_TEST(Accessor)
  {
    ma::TimeSequence foo("1D4C_Signal(markers)",4,0,100.0,0.0,ma::TimeSequence::Marker,"mm");
    ma::TimeSequence toto("1D4C_Signal(markers)",4,10,100.0,0.125,ma::TimeSequence::Marker,"mm");
    ma::TimeSequence bar("1D1C_Signal(analog)",1,100,1000.0,0.0,ma::TimeSequence::Analog,"V");
    ma::TimeSequence bar2("2D64x64C_Signal(pressure)",{32,32},2,200.0,0.0,ma::TimeSequence::Unknown,"Pa");
    
    TS_ASSERT_EQUALS(foo.data(),nullptr);
    TS_ASSERT_EQUALS(foo.name(),"1D4C_Signal(markers)");
    TS_ASSERT_EQUALS(foo.unit(),"mm");
    TS_ASSERT_EQUALS(foo.type(),ma::TimeSequence::Marker);
    TS_ASSERT_EQUALS(foo.sampleRate(),100.0);
    TS_ASSERT_EQUALS(foo.startTime(),0.0);
    TS_ASSERT_EQUALS(foo.dimensions().size(),1ul);
    TS_ASSERT_EQUALS(foo.dimensions().front(),4u);
    TS_ASSERT_EQUALS(foo.samples(),0u);
    TS_ASSERT_EQUALS(foo.components(),4u);
    TS_ASSERT_EQUALS(foo.elements(),0u);
    TS_ASSERT_EQUALS(foo.duration(),0.0);
    
    TS_ASSERT_EQUALS(toto.startTime(),0.125);
    
    TS_ASSERT_DIFFERS(toto.data(),nullptr);
    TS_ASSERT_DIFFERS(bar.data(),nullptr);
    TS_ASSERT_EQUALS(toto.duration(),0.1);
    TS_ASSERT_EQUALS(toto.duration(),bar.duration());
    
    TS_ASSERT_EQUALS(bar2.dimensions().size(),2ul);
    TS_ASSERT_EQUALS(bar2.components(),1024u);
    TS_ASSERT_EQUALS(bar2.elements(),2048u);
    
    TS_ASSERT_EQUALS(foo.property("type").cast<int>(),ma::TimeSequence::Marker);
    
    // Eigen::Matrix<double,Eigen::Dynamic,3> rv = Eigen::Matrix<double,Eigen::Dynamic,3>::Random(10,3);
    // Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,3>> values(toto.data(),toto.samples(),3);
    // values = rv;
    // // ma::TimeSequence::Values<ma::Dynamic,3> toto_values = toto.values<ma::Dynamic,3>();
    // // ma::TimeSequence::Values<5,4,ma::OuterStride<>> toto_values_slice = toto.values<5,4>(ma::OuterStride<>(toto.samples()));
    //
    // for (int i = 0 ; i < 30 ; ++i)
    //   TS_ASSERT_EQUALS(toto.data()[i],rv.data()[i]);
  };
  
  CXXTEST_TEST(FindWithType)
  {
    ma::Node root("root");
    ma::TimeSequence foo("foo",4,0,100.0,0.0,ma::TimeSequence::Marker,"mm",1.0,0.0,ma::TimeSequence::InfinityRange,&root);
    ma::TimeSequence bar("bar",4,0,100.0,0.0,ma::TimeSequence::Angle,"degree",1.0,0.0,ma::TimeSequence::InfinityRange,&root);
    ma::TimeSequence toto("toto",4,0,100.0,0.0,ma::TimeSequence::Marker,"mm",1.0,0.0,ma::TimeSequence::InfinityRange,&root);
    
    TS_ASSERT_EQUALS(root.children().size(),3ul);

    TS_ASSERT_EQUALS(root.findChild<ma::TimeSequence*>("foo",{{"type",ma::TimeSequence::Marker}},false),&foo);
    TS_ASSERT_EQUALS(root.findChild<ma::TimeSequence*>("toto",{{"type",ma::TimeSequence::Marker}},false),&toto);
    TS_ASSERT_EQUALS(root.findChild<ma::TimeSequence*>("bar",{{"type",ma::TimeSequence::Angle}},false),&bar);

    TS_ASSERT_EQUALS(root.findChildren<ma::TimeSequence*>({},{{"type",ma::TimeSequence::Marker}},false).size(),2ul);
    TS_ASSERT_EQUALS(root.findChildren<ma::TimeSequence*>({},{{"type",ma::TimeSequence::Angle}},false).size(),1ul);
  };
};

CXXTEST_SUITE_REGISTRATION(TimeSequenceTest)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, Accessor)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, FindWithType)