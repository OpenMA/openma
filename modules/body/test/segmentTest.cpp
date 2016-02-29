#include <cxxtest/TestDrive.h>

#include <openma/body/segment.h>
#include <openma/body/enums.h>

CXXTEST_SUITE(SegmentTest)
{
  CXXTEST_TEST(clone)
  {
    ma::Node root("root");
    ma::body::Segment seg("Doo", ma::body::Part::User, ma::body::Side::Left);
    auto clone = seg.clone(&root);
    TS_ASSERT_EQUALS(root.child<ma::body::Segment*>(0), clone);
    TS_ASSERT_EQUALS(clone->name(), seg.name());
    TS_ASSERT_EQUALS(clone->part(), seg.part());
    TS_ASSERT_EQUALS(clone->side(), seg.side());
    clone->setName("Clone");
    clone->setSide(ma::body::Side::Right);
    clone->setPart(ma::body::Part::User + 1);
    TS_ASSERT_DIFFERS(clone->name(), seg.name());
    TS_ASSERT_DIFFERS(clone->part(), seg.part());
    TS_ASSERT_DIFFERS(clone->side(), seg.side());
  };
  
  CXXTEST_TEST(copy)
  {
    ma::body::Segment seg1("Doo", ma::body::Part::User, ma::body::Side::Left);
    ma::body::Segment seg2("Clone", ma::body::Part::User+1, ma::body::Side::Right);
    seg2.copy(&seg1);
    TS_ASSERT_EQUALS(seg1.name(), seg2.name());
    TS_ASSERT_EQUALS(seg1.part(), seg2.part());
    TS_ASSERT_EQUALS(seg1.side(), seg2.side());
    seg2.setName("Clone");
    seg2.setSide(ma::body::Side::Right);
    seg2.setPart(ma::body::Part::User + 1);
    TS_ASSERT_DIFFERS(seg1.name(), seg2.name());
    TS_ASSERT_DIFFERS(seg1.part(), seg2.part());
    TS_ASSERT_DIFFERS(seg1.side(), seg2.side());
  };
};

CXXTEST_SUITE_REGISTRATION(SegmentTest)
CXXTEST_TEST_REGISTRATION(SegmentTest, clone)
CXXTEST_TEST_REGISTRATION(SegmentTest, copy)
