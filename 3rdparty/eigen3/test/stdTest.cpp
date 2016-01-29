#include <cxxtest/TestDrive.h>

#include "tdd_eigen_config.h"

CXXTEST_SUITE(StdTest)
{
  CXXTEST_TEST(vectorXd)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 1> data;
    data.resize(4,1);  data << 1.0, 8.0, 5.0, 7.0;
    TS_ASSERT_DELTA(data.std(), 3.095695936834452, 1e-15);
    data.resize(5,1);  data << 1.0, 8.0, 5.0, 7.0, 3.0;
    TS_ASSERT_DELTA(data.std(), 2.863564212655271, 1e-15);
    data.resize(5,1);  data << 1.0, 8.0, 5.0, 7.0, 1.0;
    TS_ASSERT_DELTA(data.std(), 3.286335345030997, 1e-15);
    data.resize(5,1);  data << 1.0, 8.0, 5.0, 1.0, 1.0;
    TS_ASSERT_DELTA(data.std(), 3.193743884534263, 1e-15);
    data.resize(2,1);  data << 1.0, 8.0;
    TS_ASSERT_DELTA(data.std(), 4.949747468305833, 1e-15);
    data.resize(1,1);  data << 0.5;
    TS_ASSERT_DELTA(data.std(), 0, 1e-15);
  };
  
  CXXTEST_TEST(rowVectorXd)
  {
    Eigen::Matrix<double, 1, Eigen::Dynamic> data;
    data.resize(1,4);  data << 1.0, 8.0, 5.0, 7.0;
    TS_ASSERT_DELTA(data.std(), 3.095695936834452, 1e-15);
    data.resize(1,5);  data << 1.0, 8.0, 5.0, 7.0, 3.0;
    TS_ASSERT_DELTA(data.std(), 2.863564212655271, 1e-15);
    data.resize(1,5);  data << 1.0, 8.0, 5.0, 7.0, 1.0;
    TS_ASSERT_DELTA(data.std(), 3.286335345030997, 1e-15);
    data.resize(1,5);  data << 1.0, 8.0, 5.0, 1.0, 1.0;
    TS_ASSERT_DELTA(data.std(), 3.193743884534263, 1e-15);
    data.resize(1,2);  data << 1.0, 8.0;
    TS_ASSERT_DELTA(data.std(), 4.949747468305833, 1e-15);
    data.resize(1,1);  data << 0.5;
    TS_ASSERT_DELTA(data.std(), 0, 1e-15);
  };
  
  CXXTEST_TEST(matrixXdColwise)
  {
    Eigen::Matrix<double, Eigen::Dynamic, 3> data;
    data.resize(6,3);  data << 0.313803798336819, 6.732991328090547, 4.368957185030585, 1.160349385283249, 8.187858082704581, 1.625225008684635, 2.508550429013066, 9.489298045196573, 3.098208693314368, 7.596749456650356, 8.743153274054658, 6.811094223541295, 8.983096251825591, 3.937363024907196, 9.340549840356969, 2.234204874958241, 9.369623161748992, 9.473892316240427;
    Eigen::Matrix<double, 1, 3> m = data.colwise().std();
    TS_ASSERT_DELTA(m(0), 3.592232373571108, 1e-15);
    TS_ASSERT_DELTA(m(1), 2.116442982468776, 1e-15);
    TS_ASSERT_DELTA(m(2), 3.281172742418526, 1e-15);
  };
  
  CXXTEST_TEST(matrixXdRowwise)
  {
    Eigen::Matrix<double, 3, Eigen::Dynamic> data;
    data.resize(3, 6);  data << 0.313803798336819, 7.596749456650356, 6.732991328090547, 8.743153274054658, 4.368957185030585, 6.811094223541295, 1.160349385283249, 8.983096251825591, 8.187858082704581, 3.937363024907196, 1.625225008684635, 9.340549840356969, 2.508550429013066, 2.234204874958241, 9.489298045196573, 9.369623161748992, 3.098208693314368, 9.473892316240427;
    Eigen::Matrix<double, 3, 1> m = data.rowwise().std();
    TS_ASSERT_DELTA(m(0), 3.030524997497967, 1e-15);
    TS_ASSERT_DELTA(m(1), 3.751945793285269, 1e-15);
    TS_ASSERT_DELTA(m(2), 3.751913688743245, 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(StdTest)
CXXTEST_TEST_REGISTRATION(StdTest, vectorXd)
CXXTEST_TEST_REGISTRATION(StdTest, rowVectorXd)
CXXTEST_TEST_REGISTRATION(StdTest, matrixXdColwise)
CXXTEST_TEST_REGISTRATION(StdTest, matrixXdRowwise)
