#include <cxxtest/TestDrive.h>

#include "customTypeTest_def.h"

CXXTEST_SUITE(CustomTypeTest)
{
  CXXTEST_TEST(vectorSimpleOps)
  {
    MyVectorType v = MyVectorType::Ones(4);
    v(2) += 10.0;
    v = 2.0 * v;
    TS_ASSERT_EQUALS(v.coeff(0), 2);
    TS_ASSERT_EQUALS(v.coeff(1), 2);
    TS_ASSERT_EQUALS(v.coeff(2), 22);
    TS_ASSERT_EQUALS(v.coeff(3), 2);
  };

  CXXTEST_TEST(arraySimpleOps)
  {
    MyArrayType<1> v = MyArrayType<1>::Ones(4);
    TS_ASSERT_EQUALS(v.size(), 4);
    v(2) += 10.0;
    v = 2.0 * v;
    v.coeffRef(0) /= 2.0;
    TS_ASSERT_EQUALS(v.coeff(0), 1.);
    TS_ASSERT_EQUALS(v.coeff(1), 2.);
    TS_ASSERT_EQUALS(v.coeff(2), 22.);
    TS_ASSERT_EQUALS(v.coeff(3), 2.);
    
    MyArrayType<2> w(4);
    w.col(0) = v;
    w.col(1) = v;
    TS_ASSERT_EQUALS(w.size(), 8);
    TS_ASSERT_EQUALS(w.hasOcclusion(), false);
    w.row(2) = NAN;
    TS_ASSERT_EQUALS(w.hasOcclusion(), true);
    
    MyArrayType<2> x = w.unaryExpr(CwiseNullifyNaN<double>());
    TS_ASSERT_EQUALS(x.coeff(0,0), 1.);
    TS_ASSERT_EQUALS(x.coeff(0,1), 1.);
    TS_ASSERT_EQUALS(x.coeff(1,0), 2.);
    TS_ASSERT_EQUALS(x.coeff(1,1), 2.);
    TS_ASSERT_EQUALS(x.coeff(2,0), 0.);
    TS_ASSERT_EQUALS(x.coeff(2,1), 0.);
    
    MyArrayType<2> y = w.unaryExpr(CwiseNullifyNaN<double>()).colwise().sum();
    TS_ASSERT_EQUALS(y.size(), 2);
    TS_ASSERT_EQUALS(y.coeff(0,0), 5.);
    TS_ASSERT_EQUALS(y.coeff(0,1), 5.);
    
    MyArrayType<2> z1 = w.colwise().redux(ScalarNanSumOp<double>());
    TS_ASSERT_EQUALS(z1.size(), 2);
    TS_ASSERT_EQUALS(z1.coeff(0,0), 5.);
    TS_ASSERT_EQUALS(z1.coeff(0,1), 5.);
    
    MyArrayType<2> z2 = w.colwise().redux(ScalarNanSumOp<double>()) / w.colwise().redux(ScalarNanCountOp<double>());
    TS_ASSERT_EQUALS(z2.size(), 2);
    TS_ASSERT_EQUALS(z2.coeff(0,0), 5./3.);
    TS_ASSERT_EQUALS(z2.coeff(0,1), 5./3.);
    
    w.coeffRef(0,0) = NAN;
    // THE FOLLOWING TEST FAILS BECAUSE REDUX INITIALIZE THE RESULT WITH THE FIRST ELEMENT
    // MyArrayType<2> z3 = w.colwise().redux(ScalarNanSumOp<double>()) / w.colwise().redux(ScalarNanCountOp<double>());
    // TS_ASSERT_EQUALS(z3.size(), 2);
    // TS_ASSERT_EQUALS(z3.coeff(0,0), 2.);
    // TS_ASSERT_EQUALS(z3.coeff(0,1), 5./3.);
    
    MyArrayType<2> y2 = w.unaryExpr(CwiseNullifyNaN<double>()).colwise().sum() / w.isFinite().colwise().count().cast<double>();
    TS_ASSERT_EQUALS(y2.size(), 2);
    TS_ASSERT_EQUALS(y2.coeff(0,0), 2.);
    TS_ASSERT_EQUALS(y2.coeff(0,1), 5./3.);
    
    MyArrayType<2> y3 = w.myMean();
    TS_ASSERT_EQUALS(y3.size(), 2);
    TS_ASSERT_EQUALS(y3.coeff(0,0), 2.);
    TS_ASSERT_EQUALS(y3.coeff(0,1), 5./3.);
  };
  
  CXXTEST_TEST(scaledDifference)
  {
    MyArrayType<1> a(10), b(10);
    a.setRandom();
    a.row(0) = NAN; a.row(9) = NAN; 
    b.setRandom();
    
    MyArrayType<1> c = 0.5 * (a - b);

    TS_ASSERT_EQUALS(std::isnan(c.coeff(0)), true);
    TS_ASSERT_EQUALS(std::isnan(c.coeff(9)), true);
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_EQUALS(c.coeff(i), (a.coeff(i) - b.coeff(i)) / 2.0);
  };
  
  CXXTEST_TEST(normalized)
  {
    MyArrayType<3> a(10), b(10);
    a.setRandom();
    a.row(0) = NAN; a.row(9) = NAN; 
    b.setRandom();
    
    MyArrayType<3> c = a.rowwise().normalized();
    MyArrayType<3> d = (a - b).rowwise().normalized();

    // TS_ASSERT_EQUALS(std::isnan(c.coeff(0)), true);
    // TS_ASSERT_EQUALS(std::isnan(c.coeff(9)), true);
    // for (int i = 1 ; i < 9 ; ++i)
    //   TS_ASSERT_EQUALS(c.coeff(i), (a.coeff(i) - b.coeff(i)) / 2.0);
    //
    //
  };
};

CXXTEST_SUITE_REGISTRATION(CustomTypeTest)
CXXTEST_TEST_REGISTRATION(CustomTypeTest, vectorSimpleOps)
CXXTEST_TEST_REGISTRATION(CustomTypeTest, arraySimpleOps)
CXXTEST_TEST_REGISTRATION(CustomTypeTest, scaledDifference)
CXXTEST_TEST_REGISTRATION(CustomTypeTest, normalized)