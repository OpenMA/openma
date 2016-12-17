#ifndef plugingaitTest_def_h
#define plugingaitTest_def_h

#include <openma/body/plugingait.h>
#include <openma/body/enums.h>
#include <openma/body/model.h>
#include <openma/base/timesequence.h>
#include <openma/base/trial.h>

#if defined(_TEST_BODY_USE_IO)

#include <openma/io/handlerreader.h>
#include <openma/io/file.h>
#include <openma/math.h>

#include <cassert>

ma::TimeSequence* make_marker(const std::string& name, double* data, ma::Trial* trial)
{
  const unsigned samples = 1;
  const unsigned components = 4;
  const double rate = 100.0;
  const double start = 0.0;
  const std::string unit = "mm";
  auto marker = new ma::TimeSequence(name, components, samples, rate, start, ma::TimeSequence::Position, unit, trial->timeSequences());
  std::copy_n(data, 4, marker->data());
  return marker;
}

void generate_static_trial_oneframe(ma::Node* root)
{
  const unsigned num_markers = 35;
  
  // Note: all the residuals were set to 0.0 as the original one were not exported.
  double raw[num_markers*4] = {
      -9.31614, 98.4004, 1708.82,   0.0, // LFHD
    -102.067,  106.608,  1699.85,   0.0, // RFHD
      49.114,  239.001,  1690.48,   0.0, // LBHD
    -135.49,   225.241,  1693.51,   0.0, // RBHD
     -32.101,  305.965,  1537.99,   0.0, // C7
     -29.3063, 366.065,  1234.47,   0.0, // T10
     -32.0801, 178.05,   1439.51,   0.0, // CLAV
     -31.3109, 151.538,  1255.23,   0.0, // STRN
    -159.875,  376.101,  1392.9,    0.0, // RBAK
     116.273,  272.807,  1522.31,   0.0, // LSHO
     404.687,  305.708,  1368.27,   0.0, // LELB
     650.832,  197.08,   1269.59,   0.0, // LWRA
     656.977,  268.677,  1239.92,   0.0, // LWRB
     729.127,  233.285,  1265.93,   0.0, // LFIN
    -188.522,  275.133,  1500.63,   0.0, // RSHO
    -461.85,   334.356,  1340.68,   0.0, // RELB
    -716.922,  233.089,  1246.88,   0.0, // RWRA
    -723.836,  310.492,  1227.42,   0.0, // RWRB
    -800.992,  267.995,  1219.17,   0.0, // RFIN
      77.72,   174.99,   1020.48,   0.0, // LASI
    -150.94,   176.197,  1024.18,   0.0, // RASI
      12.6587, 353.036,  1038.09,   0.0, // LPSI
     -85.5816, 354.41,   1038.36,   0.0, // RPSI
     186.926,  279.299,   639.768,  0.0, // LTHI
     136.557,  295.284,   521.505,  0.0, // LKNE
     188.745,  341.687,   303.991,  0.0, // LTIB
     137.672,  345.958,    77.651,  0.0, // LANK
      92.4398, 367.748,    46.3688, 0.0, // LHEE
     149.798,  172.335,    40.6635, 0.0, // LTOE
    -240.644,  259.205,   620.718,  0.0, // RTHI
    -185.973,  264.585,   506.987,  0.0, // RKNE
    -240.693,  325.189,   314.01,   0.0, // RTIB
    -157.429,  333.84,     72.7816, 0.0, // RANK
     -99.1657, 381.434,    41.7802, 0.0, // RHEE
    -150.393,  177.695,    39.8623, 0.0, // RTOE
  };
  
  const char* labels[] = {"LFHD", "RFHD", "LBHD", "RBHD", "C7", "T10", "CLAV", "STRN", "RBAK", "LSHO", "LELB", "LWRA", "LWRB", "LFIN", "RSHO", "RELB", "RWRA", "RWRB", "RFIN", "LASI", "RASI", "LPSI", "RPSI", "LTHI", "LKNE", "LTIB", "LANK", "LHEE", "LTOE", "RTHI", "RKNE", "RTIB", "RANK", "RHEE", "RTOE"};
  
  ma::Trial* trial = new ma::Trial("trial",root);
  for (unsigned i = 0 ; i < num_markers ; ++i)
    make_marker(labels[i], raw+i*4, trial);
};

void generate_trial_from_file(ma::Node* root, const char* filename)
{
  ma::io::File file;
  file.open(filename, ma::io::Mode::In);
  ma::io::HandlerReader reader(&file, "org.c3d");
  TS_ASSERT_EQUALS(reader.read(root),true);
  TS_ASSERT_EQUALS(reader.errorCode(), ma::io::Error::None);
  TS_ASSERT_EQUALS(reader.errorMessage(), "");
  assert(root->children().size() == 1u);
};

