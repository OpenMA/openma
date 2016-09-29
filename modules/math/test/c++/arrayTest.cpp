#include <cxxtest/TestDrive.h>

#include <openma/math.h>

CXXTEST_SUITE(ArrayTest)
{
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
    ma::math::Vector b = a;
    for (int i = 0 ; i < 30 ; ++i)
      TS_ASSERT_EQUALS(a.values().data()[i], b.values().data()[i]);
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.residuals().data()[i], b.residuals().data()[i]);
  };
  
  CXXTEST_TEST(norm)
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
    
    ma::math::Scalar b = a.norm();

    TS_ASSERT_DELTA(b.values().coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(1), 1.732050807568877, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(2), 0.866025403784439, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(3), 0.866025403784439, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(4), 1.732050807568877, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(5), 1.12583302491977, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(6), 0.779422863405995, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(7), 1.732050807568877, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(8), 0.0, 1e-15);
    TS_ASSERT_DELTA(b.values().coeff(9), 0.0, 1e-15);

    TS_ASSERT_EQUALS(b.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(1), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(2), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(3), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(4), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(5), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(6), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(7), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(8), 0.0);
    TS_ASSERT_EQUALS(b.residuals().coeff(9), -1.0);
  };
  
  CXXTEST_TEST(mean)
  {  
    ma::math::Scalar a(10), b(10);
    ma::math::Position c(10);
    a.values() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.residuals().setConstant(10,-1.0);
    c.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    c.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    
    ma::math::Scalar a_ = a.mean();
    ma::math::Scalar b_ = b.mean();
    ma::math::Position c_ = c.mean();

    TS_ASSERT_EQUALS(a_.values().coeff(0), 0.6375);
    TS_ASSERT_EQUALS(a_.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(a_.values().rows(), 1);
    TS_ASSERT_EQUALS(a_.residuals().rows(), 1);
    
    TS_ASSERT_EQUALS(b_.values().coeff(0), 0.0);
    TS_ASSERT_EQUALS(b_.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(b_.values().rows(), 1);
    TS_ASSERT_EQUALS(b_.residuals().rows(), 1);
    
    TS_ASSERT_EQUALS(c_.values().coeff(0,0), 0.6375);
    TS_ASSERT_EQUALS(c_.values().coeff(0,1), 0.6375);
    TS_ASSERT_EQUALS(c_.values().coeff(0,2), 0.6375);
    TS_ASSERT_EQUALS(c_.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(c_.values().rows(), 1);
    TS_ASSERT_EQUALS(c_.residuals().rows(), 1);
  };

  CXXTEST_TEST(scaledDifference)
  {
    ma::math::Scalar a(10), b(10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0,  0.0;
    
    ma::math::Scalar c = 0.5 * (a - b);

    TS_ASSERT_EQUALS(c.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(9), -1.0);
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_EQUALS(c.values().coeff(i), (a.values().coeff(i) - b.values().coeff(i)) / 2.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(1), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(2), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(3), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(4), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(5), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(6), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(7), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(8), 0.0);
    TS_ASSERT_EQUALS(c.residuals().coeff(9), -1.0);
  };
  
  CXXTEST_TEST(normalized)
  {
    ma::math::Vector a(10);
    a.values().setRandom();
    a.values().row(9) << 0.0, 0.0, 0.0;
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    ma::math::Vector b(10);
    b.values().setRandom();
    b.residuals() << 1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, 0.4;
    ma::math::Vector c = a.normalized();
    ma::math::Vector d = (b-a).normalized();

    Eigen::Matrix<double,Eigen::Dynamic,1> norm = c.values().matrix().rowwise().norm();
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_DELTA(norm.coeff(i), 1.0, 1e-16);
    TS_ASSERT_DELTA(norm.coeff(0), 0.0, 1e-16);
    TS_ASSERT_DELTA(norm.coeff(9), 0.0, 1e-16);
    
    norm = d.values().matrix().rowwise().norm();
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_DELTA(norm.coeff(i), 1.0, 1e-15);
    TS_ASSERT_DELTA(norm.coeff(0), 0.0, 1e-16);
    TS_ASSERT_DELTA(norm.coeff(9), 0.0, 1e-16);
  };

  CXXTEST_TEST(cross)
  {
    ma::math::Vector a(4), b(4);
    a.values() << 1.0, 0.0, 0.0,
                  0.0, 1.0, 0.0,
                  0.0, 0.0, 1.0,
                  0.0, 0.0, 0.0;
    a.residuals() << 1.0, 0.5, 0.5, -1.0;
    b.values() << 0.0, 1.0, 0.0,
                  0.0, 1.0, 0.0,
                  1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0;
    b.residuals() << 1.0, 0.5, 0.5, 1.0;
    ma::math::Vector c = a.cross(b);

    const ma::math::Vector::Values& cv = c.values();
    const ma::math::Vector::Residuals& cr = c.residuals();

    TS_ASSERT_DELTA(cv.coeff(0,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(0,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(0,2), 1.0, 1e-15);

    TS_ASSERT_DELTA(cv.coeff(1,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(1,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(1,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(cv.coeff(2,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(2,1), 1.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(2,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(cv.coeff(3,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(3,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(cv.coeff(3,2), 0.0, 1e-15);

    TS_ASSERT_EQUALS(cr.coeff(0), 0.0);
    TS_ASSERT_EQUALS(cr.coeff(1), 0.0);
    TS_ASSERT_EQUALS(cr.coeff(2), 0.0);
    TS_ASSERT_EQUALS(cr.coeff(3), -1.0);
  };

  CXXTEST_TEST(crossBis)
  {
    ma::math::Vector a(4), b(4), c(4);
    a.values().setZero();
    a.residuals().setZero();
    b.values() << 0.0, 0.4, 0.0,
                  0.0, 2.0, 0.0,
                  0.9, 0.0, 0.0,
                  0.0, 0.0, 0.0;
    b.residuals() << 1.0, 0.5, 0.5, 1.0;
    c.values() << 2.0, 0.0, 0.0,
                  0.0, 3.0, 0.0,
                  0.0, 0.0, 1.5,
                  0.0, 0.0, 0.0;
    c.residuals() << 1.0, 0.5, 0.5, -1.0;
    ma::math::Vector d = (c-a).normalized().cross((b-a).normalized());

    const ma::math::Vector::Values& dv = d.values();
    const ma::math::Vector::Residuals& dr = d.residuals();

    TS_ASSERT_DELTA(dv.coeff(0,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(0,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(0,2), 1.0, 1e-15);

    TS_ASSERT_DELTA(dv.coeff(1,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(1,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(1,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(dv.coeff(2,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(2,1), 1.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(2,2), 0.0, 1e-15);

    TS_ASSERT_DELTA(dv.coeff(3,0), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(3,1), 0.0, 1e-15);
    TS_ASSERT_DELTA(dv.coeff(3,2), 0.0, 1e-15);

    TS_ASSERT_EQUALS(dr.coeff(0), 0.0);
    TS_ASSERT_EQUALS(dr.coeff(1), 0.0);
    TS_ASSERT_EQUALS(dr.coeff(2), 0.0);
    TS_ASSERT_EQUALS(dr.coeff(3), -1.0);
  };
  
  CXXTEST_TEST(operatorDouble)
  {
    ma::math::Position c(10);
    c.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.5 ,  0.5 ,  0.5 , 
                   0.5 ,  0.5 ,  0.5 , 
                   1.0 ,  1.0 ,  1.0 , 
                   0.65,  0.65,  0.65, 
                   0.45,  0.45,  0.45, 
                   1.0 ,  1.0 ,  1.0 , 
                   0.0 ,  0.0 ,  0.0 , 
                  -0.4 , -0.4 , -0.4;
    c.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    double norm = 0.0;
    
    norm = c.mean().norm();
    TS_ASSERT_DELTA(norm, 1.104182389825159, 1e-15);
    
    c.residuals().setConstant(-1.0);
    norm = c.mean().norm();
    TS_ASSERT_DELTA(norm, 0.0, 1e-15);
  };
  
  CXXTEST_TEST(replicate)
  {
    ma::math::Position a(2);
    a.values() << -1.0 , -1.0 , -1.0 , 
                   1.0 ,  1.0 ,  1.0;
    a.residuals() << -1.0, 1.0;
    
    ma::math::Position b = a.replicate(3);
    const ma::math::Position::Values& bv = b.values();
    const ma::math::Position::Residuals& br = b.residuals();
    TS_ASSERT_EQUALS(b.rows(), 6);
    
    TS_ASSERT_DELTA(bv.coeff(0,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,2),   1.0, 1e-15);
    
    TS_ASSERT_DELTA(br.coeff(0), -1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(1),  1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(2), -1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(3),  1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(4), -1.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(5),  1.0, 1e-15);
  };
  
  CXXTEST_TEST(replicateBis)
  {
    ma::math::Position::Values av(2,3);
    av << -1.0 , -1.0 , -1.0 , 
           1.0 ,  1.0 ,  1.0;
           
    ma::math::Position b = ma::math::Position(av,ma::math::Position::Residuals::Zero(av.rows())).replicate(3);
    const ma::math::Position::Values& bv = b.values();
    const ma::math::Position::Residuals& br = b.residuals();
    TS_ASSERT_EQUALS(b.rows(), 6);
    
    TS_ASSERT_DELTA(bv.coeff(0,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,2),   1.0, 1e-15);
    
    TS_ASSERT_DELTA(br.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(1), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(2), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(3), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(4), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(5), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(replicateTer)
  {
    ma::math::Scalar::Values av(2,1);
    av << -1.0, 1.0;
           
    ma::math::Position b = ma::math::Scalar(av,ma::math::Position::Residuals::Zero(av.rows())).replicate<3>(3);
    const ma::math::Position::Values& bv = b.values();
    const ma::math::Position::Residuals& br = b.residuals();
    TS_ASSERT_EQUALS(b.rows(), 6);
    TS_ASSERT_EQUALS(b.cols(), 3);
    
    TS_ASSERT_DELTA(bv.coeff(0,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(0,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(1,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(2,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(3,2),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,0),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,1),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(4,2),  -1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,0),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,1),   1.0, 1e-15);
    TS_ASSERT_DELTA(bv.coeff(5,2),   1.0, 1e-15);
    
    TS_ASSERT_DELTA(br.coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(1), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(2), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(3), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(4), 0.0, 1e-15);
    TS_ASSERT_DELTA(br.coeff(5), 0.0, 1e-15);
  };
  
  CXXTEST_TEST(transpose)
  {
    ma::math::Array<9> O(2);
    O.values() << 0.90926, 0.14403, 0.39051, -0.12315, 0.98931, -0.07815, -0.39759, 0.02297, 0.91728,
                  0.9095,  0.12366, 0.39689, -0.09854, 0.99165, -0.08316, -0.40386, 0.03653, 0.91409;
    O.residuals() << 0.12345678, 0.45678910;
    ma::math::Array<9> Ot = O.transpose();
    TS_ASSERT_EIGEN_DELTA(O.values().col(0), Ot.values().col(0), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(1), Ot.values().col(3), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(2), Ot.values().col(6), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(3), Ot.values().col(1), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(4), Ot.values().col(4), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(5), Ot.values().col(7), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(6), Ot.values().col(2), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(7), Ot.values().col(5), 1e-15);
    TS_ASSERT_EIGEN_DELTA(O.values().col(8), Ot.values().col(8), 1e-15);
    TS_ASSERT_DELTA(Ot.residuals().coeff(0), 0.0, 1e-15);
    TS_ASSERT_DELTA(Ot.residuals().coeff(1), 0.0, 1e-15);
  };
    
  CXXTEST_TEST(transposeBis)
  {
    ma::math::Array<9> O(2);
    O.values() << 0.90926, 0.14403, 0.39051, -0.12315, 0.98931, -0.07815, -0.39759, 0.02297, 0.91728,
                  0.9095,  0.12366, 0.39689, -0.09854, 0.99165, -0.08316, -0.40386, 0.03653, 0.91409;
    O.residuals() << 0.1234, 0.4567;
    ma::math::Array<9> E = O.transform(O.transpose());
    ma::math::Array<2> R(2);
    R.values() << 1., 0.,
                  1., 0.;
    R.residuals() << 0., 0.;
    TS_ASSERT_EIGEN_DELTA(E.values().col(0), R.values().col(0), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(1), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(2), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(3), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(4), R.values().col(0), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(5), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(6), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(7), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(8), R.values().col(0), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.residuals(), R.residuals(), 1e-15);
  };
  
  CXXTEST_TEST(transposeTer)
  {
    ma::math::Pose T(2);
    T.values() << 0.90926, 0.14403, 0.39051, -0.12315, 0.98931, -0.07815, -0.39759, 0.02297, 0.91728, 826.32837, 81.19634, 134.22374,
                  0.9095,  0.12366, 0.39689, -0.09854, 0.99165, -0.08316, -0.40386, 0.03653, 0.91409, 832.89762, 79.14663, 133.15111;
    T.residuals() << 0.1234, 0.4567;
    ma::math::Array<9> E = T.block<9>(0).transform(T.block<9>(0).transpose());
    ma::math::Array<2> R(2);
    R.values() << 1., 0.,
                  1., 0.;
    R.residuals() << 0., 0.;
    TS_ASSERT_EIGEN_DELTA(E.values().col(0), R.values().col(0), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(1), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(2), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(3), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(4), R.values().col(0), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(5), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(6), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(7), R.values().col(1), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.values().col(8), R.values().col(0), 1e-5);
    TS_ASSERT_EIGEN_DELTA(E.residuals(), R.residuals(), 1e-15);
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
    
    pos1 += pos2;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), (val1+val2), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1 -= pos2;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), val1, 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1 *= 2.0;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), (val1*2.0), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1 /= 2.0;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), val1, 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1 += 10.0;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), (val1+10.0), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1 -= 10.0;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), val1, 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos2.residuals().coeffRef(6) = -1.0;
    rref.coeffRef(6) = -1.0;
    pos1 += pos2;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), (val1+val2), 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    pos1 -= pos2;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), val1, 1e-15);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    ma::math::Position pos3(1);
    pos3.values() << 10.0, -45.0, 0.111;
    pos3.residuals() << 0.0;
    pos1 *= pos3.replicate(10);
    TS_ASSERT_EIGEN_DELTA(pos1.x().values(), (val1.col(0) * 10.0), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.y().values(), (val1.col(1) * -45.0), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.z().values(), (val1.col(2) * 0.111), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
    
    pos1 /= pos3.replicate(10) / 2.9;
    TS_ASSERT_EIGEN_DELTA(pos1.values(), (val1 * 2.9), 5e-14);
    TS_ASSERT_EIGEN_DELTA(pos1.residuals(), rref, 1e-15);
  };
  
  CXXTEST_TEST(minmax)
  {
    ma::math::Vector foo(10);
    foo.values() << 1., 2., 3.,
                    4., 5.,-6.,
                    0., 0., 0.,
                    3., 2., 1.,
                    6., 5., 4.,
                    0., 0., 0.,
                   -1.,-2.,-3.,
                   -4., 5.,-6.,
                   -7., 8., 9.,
                    0., 0., 0.;
    foo.residuals().setZero();
    
    ma::math::Vector m = foo.min();
    TS_ASSERT_EQUALS(m.rows(), 1);
    TS_ASSERT_EQUALS(m.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(m.values().coeff(0), -7.0);
    TS_ASSERT_EQUALS(m.values().coeff(1), -2.0);
    TS_ASSERT_EQUALS(m.values().coeff(2), -6.0);
    ma::math::Vector M = foo.max();
    TS_ASSERT_EQUALS(M.rows(), 1);
    TS_ASSERT_EQUALS(M.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(M.values().coeff(0), 6.0);
    TS_ASSERT_EQUALS(M.values().coeff(1), 8.0);
    TS_ASSERT_EQUALS(M.values().coeff(2), 9.0);
    
    foo.residuals().coeffRef(8) = -1.0;
    m = foo.min();
    TS_ASSERT_EQUALS(m.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(m.values().coeff(0), -4.0);
    TS_ASSERT_EQUALS(m.values().coeff(1), -2.0);
    TS_ASSERT_EQUALS(m.values().coeff(2), -6.0);
    M = foo.max();
    TS_ASSERT_EQUALS(M.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(M.values().coeff(0), 6.0);
    TS_ASSERT_EQUALS(M.values().coeff(1), 5.0);
    TS_ASSERT_EQUALS(M.values().coeff(2), 4.0);
    
    foo.residuals().coeffRef(1) = -1.0;
    foo.residuals().coeffRef(7) = -1.0;
    m = foo.min();
    TS_ASSERT_EQUALS(m.isOccluded(), false);
    TS_ASSERT_EQUALS(m.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(m.values().coeff(0), -1.0);
    TS_ASSERT_EQUALS(m.values().coeff(1), -2.0);
    TS_ASSERT_EQUALS(m.values().coeff(2), -3.0);
    M = foo.max();
    TS_ASSERT_EQUALS(M.isOccluded(), false);
    TS_ASSERT_EQUALS(M.residuals().coeff(0), 0.0);
    TS_ASSERT_EQUALS(M.values().coeff(0), 6.0);
    TS_ASSERT_EQUALS(M.values().coeff(1), 5.0);
    TS_ASSERT_EQUALS(M.values().coeff(2), 4.0);
    
    foo.residuals().setConstant(-1.0);
    m = foo.min();
    TS_ASSERT_EQUALS(m.isOccluded(), true);
    TS_ASSERT_EQUALS(m.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(m.values().coeff(0), 0.0);
    TS_ASSERT_EQUALS(m.values().coeff(1), 0.0);
    TS_ASSERT_EQUALS(m.values().coeff(2), 0.0);
    M = foo.max();
    TS_ASSERT_EQUALS(M.isOccluded(), true);
    TS_ASSERT_EQUALS(M.residuals().coeff(0), -1.0);
    TS_ASSERT_EQUALS(M.values().coeff(0), 0.0);
    TS_ASSERT_EQUALS(M.values().coeff(1), 0.0);
    TS_ASSERT_EQUALS(M.values().coeff(2), 0.0);
  };
  
  CXXTEST_TEST(derivative)
  {
    ma::math::Position pos(10);
    ma::math::Array<3> vel, acc;
    double dt = 0.01;
    
    pos.values() << 678.95081, 516.88076, 129.51445,
                    688.18839, 515.96872, 130.71146,
                    697.74103, 515.51837, 132.09829,
                    707.47815, 515.53659, 133.68383,
                    717.27279, 516.00245, 135.47140,
                    727.02881, 516.87295, 137.45421,
                    736.70306, 518.08813, 139.61703,
                    746.30301, 519.56851, 141.93875,
                    755.84177, 521.20685, 144.39008,
                    765.28085, 522.87353, 146.93189;
    pos.residuals().setZero();
    
    // FIRST DERIVATE
    
    ma::math::Array<3>::Values vref(10,3);
    vref << 908.005000, -114.288500, 110.210000,
            939.511000,  -68.119500, 129.192000,
            964.488000,  -21.606500, 148.618500,
            976.588000,   24.204000, 168.655500,
            977.533000,   66.818000, 188.519000,
            971.513500,  104.284000, 207.281500,
            963.710000,  134.778000, 224.227000,
            956.935500,  155.936000, 238.652500,
            948.892000,  165.251000, 249.657000,
            938.924000,  168.085000, 258.705000;
    ma::math::Array<3>::Residuals rref(10,1);
    rref.setZero();
           
    vel = pos.derivative<1>(dt);
    TS_ASSERT_EIGEN_DELTA(vel.values(), vref, 1e-5);
    TS_ASSERT_EIGEN_DELTA(vel.residuals(), rref, 1e-15);
    
    // Occlusion for the first sample
    pos.residuals().coeffRef(0) = -1.0;
    vref.row(0).setZero();
    rref.row(0).setConstant(-1.0);
    vref.row(1) << 946.040000, -68.463500, 128.747500;
    vel = pos.derivative<1>(dt);
    TS_ASSERT_EIGEN_DELTA(vel.values(), vref, 1e-5);
    TS_ASSERT_EIGEN_DELTA(vel.residuals(), rref, 1e-15);
           
    // Occlusion for the eighth sample
    pos.residuals().coeffRef(7) = -1.0;
    vref.bottomRows<3>().setZero();
    rref.bottomRows<3>().setConstant(-1.0);
    vref.row(6) << 963.336500, 138.752000, 225.282500;
    vel = pos.derivative<1>(dt);
    TS_ASSERT_EIGEN_DELTA(vel.values(), vref, 1e-5);
    TS_ASSERT_EIGEN_DELTA(vel.residuals(), rref, 1e-15);
    
    // Occlusion on the whole signal
    pos.residuals().setConstant(-1.0);
    vref.setZero();
    rref.setConstant(-1.0);
    vel = pos.derivative<1>(dt);
    TS_ASSERT_EIGEN_DELTA(vel.values(), vref, 1e-5);
    TS_ASSERT_EIGEN_DELTA(vel.residuals(), rref, 1e-15);
    
    // DERIVATE OF DERIVATE
    
    ma::math::Array<3>::Values aref(10,3);
    aref << 3477.050000, 4599.700000, 1875.975000,
            2824.150000, 4634.100000, 1920.425000,
            1853.850000, 4616.175000, 1973.175000,
             652.250000, 4421.225000, 1995.025000,
            -253.725000, 4004.000000, 1931.300000,
            -691.150000, 3398.000000, 1785.400000,
            -728.900000, 2582.600000, 1568.550000,
            -740.900000, 1523.650000, 1271.500000,
            -900.575000,  607.450000, 1002.625000,
           -1093.025000,  -40.650000,  806.975000;
    rref.setZero();
    
    pos.residuals().setZero();
    acc = pos.derivative<1>(dt).derivative<1>(dt);
    TS_ASSERT_EIGEN_DELTA(acc.values(), aref, 1e-5);
    TS_ASSERT_EIGEN_DELTA(acc.residuals(), rref, 1e-15);
    
    // SECOND DERIVATE
    
    aref << 4456.400000,  4548.100000, 1809.300000,
            3150.600000,  4616.900000, 1898.200000,
            1844.800000,  4685.700000, 1987.100000,
             575.200000,  4476.400000, 2020.300000,
            -386.200000,  4046.400000, 1952.400000,
            -817.700000,  3446.800000, 1800.100000,
            -743.000000,  2652.000000, 1589.000000,
            -611.900000,  1579.600000, 1296.100000,
            -996.800000,   283.400000,  904.800000,
           -1381.700000, -1012.800000,  513.500000;
           
   acc = pos.derivative<2>(dt);
   TS_ASSERT_EIGEN_DELTA(acc.values(), aref, 1e-5);
   TS_ASSERT_EIGEN_DELTA(acc.residuals(), rref, 1e-15);
   
   // Occlusion for the sixth sample
   pos.residuals().coeffRef(5) = -1.0;
   aref.row(5).setZero();
   rref.row(5).setConstant(-1.0);
   aref.row(4) << -694.400000, 4267.100000, 2053.500000;
   aref.row(6) << -227.000000, 2875.800000, 1687.400000;
   acc = pos.derivative<2>(dt);
   TS_ASSERT_EIGEN_DELTA(acc.values(), aref, 1e-5);
   TS_ASSERT_EIGEN_DELTA(acc.residuals(), rref, 1e-15);
  };
  
  CXXTEST_TEST(derivativebis)
  {
    ma::math::Scalar temp(1000), derivate;
    double dt = 0.01;
    temp.values().setRandom();
    // No hole
    temp.residuals().setZero();
    derivate = temp.derivative<1>(dt);
    TS_ASSERT_EQUALS((derivate.residuals() == 0).all(), true);
    // Front hole
    temp.residuals().segment(0,100).setConstant(-1.0);
    derivate = temp.derivative<1>(dt);
    TS_ASSERT_DIFFERS((derivate.residuals() == 0).all(), true);
    // Back hole
    temp.residuals().setZero();
    temp.residuals().segment(920,80).setConstant(-1.0);
    derivate = temp.derivative<1>(dt);
    for (int i = 0 ; i < 920 ; ++i)
      TS_ASSERT_EQUALS(temp.residuals().coeff(i),0.);
    for (int i = 920 ; i < 1000 ; ++i)
      TS_ASSERT_EQUALS(temp.residuals().coeff(i),-1.);
    // Front and Back hole
    temp.residuals().setZero();
    temp.residuals().segment(0,100).setConstant(-1.0);
    temp.residuals().segment(920,80).setConstant(-1.0);
    derivate = temp.derivative<1>(dt);
    for (int i = 0 ; i < 100 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.);
    for (int i = 100 ; i < 920 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),0.);
    for (int i = 920 ; i < 1000  ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.0);
    // Front, Mid, and Back hole
    temp.residuals().segment(500,50).setConstant(-1.0);
    derivate = temp.derivative<1>(dt);
    for (int i = 0 ; i < 100 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.);
    for (int i = 100 ; i < 500 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),0.);
    for (int i = 500 ; i < 550 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.);
    for (int i = 550 ; i < 920 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),0.);
    for (int i = 920 ; i < 1000  ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.0);
    // Front, Mid (creating a window too small to be derivated), and Back hole
    temp.residuals().segment(552,2).setConstant(-1.0);
    derivate = temp.derivative<1>(dt);
    for (int i = 0 ; i < 100 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.);
    for (int i = 100 ; i < 500 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),0.);
    for (int i = 500 ; i < 554 ; ++i)
      TSM_ASSERT_EQUALS(std::to_string(i).c_str(),derivate.residuals().coeff(i),-1.);
    for (int i = 554 ; i < 920 ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),0.);
    for (int i = 920 ; i < 1000  ; ++i)
      TS_ASSERT_EQUALS(derivate.residuals().coeff(i),-1.0);
  }
  
  CXXTEST_TEST(skewRedux)
  {
    ma::math::Array<9> skew(3);
    skew.values() << 0., 1., 2., -1., 0., 3., -2., -3., 0.,
                     0., 4., 5., -4., 0., 6., -5., -6., 0.,
                     0., 7., 8., -7., 0., 9., -8., -9., 0.;
    ma::math::Array<1>::Values sx(3,1), sy(3,1), sz(3,1);
    sx <<  3.,  6.,  9.;
    sy << -2., -5., -8.;
    sz <<  1.,  4.,  7.;
    ma::math::Vector::Values redux = skew.skewRedux().values();
    TS_ASSERT_EIGEN_DELTA(redux.col(0), sx, 1e-15);
    TS_ASSERT_EIGEN_DELTA(redux.col(1), sy, 1e-15);
    TS_ASSERT_EIGEN_DELTA(redux.col(2), sz, 1e-15);
  };
  
  CXXTEST_TEST(downsample)
  {
    ma::math::Array<3> d(30);
    d.values().setRandom();
    d.residuals().setRandom();
    ma::math::Array<3> dds = d.downsample(3);
    TS_ASSERT_EQUALS(dds.rows(), 10);
    TS_ASSERT_EQUALS(dds.cols(), 3);
    const auto& dv = d.values();
    const auto& dr = d.residuals();
    const auto& ddsv = dds.values();
    const auto& ddsr = dds.residuals();
    for (unsigned i = 0 ; i < 10 ; ++i)
    {
      TS_ASSERT_DELTA(ddsv.coeff(i,0), dv.coeff(i*3,0), 1e-15);
      TS_ASSERT_DELTA(ddsv.coeff(i,1), dv.coeff(i*3,1), 1e-15);
      TS_ASSERT_DELTA(ddsv.coeff(i,2), dv.coeff(i*3,2), 1e-15);
      TS_ASSERT_DELTA(ddsr.coeff(i,0), dr.coeff(i*3,0), 1e-15);
    }
  };
  
  CXXTEST_TEST(resize)
  {
    TS_WARN("TODO");
  };
  
  CXXTEST_TEST(dot)
  {
    ma::math::Array<3> d1(10), d2(10);
    d1.values().setRandom(); d1.residuals().setZero();
    d2.values().setRandom(); d2.residuals().setZero();
    ma::math::Array<1> dd = d1.dot(d2);
    const auto& ddv = dd.values();
    ma::math::Array<1>::Values ref = d1.values().col(0) * d2.values().col(0) + d1.values().col(1) * d2.values().col(1) + d1.values().col(2) * d2.values().col(2);
    for (unsigned i = 0 ; i < 10 ; ++i)
      TS_ASSERT_DELTA(ddv.coeff(i), ref.coeff(i), 1e-15);
  };
  
  CXXTEST_TEST(dotReplicate)
  {
    ma::math::Array<3> d1(10), d2(10);
    d1.values().setRandom(); d1.residuals().setZero();
    d2.values().setRandom(); d2.residuals().setZero();
    ma::math::Array<3> dd = d1.dot(d2).replicate<3>();
    const auto& ddv = dd.values();
    ma::math::Array<1>::Values ref = d1.values().col(0) * d2.values().col(0) + d1.values().col(1) * d2.values().col(1) + d1.values().col(2) * d2.values().col(2);
    for (unsigned i = 0 ; i < 10 ; ++i)
    {
      TS_ASSERT_DELTA(ddv.coeff(i,0), ref.coeff(i), 1e-15);
      TS_ASSERT_DELTA(ddv.coeff(i,1), ref.coeff(i), 1e-15);
      TS_ASSERT_DELTA(ddv.coeff(i,2), ref.coeff(i), 1e-15);
    }
  };
};

CXXTEST_SUITE_REGISTRATION(ArrayTest)
CXXTEST_TEST_REGISTRATION(ArrayTest, assignment)
CXXTEST_TEST_REGISTRATION(ArrayTest, norm)
CXXTEST_TEST_REGISTRATION(ArrayTest, mean)
CXXTEST_TEST_REGISTRATION(ArrayTest, scaledDifference)
CXXTEST_TEST_REGISTRATION(ArrayTest, normalized)
CXXTEST_TEST_REGISTRATION(ArrayTest, cross)
CXXTEST_TEST_REGISTRATION(ArrayTest, crossBis)
CXXTEST_TEST_REGISTRATION(ArrayTest, operatorDouble)
CXXTEST_TEST_REGISTRATION(ArrayTest, replicate)
CXXTEST_TEST_REGISTRATION(ArrayTest, replicateBis)
CXXTEST_TEST_REGISTRATION(ArrayTest, replicateTer)
CXXTEST_TEST_REGISTRATION(ArrayTest, transpose)
CXXTEST_TEST_REGISTRATION(ArrayTest, transposeBis)
CXXTEST_TEST_REGISTRATION(ArrayTest, transposeTer)
CXXTEST_TEST_REGISTRATION(ArrayTest, compoundAssignmentOperators)
CXXTEST_TEST_REGISTRATION(ArrayTest, minmax)
CXXTEST_TEST_REGISTRATION(ArrayTest, derivative)
CXXTEST_TEST_REGISTRATION(ArrayTest, derivativebis)
CXXTEST_TEST_REGISTRATION(ArrayTest, skewRedux)
CXXTEST_TEST_REGISTRATION(ArrayTest, downsample)
CXXTEST_TEST_REGISTRATION(ArrayTest, resize)
CXXTEST_TEST_REGISTRATION(ArrayTest, dot)
CXXTEST_TEST_REGISTRATION(ArrayTest, dotReplicate)
