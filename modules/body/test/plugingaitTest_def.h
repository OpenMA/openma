#ifndef plugingaitTest_def_h
#define plugingaitTest_def_h

#include <openma/body/plugingait.h>
#include <openma/body/enums.h>
#include <openma/base/timesequence.h>
#include <openma/base/trial.h>
#include <openma/io/handlerreader.h>
#include <openma/io/file.h>
#include <openma/maths.h>

#include <cassert>

ma::TimeSequence* make_marker(const std::string& name, double* data, ma::Trial* trial)
{
  const unsigned samples = 1;
  const unsigned components = 4;
  const double rate = 100.0;
  const double start = 0.0;
  const std::string unit = "mm";
  auto marker = new ma::TimeSequence(name, components, samples, rate, start, ma::TimeSequence::Marker, unit, trial->timeSequences());
  std::copy(data, data+4, marker->data());
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
}

void compare_segment_motion(ma::Trial* trial, const std::string& segment, const std::vector<std::string>& markers, std::vector<double> precision = std::vector<double>(4,1e-5))
{
  assert(markers.size() == 4);
  precision.resize(4,1e-5);
  const auto mot = ma::maths::to_pose(trial->timeSequences()->findChild<const ma::TimeSequence*>(segment));
  const auto o = ma::maths::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[0]));
  const auto a = ma::maths::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[1]));
  const auto l = ma::maths::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[2]));  
  const auto p = ma::maths::to_position(trial->timeSequences()->findChild<const ma::TimeSequence*>(markers[3]));
  TSM_ASSERT_EQUALS(segment+": mapped pose", mot.isValid(), true);
  TSM_ASSERT_EQUALS(segment+": mapped origin", o.isValid(), true);
  TSM_ASSERT_EQUALS(segment+": mapped AP", a.isValid(), true);
  TSM_ASSERT_EQUALS(segment+": mapped LM", l.isValid(), true);
  TSM_ASSERT_EQUALS(segment+": mapped PD", p.isValid(), true);  
  TSM_ASSERT_EIGEN_DELTA(segment+": origin", mot.o().values(), o.values(), precision[0]);
  TSM_ASSERT_EIGEN_DELTA(segment+": u axis", mot.u().values(), (a-o).normalized().values(), precision[1]);
  TSM_ASSERT_EIGEN_DELTA(segment+": v axis", mot.v().values(), (l-o).normalized().values(), precision[2]);
  TSM_ASSERT_EIGEN_DELTA(segment+": w axis", mot.w().values(), (p-o).normalized().values(), precision[3]);
}

#endif // plugingaitTest_def_h