void compare_segment_motion(ma::body::Model* model, ma::Trial* trial, const std::string& frame, const std::vector<std::string>& markers, std::vector<double> precision = std::vector<double>(4,1e-5))
{
  assert(markers.size() == 4);
  precision.resize(4,1e-5);
  const auto mot = ma::math::to_pose(model->segments()->findChild<const ma::TimeSequence*>(frame));
  const auto o = ma::math::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[0]));
  const auto a = ma::math::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[1]));
  const auto l = ma::math::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[2]));  
  const auto p = ma::math::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[3]));
  TSM_ASSERT_EQUALS(frame+": mapped pose", mot.isValid(), true);
  TSM_ASSERT_EQUALS(frame+": mapped origin", o.isValid(), true);
  TSM_ASSERT_EQUALS(frame+": mapped AP", a.isValid(), true);
  TSM_ASSERT_EQUALS(frame+": mapped LM", l.isValid(), true);
  TSM_ASSERT_EQUALS(frame+": mapped PD", p.isValid(), true);  
  TSM_ASSERT_EIGEN_DELTA(frame+": origin", mot.o().values(), o.values(), precision[0]);
  TSM_ASSERT_EIGEN_DELTA(frame+": u axis", mot.u().values(), (a-o).normalized().values(), precision[1]);
  TSM_ASSERT_EIGEN_DELTA(frame+": v axis", mot.v().values(), (l-o).normalized().values(), precision[2]);
  TSM_ASSERT_EIGEN_DELTA(frame+": w axis", mot.w().values(), (p-o).normalized().values(), precision[3]);
};

void compare_joint_kinematics(ma::Node* kinematics, ma::Trial* trial, const std::string& descriptor, const std::string angle, std::vector<double> precision = std::vector<double>(3,1e-4))
{
  precision.resize(3,1e-4);
  const auto d = ma::math::to_vector(kinematics->findChild<const ma::TimeSequence*>(descriptor));
  const auto a = ma::math::to_vector(trial->timeSequences()->findChild<const ma::TimeSequence*>(angle));
  TSM_ASSERT_EQUALS(descriptor+": mapped descriptor", d.isValid(), true);
  TSM_ASSERT_EQUALS(angle+": mapped angle", a.isValid(), true);
  TSM_ASSERT_EIGEN_DELTA(descriptor+": 1st axis", d.block<1>(0).values(), a.block<1>(0).values(), precision[0]);
  TSM_ASSERT_EIGEN_DELTA(descriptor+": 2nd axis", d.block<1>(1).values(), a.block<1>(1).values(), precision[1]);
  TSM_ASSERT_EIGEN_DELTA(descriptor+": 3rd axis", d.block<1>(2).values(), a.block<1>(2).values(), precision[2]);
  
}

#endif

void fill_parameters_with_fake_value(ma::body::PluginGait* helper)
{
  helper->setMarkerDiameter(14.0);
  helper->setHeadOffsetEnabled(true);
  helper->setRightShoulderOffset(70.0);
  helper->setLeftShoulderOffset(71.0);
  helper->setRightElbowWidth(50.0);
  helper->setLeftElbowWidth(52.0);
  helper->setRightWristWidth(30.0);
  helper->setLeftWristWidth(34.0);
  helper->setRightHandThickness(20.0);
  helper->setLeftHandThickness(22.0);
  helper->setInterAsisDistance(123.56);
  helper->setRightLegLength(950.0);
  helper->setLeftLegLength(940.0);
  helper->setRightAsisTrochanterAPDistance(123.3); 
  helper->setLeftAsisTrochanterAPDistance(122.8);
  helper->setRightKneeWidth(90.0);
  helper->setLeftKneeWidth(95.0);
  helper->setRightAnkleWidth(65.0);
  helper->setLeftAnkleWidth(56.9);
  helper->setRightFootFlatEnabled(true);
  helper->setLeftFootFlatEnabled(true);
};

void compare_parameters_with_fake_value(ma::body::PluginGait* helper)
{
  TS_ASSERT_EQUALS(helper->markerDiameter(), 14.0);
  TS_ASSERT_EQUALS(helper->headOffsetEnabled(), true);
  TS_ASSERT_EQUALS(helper->rightShoulderOffset(), 70.0);
  TS_ASSERT_EQUALS(helper->leftShoulderOffset(), 71.0);
  TS_ASSERT_EQUALS(helper->rightElbowWidth(), 50.0);
  TS_ASSERT_EQUALS(helper->leftElbowWidth(), 52.0);
  TS_ASSERT_EQUALS(helper->rightWristWidth(), 30.0);
  TS_ASSERT_EQUALS(helper->leftWristWidth(), 34.0);
  TS_ASSERT_EQUALS(helper->rightHandThickness(), 20.0);
  TS_ASSERT_EQUALS(helper->leftHandThickness(), 22.0);
  TS_ASSERT_EQUALS(helper->interAsisDistance(), 123.56);
  TS_ASSERT_EQUALS(helper->rightLegLength(), 950.0);
  TS_ASSERT_EQUALS(helper->leftLegLength(), 940.0);
  TS_ASSERT_EQUALS(helper->rightAsisTrochanterAPDistance(), 123.3);
  TS_ASSERT_EQUALS(helper->leftAsisTrochanterAPDistance(), 122.8);
  TS_ASSERT_EQUALS(helper->rightKneeWidth(), 90.0);
  TS_ASSERT_EQUALS(helper->leftKneeWidth(), 95.0);
  TS_ASSERT_EQUALS(helper->rightAnkleWidth(), 65.0);
  TS_ASSERT_EQUALS(helper->leftAnkleWidth(), 56.9);
  TS_ASSERT_EQUALS(helper->rightFootFlatEnabled(), true);
  TS_ASSERT_EQUALS(helper->leftFootFlatEnabled(), true);
  TS_ASSERT_EQUALS(helper->headOffset(), 0.0);
  TS_ASSERT_EQUALS(helper->rightStaticPlantarFlexionOffset(), 0.0);
  TS_ASSERT_EQUALS(helper->rightStaticRotationOffset(), 0.0);
  TS_ASSERT_EQUALS(helper->leftStaticPlantarFlexionOffset(), 0.0);
  TS_ASSERT_EQUALS(helper->leftStaticRotationOffset(), 0.0);
};

#endif // plugingaitTest_def_h