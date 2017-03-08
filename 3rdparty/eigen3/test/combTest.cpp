#include "catch_and_extras.hpp"

#include <Eigen_openma/Utils/comb.h>

TEST_CASE("comb function", "[eigen][comb]")
{
  SECTION("floats")
  {
    CHECK( comb(2.0f, 4.0f) == AROUND(0.0f, 1e-5) );
    CHECK( comb(4.0f, 2.0f) == AROUND(6.0f, 1e-5) );
    CHECK( comb(0.0f, 0.0f) == AROUND(1.0f, 1e-5) );
    CHECK( comb(3.0f, 2.0f) == AROUND(2.9999999999999996f, 1e-5) );
    CHECK( comb(3.5f, 2.45f) == AROUND(3.6167234401282293f, 1e-5) );
    CHECK( comb(25.0f, 78.0f) == AROUND(0.0f, 1e-5) );
    CHECK( comb(25.0f, 25.0f) == AROUND(1.0f, 1e-5) );
    CHECK( comb(25.0f, 22.0f) == AROUND(2299.9999999999886f, 1e-1) ); // Less accurate ...
    CHECK( comb(25.4589653f, 22.4567891235f) == AROUND(2444.6161943794596f, 1e-1) ); // Less accurate ...
  }

  SECTION("doubles")
  {
    CHECK( comb(2.0, 4.0) == AROUND(0.0, 1e-15) );
    CHECK( comb(4.0, 2.0) == AROUND(6.0, 5e-15) );
    CHECK( comb(0.0, 0.0) == AROUND(1.0, 1e-15) );
    CHECK( comb(3.0, 2.0) == AROUND(2.9999999999999996, 1e-13) );
    CHECK( comb(3.5, 2.45) == AROUND(3.6167234401282293, 1e-13) );
    CHECK( comb(25.0, 78.0) == AROUND(0.0, 1e-15) );
    CHECK( comb(25.0, 25.0) == AROUND(1.0, 1e-15) );
    CHECK( comb(25.0, 22.0) == AROUND(2299.9999999999886, 5e-11) ); // The difference is due to the method used in the computation of gammaln
    CHECK( comb(25.4589653, 22.4567891235) == AROUND(2444.6161943794596, 5e-11) ); // The difference is due to the method used in the computation of gammaln
  }

  SECTION("integers")
  {
    CHECK( comb(2, 4) == 0);
    CHECK( comb(4, 2) == 6);
    CHECK( comb(0, 0) == 1);
    CHECK( comb(25, 78) == 0);
    CHECK( comb(25, 25) == 1);
    CHECK( comb(25, 22) == 2300);
  }
}