#ifndef forceplateTest_def_h
#define forceplateTest_def_h

#include <openma/instrument/forceplate.h>
#include <openma/base/timesequence.h>

static unsigned sample10_fpsamples = 22;
static unsigned gait1_fpsamples = 17;

const double fp2data[132] = {
  0., -177.80544, 0., 0., 0., 0., 0., -177.8054363, 0., 0., 0., 0., -177.8054363, 0., -177.8054363, 0., 381.9178679, 0., 0., 0., 0., -382.2910167,
  0., 0., -179.17317, 0., 0., 0., 0., -179.1731704, -179.1731704, 0., 0., 0., 0., -179.1731704, -179.1731704, 0., 0., 384.8556976, 0., 0., -385.2317169, 0.,
  0., 0., 0., -696.99209, 0., 0., 0., 0., -696.992086, -696.992086, 0., 0., -696.992086, 0., -696.992086, 0., 0., 0., 1497.106821, -1498.569553, 0., 0.,
  0., 0., 0., 0., -86396.55843, 0., 0., 0., 0., -86396.55843, -86396.55843, 0., 0., -86396.55843, 0., -86396.55843, 0., 0., -185757.1335, 185575.8186, 0., 0.,
  0., 0., 0., 0., 0., -86140.94731, 0., 0., 0., 0., -86140.94731, -86140.94731, -86140.94731, 0., 0., -86140.94731, 0., -185207.5562, 0., 0., 185026.7777, 0.,
  0., 0., 0., 0., 0., 0., -45635.79967, 0., 0., 0., 0., -45635.79967, 0., -45635.79967, 0., -45635.79967, -98119.36362, 0., 0., 0., 0., 98023.59069
};

const double fp4datain[132] = {
  0., -116.4625608, 0., 0., 0., 0., 0., -116.4625608, 0., 0., 0., 0., -116.4625608, 0., -116.4625608, 0., 250.1562034, 0., 0., 0., 0., -250.400616,
  0., 0., -116.4625608, 0., 0., 0., 0., -116.4625608, -116.4625608, 0., 0., 0., 0., -116.4625608, -116.4625608, 0., 0., 250.1562034, 0., 0., -250.400616, 0.,
  0., 0., 0., -116.3976784, 0., 0., 0., 0., -116.3976784, -116.3976784, 0., 0., -116.3976784, 0., -116.3976784, 0., 0., 0., 250.016839, -250.2611154, 0., 0.,
  0., 0., 0., 0., -116.4625608, 0., 0., 0., 0., -116.4625608, -116.4625608, 0., 0., -116.4625608, 0., -116.4625608, 0., 0., -250.400616, 250.1562034, 0., 0.,
  0., 0., 0., 0., 0., -116.4625608, 0., 0., 0., 0., -116.4625608, -116.4625608, -116.4625608, 0., 0., -116.4625608, 0., -250.400616, 0., 0., 250.1562034, 0.,
  0., 0., 0., 0., 0., 0., -116.4625608, 0., 0., 0., 0., -116.4625608, 0., -116.4625608, 0., -116.4625608, -250.400616, 0., 0., 0., 0., 250.1562034
};

const double fp4dataout[132] = {
  0., -177.7969868, 0.539989329, -0.010413302, 0.000687888, -0.262339768, 0.207522954, -177.2569975, 0.529576027, -0.009725415, -0.26165188, -0.054816814, -178.0697399, 0.748200171, -177.2674108, -0.054128926, 382.345904, -1.723916413, 0.023846287, -0.023866697, 1.724499098, -382.7185996,
  0., -1.628401763, -179.1904518, 1.063254319, -0.266797539, 0.133641951, 0.564268206, -180.8188536, -178.1271975, 0.79645678, -0.133155587, 0.697910158, -0.431505493, -178.8929812, -179.7555993, 0.431112619, 4.710938051, 385.1801546, -2.857449892, 2.859121365, -385.5559296, -4.71317128,
  0., -5.301769525, 2.155438772, -697.0083441, 0.002799273, -0.008031187, 0.27274449, -3.146330754, -694.8529054, -697.0055449, -0.005231914, 0.264713304, -702.3181449, 2.430982535, -700.1546749, 0.267512577, 11.97437112, -4.647050437, 1497.147761, -1498.610522, 4.651557064, -11.98492521,
  0., 498.9583242, -1998.45586, -1540.288983, -86400.23046, 195.8926534, 378.5433657, -1499.497536, -3538.744842, -87940.51944, -86204.3378, 574.4360191, -845.4380049, -88020.14295, -3039.786518, -85825.79444, -257.8513468, 4713.770401, -182456.5589, 182272.0038, -4717.553325, 259.6929033,
  0., 639.869747, -660.0832727, -1542.299908, 62.81959263, -86139.29386, 69.15822443, -20.21352564, -2202.38318, -1479.480315, -86076.47426, -86070.13563, -87041.72402, -528.1054556, -1562.513433, -86007.31604, -1225.716863, -183786.1728, 3447.854414, -3450.959304, 183604.0125, 1227.204854,
  0., -332.6198257, 1122.575481, 536.5348869, 69.55595595, -237.7745886, -45639.25153, 789.9556553, 1659.110368, 606.0908428, -168.2186326, -45877.02612, -33.8595274, -44447.1201, 1326.490542, -45807.47016, -97412.33327, -2922.468144, -1002.903211, 1004.175175, 2924.325019, 97315.85505
};

