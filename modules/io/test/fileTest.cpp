#include <cxxtest/TestDrive.h>

#include <openma/io/file.h>
#include <cstdio>

#include "tdd_file_path.h"

CXXTEST_SUITE(FileTest)
{
  CXXTEST_TEST(defaultConstructor)
  {
    ma::io::File file;
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    TS_ASSERT_EQUALS(file.exceptions(), ma::io::State::Good);
  };
  
  CXXTEST_TEST(setExceptions)
  {
    ma::io::File file;
    file.setExceptions(ma::io::State::End | ma::io::State::Fail | ma::io::State::Error);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    TS_ASSERT_EQUALS(file.exceptions(), ma::io::State::End | ma::io::State::Fail | ma::io::State::Error);
  };
  
  CXXTEST_TEST(closeNoFile)
  {
    ma::io::File file;
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(closeNoFileException)
  {
    ma::io::File file;
    file.setExceptions(ma::io::State::End | ma::io::State::Fail | ma::io::State::Error);
    TS_ASSERT_THROWS(file.close(), ma::io::File::Failure);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(secondConstructorRead)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(secondConstructorWrongFilenameRead)
  {
    ma::io::File file;
    file.open("Wrong.test", ma::io::Mode::In);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(secondConstructorWriteNewFile)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::Mode::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(secondConstructorWriteExistedFile)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d"), ma::io::Mode::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(openReadMode)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(openReadModeEmptyContent)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Empty.c3d"), ma::io::Mode::In);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(openWriteMode)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::Mode::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(openWriteModeFromExistingFile)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d"), ma::io::Mode::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(read)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(readNoFile)
  {
    ma::io::File file;
    char buf[3] = {0};
    file.read(buf,3);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(seekBegin)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(0, ma::io:: Origin::Begin);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(seekEnd)
  {
    ma::io::File file;
    char buf[1] = {0};
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(0, ma::io:: Origin::End);
    TS_ASSERT(file.tell() == ma::io::File::Position(406528));
    file.read(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.seek(-2, ma::io:: Origin::End);
    file.read(buf,1);
    TS_ASSERT_EQUALS(buf[0], 0x15);
    file.read(buf,1);
    TS_ASSERT_EQUALS(buf[0], 0x00);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(seekBeginInvalid)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(-1, ma::io:: Origin::Begin);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(seekEndInvalid)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(1, ma::io:: Origin::End);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(seekEndInvalidBis)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(-(406528 + 1), ma::io:: Origin::End);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(seekCurrentInvalidForward)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(406528 + 1, ma::io::Origin::Current);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(seekCurrentInvalidBackward)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(-1, ma::io::Origin::Current);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x02);
    TS_ASSERT_EQUALS(buf[1], 0x50);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(seekCurrentInvalidBackwardBis)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(0, ma::io:: Origin::End);
    file.seek(-(406528 + 1), ma::io::Origin::Current);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.clear();
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
    file.close();
  };
  
  CXXTEST_TEST(readEOFException)
  {
    ma::io::File file;
    file.setExceptions(ma::io::State::End | ma::io::State::Fail | ma::io::State::Error);
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::Mode::In);
    file.seek(406526, ma::io:: Origin::Begin);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x15);
    TS_ASSERT_EQUALS(buf[1], 0x00);
    TS_ASSERT_THROWS(file.read(buf,1), ma::io::File::Failure);
  };
  
  CXXTEST_TEST(write)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::Mode::Out);
    char buf[1] = {0x16};
    file.write(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.openMode(), ma::io::Mode::Out);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
  };
  
  CXXTEST_TEST(seekWrite)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::Mode::Out);
#ifdef _MSC_VER // The granularity is not the same
    file.seek(65536, ma::io:: Origin::Begin);
#else
    file.seek(4096, ma::io:: Origin::Begin);
#endif
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    char buf[1] = {0x16};
    file.write(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
  
  CXXTEST_TEST(superSeekWrite)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::Mode::Out);
    file.seek(400000, ma::io:: Origin::Begin);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    char buf[1] = {0x16};
    file.write(buf,1);
    TS_ASSERT_EQUALS(file.isOpen(), true);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
  };
};

CXXTEST_SUITE_REGISTRATION(FileTest)
CXXTEST_TEST_REGISTRATION(FileTest, defaultConstructor)
CXXTEST_TEST_REGISTRATION(FileTest, setExceptions)
CXXTEST_TEST_REGISTRATION(FileTest, closeNoFile)
CXXTEST_TEST_REGISTRATION(FileTest, closeNoFileException)
CXXTEST_TEST_REGISTRATION(FileTest, secondConstructorRead)
CXXTEST_TEST_REGISTRATION(FileTest, secondConstructorWrongFilenameRead)
CXXTEST_TEST_REGISTRATION(FileTest, secondConstructorWriteNewFile)
CXXTEST_TEST_REGISTRATION(FileTest, secondConstructorWriteExistedFile)
CXXTEST_TEST_REGISTRATION(FileTest, openReadMode)
CXXTEST_TEST_REGISTRATION(FileTest, openReadModeEmptyContent)
CXXTEST_TEST_REGISTRATION(FileTest, openWriteMode)
CXXTEST_TEST_REGISTRATION(FileTest, openWriteModeFromExistingFile)
CXXTEST_TEST_REGISTRATION(FileTest, read)
CXXTEST_TEST_REGISTRATION(FileTest, readNoFile)
CXXTEST_TEST_REGISTRATION(FileTest, seekBegin)
CXXTEST_TEST_REGISTRATION(FileTest, seekEnd)
CXXTEST_TEST_REGISTRATION(FileTest, seekBeginInvalid)
CXXTEST_TEST_REGISTRATION(FileTest, seekEndInvalid)
CXXTEST_TEST_REGISTRATION(FileTest, seekEndInvalidBis)
CXXTEST_TEST_REGISTRATION(FileTest, seekCurrentInvalidForward)
CXXTEST_TEST_REGISTRATION(FileTest, seekCurrentInvalidBackward)
CXXTEST_TEST_REGISTRATION(FileTest, seekCurrentInvalidBackwardBis)
CXXTEST_TEST_REGISTRATION(FileTest, readEOFException)
CXXTEST_TEST_REGISTRATION(FileTest, write)
CXXTEST_TEST_REGISTRATION(FileTest, seekWrite)
CXXTEST_TEST_REGISTRATION(FileTest, superSeekWrite)
