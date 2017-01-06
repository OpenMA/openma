#include <cxxtest/TestDrive.h>

#include <Eigen_openma/SignalProcessing/DetectPeaks.h>

void fill_vector(Eigen::Matrix<double, Eigen::Dynamic, 1>* data)
{
  data->resize(26,1);
  *data << 0., 6., 25., 20., 15., 8., 15., 6., 0., 6., 0., -5., -15., -3., 4., 10., 8., 13., 8., 10., 3., 1., 20., 7., 3., 0.;
}

CXXTEST_SUITE(DetectPeaksTest)
{
  CXXTEST_TEST(vectorWithoutFilters)
  {
    auto data = Eigen::Matrix<double, Eigen::Dynamic, 1>();
    fill_vector(&data);
    auto indexes = Eigen::detect_peaks(data);
    TS_ASSERT_EQUALS(indexes.rows(),7);
    TS_ASSERT_EQUALS(indexes[0],2);
    TS_ASSERT_EQUALS(indexes[1],6);
    TS_ASSERT_EQUALS(indexes[2],9);
    TS_ASSERT_EQUALS(indexes[3],15);
    TS_ASSERT_EQUALS(indexes[4],17);
    TS_ASSERT_EQUALS(indexes[5],19);
    TS_ASSERT_EQUALS(indexes[6],22);
  };
  
  CXXTEST_TEST(vectorWithMiniHeightFilter)
  {
    auto data = Eigen::Matrix<double, Eigen::Dynamic, 1>();
    fill_vector(&data);
    auto indexes = Eigen::detect_peaks(data,7);
    TS_ASSERT_EQUALS(indexes.rows(),6);
    TS_ASSERT_EQUALS(indexes[0],2);
    TS_ASSERT_EQUALS(indexes[1],6);
    TS_ASSERT_EQUALS(indexes[2],15);
    TS_ASSERT_EQUALS(indexes[3],17);
    TS_ASSERT_EQUALS(indexes[4],19);
    TS_ASSERT_EQUALS(indexes[5],22);
  };
  
  CXXTEST_TEST(vectorWithMiniDistanceFilter)
  {
    auto data = Eigen::Matrix<double, Eigen::Dynamic, 1>();
    fill_vector(&data);
    auto indexes = Eigen::detect_peaks(data,std::nan(""),2);
    TS_ASSERT_EQUALS(indexes.rows(),5);
    TS_ASSERT_EQUALS(indexes[0],2);
    TS_ASSERT_EQUALS(indexes[1],6);
    TS_ASSERT_EQUALS(indexes[2],9);
    TS_ASSERT_EQUALS(indexes[3],17);
    TS_ASSERT_EQUALS(indexes[4],22);
  };
  
  CXXTEST_TEST(vectorWithMiniHeightAndDistanceFilters)
  {
    auto data = Eigen::Matrix<double, Eigen::Dynamic, 1>();
    fill_vector(&data);
    auto indexes = Eigen::detect_peaks(data,7.,2);
    TS_ASSERT_EQUALS(indexes.rows(),4);
    TS_ASSERT_EQUALS(indexes[0],2);
    TS_ASSERT_EQUALS(indexes[1],6);
    TS_ASSERT_EQUALS(indexes[2],17);
    TS_ASSERT_EQUALS(indexes[3],22);
  };
};

CXXTEST_SUITE_REGISTRATION(DetectPeaksTest)
CXXTEST_TEST_REGISTRATION(DetectPeaksTest, vectorWithoutFilters)
CXXTEST_TEST_REGISTRATION(DetectPeaksTest, vectorWithMiniHeightFilter)
CXXTEST_TEST_REGISTRATION(DetectPeaksTest, vectorWithMiniDistanceFilter)
CXXTEST_TEST_REGISTRATION(DetectPeaksTest, vectorWithMiniHeightAndDistanceFilters)