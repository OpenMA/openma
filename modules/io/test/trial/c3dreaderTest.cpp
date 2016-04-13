#include <cxxtest/TestDrive.h>

#include <openma/io/handlerreader.h>
#include <openma/io/file.h>

#include "c3dhandlerTest_def.h"
#include "test_file_path.h"

CXXTEST_SUITE(C3DReaderTest)
{
  CXXTEST_TEST(capability)
  {
    ma::io::HandlerReader reader(nullptr,"org.c3d");
    TS_ASSERT_EQUALS(reader.canRead(), false);
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"), ma::io::Mode::In);
    reader.setDevice(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.format(), "org.c3d");
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(supportedTrialFormats)
  {
    ma::io::HandlerReader reader;
    auto formats = reader.availableFormats();
    TS_ASSERT_EQUALS(std::find(formats.cbegin(), formats.cend(), "org.c3d") != formats.cend(), true);
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
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Empty.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::InvalidData);
  };

  CXXTEST_TEST(detectFail)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Fail.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), false);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::Device);
  };

  CXXTEST_TEST(detectOk)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkOne)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "org.c3d");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkTwo)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader(nullptr, "org.c3d");
    reader.setDevice(&file);
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };
  
  CXXTEST_TEST(queryOkThree)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"), ma::io::Mode::In);
    ma::io::HandlerReader reader;
    reader.setDevice(&file);
    reader.setFormat("org.c3d");
    TS_ASSERT_EQUALS(reader.canRead(), true);
    TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  };

  CXXTEST_TEST(sample01)
  {
    c3dhandlertest_read_sample01("PI", "Eb015pi.c3d", OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"));
    c3dhandlertest_read_sample01("SI", "Eb015si.c3d", OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015si.c3d"));
    c3dhandlertest_read_sample01("VI", "Eb015vi.c3d", OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015vi.c3d"));
    c3dhandlertest_read_sample01("PR", "Eb015pr.c3d", OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pr.c3d"));
    c3dhandlertest_read_sample01("SR", "Eb015sr.c3d", OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015sr.c3d"));
    c3dhandlertest_read_sample01("VR", "Eb015vr.c3d", OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015vr.c3d"));
  };
};

CXXTEST_SUITE_REGISTRATION(C3DReaderTest)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, capability)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, supportedTrialFormats)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, detectNoDevice)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, detectDeviceNotOpen)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, detectEmptyFile)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, detectFail)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, detectOk)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, queryOkOne)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, queryOkTwo)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, queryOkThree)
CXXTEST_TEST_REGISTRATION(C3DReaderTest, sample01)  