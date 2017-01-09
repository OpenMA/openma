#include <cxxtest/TestDrive.h>

#include <openma/io.h>
#include <openma/base/node.h>
#include <openma/base/timesequence.h>

#include "test_file_path.h"

CXXTEST_SUITE(BSFReaderTest)
{
  CXXTEST_TEST(capability)
  {
    ma::io::HandlerReader reader(nullptr,"amti.bsf");
    TS_ASSERT_EQUALS(reader.canRead(), false);
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Trial01868.bsf"), ma::io::Mode::In);
    reader.setDevice(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.format(), "amti.bsf");
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(supportedTrialFormats)
  {
    ma::io::HandlerReader reader;
    auto formats = reader.availableFormats();
    TS_ASSERT_EQUALS(std::find(formats.cbegin(), formats.cend(), "amti.bsf") != formats.cend(), true);
  };
  
  CXXTEST_TEST(detectNoDevice)
  {
    ma::io::HandlerReader reader;
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::Device);
  };
  
  CXXTEST_TEST(detectDeviceNotOpen)
  {
    ma::io::File file;
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::Device);
  };

  CXXTEST_TEST(detectEmptyFile)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Empty.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::InvalidData);
  };

  CXXTEST_TEST(detectFail)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Fail.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::Device);
  };

  CXXTEST_TEST(detectOk)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Trial01868.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkOne)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Trial01868.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "amti.bsf");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkTwo)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Trial01868.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(nullptr, "amti.bsf");
    reader.setDevice(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkThree)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Trial01868.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader;
    reader.setDevice(&file);
    reader.setFormat("amti.bsf");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(fileTrial01868)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("bsf/Trial01868.bsf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "amti.bsf");
    ma::Node root("root");
    TS_ASSERT_EQUALS(reader.read(&root), true);
    TS_ASSERT_EQUALS(root.children().size(), 1u);
    auto analogs = root.findChildren<ma::TimeSequence*>();
    TS_ASSERT_EQUALS(analogs.size(), 6u);
    for (auto analog : analogs)
    {
      TS_ASSERT_EQUALS(analog->startTime(), 0.0);
      TS_ASSERT_EQUALS(analog->sampleRate(), 200.0);
      TS_ASSERT_EQUALS(analog->components(), 1u);
      TS_ASSERT_EQUALS(analog->samples(), 400u);
    }
    TS_ASSERT_EQUALS(analogs[0]->name(), "Fx");
    TS_ASSERT_EQUALS(analogs[0]->unit(), "N");
    TS_ASSERT_EQUALS(analogs[1]->name(), "Fy");
    TS_ASSERT_EQUALS(analogs[1]->unit(), "N");
    TS_ASSERT_EQUALS(analogs[2]->name(), "Fz");
    TS_ASSERT_EQUALS(analogs[2]->unit(), "N");
    TS_ASSERT_EQUALS(analogs[3]->name(), "Mx");
    TS_ASSERT_EQUALS(analogs[3]->unit(), "Nm");
    TS_ASSERT_EQUALS(analogs[4]->name(), "My");
    TS_ASSERT_EQUALS(analogs[4]->unit(), "Nm");
    TS_ASSERT_EQUALS(analogs[5]->name(), "Mz");
    TS_ASSERT_EQUALS(analogs[5]->unit(), "Nm");
    double* data = 0;
    data = analogs[0]->data();
    TS_ASSERT_DELTA(data[0], 0.123, 5e-4);
    TS_ASSERT_DELTA(data[1], 0.736, 5e-4);
    TS_ASSERT_DELTA(data[2], 0.491, 5e-4);
    TS_ASSERT_DELTA(data[399], 0.0, 5e-4);
    data = analogs[1]->data();
    TS_ASSERT_DELTA(data[0], -0.260, 5e-4);
    data = analogs[2]->data();
    TS_ASSERT_DELTA(data[0], 0.0, 5e-4);
    data = analogs[3]->data();
    TS_ASSERT_DELTA(data[0], 0.0, 5e-4);
    data = analogs[4]->data();
    TS_ASSERT_DELTA(data[0], 0.0, 5e-4);
    data = analogs[5]->data();
    TS_ASSERT_DELTA(data[0], -0.032, 5e-4);
    TS_ASSERT_DELTA(data[399], -0.446, 5e-4);
  };
};

CXXTEST_SUITE_REGISTRATION(BSFReaderTest)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, capability)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, supportedTrialFormats)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, detectNoDevice)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, detectDeviceNotOpen)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, detectEmptyFile)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, detectFail)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, detectOk)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, queryOkOne)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, queryOkTwo)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, queryOkThree)
CXXTEST_TEST_REGISTRATION(BSFReaderTest, fileTrial01868)
