#include <cxxtest/TestDrive.h>

#include <openma/io/file.h>
#include <cstdio>

#include "tdd_file_path.h"

CXXTEST_SUITE(FileTest)
{
  CXXTEST_TEST(DefaultConstructor)
  {
    ma::io::File file;
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    TS_ASSERT_EQUALS(file.exceptions(), ma::io::File::State::Good);
  };
  
  CXXTEST_TEST(SetExceptions)
  {
    ma::io::File file;
    file.setExceptions(ma::io::File::State::End | ma::io::File::State::Fail | ma::io::File::State::Error);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), true);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), false);
    TS_ASSERT_EQUALS(file.exceptions(), ma::io::File::State::End | ma::io::File::State::Fail | ma::io::File::State::Error);
  };
  
  CXXTEST_TEST(CloseNoFile)
  {
    ma::io::File file;
    file.close();
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(CloseNoFileException)
  {
    ma::io::File file;
    file.setExceptions(ma::io::File::State::End | ma::io::File::State::Fail | ma::io::File::State::Error);
    TS_ASSERT_THROWS(file.close(), ma::io::File::Failure);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SecondConstructorRead)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
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
  
  CXXTEST_TEST(SecondConstructorWrongFilenameRead)
  {
    ma::io::File file;
    file.open("Wrong.test", ma::io::File::Mode::In);
    TS_ASSERT_EQUALS(file.isOpen(), false);
    TS_ASSERT_EQUALS(file.isGood(), false);
    TS_ASSERT_EQUALS(file.atEnd(), false);
    TS_ASSERT_EQUALS(file.hasError(), false);
    TS_ASSERT_EQUALS(file.hasFailure(), true);
  };
  
  CXXTEST_TEST(SecondConstructorWriteNewFile)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::File::Mode::Out);
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
  
  CXXTEST_TEST(SecondConstructorWriteExistedFile)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d"), ma::io::File::Mode::Out);
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
  
  CXXTEST_TEST(OpenReadMode)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
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
  
  CXXTEST_TEST(OpenWriteMode)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::File::Mode::Out);
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
  
  CXXTEST_TEST(OpenWriteModeFromExistingFile)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d"), ma::io::File::Mode::Out);
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
  
  CXXTEST_TEST(Read)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
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
  
  CXXTEST_TEST(ReadNoFile)
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
  
  CXXTEST_TEST(SeekBegin)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(0, ma::io::File::Origin::Begin);
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
  
  CXXTEST_TEST(SeekEnd)
  {
    ma::io::File file;
    char buf[1] = {0};
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(0, ma::io::File::Origin::End);
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
    file.seek(-2, ma::io::File::Origin::End);
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
  
  CXXTEST_TEST(SeekBeginInvalid)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(-1, ma::io::File::Origin::Begin);
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
  
  CXXTEST_TEST(SeekEndInvalid)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(1, ma::io::File::Origin::End);
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
  
  CXXTEST_TEST(SeekEndInvalidBis)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(-(406528 + 1), ma::io::File::Origin::End);
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
  
  CXXTEST_TEST(SeekCurrentInvalidForward)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(406528 + 1, ma::io::File::Origin::Current);
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
  
  CXXTEST_TEST(SeekCurrentInvalidBackward)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(-1, ma::io::File::Origin::Current);
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
  
  CXXTEST_TEST(SeekCurrentInvalidBackwardBis)
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(0, ma::io::File::Origin::End);
    file.seek(-(406528 + 1), ma::io::File::Origin::Current);
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
  
  CXXTEST_TEST(ReadEOFException)
  {
    ma::io::File file;
    file.setExceptions(ma::io::File::State::End | ma::io::File::State::Fail | ma::io::File::State::Error);
    file.open(OPENMA_TDD_PATH_IN("c3d/other/Gait.c3d"), ma::io::File::Mode::In);
    file.seek(406526, ma::io::File::Origin::Begin);
    char buf[2] = {0};
    file.read(buf,2);
    TS_ASSERT_EQUALS(buf[0], 0x15);
    TS_ASSERT_EQUALS(buf[1], 0x00);
    TS_ASSERT_THROWS(file.read(buf,1), ma::io::File::Failure);
  };
  
  CXXTEST_TEST(Write)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::File::Mode::Out);
    char buf[1] = {0x16};
    file.write(buf,1);
    file.close();
  };
  
  CXXTEST_TEST(SeekWrite)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::File::Mode::Out);
#ifdef _MSC_VER // The granularity is not the same
    file.seek(65536, ma::io::File::Origin::Begin);
#else
    file.seek(4096, ma::io::File::Origin::Begin);
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
  
  CXXTEST_TEST(SuperSeekWrite)
  {
    const char* filename = OPENMA_TDD_PATH_OUT("c3d/mmfstream.c3d");
    std::remove(filename);
    ma::io::File file;
    file.open(filename, ma::io::File::Mode::Out);
    file.seek(400000, ma::io::File::Origin::Begin);
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
CXXTEST_TEST_REGISTRATION(FileTest, DefaultConstructor)
CXXTEST_TEST_REGISTRATION(FileTest, SetExceptions)
CXXTEST_TEST_REGISTRATION(FileTest, CloseNoFile)
CXXTEST_TEST_REGISTRATION(FileTest, CloseNoFileException)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorRead)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorWrongFilenameRead)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorWriteNewFile)
CXXTEST_TEST_REGISTRATION(FileTest, SecondConstructorWriteExistedFile)
CXXTEST_TEST_REGISTRATION(FileTest, OpenReadMode)
CXXTEST_TEST_REGISTRATION(FileTest, OpenWriteMode)
CXXTEST_TEST_REGISTRATION(FileTest, OpenWriteModeFromExistingFile)
CXXTEST_TEST_REGISTRATION(FileTest, Read)
CXXTEST_TEST_REGISTRATION(FileTest, ReadNoFile)
CXXTEST_TEST_REGISTRATION(FileTest, SeekBegin)
CXXTEST_TEST_REGISTRATION(FileTest, SeekEnd)
CXXTEST_TEST_REGISTRATION(FileTest, SeekBeginInvalid)
CXXTEST_TEST_REGISTRATION(FileTest, SeekEndInvalid)
CXXTEST_TEST_REGISTRATION(FileTest, SeekEndInvalidBis)
CXXTEST_TEST_REGISTRATION(FileTest, SeekCurrentInvalidForward)
CXXTEST_TEST_REGISTRATION(FileTest, SeekCurrentInvalidBackward)
CXXTEST_TEST_REGISTRATION(FileTest, SeekCurrentInvalidBackwardBis)
CXXTEST_TEST_REGISTRATION(FileTest, ReadEOFException)
CXXTEST_TEST_REGISTRATION(FileTest, Write)
CXXTEST_TEST_REGISTRATION(FileTest, SeekWrite)
CXXTEST_TEST_REGISTRATION(FileTest, SuperSeekWrite)
