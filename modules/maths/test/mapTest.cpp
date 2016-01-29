#include <cxxtest/TestDrive.h>

#include <openma/maths.h>

using MappedScalar = ma::maths::Map<ma::maths::Scalar>;
using MappedConstScalar = ma::maths::Map<const ma::maths::Scalar>;

CXXTEST_SUITE(MapTest)
{
  CXXTEST_TEST(scaledDifference)
  {
    double _a[20] = {0};
    double _b[20] = {0};
    double _c[20] = {0};

    MappedScalar a(10,_a,_a+10), b(10,_b,_b+10), c(10,_c,_c+10);
    a.values().setRandom();
    a.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    b.values().setRandom();
    b.residuals() <<  0.5, 0.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0,  0.0;

    c = 0.5 * (a - b);

    // Values
    for (int i = 1 ; i < 9 ; ++i)
      TS_ASSERT_EQUALS(_c[i], (_a[i] - _b[i]) / 2.0);
    TS_ASSERT_EQUALS(_c[0], 0.0);
    TS_ASSERT_EQUALS(_c[9], 0.0);
    // Residuals
    TS_ASSERT_EQUALS(_c[10], -1.0);
    TS_ASSERT_EQUALS(_c[11], 0.0);
    TS_ASSERT_EQUALS(_c[12], 0.0);
    TS_ASSERT_EQUALS(_c[13], 0.0);
    TS_ASSERT_EQUALS(_c[14], 0.0);
    TS_ASSERT_EQUALS(_c[15], 0.0);
    TS_ASSERT_EQUALS(_c[16], 0.0);
    TS_ASSERT_EQUALS(_c[17], 0.0);
    TS_ASSERT_EQUALS(_c[18], 0.0);
    TS_ASSERT_EQUALS(_c[19], -1.0);
  };

  CXXTEST_TEST(null)
  {
    MappedScalar a(0,nullptr,nullptr);
    MappedScalar b = a;
    TS_ASSERT_EQUALS(a.isValid(),false);
    TS_ASSERT_EQUALS(b.isValid(),false);
  };
  
  CXXTEST_TEST(nonConstToConstAssignment)
  {
    double _a[20] = {0};
    MappedScalar a(10,_a,_a+10);
    MappedScalar b = a;
    a.values().setRandom();
    a.residuals().setZero();
    MappedConstScalar c = a;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.values()[i], c.values()[i]);
    a.values()[0] = 10.0;
    TS_ASSERT_EQUALS(a.values()[0], b.values()[0]);
    TS_ASSERT_EQUALS(a.values()[0], c.values()[0]);
    MappedConstScalar d = b;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.values()[i], d.values()[i]);
    ma::maths::Scalar e = a;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.values()[i], e.values()[i]);
    e.values().setRandom();
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_DIFFERS(a.values()[i], e.values()[i]);
    const ma::maths::Scalar f = a;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.values()[i], f.values()[i]);
    ma::maths::Scalar g = d;
    g.values().setRandom();
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_DIFFERS(d.values()[i], g.values()[i]);
    const ma::maths::Scalar h = d;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(d.values()[i], h.values()[i]);
  };
};

CXXTEST_SUITE_REGISTRATION(MapTest)
CXXTEST_TEST_REGISTRATION(MapTest, scaledDifference)
CXXTEST_TEST_REGISTRATION(MapTest, null)
CXXTEST_TEST_REGISTRATION(MapTest, nonConstToConstAssignment)