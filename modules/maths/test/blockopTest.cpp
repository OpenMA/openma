#include <cxxtest/TestDrive.h>

#include <openma/maths.h>

CXXTEST_SUITE(BlockOpTest)
{
  CXXTEST_TEST(block)
  {
    ma::maths::Pose motion(3);
    motion.values().setRandom();
    motion.residuals() << -1.0, 1.0, 0.5;
    TS_ASSERT_EQUALS(((motion.block<1>(0).values() - motion.values().block(0,0,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(1).values() - motion.values().block(0,1,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(2).values() - motion.values().block(0,2,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(0).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(1).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(2).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    ma::maths::Position o = motion.o();
    TS_ASSERT_EQUALS(((o.values().block(0,0,3,1) - motion.values().block(0,9,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((o.values().block(0,1,3,1) - motion.values().block(0,10,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((o.values().block(0,2,3,1) - motion.values().block(0,11,3,1)).abs() < 1e-15).all(), true);
  };
  
  CXXTEST_TEST(assignment)
  {
    ma::maths::Position a(10),b(10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, -1.0, 0.5, 0.5, 1.0, 0.65, 0.45, -1.0,  0.0;
    a.z() = b.z();
    TS_ASSERT_EQUALS(((a.values().block(0,2,3,1) - b.values().block(0,2,3,1)).abs() < 1e-15).all(), true);
    ma::maths::Position::Residuals res(10);
    res << -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -1.0;
    TS_ASSERT_EQUALS(((a.residuals() - res).abs() < 1e-15).all(), true);
  };
};

CXXTEST_SUITE_REGISTRATION(BlockOpTest)
CXXTEST_TEST_REGISTRATION(BlockOpTest, block)
CXXTEST_TEST_REGISTRATION(BlockOpTest, assignment)
