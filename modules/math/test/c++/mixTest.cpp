#include <cxxtest/TestDrive.h>

#include <openma/math.h>

CXXTEST_SUITE(MixTest)
{
  CXXTEST_TEST(operatorsPlusAndDivide)
  {
    double _a[8] = {0};
    double _b[26] = {0};

    ma::math::Map<ma::math::Vector> a(2,_a,_a+6);
    ma::math::Map<ma::math::Pose> b(2,_b,_b+24);
    ma::math::Vector c(2), d(2), e(2);
    
    a.values() << 1., 2., 3., 4., 5., 6.;
    a.residuals() << 0., 0.;
    
    b.values().setZero();
    b.residuals() << 0.3, -1.;
    
    c.values() << 7., 8., 9., 10., 11., 12.;
    c.residuals() << 0., 0.;
    d.values() << 13., 14., 15., 16., 17., 18.;
    d.residuals() << 0., 0.;
    e.values().setZero();
    e.residuals() << 0., 0.;
    
    ma::math::Vector f;
    f = (a + b.u() + c + d) / 4.0;
    TS_ASSERT_EQUALS(f.isValid(),true);
    TS_ASSERT_EQUALS(f.isOccluded(),false);
    TS_ASSERT_EQUALS(f.rows(),2);
    TS_ASSERT_EQUALS(f.cols(),3);
    const auto& fr = f.residuals();
    TS_ASSERT_EQUALS(fr.coeff(0), 0.);
    TS_ASSERT_EQUALS(fr.coeff(1), -1.);
    const auto& fv = f.values();
    TS_ASSERT_EQUALS(fv.coeff(0,0), 5.25);
    TS_ASSERT_EQUALS(fv.coeff(0,1), 6.);
    TS_ASSERT_EQUALS(fv.coeff(0,2), 6.75);
    TS_ASSERT_EQUALS(fv.coeff(1,0), 0.);
    TS_ASSERT_EQUALS(fv.coeff(1,1), 0.);
    TS_ASSERT_EQUALS(fv.coeff(1,2), 0.);
    
    ma::math::Vector g = (a + c + d + e) / 4.0;
    TS_ASSERT_EQUALS(g.isValid(),true);
    TS_ASSERT_EQUALS(g.isOccluded(),false);
    TS_ASSERT_EQUALS(g.rows(),2);
    TS_ASSERT_EQUALS(g.cols(),3);
    const auto& gr = g.residuals();
    TS_ASSERT_EQUALS(gr.coeff(0), 0.);
    TS_ASSERT_EQUALS(gr.coeff(1), 0.);
    const auto& gv = g.values();
    TS_ASSERT_EQUALS(gv.coeff(0,0), 5.25);
    TS_ASSERT_EQUALS(gv.coeff(0,1), 6.);
    TS_ASSERT_EQUALS(gv.coeff(0,2), 6.75);
    TS_ASSERT_EQUALS(gv.coeff(1,0), 7.5);
    TS_ASSERT_EQUALS(gv.coeff(1,1), 8.25);
    TS_ASSERT_EQUALS(gv.coeff(1,2), 9);
  };
  
  CXXTEST_TEST(assignment)
  {
    ma::math::Vector a(10);
    a.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    
    double _b[40] = {0};
    ma::math::Map<ma::math::Vector> b(10,_b,_b+30);
    
    b = a;
    
    for (int i = 0 ; i < 30 ; ++i)
      TS_ASSERT_EQUALS(a.values().data()[i],_b[i]);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.residuals().data()[i],_b[i+30]);
  };
  
  CXXTEST_TEST(toArray)
  {
    ma::TimeSequence marker("MARKER",4,10,100.0,0.0,ma::TimeSequence::Position,"mm");
    
    auto data = Eigen::Map<ma::math::Array<4>::Values>(marker.data(),10,4);
    data.setRandom();
    
    auto a = ma::math::to_array<3>(&marker);
    TS_ASSERT_EQUALS(a.rows(), 10);
    TS_ASSERT_EQUALS(a.cols(), 3);
    TS_ASSERT_EIGEN_DELTA(a.values(), data.block(0,0,10,3), 1e-15);
    TS_ASSERT_EIGEN_DELTA(a.residuals(), data.block(0,3,10,1), 1e-15);
    
    auto b1 = ma::math::to_array<1>(&marker, 0);
    TS_ASSERT_EIGEN_DELTA(b1.values(), data.block(0,0,10,1), 1e-15);
    TS_ASSERT_EIGEN_DELTA(b1.residuals(), data.block(0,3,10,1), 1e-15);
    auto b2 = ma::math::to_array<1>(&marker, 1);
    TS_ASSERT_EIGEN_DELTA(b2.values(), data.block(0,1,10,1), 1e-15);
    TS_ASSERT_EIGEN_DELTA(b2.residuals(), data.block(0,3,10,1), 1e-15);
    auto b3 = ma::math::to_array<1>(&marker, 2);
    TS_ASSERT_EIGEN_DELTA(b3.values(), data.block(0,2,10,1), 1e-15);
    TS_ASSERT_EIGEN_DELTA(b3.residuals(), data.block(0,3,10,1), 1e-15);
    auto b4 = ma::math::to_array<1>(&marker, 3);
    TS_ASSERT_EQUALS(b4.isValid(), true);
  }
  
  CXXTEST_TEST(toArrayBis)
  {
    ma::TimeSequence marker("MARKER",4,1,100.0,0.0,ma::TimeSequence::Position,"mm");
    ma::TimeSequence angle("ANGLE",4,2,100.0,0.0,ma::TimeSequence::Angle,"degree");
    ma::TimeSequence analog("ANALOG",1,1,1000.0,0.0,ma::TimeSequence::Analog,"V");
    
    auto a = ma::math::to_array<3>(&marker,0,ma::TimeSequence::Position);
    TS_ASSERT_EQUALS(a.isValid(), true);
    TS_ASSERT_EQUALS(a.rows(), 1);
    TS_ASSERT_EQUALS(a.cols(), 3);
    
    auto b = ma::math::to_array<3>(&angle,0,ma::TimeSequence::Angle);
    TS_ASSERT_EQUALS(b.isValid(), true);
    TS_ASSERT_EQUALS(b.rows(), 2);
    TS_ASSERT_EQUALS(b.cols(), 3);
    
    auto c = ma::math::to_array<1>(&analog);
    TS_ASSERT_EQUALS(c.isValid(), false); // Not reconstructed
    
    auto d = ma::math::to_array<3>(&marker,0,ma::TimeSequence::Force);
    TS_ASSERT_EQUALS(d.isValid(), false);
    
    auto e = ma::math::to_array<5>(&marker);
    TS_ASSERT_EQUALS(e.isValid(), false);
    
    auto f = ma::math::to_array<3>(&marker,3);
    TS_ASSERT_EQUALS(f.isValid(), false);
    
    auto g = ma::math::to_array<3>(&angle,0,ma::TimeSequence::Analog);
    TS_ASSERT_EQUALS(g.isValid(), false);
    
    auto h = ma::math::to_position(&marker);
    TS_ASSERT_EQUALS(h.isValid(), true);
    TS_ASSERT_EQUALS(h.rows(), 1);
    TS_ASSERT_EQUALS(h.cols(), 3);
    
    auto i = ma::math::to_vector(&angle);
    TS_ASSERT_EQUALS(i.isValid(), true);
    TS_ASSERT_EQUALS(i.rows(), 2);
    TS_ASSERT_EQUALS(i.cols(), 3);
  }
  
  CXXTEST_TEST(toPose)
  {
    ma::TimeSequence pose("POSE",13,3,100.0,0.0,ma::TimeSequence::Pose,"");
    auto data = Eigen::Map<ma::math::Array<13>::Values>(pose.data(),3,13);
    auto motion = ma::math::to_pose(&pose);
    TS_ASSERT_EQUALS(motion.isValid(), true);
    TS_ASSERT_EQUALS(motion.rows(), 3);
    TS_ASSERT_EQUALS(motion.cols(), 12);
    motion.values() << 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 10.0, -43.2, 100.19,
                       0.299252, 0.947355, 0.113874, -0.361734, 0.002203, 0.932279, 0.882948, -0.320179, 0.343350, 10.0, -43.2, 100.19,
                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0;
    auto x = ma::math::to_vector(&pose);
    TS_ASSERT_EQUALS(x.isValid(), true);
    TS_ASSERT_EIGEN_DELTA(x.values(),data.block(0,0,3,3),1e-15);
    auto y = ma::math::to_vector(&pose,3);
    TS_ASSERT_EQUALS(y.isValid(), true);
    TS_ASSERT_EIGEN_DELTA(y.values(),data.block(0,3,3,3),1e-15);
    auto z = ma::math::to_vector(&pose,6);
    TS_ASSERT_EQUALS(z.isValid(), true);
    TS_ASSERT_EIGEN_DELTA(z.values(),data.block(0,6,3,3),1e-15);
    auto p = ma::math::to_vector(&pose,9);
    TS_ASSERT_EQUALS(p.isValid(), true);
    TS_ASSERT_EIGEN_DELTA(p.values(),data.block(0,9,3,3),1e-15);
  }
};

CXXTEST_SUITE_REGISTRATION(MixTest)
CXXTEST_TEST_REGISTRATION(MixTest, operatorsPlusAndDivide)
CXXTEST_TEST_REGISTRATION(MixTest, assignment)
CXXTEST_TEST_REGISTRATION(MixTest, toArray)
CXXTEST_TEST_REGISTRATION(MixTest, toArrayBis)
CXXTEST_TEST_REGISTRATION(MixTest, toPose)