#ifndef __cxxtest_tdd_h
#define __cxxtest_tdd_h

#ifndef CXXTEST_RUNNING
    #define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH

#include <cxxtest/TestGenerator.h>

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

#define TS_ASSERT_EIGEN_DELTA(lhs,rhs,prec) \
  TS_ASSERT((lhs - rhs).cwiseAbs().maxCoeff() <= prec)
    
#define TSM_ASSERT_EIGEN_DELTA(msg,lhs,rhs,prec) \
  TSM_ASSERT(msg + " (" + std::to_string((lhs - rhs).cwiseAbs().maxCoeff()) + " > " + std::to_string(prec) + ")", (lhs - rhs).cwiseAbs().maxCoeff() <= prec)

#endif // __cxxtest_tdd_h