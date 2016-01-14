#include <cxxtest/TestDrive.h>

#include <openma/base/event.h>

CXXTEST_SUITE(EventTest)
{
  CXXTEST_TEST(accessor)
  {
    ma::Event evtA("evtA",0.0,"Right","JDoe");
    
    TS_ASSERT_EQUALS(evtA.name(),"evtA");
    TS_ASSERT_EQUALS(evtA.property("name").cast<std::string>(),"evtA");
    TS_ASSERT_EQUALS(evtA.time(),0.0);
    TS_ASSERT_EQUALS(evtA.property("time"),0.0);
    TS_ASSERT_EQUALS(evtA.context(),"Right");
    TS_ASSERT_EQUALS(evtA.property("context").cast<std::string>(),"Right");
    TS_ASSERT_EQUALS(evtA.subject(),"JDoe");
  };
  
  CXXTEST_TEST(mutator)
  {
    ma::Event evtA("evtA",0.0,"Right","JDoe");
    unsigned long ts = evtA.timestamp();
    
    evtA.setName("foo");
    TS_ASSERT_EQUALS(evtA.name(),"foo");
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    evtA.setProperty("name",std::string("event A"));
    TS_ASSERT_EQUALS(evtA.property("name").cast<std::string>(),"event A");
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    
    evtA.setTime(0.1);
    TS_ASSERT_EQUALS(evtA.time(),0.1);
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    evtA.setProperty("time",0.2);
    TS_ASSERT_EQUALS(evtA.property("time"),0.2);
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    
    evtA.setContext("General");
    TS_ASSERT_EQUALS(evtA.context(),"General");
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    evtA.setProperty("context",std::string("Other"));
    TS_ASSERT_EQUALS(evtA.property("context").cast<std::string>(),"Other");
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    
    evtA.setSubject("Me");
    TS_ASSERT_EQUALS(evtA.subject(),"Me");
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
    evtA.setProperty("subject",std::string("You"));
    TS_ASSERT_EQUALS(evtA.property("subject").cast<std::string>(),"You");
    TS_ASSERT_DIFFERS(evtA.timestamp(),ts);
    ts = evtA.timestamp();
  };
  
  CXXTEST_TEST(findChildren)
  {
    ma::Node events("Events");
    ma::Event evtA("Foo",0.0,"Right","JDoe",&events);
    ma::Event evtB("Bar",0.0,"Left","JDoe",&events);
    ma::Event evtC("Toto",1.1,"Left","JDoe",&events);
    ma::Event evtD("Toto",1.5,"Right","Babar",&events);

    TS_ASSERT_EQUALS(events.children().size(),4ul);

    TS_ASSERT_EQUALS(events.findChild("Toto",{{"time",1.5}},false),&evtD);
    TS_ASSERT_EQUALS(events.findChild<ma::Event*>("Toto",{{"time",1.5}},false),&evtD);
    TS_ASSERT_EQUALS(events.findChild<ma::Event*>({},{{"time",0.0},{"context","Left"}},false),&evtB);

    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>({},{{"time",0.0}},false).size(),2ul);
    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>({},{{"context","Right"}},false).size(),2ul);
    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>({},{{"context","Left"}},false).size(),2ul);
    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>({},{{"subject","JDoe"}},false).size(),3ul);
    
    auto lst = events.findChildren<ma::Event*>({},{{"context","Left"}});
    auto it = lst.cbegin();
    TS_ASSERT_EQUALS(*it,&evtB);
    it++;
    TS_ASSERT_EQUALS(*it,&evtC);
    
#if !defined(__GNUC__) || (defined(__GNUC__) && defined(__clang__)) || (__GNUC__ >= 4 && __GNUC_MINOR__ >= 9)
    // The C++11 regex feature was implemented in GCC 4.9
    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>(std::regex("Foo|Bar")).size(),2u);
    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>(std::regex("Foo|Bar|Toto")).size(),4u);
    TS_ASSERT_EQUALS(events.findChildren<ma::Event*>(std::regex("Foo|Bar|Toto"),{{"context","Right"}}).size(),2u);
#endif
  };
};

CXXTEST_SUITE_REGISTRATION(EventTest)
CXXTEST_TEST_REGISTRATION(EventTest, accessor)
CXXTEST_TEST_REGISTRATION(EventTest, mutator)
CXXTEST_TEST_REGISTRATION(EventTest, findChildren)