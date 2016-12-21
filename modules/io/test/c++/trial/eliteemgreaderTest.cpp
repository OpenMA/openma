#include <cxxtest/TestDrive.h>

#include <openma/io.h>
#include <openma/base/node.h>
#include <openma/base/timesequence.h>

#include "test_file_path.h"

CXXTEST_SUITE(EliteEmgReaderTest)
{
  CXXTEST_TEST(capability)
  {
    ma::io::HandlerReader reader(nullptr,"bts.emg");
    TS_ASSERT_EQUALS(reader.canRead(), false);
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMG"), ma::io::Mode::In);
    reader.setDevice(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.format(), "bts.emg");
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(supportedTrialFormats)
  {
    ma::io::HandlerReader reader;
    auto formats = reader.availableFormats();
    TS_ASSERT_EQUALS(std::find(formats.cbegin(), formats.cend(), "bts.emg") != formats.cend(), true);
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
    file.open(OPENMA_TDD_PATH_IN("elite/empty/Empty.emg"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::InvalidData);
  };

  CXXTEST_TEST(detectFail)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/Fail.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::Device);
  };

  CXXTEST_TEST(detectOk)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMG"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkOne)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMG"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "bts.emg");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkTwo)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMG"), ma::io::Mode::In);
    ma::io::HandlerReader reader(nullptr, "bts.emg");
    reader.setDevice(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkThree)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMG"), ma::io::Mode::In);
    ma::io::HandlerReader reader;
    reader.setDevice(&file);
    reader.setFormat("bts.emg");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(detectVariantEmr)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMR"), ma::io::Mode::In);
    ma::io::HandlerReader reader;
    reader.setDevice(&file);
    reader.setFormat("bts.emg");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(detectVariantEmf)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMF"), ma::io::Mode::In);
    ma::io::HandlerReader reader;
    reader.setDevice(&file);
    reader.setFormat("bts.emg");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(file1123xa01G)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMG"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "bts.emg");
    ma::Node root("root");
    TS_ASSERT_EQUALS(reader.read(&root), true);
    TS_ASSERT_EQUALS(root.children().size(), 1u);
    auto analogs = root.findChildren<ma::TimeSequence*>();
    TS_ASSERT_EQUALS(analogs.size(), 8u);
    for (auto analog : analogs)
    {
      TS_ASSERT_EQUALS(analog->startTime(), 0.0);
      TS_ASSERT_EQUALS(analog->sampleRate(), 1000.0);
      TS_ASSERT_EQUALS(analog->components(), 1u);
      TS_ASSERT_EQUALS(analog->samples(), 4564u);
      TS_ASSERT_EQUALS(analog->unit(), "mV");
    }
    TS_ASSERT_EQUALS(analogs[0]->name(), "R_RF");
    TS_ASSERT_EQUALS(analogs[1]->name(), "R_BFCL");
    TS_ASSERT_EQUALS(analogs[2]->name(), "R_TA");
    TS_ASSERT_EQUALS(analogs[3]->name(), "R_GAL");
    TS_ASSERT_EQUALS(analogs[4]->name(), "L_RF");
    TS_ASSERT_EQUALS(analogs[5]->name(), "L_BFCL");
    TS_ASSERT_EQUALS(analogs[6]->name(), "L_TA");
    TS_ASSERT_EQUALS(analogs[7]->name(), "L_GAL");
    ma::Node ref("ref");
    TS_ASSERT_EQUALS(ma::io::read(&ref, OPENMA_TDD_PATH_IN("c3d/other/1123x01EMG.c3d")),true);
    auto aref = ref.findChildren<ma::TimeSequence*>();
    for (int i = 1 ; i < 50 ; i+=10)
    {
      TS_ASSERT_DELTA(analogs[0]->data()[i], aref[0]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[1]->data()[i], aref[1]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[2]->data()[i], aref[2]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[3]->data()[i], aref[3]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[4]->data()[i], aref[4]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[5]->data()[i], aref[5]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[6]->data()[i], aref[6]->data()[i], 1e-5);
      TS_ASSERT_DELTA(analogs[7]->data()[i], aref[7]->data()[i], 1e-5);
    }
  };

  CXXTEST_TEST(file1123xa01F)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMF"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "bts.emg");
    ma::Node root("root");
    TS_ASSERT_EQUALS(reader.read(&root), true);
    TS_ASSERT_EQUALS(root.children().size(), 1u);
    auto analogs = root.findChildren<ma::TimeSequence*>();
    TS_ASSERT_EQUALS(analogs.size(), 8u);
    for (auto analog : analogs)
    {
      TS_ASSERT_EQUALS(analog->startTime(), 0.0);
      TS_ASSERT_EQUALS(analog->sampleRate(), 1000.0);
      TS_ASSERT_EQUALS(analog->components(), 1u);
      TS_ASSERT_EQUALS(analog->samples(), 4564u);
      TS_ASSERT_EQUALS(analog->unit(), "mV");
    }
    TS_ASSERT_EQUALS(analogs[0]->name(), "R_RF");
    TS_ASSERT_EQUALS(analogs[1]->name(), "R_BFCL");
    TS_ASSERT_EQUALS(analogs[2]->name(), "R_TA");
    TS_ASSERT_EQUALS(analogs[3]->name(), "R_GAL");
    TS_ASSERT_EQUALS(analogs[4]->name(), "L_RF");
    TS_ASSERT_EQUALS(analogs[5]->name(), "L_BFCL");
    TS_ASSERT_EQUALS(analogs[6]->name(), "L_TA");
    TS_ASSERT_EQUALS(analogs[7]->name(), "L_GAL");
  };
  
  CXXTEST_TEST(file1123xa01R)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("elite/1123xa01/1123xa01.EMR"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "bts.emg");
    ma::Node root("root");
    TS_ASSERT_EQUALS(reader.read(&root), true);
    TS_ASSERT_EQUALS(root.children().size(), 1u);
    auto analogs = root.findChildren<ma::TimeSequence*>();
    TS_ASSERT_EQUALS(analogs.size(), 8u);
    for (auto analog : analogs)
    {
      TS_ASSERT_EQUALS(analog->startTime(), 0.0);
      TS_ASSERT_EQUALS(analog->sampleRate(), 1000.0);
      TS_ASSERT_EQUALS(analog->components(), 1u);
      TS_ASSERT_EQUALS(analog->samples(), 4564u);
      TS_ASSERT_EQUALS(analog->unit(), "mV");
    }
    TS_ASSERT_EQUALS(analogs[0]->name(), "R_RF");
    TS_ASSERT_EQUALS(analogs[1]->name(), "R_BFCL");
    TS_ASSERT_EQUALS(analogs[2]->name(), "R_TA");
    TS_ASSERT_EQUALS(analogs[3]->name(), "R_GAL");
    TS_ASSERT_EQUALS(analogs[4]->name(), "L_RF");
    TS_ASSERT_EQUALS(analogs[5]->name(), "L_BFCL");
    TS_ASSERT_EQUALS(analogs[6]->name(), "L_TA");
    TS_ASSERT_EQUALS(analogs[7]->name(), "L_GAL");
  };
};

CXXTEST_SUITE_REGISTRATION(EliteEmgReaderTest)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, capability)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, supportedTrialFormats)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectNoDevice)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectDeviceNotOpen)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectEmptyFile)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectFail)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectOk)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, queryOkOne)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, queryOkTwo)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, queryOkThree)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectVariantEmr)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, detectVariantEmf)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, file1123xa01G)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, file1123xa01F)
CXXTEST_TEST_REGISTRATION(EliteEmgReaderTest, file1123xa01R)