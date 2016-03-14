#include <cxxtest/TestDrive.h>

#include "handlerpluginTest_def.h"

#include <openma/io/file.h>

#include "test_file_path.h"

CXXTEST_SUITE(HandlerPluginTest)
{
  CXXTEST_TEST(supportedFormats)
  {
    DummyHandlerPlugin dp;
    TS_ASSERT_EQUALS(dp.supportedFormats().size(), 2ul);
    TS_ASSERT_EQUALS(dp.supportedFormats()[0], std::string("openma.dummy1"));
    TS_ASSERT_EQUALS(dp.supportedFormats()[1], std::string("openma.dummy2"));
  };

  CXXTEST_TEST(capabilities)
  {
    DummyHandlerPlugin dp;
    ma::io::Capability cap = ma::io::Capability::None;
    cap = dp.capabilities("openma.dummy1");
    TS_ASSERT_EQUALS(cap, ma::io::Capability::CanReadAndWrite);
    cap = dp.capabilities("openma.dummy2");
    TS_ASSERT_EQUALS(cap, ma::io::Capability::CanRead);
  };
  
  CXXTEST_TEST(dummyOne)
  {
    DummyHandlerPlugin dp;
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    std::string format;
    TS_ASSERT_EQUALS(dp.detectSignature(&file, &format), ma::io::Signature::Valid);
    TS_ASSERT_EQUALS(format, "openma.dummy1");
    auto handler = dp.create(&file, "openma.dummy1");
    TS_ASSERT_DIFFERS(handler, nullptr);
    ma::Node root("root");
    TS_ASSERT_EQUALS(handler->read(&root), true);
    TS_ASSERT_EQUALS(handler->errorCode(), ma::io::Error::None);
    TS_ASSERT_EQUALS(root.property("Dummy:One").cast<std::string>(), "I was added by a dummy handler!");
    delete handler;
  }
  
  CXXTEST_TEST(dummyTwo)
  {
    DummyHandlerPlugin dp;
    DummyDeviceTwo dd;
    TS_ASSERT_EQUALS(dp.detectSignature(&dd), ma::io::Signature::NotAvailable);
    std::string format;
    TS_ASSERT_EQUALS(dp.detectExtension(dd.name(), &format), true);
    TS_ASSERT_EQUALS(format, "openma.dummy2");
    auto handler = dp.create(&dd, "openma.dummy2");
    TS_ASSERT_DIFFERS(handler, nullptr);
    ma::Node root("root");
    TS_ASSERT_EQUALS(handler->read(&root), true);
    TS_ASSERT_EQUALS(handler->errorCode(), ma::io::Error::None);
    TS_ASSERT_EQUALS(root.property("Dummy:Two").cast<std::string>(), "I was added by the other dummy handler.");
    delete handler;
  }
};

CXXTEST_SUITE_REGISTRATION(HandlerPluginTest)
CXXTEST_TEST_REGISTRATION(HandlerPluginTest, supportedFormats)
CXXTEST_TEST_REGISTRATION(HandlerPluginTest, capabilities)
CXXTEST_TEST_REGISTRATION(HandlerPluginTest, dummyOne)
CXXTEST_TEST_REGISTRATION(HandlerPluginTest, dummyTwo)