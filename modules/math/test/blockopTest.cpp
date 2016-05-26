#include <cxxtest/TestDrive.h>

#include <openma/math.h>

CXXTEST_SUITE(BlockOpTest)
{
  CXXTEST_TEST(block)
  {
    ma::math::Pose motion(3);
    motion.values().setRandom();
    motion.residuals() << -1.0, 1.0, 0.5;
    TS_ASSERT_EQUALS(((motion.block<1>(0).values() - motion.values().block(0,0,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(1).values() - motion.values().block(0,1,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(2).values() - motion.values().block(0,2,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(0).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(1).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((motion.block<1>(2).residuals() - motion.residuals()).abs() < 1e-15).all(), true);
    ma::math::Position o = motion.o();
    TS_ASSERT_EQUALS(((o.values().block(0,0,3,1) - motion.values().block(0,9,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((o.values().block(0,1,3,1) - motion.values().block(0,10,3,1)).abs() < 1e-15).all(), true);
    TS_ASSERT_EQUALS(((o.values().block(0,2,3,1) - motion.values().block(0,11,3,1)).abs() < 1e-15).all(), true);
  };
  
  CXXTEST_TEST(assignment)
  {
    ma::math::Position a(10),b(10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, -1.0, 0.5, 0.5, 1.0, 0.65, 0.45, -1.0,  0.0;
    a.z() = b.z();
    TS_ASSERT_EQUALS(((a.values().block(0,2,3,1) - b.values().block(0,2,3,1)).abs() < 1e-15).all(), true);
    ma::math::Position::Residuals res(10);
    res << -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, -1.0;
    TS_ASSERT_EQUALS(((a.residuals() - res).abs() < 1e-15).all(), true);
  };
  
  CXXTEST_TEST(compoundAssignmentOperators)
  {
    ma::math::Array<3>::Residuals rref = ma::math::Position::Residuals::Zero(10,1);
    
    ma::math::Position::Values val1 = ma::math::Position::Values::Random(10,3);
    ma::math::Position::Residuals res1 = rref;
    ma::math::Position pos1(val1,res1);
    
    ma::math::Position::Values val2 = ma::math::Position::Values::Random(10,3);
    ma::math::Position::Residuals res2 = rref;
    ma::math::Position pos2(val2,res2);
    
    pos1.x() /= 2.0;
    pos1.x() += 5.5;
    pos1.y() *= 10.0;
    pos1.z() -= 100.0;
    TS_ASSERT_EIGEN_DELTA(pos1.values().col(0), ((val1.col(0) / 2.0) + 5.5), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.values().col(1), (val1.col(1) * 10.0), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.values().col(2), (val1.col(2) - 100.0), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1.z() += 100.0;
    pos1.y() /= 10.0;
    pos1.x() -= 5.5;
    pos1.x() *= 2.0;
    
    TS_ASSERT_EIGEN_DELTA(pos1.values(), val1, 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1.x() += pos2.y();
    pos1.y() -= pos2.z();
    pos1.z() *= pos2.x();
    
    TS_ASSERT_EIGEN_DELTA(pos1.values().col(0), (val1.col(0) + val2.col(1)), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.values().col(1), (val1.col(1) - val2.col(2)), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.values().col(2), (val1.col(2) * val2.col(0)), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1.residuals().topRows<2>().setConstant(-1.0);
    rref.topRows<2>().setConstant(-1.0);
    pos2.residuals().bottomRows<3>().setConstant(-1.0);
    rref.bottomRows<3>().setConstant(-1.0);
    pos1.x() -= pos2.y();
    pos1.y() += pos2.z();
    pos1.z() /= pos2.x();
    TS_ASSERT_EIGEN_DELTA(pos1.values(), val1, 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(BlockOpTest)
CXXTEST_TEST_REGISTRATION(BlockOpTest, block)
CXXTEST_TEST_REGISTRATION(BlockOpTest, assignment)
CXXTEST_TEST_REGISTRATION(BlockOpTest, compoundAssignmentOperators)