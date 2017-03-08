#include "catch_and_extras.hpp"

#include <Eigen_openma/Core/Cumtrapz.h>

TEST_CASE("cumptraz function", "[eigen][cumptraz]")
{

  SECTION("Column Eigen::VectorXd")
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> y(6,1), z;
    y << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
    z = Eigen::cumtrapz(y);
    CHECK( z.coeff(0) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(1) == AROUND(1.5, 1e-15) );
    CHECK( z.coeff(2) == AROUND(4.0, 1e-15) );
    CHECK( z.coeff(3) == AROUND(7.5, 1e-15) );
    CHECK( z.coeff(4) == AROUND(12.0, 1e-15) );
    CHECK( z.coeff(5) == AROUND(17.5, 1e-15) );
  }

  SECTION("Eigen::MatrixX2d")
  {
    Eigen::Matrix<double, Eigen::Dynamic, 2> y(6,2), z;
    y << 1.0, 7.0, 2.0, 8.0, 3.0, 9.0, 4.0, 10.0, 5.0, 11.0, 6.0, 12.0;
    z = Eigen::cumtrapz(y);
    CHECK( z.coeff(0,0) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(1,0) == AROUND(1.5, 1e-15) );
    CHECK( z.coeff(2,0) == AROUND(4.0, 1e-15) );
    CHECK( z.coeff(3,0) == AROUND(7.5, 1e-15) );
    CHECK( z.coeff(4,0) == AROUND(12.0, 1e-15) );
    CHECK( z.coeff(5,0) == AROUND(17.5, 1e-15) );
    CHECK( z.coeff(0,1) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(1,1) == AROUND(7.5, 1e-15) );
    CHECK( z.coeff(2,1) == AROUND(16.0, 1e-15) );
    CHECK( z.coeff(3,1) == AROUND(25.5, 1e-15) );
    CHECK( z.coeff(4,1) == AROUND(36.0, 1e-15) );
    CHECK( z.coeff(5,1) == AROUND(47.5, 1e-15) );
  }

  SECTION("Eigen::Matrix2Xd")
  {
    Eigen::Matrix<double, 2, Eigen::Dynamic> y(2,6), z;
    y << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0;
    z = Eigen::cumtrapz(y);
    CHECK( z.coeff(0,0) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(0,1) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(0,2) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(0,3) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(0,4) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(0,5) == AROUND(0.0, 1e-15) );
    CHECK( z.coeff(1,0) == AROUND(4.0, 1e-15) );
    CHECK( z.coeff(1,1) == AROUND(5.0, 1e-15) );
    CHECK( z.coeff(1,2) == AROUND(6.0, 1e-15) );
    CHECK( z.coeff(1,3) == AROUND(7.0, 1e-15) );
    CHECK( z.coeff(1,4) == AROUND(8.0, 1e-15) );
    CHECK( z.coeff(1,5) == AROUND(9.0, 1e-15) );
  }

  SECTION("Eigen::MatrixX2d per column")
  {
    Eigen::Matrix<double, Eigen::Dynamic, 2> y(6,2);
    Eigen::Matrix<double, Eigen::Dynamic, 1> z1, z2;
    y << 1.0, 7.0, 2.0, 8.0, 3.0, 9.0, 4.0, 10.0, 5.0, 11.0, 6.0, 12.0;
    z1 = Eigen::cumtrapz(y.col(0));
    z2 = Eigen::cumtrapz(y.col(1));
    CHECK( z1.coeff(0) == AROUND(0.0, 1e-15) );
    CHECK( z1.coeff(1) == AROUND(1.5, 1e-15) );
    CHECK( z1.coeff(2) == AROUND(4.0, 1e-15) );
    CHECK( z1.coeff(3) == AROUND(7.5, 1e-15) );
    CHECK( z1.coeff(4) == AROUND(12.0, 1e-15) );
    CHECK( z1.coeff(5) == AROUND(17.5, 1e-15) );
    CHECK( z2.coeff(0) == AROUND(0.0, 1e-15) );
    CHECK( z2.coeff(1) == AROUND(7.5, 1e-15) );
    CHECK( z2.coeff(2) == AROUND(16.0, 1e-15) );
    CHECK( z2.coeff(3) == AROUND(25.5, 1e-15) );
    CHECK( z2.coeff(4) == AROUND(36.0, 1e-15) );
    CHECK( z2.coeff(5) == AROUND(47.5, 1e-15) );
  }

}