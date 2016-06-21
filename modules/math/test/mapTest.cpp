#include <cxxtest/TestDrive.h>

#include <openma/math.h>

using MappedScalar = ma::math::Map<ma::math::Scalar>;
using MappedConstScalar = ma::math::Map<const ma::math::Scalar>;

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
    ma::math::Scalar e = a;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.values()[i], e.values()[i]);
    e.values().setRandom();
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_DIFFERS(a.values()[i], e.values()[i]);
    const ma::math::Scalar f = a;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(a.values()[i], f.values()[i]);
    ma::math::Scalar g = d;
    g.values().setRandom();
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_DIFFERS(d.values()[i], g.values()[i]);
    const ma::math::Scalar h = d;
    for (int i = 0 ; i < 10 ; ++i)
      TS_ASSERT_EQUALS(d.values()[i], h.values()[i]);
  };
  
  CXXTEST_TEST(downsample)
  {
    double dv[90], dr[30];
    auto d = ma::math::Map<ma::math::Array<3>>(30,dv,dr);
    d.values().setRandom();
    d.residuals().setRandom();
    ma::math::Array<3> dds = d.downsample(3);
    TS_ASSERT_EQUALS(dds.rows(), 10);
    TS_ASSERT_EQUALS(dds.cols(), 3);
    const auto& ddsv = dds.values();
    const auto& ddsr = dds.residuals();
    for (unsigned i = 0 ; i < 10 ; ++i)
    {
      TS_ASSERT_DELTA(ddsv.coeff(i,0), dv[i*3],    1e-15);
      TS_ASSERT_DELTA(ddsv.coeff(i,1), dv[i*3+30], 1e-15);
      TS_ASSERT_DELTA(ddsv.coeff(i,2), dv[i*3+60], 1e-15);
      TS_ASSERT_DELTA(ddsr.coeff(i,0), dr[i*3],    1e-15);
    }
  };
};

CXXTEST_SUITE_REGISTRATION(MapTest)
CXXTEST_TEST_REGISTRATION(MapTest, scaledDifference)
CXXTEST_TEST_REGISTRATION(MapTest, null)
CXXTEST_TEST_REGISTRATION(MapTest, nonConstToConstAssignment)
CXXTEST_TEST_REGISTRATION(MapTest, downsample)