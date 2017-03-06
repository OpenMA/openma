#ifndef __catch_extras_h
#define __catch_extras_h

#include "catch.hpp"

#define AROUND(value, absTol) \
  Approx(value).epsilon(0.).margin(absTol)

#endif // __catch_extras_h