const double fp4cal[36] = {
  1.526645006, 0.013982191, 0.045523381, -4.284280896, -5.494209837, 2.856023631,
  -0.004636592, 1.538609925, -0.018507568, 17.15964209, 5.667772272, -9.638938674,
  8.94631E-05, -0.009134669, 5.98816363, 13.23298715, 13.25026349, -4.609498183,
  -5.90651E-06, 0.002290844, -2.40358E-05, 741.8712923, -0.539397315, -0.597238765,
  0.002252567, -0.00114751, 6.89594E-05, -1.68202255, 739.6307731, 2.041639708,
  -0.001781886, -0.004845061, -0.002341907, -3.250343829, -0.593823663, 391.879169
};

const double fp5datain[136] = {
  -0.162814, -0.210117, -0.181125, -0.182956, -0.160678, -0.208896, -0.202182, -0.187839, -0.183566, -0.175937, -0.18265,  -0.186313, -0.19028,  -0.17258,  -0.184176, -0.189364, -0.203098,
  -0.066682, -0.080415, -0.065156, -0.078584, -0.052338, -0.092622, -0.081331, -0.075837, -0.075532, -0.063325, -0.076143, -0.073091, -0.095979, -0.065766, -0.074617, -0.079194, -0.0795,
  -0.043793, -0.052338, -0.025177, -0.046845, -0.031281, -0.050507, -0.061494, -0.04715,  -0.054475, -0.031891, -0.030671, -0.066682, -0.045319, -0.041962, -0.042267, -0.048981, -0.058137,
  -0.07187,  -0.083467, -0.065156, -0.062104, -0.059968, -0.075837, -0.086213, -0.086213, -0.077363, -0.062715, -0.061189, -0.076753, -0.070344, -0.077974, -0.078279, -0.074311, -0.08957,
  -0.013886, -0.017853, -0.016632, -0.001068,  0.005341, -0.016632, -0.022736, -0.015106, -0.013275, -0.000458, -0.008392, -0.004425, -0.011749, -0.009613, -0.018463, -0.017548, -0.030671,
  -0.067292, -0.065766, -0.023346, -0.027314, -0.006867, -0.027008, -0.043793, -0.032807, -0.043183, -0.020295, -0.046845, -0.042878, -0.040742, -0.03006,  -0.039826, -0.046845, -0.066987,
  -0.048981, -0.04654,  -0.020905, -0.043488, -0.027924, -0.034028, -0.07187,  -0.044709, -0.041352, -0.031891, -0.033112, -0.036164, -0.029755, -0.037385, -0.043793, -0.049592, -0.080415,
  -0.027924, -0.027619, -0.007782, -0.019989, -0.017548, -0.029755, -0.037995, -0.017853, -0.006867, -0.012055, -0.038605, -0.034028, -0.015412, -0.02884,  -0.052338, -0.034333, -0.058747
};

