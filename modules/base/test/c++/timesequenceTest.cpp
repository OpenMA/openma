#include <cxxtest/TestDrive.h>

#include <openma/base/timesequence.h>

CXXTEST_SUITE(TimeSequenceTest)
{
  CXXTEST_TEST(accessor)
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
  
  CXXTEST_TEST(findWithType)
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
  
  CXXTEST_TEST(clone)
  {
    ma::Node root("root");
    ma::TimeSequence bar("1D1C_Signal(analog)",1,100,1000.0,2.39,ma::TimeSequence::Analog,"V", -1.0,234,{{-10.0,10.0}}, &root);
    for (unsigned i = 0 ; i < 100 ; ++i)
      bar.data()[i] = double(i);
    
    auto rootcloned = root.clone();
    TS_ASSERT_EQUALS(rootcloned->hasChildren(),true);
    TS_ASSERT_EQUALS(rootcloned->hasParents(),false);
    TS_ASSERT_EQUALS(rootcloned->name(),"root");
    auto barcloned = rootcloned->findChild<ma::TimeSequence*>();
    TS_ASSERT_DIFFERS(barcloned,nullptr);
    TS_ASSERT_EQUALS(barcloned->hasParents(),true);
    TS_ASSERT_EQUALS(barcloned->hasChildren(),false);
    TS_ASSERT_EQUALS(barcloned->name(),"1D1C_Signal(analog)");
    TS_ASSERT_EQUALS(barcloned->unit(),"V");
    TS_ASSERT_EQUALS(barcloned->type(),ma::TimeSequence::Analog);
    TS_ASSERT_EQUALS(barcloned->sampleRate(),1000.0);
    TS_ASSERT_EQUALS(barcloned->startTime(),2.39);
    TS_ASSERT_EQUALS(barcloned->dimensions().size(),1ul);
    TS_ASSERT_EQUALS(barcloned->dimensions().front(),1u);
    TS_ASSERT_EQUALS(barcloned->samples(),100u);
    TS_ASSERT_EQUALS(barcloned->components(),1u);
    TS_ASSERT_EQUALS(barcloned->elements(),100u);
    TS_ASSERT_EQUALS(barcloned->duration(),0.1);
    for (unsigned i = 0 ; i < barcloned->elements() ; ++i)
      TS_ASSERT_EQUALS(barcloned->data()[i], bar.data()[i]);
    delete rootcloned; // This will delete also barcloned
    
    barcloned = bar.clone(&root);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.children().size(),2ul);
    auto child = root.child<ma::TimeSequence*>(1);
    TS_ASSERT_EQUALS(child, barcloned);
    TS_ASSERT_EQUALS(barcloned->hasParents(),true);
    TS_ASSERT_EQUALS(barcloned->parents().front(),&root);
    TS_ASSERT_EQUALS(barcloned->hasParents(),true);
    TS_ASSERT_EQUALS(barcloned->hasChildren(),false);
    TS_ASSERT_EQUALS(barcloned->name(),"1D1C_Signal(analog)");
    TS_ASSERT_EQUALS(barcloned->unit(),"V");
    TS_ASSERT_EQUALS(barcloned->type(),ma::TimeSequence::Analog);
    TS_ASSERT_EQUALS(barcloned->sampleRate(),1000.0);
    TS_ASSERT_EQUALS(barcloned->startTime(),2.39);
    TS_ASSERT_EQUALS(barcloned->dimensions().size(),1ul);
    TS_ASSERT_EQUALS(barcloned->dimensions().front(),1u);
    TS_ASSERT_EQUALS(barcloned->samples(),100u);
    TS_ASSERT_EQUALS(barcloned->components(),1u);
    TS_ASSERT_EQUALS(barcloned->elements(),100u);
    TS_ASSERT_EQUALS(barcloned->duration(),0.1);
    for (unsigned i = 0 ; i < barcloned->elements() ; ++i)
      TS_ASSERT_EQUALS(barcloned->data()[i], bar.data()[i]);
  };
  
  CXXTEST_TEST(copy)
  {
    ma::Node root("root");
    ma::TimeSequence foo("foo",4,0,100.0,0.0,ma::TimeSequence::Marker,"mm",1.0,0.0,ma::TimeSequence::InfinityRange,&root);
    ma::TimeSequence bar("1D1C_Signal(analog)",1,100,1000.0,2.39,ma::TimeSequence::Analog,"V", -1.0,234,{{-10.0,10.0}});
    for (unsigned i = 0 ; i < 100 ; ++i)
      bar.data()[i] = double(i);
    
    foo.copy(&bar);
    TS_ASSERT_EQUALS(foo.hasParents(),false);
    TS_ASSERT_EQUALS(foo.hasChildren(),false);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    TS_ASSERT_EQUALS(foo.name(),"1D1C_Signal(analog)");
    TS_ASSERT_EQUALS(foo.unit(),"V");
    TS_ASSERT_EQUALS(foo.type(),ma::TimeSequence::Analog);
    TS_ASSERT_EQUALS(foo.sampleRate(),1000.0);
    TS_ASSERT_EQUALS(foo.startTime(),2.39);
    TS_ASSERT_EQUALS(foo.dimensions().size(),1ul);
    TS_ASSERT_EQUALS(foo.dimensions().front(),1u);
    TS_ASSERT_EQUALS(foo.samples(),100u);
    TS_ASSERT_EQUALS(foo.components(),1u);
    TS_ASSERT_EQUALS(foo.elements(),100u);
    TS_ASSERT_EQUALS(foo.duration(),0.1);
    for (unsigned i = 0 ; i < foo.elements() ; ++i)
      TS_ASSERT_EQUALS(foo.data()[i], bar.data()[i]);
  };
  
  CXXTEST_TEST(checkCommonProperties)
  {
    std::vector<ma::TimeSequence*> tss;
    ma::TimeSequence foo1("foo1",4,5,100.0,1.0,ma::TimeSequence::Marker,"mm");
    tss.push_back(&foo1);
    ma::TimeSequence foo2("foo2",4,5,100.0,1.0,ma::TimeSequence::Marker,"mm");
    tss.push_back(&foo2);
    ma::TimeSequence foo3("foo3",4,5,100.0,1.0,ma::TimeSequence::Marker,"mm");
    tss.push_back(&foo3);
    double sampleRate = 0., startTime = 0.;
    unsigned samples = 0;
    TS_ASSERT_EQUALS(ma::compare_timesequences_properties(tss, sampleRate, startTime, samples), true);
    TS_ASSERT_EQUALS(sampleRate, 100.0);
    TS_ASSERT_EQUALS(startTime, 1.0);
    TS_ASSERT_EQUALS(samples, 5u);
    foo1.setSampleRate(50.0);
    TS_ASSERT_EQUALS(ma::compare_timesequences_properties(tss, sampleRate, startTime, samples), false);
    foo1.setSampleRate(100.0);
    foo2.setStartTime(0.0);
    TS_ASSERT_EQUALS(ma::compare_timesequences_properties(tss, sampleRate, startTime, samples), false);
    foo2.setStartTime(1.0);
    foo3.resize(10);
    TS_ASSERT_EQUALS(ma::compare_timesequences_properties(tss, sampleRate, startTime, samples), false);
    foo3.resize(5);
    TS_ASSERT_EQUALS(ma::compare_timesequences_properties(tss, sampleRate, startTime, samples), true);
    TS_ASSERT_EQUALS(sampleRate, 100.0);
    TS_ASSERT_EQUALS(startTime, 1.0);
    TS_ASSERT_EQUALS(samples, 5u);
  };
};

CXXTEST_SUITE_REGISTRATION(TimeSequenceTest)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, accessor)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, findWithType)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, clone)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, copy)
CXXTEST_TEST_REGISTRATION(TimeSequenceTest, checkCommonProperties)