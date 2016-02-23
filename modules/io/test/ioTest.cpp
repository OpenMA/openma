#include <cxxtest/TestDrive.h>

#include <openma/io.h>

#include "trial/c3dhandlerTest_def.h"
#include "tdd_file_path.h"

CXXTEST_SUITE(IoTest)
{
  CXXTEST_TEST(readOne)
  {
    auto root = ma::io::read(OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d"));
    c3dhandlertest_sample01("PI", "Eb015pi.c3d", root);
  };
  
  CXXTEST_TEST(readTwo)
  {
    ma::Node root("root");
    TS_ASSERT_EQUALS(ma::io::read(&root, OPENMA_TDD_PATH_IN("c3d/standard/sample01/Eb015pi.c3d")), true);
    c3dhandlertest_sample01("PI", "Eb015pi.c3d", &root);
  };
};

CXXTEST_SUITE_REGISTRATION(IoTest)
CXXTEST_TEST_REGISTRATION(IoTest, readOne)
CXXTEST_TEST_REGISTRATION(IoTest, readTwo)