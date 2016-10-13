import ma.instrument
import numpy as np
import unittest

fp5_rso = [0., 0., -16.33887]
fp5_sc1 = [-501.800001, 497.799993, 0]
fp5_sc2 = [-4.0, 497.799993, 0.]
fp5_sc3 = [-4.0, 0., 0.]
fp5_sc4 = [-501.800001, 0., 0.]
fp5_calib6x8 = [
    -1.6121, 0.976148, 105.019852, -19528.01563, -19094.32813, -158.667999,
    2.450264, 3.797407, 102.728424, -19156.98047, 18638.98242, -155.205994,
    -1.303078, 0.46725, 103.483261, 19787.05273, -18830.75977, -156.345993,
    1.663982, 0.631969, 103.279465, 19710.96484, 18588.61719, -156.037994,
    -1.019092, -49.462944, -0.851459, 0, 0, -13128.5293,
    49.613453, -1.280832, -0.229795, 0, 0, 13479.35742,
    -47.475616, 1.225642, -0.148471, 0, 0, 12898.53516,
    -1.016865, -49.354858, 0.187208, 0, 0, 13099.8418
]
fp5_datain = [
    -0.162814, -0.210117, -0.181125, -0.182956, -0.160678, -0.208896, -0.202182, -0.187839, -0.183566, -0.175937, -0.18265,  -0.186313, -0.19028,  -0.17258,  -0.184176, -0.189364, -0.203098,
    -0.066682, -0.080415, -0.065156, -0.078584, -0.052338, -0.092622, -0.081331, -0.075837, -0.075532, -0.063325, -0.076143, -0.073091, -0.095979, -0.065766, -0.074617, -0.079194, -0.0795,
    -0.043793, -0.052338, -0.025177, -0.046845, -0.031281, -0.050507, -0.061494, -0.04715,  -0.054475, -0.031891, -0.030671, -0.066682, -0.045319, -0.041962, -0.042267, -0.048981, -0.058137,
    -0.07187,  -0.083467, -0.065156, -0.062104, -0.059968, -0.075837, -0.086213, -0.086213, -0.077363, -0.062715, -0.061189, -0.076753, -0.070344, -0.077974, -0.078279, -0.074311, -0.08957,
    -0.013886, -0.017853, -0.016632, -0.001068,  0.005341, -0.016632, -0.022736, -0.015106, -0.013275, -0.000458, -0.008392, -0.004425, -0.011749, -0.009613, -0.018463, -0.017548, -0.030671,
    -0.067292, -0.065766, -0.023346, -0.027314, -0.006867, -0.027008, -0.043793, -0.032807, -0.043183, -0.020295, -0.046845, -0.042878, -0.040742, -0.03006,  -0.039826, -0.046845, -0.066987,
    -0.048981, -0.04654,  -0.020905, -0.043488, -0.027924, -0.034028, -0.07187,  -0.044709, -0.041352, -0.031891, -0.033112, -0.036164, -0.029755, -0.037385, -0.043793, -0.049592, -0.080415,
    -0.027924, -0.027619, -0.007782, -0.019989, -0.017548, -0.029755, -0.037995, -0.017853, -0.006867, -0.012055, -0.038605, -0.034028, -0.015412, -0.02884,  -0.052338, -0.034333, -0.058747
]
fp5_dataout = [
    0.934072, 0.936098, 0.084225, -0.791014, -1.069198, -0.372141, -1.364467, -0.563446, 0.105632, -0.585571, 0.658296, 0.290865, 0.567465, -0.36459, -0.214151, -0.134446, -0.644618,
    -1.613139, -1.685705, -0.733881, -0.482943, -0.168369, -1.656966, -2.378406, -1.067708, -0.459805, -0.137758, -1.819456, -1.369817, -0.74167, -1.404462, -2.961395, -2.006249, -3.819845,
    35.874054, 44.33185, 35.028599, 38.538669, 31.683601, 44.492424, 44.822778, 41.277087, 40.638552, 34.751917, 36.48176, 41.889862, 41.776964, 37.260579, 39.444418, 40.7395, 44.72067,
    -2.173685, -2.962837, -3.002722, -2.92713, -2.339375, -3.359468, -2.59013, -2.488611, -2.42885, -2.781605, -3.212493, -2.206202, -3.271178, -2.262776, -2.646729, -2.781088, -2.573186,
    -1.354635, -1.947222, -1.506959, -1.75639, -1.566817, -1.803725, -1.899998, -1.458408, -1.684952, -1.613838, -1.508508, -2.02412, -1.390111, -1.410233, -1.466781, -1.680697, -1.825992,
    1.668103, 1.547178, 0.414967, 1.118698, 0.70486, 0.907141, 1.648793, 0.992044, 0.969687, 0.784297, 1.398942, 1.36878, 0.917455, 1.082669, 1.485301, 1.428891, 2.239469,
    37.335437, 43.578775, 42.981543, 45.910098, 50.003366, 40.676707, 42.886485, 35.555181, 41.419439, 46.714136, 41.054816, 48.20659, 33.05264, 38.007717, 37.274738, 41.308647, 41.066577,
    -59.857415, -66.211856, -85.379708, -75.748326, -73.748674, -74.898042, -56.919032, -59.867737, -59.582283, -79.977002, -87.242645, -52.132435, -78.010943, -60.112557, -65.873538, -67.460514, -56.143471,
    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0
]