const double fp5dataout[153] = {
  0.934072, 0.936098, 0.084225, -0.791014, -1.069198, -0.372141, -1.364467, -0.563446, 0.105632, -0.585571, 0.658296, 0.290865, 0.567465, -0.36459, -0.214151, -0.134446, -0.644618,
  -1.613139, -1.685705, -0.733881, -0.482943, -0.168369, -1.656966, -2.378406, -1.067708, -0.459805, -0.137758, -1.819456, -1.369817, -0.74167, -1.404462, -2.961395, -2.006249, -3.819845,
  35.874054, 44.33185, 35.028599, 38.538669, 31.683601, 44.492424, 44.822778, 41.277087, 40.638552, 34.751917, 36.48176, 41.889862, 41.776964, 37.260579, 39.444418, 40.7395, 44.72067,
  -2.173685, -2.962837, -3.002722, -2.92713, -2.339375, -3.359468, -2.59013, -2.488611, -2.42885, -2.781605, -3.212493, -2.206202, -3.271178, -2.262776, -2.646729, -2.781088, -2.573186,
  -1.354635, -1.947222, -1.506959, -1.75639, -1.566817, -1.803725, -1.899998, -1.458408, -1.684952, -1.613838, -1.508508, -2.02412, -1.390111, -1.410233, -1.466781, -1.680697, -1.825992,
  1.668103, 1.547178, 0.414967, 1.118698, 0.70486, 0.907141, 1.648793, 0.992044, 0.969687, 0.784297, 1.398942, 1.36878, 0.917455, 1.082669, 1.485301, 1.428891, 2.239469,
  37.335437, 43.578775, 42.981543, 45.910098, 50.003366, 40.676707, 42.886485, 35.555181, 41.419439, 46.714136, 41.054816, 48.20659, 33.05264, 38.007717, 37.274738, 41.308647, 41.066577,
  -59.857415, -66.211856, -85.379708, -75.748326, -73.748674, -74.898042, -56.919032, -59.867737, -59.582283, -79.977002, -87.242645, -52.132435, -78.010943, -60.112557, -65.873538, -67.460514, -56.143471,
  0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0
};

const double fp5rso[3] = { 0., 0., -16.33887 };

const double fp5sc1[3] = { -501.800001, 497.799993, 0.};

const double fp5sc2[3] = { -4.0, 497.799993, 0. };

const double fp5sc3[3] = { -4.0, 0., 0. };

const double fp5sc4[3] = { -501.800001, 0., 0. };

const double fp5cal[48] = {
  -1.6121, 0.976148, 105.019852, -19528.01563, -19094.32813, -158.667999,
  2.450264, 3.797407, 102.728424, -19156.98047, 18638.98242, -155.205994,
  -1.303078, 0.46725, 103.483261, 19787.05273, -18830.75977, -156.345993,
  1.663982, 0.631969, 103.279465, 19710.96484, 18588.61719, -156.037994,
  -1.019092, -49.462944, -0.851459, 0, 0, -13128.5293,
  49.613453, -1.280832, -0.229795, 0, 0, 13479.35742,
  -47.475616, 1.225642, -0.148471, 0, 0, 12898.53516,
  -1.016865, -49.354858, 0.187208, 0, 0, 13099.8418
};

