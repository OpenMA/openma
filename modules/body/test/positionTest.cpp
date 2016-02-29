#include <cxxtest/TestDrive.h>

#include <openma/body/position.h>

const double datapos1[3] = {3.14, 420., 1429.54};
const double datapos2[3] = {1.234, 4.567, 8.901};

CXXTEST_SUITE(PositionTest)
{
  CXXTEST_TEST(setData)
  {
    ma::body::Position pos("Doo", datapos1);
    TS_ASSERT_DELTA(pos.data()[0], datapos1[0], 1e-15);
    TS_ASSERT_DELTA(pos.data()[1], datapos1[1], 1e-15);
    TS_ASSERT_DELTA(pos.data()[2], datapos1[2], 1e-15);
    auto ts = pos.timestamp();
    pos.setData(datapos2);
    TS_ASSERT_DIFFERS(ts, pos.timestamp());
    TS_ASSERT_DELTA(pos.data()[0], datapos2[0], 1e-15);
    TS_ASSERT_DELTA(pos.data()[1], datapos2[1], 1e-15);
    TS_ASSERT_DELTA(pos.data()[2], datapos2[2], 1e-15);
    ts = pos.timestamp();
    pos.setData(datapos2);
    TS_ASSERT_EQUALS(ts, pos.timestamp());
    TS_ASSERT_DELTA(pos.data()[0], datapos2[0], 1e-15);
    TS_ASSERT_DELTA(pos.data()[1], datapos2[1], 1e-15);
    TS_ASSERT_DELTA(pos.data()[2], datapos2[2], 1e-15);
  };
    
  CXXTEST_TEST(clone)
  {
    ma::Node root("root");
    ma::body::Position pos("Doo", datapos1);
    auto clone = pos.clone(&root);
    TS_ASSERT_EQUALS(root.child<ma::body::Position*>(0), clone);
    TS_ASSERT_DELTA(clone->data()[0], pos.data()[0], 1e-15);
    TS_ASSERT_DELTA(clone->data()[1], pos.data()[1], 1e-15);
    TS_ASSERT_DELTA(clone->data()[2], pos.data()[2], 1e-15);
    clone->setName("Clone");
    std::copy_n(datapos2,3,clone->data());
    TS_ASSERT_DIFFERS(clone->name(), pos.name());
    TS_ASSERT_DELTA(clone->data()[0], datapos2[0], 1e-15);
    TS_ASSERT_DELTA(clone->data()[1], datapos2[1], 1e-15);
    TS_ASSERT_DELTA(clone->data()[2], datapos2[2], 1e-15);
  };
  
  CXXTEST_TEST(copy)
  {
    ma::body::Position pos1("Doo", datapos1);
    ma::body::Position pos2("Clone", datapos2);
    pos2.copy(&pos1);
    TS_ASSERT_EQUALS(pos1.name(), pos2.name());
    TS_ASSERT_DELTA(pos1.data()[0], pos2.data()[0], 1e-15);
    TS_ASSERT_DELTA(pos1.data()[1], pos2.data()[1], 1e-15);
    TS_ASSERT_DELTA(pos1.data()[2], pos2.data()[2], 1e-15);
    pos2.setName("Clone");
    std::copy_n(datapos2,3,pos2.data());
    TS_ASSERT_DIFFERS(pos2.name(), pos1.name());
    TS_ASSERT_DELTA(pos2.data()[0], datapos2[0], 1e-15);
    TS_ASSERT_DELTA(pos2.data()[1], datapos2[1], 1e-15);
    TS_ASSERT_DELTA(pos2.data()[2], datapos2[2], 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(PositionTest)
CXXTEST_TEST_REGISTRATION(PositionTest, setData)
CXXTEST_TEST_REGISTRATION(PositionTest, clone)
CXXTEST_TEST_REGISTRATION(PositionTest, copy)
