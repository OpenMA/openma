#include <cxxtest/TestDrive.h>

#include <openma/math.h>

#include <cmath> // M_PI

CXXTEST_SUITE(PoseTest)
{
  CXXTEST_TEST(constructor)
  {
    ma::math::Position a(1), b(1), c(1);
    a.values() << 0.0, 0.0, 0.0; a.residuals() << 0.0;
    b.values() << 9.9, 0.0, 0.0; b.residuals() << 0.0;
    c.values() << 0.0,-4.1, 0.0; c.residuals() << 0.0;
      
    auto u = (b-a).normalized();
    auto v = (c-a).normalized();
    ma::math::Pose motion(u, v, u.cross(v), (a+b+c)/3.0);
    TS_ASSERT_EQUALS(motion.isValid(),true);
    TS_ASSERT_EQUALS(motion.isOccluded(),false);
    
    const ma::math::Pose::Values& mv = motion.values();
    TS_ASSERT_DELTA(mv.coeff(0,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,1),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,2),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,3),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,4),  -1.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,5),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,6),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,7),   0.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,8),  -1.0, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,9),   3.3, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,10),  -1.366666666666667, 1e-15);
    TS_ASSERT_DELTA(mv.coeff(0,11),  0.0, 1e-15);
    
    TS_ASSERT_DELTA(motion.residuals().coeff(0), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(inverse)
  {
    ma::math::Pose motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    ma::math::Pose::Values inv = motion.inverse().values();
    // Row #1
    TS_ASSERT_DELTA(inv.coeff(0, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 1), -1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 4), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 5), -1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 6), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 8), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0, 9), -100.190000, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0,10), 10.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(0,11), -43.200000, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(inv.coeff(1, 0), 0.299252, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 1), -0.361734, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 2), 0.882948, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 3), 0.947355, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 4), 0.002203, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 5), -0.320179, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 6), 0.113874, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 7), 0.932279, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 8), 0.343350, 5e-7);
    TS_ASSERT_DELTA(inv.coeff(1, 9), 26.524142, 5e-5);
    TS_ASSERT_DELTA(inv.coeff(1,10), -89.692480, 5e-5);
    TS_ASSERT_DELTA(inv.coeff(1,11), -57.061415, 5e-5);
    // Row #3
    TS_ASSERT_DELTA(inv.coeff(2, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 8), 1.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2, 9), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2,10), 0.0, 1e-15);
    TS_ASSERT_DELTA(inv.coeff(2,11), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(transformPose)
  {
    ma::math::Pose motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    ma::math::Pose::Values eye = motion.transform(motion.inverse()).values();
    
    // Row #1
    TS_ASSERT_DELTA(eye.coeff(0, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 8), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0, 9), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0,10), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(0,11), 0.0, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(eye.coeff(1, 0), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 1), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 2), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 3), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 4), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 5), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 6), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 7), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 8), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye.coeff(1, 9), 0.0, 1e-4);
    TS_ASSERT_DELTA(eye.coeff(1,10), 0.0, 1e-4);
    TS_ASSERT_DELTA(eye.coeff(1,11), 0.0, 1e-4);
    // Row #3
    TS_ASSERT_DELTA(eye.coeff(2, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 8), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2, 9), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2,10), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye.coeff(2,11), 0.0, 1e-15);
    
    ma::math::Array<9>::Values eye2 = motion.block<9>(0).transform(motion.block<9>(0).transpose()).values();
    
    // Row #1
    TS_ASSERT_DELTA(eye2.coeff(0, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(0, 8), 1.0, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(eye2.coeff(1, 0), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 1), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 2), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 3), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 4), 1.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 5), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 6), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 7), 0.0, 1e-6);
    TS_ASSERT_DELTA(eye2.coeff(1, 8), 1.0, 1e-6);
    // Row #3
    TS_ASSERT_DELTA(eye2.coeff(2, 0), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 2), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 3), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 4), 1.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 5), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 6), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 7), 0.0, 1e-15);
    TS_ASSERT_DELTA(eye2.coeff(2, 8), 1.0, 1e-15);
  };
  
  CXXTEST_TEST(transformPosition)
  {
    ma::math::Pose motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    
    ma::math::Position traj(3);
    traj.values() << 33.1,-13.56, 1034.12,
                     0.23, 34.12, -98.0,
                     66.0, 333.869, 4598.234;
      
    ma::math::Position::Values result = motion.transform(traj).values();
    
    // Row #1
    TS_ASSERT_DELTA(result.coeff(0, 0),    23.56, 5e-15);
    TS_ASSERT_DELTA(result.coeff(0, 1), -1077.32, 5e-15);
    TS_ASSERT_DELTA(result.coeff(0, 2),   133.29, 5e-15);
    // Row #2
    TS_ASSERT_DELTA(result.coeff(1, 0), -88.802402038427090, 4e-5);
    TS_ASSERT_DELTA(result.coeff(1, 1), -11.529404173918849, 4e-5);
    TS_ASSERT_DELTA(result.coeff(1, 2),  98.377264095895953, 4e-5);
    // Row #3
    TS_ASSERT_DELTA(result.coeff(2, 0), 66.0, 1e-15);
    TS_ASSERT_DELTA(result.coeff(2, 1), 333.869, 1e-15);
    TS_ASSERT_DELTA(result.coeff(2, 2), 4598.234, 1e-15);
    
    result = motion.block<9>(0).transform(traj).values();
    // Row #1
    TS_ASSERT_DELTA(result.coeff(0, 0),    13.56, 5e-15);
    TS_ASSERT_DELTA(result.coeff(0, 1), -1034.12, 5e-15);
    TS_ASSERT_DELTA(result.coeff(0, 2),    33.10, 5e-15);
    // Row #2
    TS_ASSERT_DELTA(result.coeff(1, 0), -98.802402038427090, 4e-5);
    TS_ASSERT_DELTA(result.coeff(1, 1),  31.670595826081151, 4e-5);
    TS_ASSERT_DELTA(result.coeff(1, 2),  -1.812735904104047, 4e-5);
    // Row #3
    TS_ASSERT_DELTA(result.coeff(2, 0), 66.0, 1e-15);
    TS_ASSERT_DELTA(result.coeff(2, 1), 333.869, 1e-15);
    TS_ASSERT_DELTA(result.coeff(2, 2), 4598.234, 1e-15);
  };
  
  CXXTEST_TEST(transformPositionBis)
  {
    ma::math::Pose motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    motion.residuals().setZero();
    
    TS_ASSERT_EQUALS(motion.rows(),3);
    TS_ASSERT_EQUALS(motion.cols(),12);

    const auto values = Eigen::Matrix<double,1,3>(0.0,0.0,0.0);
    ma::math::Position position(values);
    TS_ASSERT_EQUALS(position.rows(),1);
    
    const auto replicate = position.replicate(motion.rows());
    TS_ASSERT_EQUALS(replicate.rows(),3);
    TS_ASSERT_EQUALS(replicate.cols(),3);
    
    const auto transform = motion.transform(position.replicate(motion.rows()));
    TS_ASSERT_EQUALS(transform.rows(),3);
    TS_ASSERT_EQUALS(transform.cols(),3);
    
    ma::math::Position t = transform;
    TS_ASSERT_EQUALS(t.rows(),3);
    TS_ASSERT_EQUALS(t.cols(),3);
    // Row #1
    TS_ASSERT_DELTA(t.values().coeff(0, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(0, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(0, 2),  100.19, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(t.values().coeff(1, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(1, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(1, 2),  100.19, 1e-15);
    // Row #3
    TS_ASSERT_DELTA(t.values().coeff(2, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(t.values().coeff(2, 2), 0.0, 1e-15);
    
    ma::math::Position tt = motion.transform(ma::math::Position(Eigen::Matrix<double,1,3>(0.0,0.0,0.0)).replicate(motion.rows()));
    TS_ASSERT_EQUALS(tt.rows(),3);
    TS_ASSERT_EQUALS(tt.cols(),3);
    // Row #1
    TS_ASSERT_DELTA(tt.values().coeff(0, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(0, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(0, 2),  100.19, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(tt.values().coeff(1, 0),   10.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(1, 1),  -43.2, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(1, 2),  100.19, 1e-15);
    // Row #3
    TS_ASSERT_DELTA(tt.values().coeff(2, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(tt.values().coeff(2, 2), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(eulerAngles)
  {
    ma::math::Pose motion(3);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    motion.residuals() << 0.0, 0.0, -1.0;                   
    ma::math::Array<3> angles = motion.eulerAngles(0,1,2);
    TS_ASSERT_EQUALS(angles.values().rows(),3);
    TS_ASSERT_EQUALS(angles.values().cols(),3);
    // Row #1
    TS_ASSERT_DELTA(angles.values().coeff(0, 0), M_PI/2.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(0, 1),      0.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(0, 2), M_PI/2.0, 1e-15);
    // Row #2
    TS_ASSERT_DELTA(angles.values().coeff(1, 0), 0.750491578357562, 1e-5);
    TS_ASSERT_DELTA(angles.values().coeff(1, 1), 1.082104136236484, 1e-5);
    TS_ASSERT_DELTA(angles.values().coeff(1, 2), 0.879645943005142, 1e-5);
    // Row #3
    TS_ASSERT_DELTA(angles.values().coeff(2, 0), 0.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(2, 1), 0.0, 1e-15);
    TS_ASSERT_DELTA(angles.values().coeff(2, 2), 0.0, 1e-15);
    
    ma::math::Array<3> mean = motion.eulerAngles(0,1,2).mean();
    TS_ASSERT_DELTA(mean.values().coeff(0, 0), 1.160643952576229, 1e-5);
    TS_ASSERT_DELTA(mean.values().coeff(0, 1), 0.541052068118242, 1e-5);
    TS_ASSERT_DELTA(mean.values().coeff(0, 2), 1.225221134900019, 1e-5);
    
    ma::math::Array<3> meanbis = angles.mean();
    TS_ASSERT_DELTA(meanbis.values().coeff(0, 0), 1.160643952576229, 1e-5);
    TS_ASSERT_DELTA(meanbis.values().coeff(0, 1), 0.541052068118242, 1e-5);
    TS_ASSERT_DELTA(meanbis.values().coeff(0, 2), 1.225221134900019, 1e-5);
  };
};

CXXTEST_SUITE_REGISTRATION(PoseTest)
CXXTEST_TEST_REGISTRATION(PoseTest, constructor)
CXXTEST_TEST_REGISTRATION(PoseTest, inverse)
CXXTEST_TEST_REGISTRATION(PoseTest, transformPose)
CXXTEST_TEST_REGISTRATION(PoseTest, transformPosition)
CXXTEST_TEST_REGISTRATION(PoseTest, transformPositionBis)
CXXTEST_TEST_REGISTRATION(PoseTest, eulerAngles)