void forceplatetest_fill_sample10(ma::instrument::ForcePlate* fp, const double* data)
{
  double rate = 60.0;
  double start = 0.0;
  auto fx = new ma::TimeSequence("FX1",1,sample10_fpsamples,rate,start,ma::TimeSequence::Analog,"N");
  std::copy(data   , data+22, fx->data());
  fp->setChannel("Fx", fx);
  auto fy = new ma::TimeSequence("FY1",1,sample10_fpsamples,rate,start,ma::TimeSequence::Analog,"N");
  std::copy(data+22, data+44, fy->data());
  fp->setChannel("Fy", fy);
  auto fz = new ma::TimeSequence("FZ1",1,sample10_fpsamples,rate,start,ma::TimeSequence::Analog,"N");
  std::copy(data+44, data+66, fz->data());
  fp->setChannel("Fz", fz);
  auto mx = new ma::TimeSequence("MX1",1,sample10_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(data+66, data+88, mx->data());
  fp->setChannel("Mx", mx);
  auto my = new ma::TimeSequence("MY1",1,sample10_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(data+88, data+110, my->data());
  fp->setChannel("My", my);
  auto mz = new ma::TimeSequence("MZ1",1,sample10_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(data+110, data+132, mz->data());
  fp->setChannel("Mz", mz);
};

void forceplatetest_fill_sample10_type2(ma::instrument::ForcePlate* fp)
{
  forceplatetest_fill_sample10(fp, fp2data);
}

void forceplatetest_fill_sample10_type4(ma::instrument::ForcePlate* fp)
{
  forceplatetest_fill_sample10(fp, fp4datain);
  fp->setCalibrationMatrixData(fp4cal);
};

void forceplatetest_fill_gait1_type5(ma::instrument::ForcePlate* fp)
{
  double rate = 1000.0;
  double start = 0.0;
  auto p1 = new ma::TimeSequence("Pin1",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"N");
  std::copy(fp5datain   , fp5datain+17, p1->data());
  fp->setChannel("Fz1", p1);
  auto p2 = new ma::TimeSequence("Pin2",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"N");
  std::copy(fp5datain+17, fp5datain+34, p2->data());
  fp->setChannel("Fz2", p2);
  auto p3 = new ma::TimeSequence("Pin3",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"N");
  std::copy(fp5datain+34, fp5datain+51, p3->data());
  fp->setChannel("Fz3", p3);
  auto p4 = new ma::TimeSequence("Pin4",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(fp5datain+51, fp5datain+68, p4->data());
  fp->setChannel("Fz4", p4);
  auto p5 = new ma::TimeSequence("Pin5",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(fp5datain+68, fp5datain+85, p5->data());
  fp->setChannel("Fx12", p5);
  auto p6 = new ma::TimeSequence("Pin6",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(fp5datain+85, fp5datain+102, p6->data());
  fp->setChannel("Fx34", p6);
  auto p7 = new ma::TimeSequence("Pin7",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(fp5datain+102, fp5datain+119, p7->data());
  fp->setChannel("Fy14", p7);
  auto p8 = new ma::TimeSequence("Pin8",1,gait1_fpsamples,rate,start,ma::TimeSequence::Analog,"Nmm");
  std::copy(fp5datain+119, fp5datain+136, p8->data());
  fp->setChannel("Fy23", p8);
  fp->setGeometry(fp5rso, fp5sc1, fp5sc2, fp5sc3, fp5sc4);
  fp->setCalibrationMatrixData(fp5cal);
};

void forceplatetest_compare_sample10_wrench_at_origin(ma::instrument::ForcePlate* fp, const double* dataref)
{
  auto w = fp->wrench(ma::instrument::Location::Origin,false);
  for (unsigned i = 0 ; i < sample10_fpsamples ; ++i)
  {
    const std::string s = std::to_string(i);
    TSM_ASSERT_DELTA(s, w->data()[i],                      dataref[i],                      1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+  sample10_fpsamples], dataref[i+  sample10_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+2*sample10_fpsamples], dataref[i+2*sample10_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+3*sample10_fpsamples], dataref[i+3*sample10_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+4*sample10_fpsamples], dataref[i+4*sample10_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+5*sample10_fpsamples], dataref[i+5*sample10_fpsamples], 1e-4);
  }
};

void forceplatetest_compare_gait1_wrench_at_origin(ma::instrument::ForcePlate* fp)
{
  auto w = fp->wrench(ma::instrument::Location::Origin,false);
  for (unsigned i = 0 ; i < gait1_fpsamples ; ++i)
  {
    const std::string s = std::to_string(i);
    TSM_ASSERT_DELTA(s, w->data()[i],                          -fp5dataout[i],                   1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+  gait1_fpsamples],        -fp5dataout[i+  gait1_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+2*gait1_fpsamples],        -fp5dataout[i+2*gait1_fpsamples], 5e-4); // All differences are below 1-4 except for the sample #5 and #15.
    TSM_ASSERT_DELTA(s, w->data()[i+3*gait1_fpsamples]/1000.0, -fp5dataout[i+3*gait1_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+4*gait1_fpsamples]/1000.0, -fp5dataout[i+4*gait1_fpsamples], 1e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+5*gait1_fpsamples]/1000.0, -fp5dataout[i+5*gait1_fpsamples], 1e-4);
  }
};

void forceplatetest_compare_gait1_wrench_position_at_cop(ma::instrument::ForcePlate* fp)
{
  auto w = fp->wrench(ma::instrument::Location::CentreOfPressure,false);
  for (unsigned i = 0 ; i < gait1_fpsamples ; ++i)
  {
    const std::string s = std::to_string(i);
    // NOTE: Most of the differences are below 5e-4
    TSM_ASSERT_DELTA(s, w->data()[i+6*gait1_fpsamples], fp5dataout[i+6*gait1_fpsamples], 7.5e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+7*gait1_fpsamples], fp5dataout[i+7*gait1_fpsamples], 7.5e-4);
    TSM_ASSERT_DELTA(s, w->data()[i+8*gait1_fpsamples], fp5dataout[i+8*gait1_fpsamples], 7.5e-4);
  }
};

#endif // forceplateTest_def_h