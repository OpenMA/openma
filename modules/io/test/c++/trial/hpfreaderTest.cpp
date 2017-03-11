#include "catch_and_extras.hpp"

#include "hpfhandlerTest_def.h"

#include "test_file_path.h"

TEST_CASE("Delsys HPF format - reader", "[openma][io][format][delsys.hpf]")
{

  SECTION("capability")
  {
    ma::io::HandlerReader reader(nullptr,"delsys.hpf");
    CHECK( !reader.canRead() );
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Set1[Rep2]_v3.emg.hpf"), ma::io::Mode::In);
    reader.setDevice(&file);
    CHECK( reader.canRead() );
    CHECK( reader.format() == "delsys.hpf");
    CHECK( reader.errorCode() == ma::io::Error::None );
  }

  SECTION("supported trial formats")
  {
    ma::io::HandlerReader reader;
    auto formats = reader.availableFormats();
    CHECK( std::find(formats.cbegin(), formats.cend(), "delsys.hpf") != formats.cend());
  }

  SECTION("detect no device")
  {
    ma::io::HandlerReader reader;
    CHECK( !reader.canRead() );
    CHECK( reader.errorCode() == ma::io::Error::Device );
  }

  SECTION("detect device not open")
  {
    ma::io::File file;
    ma::io::HandlerReader reader(&file);
    CHECK( !reader.canRead() );
    CHECK( reader.errorCode() == ma::io::Error::Device );
  }

  SECTION("detect empty file")
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Empty.hpf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    CHECK( !reader.canRead());
    CHECK( reader.errorCode() == ma::io::Error::InvalidData);
  }

  SECTION("detect fail")
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Fail.hpf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    CHECK( !reader.canRead() );
    CHECK( reader.errorCode() == ma::io::Error::Device );
  }

  SECTION("detect ok")
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Set1[Rep2]_v3.emg.hpf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file);
    CHECK( reader.canRead() );
    CHECK( reader.errorCode() == ma::io::Error::None);
  }

  SECTION("query ok one")
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Set1[Rep2]_v3.emg.hpf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(&file, "delsys.hpf");
    CHECK( reader.canRead() );
    CHECK( reader.errorCode() == ma::io::Error::None );
  }

  SECTION("query ok two")
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Set1[Rep2]_v3.emg.hpf"), ma::io::Mode::In);
    ma::io::HandlerReader reader(nullptr, "delsys.hpf");
    reader.setDevice(&file);
    CHECK( reader.canRead() );
    CHECK( reader.errorCode() == ma::io::Error::None );
  }

  SECTION("query ok three")
  {
    ma::io::File file;
    file.open(OPENMA_TDD_PATH_IN("hpf/Set1[Rep2]_v3.emg.hpf"), ma::io::Mode::In);
    ma::io::HandlerReader reader;
    reader.setDevice(&file);
    reader.setFormat("delsys.hpf");
    CHECK( reader.canRead());
    CHECK( reader.errorCode() == ma::io::Error::None);
  }

}

TEST_CASE("Delsys HPF format - file content", "[openma][io][format][delsys.hpf]")
{

  ma::io::File file;
  ma::io::HandlerReader reader(&file, "delsys.hpf");
  ma::Node root("root");

  SECTION("file Run_number_34_VTT_Rep_1DOT6")
  {
    file.open(OPENMA_TDD_PATH_IN("hpf/Run_number_34_VTT_Rep_1.6.hpf"), ma::io::Mode::In);
    CHECK( reader.read(&root) );
    hpfhandlertest_read_Run_number_34_VTT_Rep_1_6(&root);
  }

  SECTION("file Set1[Rep2]_v3")
  {
    file.open(OPENMA_TDD_PATH_IN("hpf/Set1[Rep2]_v3.emg.hpf"), ma::io::Mode::In);
    CHECK( reader.read(&root) );
    hpfhandlertest_read_Set1_Rep2__v3(&root);
  }

  SECTION("file Run_number_53_Plot_and_Store_Rep_1")
  {
    file.open(OPENMA_TDD_PATH_IN("hpf/Run_number_53_Plot_and_Store_Rep_1.1.hpf"), ma::io::Mode::In);
    CHECK( reader.read(&root) );
    hpfhandlertest_read_Run_number_53_Plot_and_Store_Rep_1_1(&root);
  }

}

TEST_CASE("Delsys HPF format - conversion", "[openma][io][format][delsys.hpf]")
{
  ma::Node original("original"), converted("converted");

  SECTION("HPF -> C3D")
  {
    REQUIRE( ma::io::read(&original, OPENMA_TDD_PATH_IN("hpf/Run_number_53_Plot_and_Store_Rep_1.1.hpf")) );
    REQUIRE( ma::io::write(&original, OPENMA_TDD_PATH_OUT("c3d/HPF_Run_number_53_Plot_and_Store_Rep_1.1.c3d")) );
    REQUIRE( ma::io::read(&converted, OPENMA_TDD_PATH_OUT("c3d/HPF_Run_number_53_Plot_and_Store_Rep_1.1.c3d")) );
    hpfhandlertest_read_Run_number_53_Plot_and_Store_Rep_1_1(&original);
    hpfhandlertest_read_Run_number_53_Plot_and_Store_Rep_1_1(&converted, false);
  }

}