class ForcePlateType5Test(unittest.TestCase):
    def test_setget(self):
        fp5 = ma.instrument.ForcePlateType5('FP')
        fp5.setGeometry(fp5_rso,fp5_sc1,fp5_sc2,fp5_sc3,fp5_sc4)
        fp5.setCalibrationMatrixData(fp5_calib6x8)
        self.assertEqual(fp5.type(), ma.instrument.ForcePlate.Type_Type5)
        self.assertEqual(fp5.type(), ma.instrument.ForcePlate.Type_Cal6x8)
        rf = fp5.referenceFrame()
        self.assertAlmostEqual(rf[ 0], -1., 5)
        self.assertAlmostEqual(rf[ 1],  0., 5)
        self.assertAlmostEqual(rf[ 2],  0., 5)
        self.assertAlmostEqual(rf[ 3],  0., 5)
        self.assertAlmostEqual(rf[ 4],  1., 5)
        self.assertAlmostEqual(rf[ 5],  0., 5)
        self.assertAlmostEqual(rf[ 6],  0., 5)
        self.assertAlmostEqual(rf[ 7],  0., 5)
        self.assertAlmostEqual(rf[ 8], -1., 5)
        self.assertAlmostEqual(rf[ 9], -252.9000005, 5)
        self.assertAlmostEqual(rf[10], 248.8999965, 5)
        self.assertAlmostEqual(rf[11], -16.33887, 5)
        c = fp5.surfaceCorners()
        self.assertAlmostEqual(c[ 0], -501.800001, 5)
        self.assertAlmostEqual(c[ 1], 497.799993, 5)
        self.assertAlmostEqual(c[ 2], 0., 5)
        self.assertAlmostEqual(c[ 3], -4.0, 5)
        self.assertAlmostEqual(c[ 4], 497.799993, 5)
        self.assertAlmostEqual(c[ 5], 0., 5)
        self.assertAlmostEqual(c[ 6], -4., 5)
        self.assertAlmostEqual(c[ 7], 0., 5)
        self.assertAlmostEqual(c[ 8], 0., 5)
        self.assertAlmostEqual(c[ 9], -501.800001, 5)
        self.assertAlmostEqual(c[10], 0., 5)
        self.assertAlmostEqual(c[11], 0., 5)
        so = fp5.relativeSurfaceOrigin()
        self.assertAlmostEqual(so[0], 0., 5)
        self.assertAlmostEqual(so[1], 0., 5)
        self.assertAlmostEqual(so[2], -16.33887, 5)
        cmdim = fp5.calibrationMatrixDimensions()
        self.assertEqual(cmdim[0], 6)
        self.assertEqual(cmdim[1], 8)
        cmdata = fp5.calibrationMatrixData()
        for i in xrange(0,48):
            self.assertAlmostEqual(cmdata[i], fp5_calib6x8[i], 5)
    
    def test_wrenches(self):
        fp5 = ma.instrument.ForcePlateType5('FP')
        fp5.setGeometry(fp5_rso,fp5_sc1,fp5_sc2,fp5_sc3,fp5_sc4)
        fp5.setCalibrationMatrixData(fp5_calib6x8)
        
        rate = 1000.0
        start = 0.0
        samples = 17
        p1 = ma.TimeSequence('Pin1',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p2 = ma.TimeSequence('Pin2',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p3 = ma.TimeSequence('Pin3',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p4 = ma.TimeSequence('Pin4',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p5 = ma.TimeSequence('Pin5',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p6 = ma.TimeSequence('Pin6',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p7 = ma.TimeSequence('Pin7',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        p8 = ma.TimeSequence('Pin8',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N')
        
        p1.setData(np.array([fp5_datain[0*samples:1*samples]]).T)
        p2.setData(np.array([fp5_datain[1*samples:2*samples]]).T)
        p3.setData(np.array([fp5_datain[2*samples:3*samples]]).T)
        p4.setData(np.array([fp5_datain[3*samples:4*samples]]).T)
        p5.setData(np.array([fp5_datain[4*samples:5*samples]]).T)
        p6.setData(np.array([fp5_datain[5*samples:6*samples]]).T)
        p7.setData(np.array([fp5_datain[6*samples:7*samples]]).T)
        p8.setData(np.array([fp5_datain[7*samples:8*samples]]).T)
        
        fp5.setChannel('Fz1', p1)
        fp5.setChannel('Fz2', p2)
        fp5.setChannel('Fz3', p3)
        fp5.setChannel('Fz4', p4)
        fp5.setChannel('Fx12', p5)
        fp5.setChannel('Fx34', p6)
        fp5.setChannel('Fy14', p7)
        fp5.setChannel('Fy23', p8)

        self.assertEqual(len(fp5.channels().findChildren(ma.T_TimeSequence,'.*',[['type',ma.TimeSequence.Type_Analog]])), 8)
        self.assertEqual(fp5.channel(0).name(),p1.name())
        self.assertEqual(fp5.channel(1).name(),p2.name())
        self.assertEqual(fp5.channel(2).name(),p3.name())
        self.assertEqual(fp5.channel(3).name(),p4.name())
        self.assertEqual(fp5.channel(4).name(),p5.name())
        self.assertEqual(fp5.channel(5).name(),p6.name())
        self.assertEqual(fp5.channel(6).name(),p7.name())
        self.assertEqual(fp5.channel(7).name(),p8.name())
        
        # NOTE: Tolerance are higher than in the C++ due to the difference in the method to compare real numbers.
        wlo = fp5.wrench(ma.instrument.Location_Origin, False).data()
        for i in xrange(0,samples):
            self.assertAlmostEqual(wlo[i,0], -fp5_dataout[i+0*samples], 3)
            self.assertAlmostEqual(wlo[i,1], -fp5_dataout[i+1*samples], 3)
            self.assertAlmostEqual(wlo[i,2], -fp5_dataout[i+2*samples], 3) 
            self.assertAlmostEqual(wlo[i,3]/1000., -fp5_dataout[i+3*samples], 3)
            self.assertAlmostEqual(wlo[i,4]/1000., -fp5_dataout[i+4*samples], 3)
            self.assertAlmostEqual(wlo[i,5]/1000., -fp5_dataout[i+5*samples], 3)
        wlc = fp5.wrench(ma.instrument.Location_CentreOfPressure, False).data()
        for i in xrange(0,samples):
            self.assertAlmostEqual(wlc[i,6], fp5_dataout[i+6*samples], 2)
            self.assertAlmostEqual(wlc[i,7], fp5_dataout[i+7*samples], 2)
            self.assertAlmostEqual(wlc[i,8], fp5_dataout[i+8*samples]-16.33887, 2)