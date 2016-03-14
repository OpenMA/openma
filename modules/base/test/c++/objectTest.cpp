#include <cxxtest/TestDrive.h>

#include "objectTest_def.h"

CXXTEST_SUITE(ObjectTest)
{
  CXXTEST_TEST(defaultPimplConstructor)
  {
    ObjectDefaultPimpl obj;
    TS_ASSERT_EQUALS(obj.timestamp(),0ul);
  };
  
  CXXTEST_TEST(defaultPimplModified)
  {
    ObjectDefaultPimpl obj;
    unsigned long ts = obj.timestamp();
    obj.modified();
    TS_ASSERT_DIFFERS(ts, obj.timestamp());
  };
  
  CXXTEST_TEST(customPimplConstructor)
  {
    ObjectCustomPimpl obj("foo");
    TS_ASSERT_EQUALS(obj.timestamp(),0ul);
    TS_ASSERT_EQUALS(obj.name(),"foo");
    TS_ASSERT_EQUALS(obj.version(),1);
  };
  
  CXXTEST_TEST(customPimplMutate)
  {
    ObjectCustomPimpl obj("foo");
    unsigned long ts = obj.timestamp();
    obj.setName("bar");
    TS_ASSERT_EQUALS(obj.name(),"bar");
    TS_ASSERT_DIFFERS(ts, obj.timestamp());
    ts = obj.timestamp();
    obj.setName("bar");
    TS_ASSERT_EQUALS(obj.name(),"bar");
    TS_ASSERT_EQUALS(ts, obj.timestamp());
    obj.setVersion(2);
    TS_ASSERT_EQUALS(obj.version(),2);
    TS_ASSERT_DIFFERS(ts, obj.timestamp());
  };
  
  CXXTEST_TEST(customPimplCopy)
  {
    ObjectCustomPimpl obj("foo");
    ObjectCustomPimpl obj2("bar");
    obj2.copy(&obj);
    obj2.setName("bar");
    TS_ASSERT_EQUALS(obj2.name(),"bar");
    TS_ASSERT_EQUALS(obj.name(),"foo");
    TS_ASSERT_DIFFERS(obj.timestamp(), obj2.timestamp());
    obj.setName("foo");
    TS_ASSERT_EQUALS(obj2.name(),"bar");
    TS_ASSERT_DIFFERS(obj.name(),obj2.name());
    TS_ASSERT_DIFFERS(obj.timestamp(), obj2.timestamp());
  };
  
  CXXTEST_TEST(customPimplClone)
  {
    ObjectCustomPimpl obj("foo");
    auto pobj2 = std::shared_ptr<ObjectCustomPimpl>(obj.clone());
    TS_ASSERT_EQUALS(pobj2->name(),"foo");
    TS_ASSERT_EQUALS(obj.name(),pobj2->name());
    TS_ASSERT_EQUALS(obj.timestamp(), pobj2->timestamp());
    pobj2->setName("bar");
    TS_ASSERT_EQUALS(pobj2->name(),"bar");
    TS_ASSERT_EQUALS(obj.name(),"foo");
    TS_ASSERT_DIFFERS(obj.timestamp(), pobj2->timestamp());
    obj.setName("foo");
    TS_ASSERT_EQUALS(pobj2->name(),"bar");
    TS_ASSERT_DIFFERS(obj.name(),pobj2->name());
    TS_ASSERT_DIFFERS(obj.timestamp(), pobj2->timestamp());
  }
};

CXXTEST_SUITE_REGISTRATION(ObjectTest)
CXXTEST_TEST_REGISTRATION(ObjectTest, defaultPimplConstructor)
CXXTEST_TEST_REGISTRATION(ObjectTest, defaultPimplModified)
CXXTEST_TEST_REGISTRATION(ObjectTest, customPimplConstructor)
CXXTEST_TEST_REGISTRATION(ObjectTest, customPimplMutate)
CXXTEST_TEST_REGISTRATION(ObjectTest, customPimplCopy)
CXXTEST_TEST_REGISTRATION(ObjectTest, customPimplClone)