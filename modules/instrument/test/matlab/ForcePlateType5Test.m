

classdef ForcePlateType5Test < matlab.unittest.TestCase

    properties (Constant)
    
        rso = [0., 0., -16.33887];
        sc1 = [-501.800001, 497.799993, 0];
        sc2 = [-4.0, 497.799993, 0.];
        sc3 = [-4.0, 0., 0.];
        sc4 = [-501.800001, 0., 0.];
        fp5_calib6x8 = [ ...
            -1.6121, 0.976148, 105.019852, -19528.01563, -19094.32813, -158.667999, ...
            2.450264, 3.797407, 102.728424, -19156.98047, 18638.98242, -155.205994, ...
            -1.303078, 0.46725, 103.483261, 19787.05273, -18830.75977, -156.345993, ...
            1.663982, 0.631969, 103.279465, 19710.96484, 18588.61719, -156.037994, ...
            -1.019092, -49.462944, -0.851459, 0, 0, -13128.5293, ...
            49.613453, -1.280832, -0.229795, 0, 0, 13479.35742, ...
            -47.475616, 1.225642, -0.148471, 0, 0, 12898.53516, ...
            -1.016865, -49.354858, 0.187208, 0, 0, 13099.8418 ...
        ];
        fp5_datain = [ ...
            -0.162814, -0.210117, -0.181125, -0.182956, -0.160678, -0.208896, -0.202182, -0.187839, -0.183566, -0.175937, -0.18265,  -0.186313, -0.19028,  -0.17258,  -0.184176, -0.189364, -0.203098, ...
            -0.066682, -0.080415, -0.065156, -0.078584, -0.052338, -0.092622, -0.081331, -0.075837, -0.075532, -0.063325, -0.076143, -0.073091, -0.095979, -0.065766, -0.074617, -0.079194, -0.0795, ...
            -0.043793, -0.052338, -0.025177, -0.046845, -0.031281, -0.050507, -0.061494, -0.04715,  -0.054475, -0.031891, -0.030671, -0.066682, -0.045319, -0.041962, -0.042267, -0.048981, -0.058137, ...
            -0.07187,  -0.083467, -0.065156, -0.062104, -0.059968, -0.075837, -0.086213, -0.086213, -0.077363, -0.062715, -0.061189, -0.076753, -0.070344, -0.077974, -0.078279, -0.074311, -0.08957, ...
            -0.013886, -0.017853, -0.016632, -0.001068,  0.005341, -0.016632, -0.022736, -0.015106, -0.013275, -0.000458, -0.008392, -0.004425, -0.011749, -0.009613, -0.018463, -0.017548, -0.030671, ...
            -0.067292, -0.065766, -0.023346, -0.027314, -0.006867, -0.027008, -0.043793, -0.032807, -0.043183, -0.020295, -0.046845, -0.042878, -0.040742, -0.03006,  -0.039826, -0.046845, -0.066987, ...
            -0.048981, -0.04654,  -0.020905, -0.043488, -0.027924, -0.034028, -0.07187,  -0.044709, -0.041352, -0.031891, -0.033112, -0.036164, -0.029755, -0.037385, -0.043793, -0.049592, -0.080415, ...
            -0.027924, -0.027619, -0.007782, -0.019989, -0.017548, -0.029755, -0.037995, -0.017853, -0.006867, -0.012055, -0.038605, -0.034028, -0.015412, -0.02884,  -0.052338, -0.034333, -0.058747 ...
        ];
        fp5_dataout = [ ...
            0.934072, 0.936098, 0.084225, -0.791014, -1.069198, -0.372141, -1.364467, -0.563446, 0.105632, -0.585571, 0.658296, 0.290865, 0.567465, -0.36459, -0.214151, -0.134446, -0.644618, ...
            -1.613139, -1.685705, -0.733881, -0.482943, -0.168369, -1.656966, -2.378406, -1.067708, -0.459805, -0.137758, -1.819456, -1.369817, -0.74167, -1.404462, -2.961395, -2.006249, -3.819845, ...
            35.874054, 44.33185, 35.028599, 38.538669, 31.683601, 44.492424, 44.822778, 41.277087, 40.638552, 34.751917, 36.48176, 41.889862, 41.776964, 37.260579, 39.444418, 40.7395, 44.72067, ...
            -2.173685, -2.962837, -3.002722, -2.92713, -2.339375, -3.359468, -2.59013, -2.488611, -2.42885, -2.781605, -3.212493, -2.206202, -3.271178, -2.262776, -2.646729, -2.781088, -2.573186, ...
            -1.354635, -1.947222, -1.506959, -1.75639, -1.566817, -1.803725, -1.899998, -1.458408, -1.684952, -1.613838, -1.508508, -2.02412, -1.390111, -1.410233, -1.466781, -1.680697, -1.825992, ...
            1.668103, 1.547178, 0.414967, 1.118698, 0.70486, 0.907141, 1.648793, 0.992044, 0.969687, 0.784297, 1.398942, 1.36878, 0.917455, 1.082669, 1.485301, 1.428891, 2.239469, ...
            37.335437, 43.578775, 42.981543, 45.910098, 50.003366, 40.676707, 42.886485, 35.555181, 41.419439, 46.714136, 41.054816, 48.20659, 33.05264, 38.007717, 37.274738, 41.308647, 41.066577, ...
            -59.857415, -66.211856, -85.379708, -75.748326, -73.748674, -74.898042, -56.919032, -59.867737, -59.582283, -79.977002, -87.242645, -52.132435, -78.010943, -60.112557, -65.873538, -67.460514, -56.143471, ...
            0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0 ...
        ];
        
    end

    methods (Test)
    
        function setget(testCase)
            fp5 = ma.instrument.ForcePlateType5('FP');
            fp5.setGeometry(ForcePlateType5Test.rso,ForcePlateType5Test.sc1,ForcePlateType5Test.sc2,ForcePlateType5Test.sc3,ForcePlateType5Test.sc4);
            fp5.setCalibrationMatrixData(ForcePlateType5Test.fp5_calib6x8);
            testCase.verifyEqual(fp5.type(), ma.instrument.ForcePlate.Type_Type5);
            testCase.verifyEqual(fp5.type(), ma.instrument.ForcePlate.Type_Cal6x8);
            rf = fp5.referenceFrame();
            testCase.verifyEqual(rf( 1), -1., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 2),  0., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 3),  0., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 4),  0., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 5),  1., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 6),  0., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 7),  0., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 8),  0., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf( 9), -1., 'AbsTol', 1e-5);
            testCase.verifyEqual(rf(10), -252.9000005, 'AbsTol', 1e-5);
            testCase.verifyEqual(rf(11), 248.8999965, 'AbsTol', 1e-5);
            testCase.verifyEqual(rf(12), -16.33887, 'AbsTol', 1e-5);
            c = fp5.surfaceCorners();
            testCase.verifyEqual(c( 1), -501.800001, 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 2), 497.799993, 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 3), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 4), -4.0, 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 5), 497.799993, 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 6), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 7), -4., 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 8), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(c( 9), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(c(10), -501.800001, 'AbsTol', 1e-5);
            testCase.verifyEqual(c(11), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(c(12), 0., 'AbsTol', 1e-5);
            so = fp5.relativeSurfaceOrigin();
            testCase.verifyEqual(so(1), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(so(2), 0., 'AbsTol', 1e-5);
            testCase.verifyEqual(so(3), -16.33887, 'AbsTol', 1e-5);
            cmdim = fp5.calibrationMatrixDimensions();
            testCase.verifyEqual(cmdim(1), 6);
            testCase.verifyEqual(cmdim(2), 8);
            cmdata = fp5.calibrationMatrixData();
            for i = 1:48
                testCase.verifyEqual(cmdata(i), cmdref(i), 'AbsTol', 1e-5);
            end
        end
        
        function wrenches(testCase)
            fp5 = ma.instrument.ForcePlateType5('FP');
            fp5.setGeometry(ForcePlateType5Test.rso,ForcePlateType5Test.sc1,ForcePlateType5Test.sc2,ForcePlateType5Test.sc3,ForcePlateType5Test.sc4);
            fp5.setCalibrationMatrixData(ForcePlateType5Test.fp5_calib6x8);
        
            rate = 1000.0;
            start = 0.0;
            samples = 17;
            p1 = ma.TimeSequence('Pin1',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p2 = ma.TimeSequence('Pin2',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p3 = ma.TimeSequence('Pin3',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p4 = ma.TimeSequence('Pin4',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p5 = ma.TimeSequence('Pin5',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p6 = ma.TimeSequence('Pin6',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p7 = ma.TimeSequence('Pin7',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
            p8 = ma.TimeSequence('Pin8',1,samples,rate,start,ma.TimeSequence.Type_Analog,'N');
        
            p1.setData(ForcePlateType5Test.fp5_datain(1+(0*samples:1*samples))');
            p2.setData(ForcePlateType5Test.fp5_datain(1+(1*samples:2*samples))');
            p3.setData(ForcePlateType5Test.fp5_datain(1+(2*samples:3*samples))');
            p4.setData(ForcePlateType5Test.fp5_datain(1+(3*samples:4*samples))');
            p5.setData(ForcePlateType5Test.fp5_datain(1+(4*samples:5*samples))');
            p6.setData(ForcePlateType5Test.fp5_datain(1+(5*samples:6*samples))');
            p7.setData(ForcePlateType5Test.fp5_datain(1+(6*samples:7*samples))');
            p8.setData(ForcePlateType5Test.fp5_datain(1+(7*samples:8*samples))');
        
            fp5.setChannel('Fz1', p1)
            fp5.setChannel('Fz2', p2)
            fp5.setChannel('Fz3', p3)
            fp5.setChannel('Fz4', p4)
            fp5.setChannel('Fx12', p5)
            fp5.setChannel('Fx34', p6)
            fp5.setChannel('Fy14', p7)
            fp5.setChannel('Fy23', p8)

            testCase.verifyEqual(length(fp5.channels().findChildren(ma.T_TimeSequence,'.*',[['type',ma.TimeSequence.Type_Analog]])), 8)
            testCase.verifyEqual(fp5.channel(1),p1)
            testCase.verifyEqual(fp5.channel(2),p2)
            testCase.verifyEqual(fp5.channel(3),p3)
            testCase.verifyEqual(fp5.channel(4),p4)
            testCase.verifyEqual(fp5.channel(5),p5)
            testCase.verifyEqual(fp5.channel(6),p6)
            testCase.verifyEqual(fp5.channel(7),p7)
            testCase.verifyEqual(fp5.channel(8),p8)
            
            wlo = fp5.wrench(ma.instrument.Location_Origin, false).data()
            for i = 1:samples
                testCase.verifyEqual(wlo(i,1), -ForcePlateType5Test.fp5_dataout(i+0*samples), 1e-4)
                testCase.verifyEqual(wlo(i,2), -ForcePlateType5Test.fp5_dataout(i+1*samples), 1e-4)
                testCase.verifyEqual(wlo(i,3), -ForcePlateType5Test.fp5_dataout(i+2*samples), 1e-4) 
                testCase.verifyEqual(wlo(i,4)/1000., -ForcePlateType5Test.fp5_dataout(i+3*samples), 1e-4)
                testCase.verifyEqual(wlo(i,5)/1000., -ForcePlateType5Test.fp5_dataout(i+4*samples), 1e-4)
                testCase.verifyEqual(wlo(i,6)/1000., -ForcePlateType5Test.fp5_dataout(i+5*samples), 1e-4)
            end
            wlc = fp5.wrench(ma.instrument.Location_CentreOfPressure, false).data()
            for i = 1:samples
                testCase.verifyEqual(wlc(i,7), ForcePlateType5Test.fp5_dataout(i+6*samples), 1e-3);
                testCase.verifyEqual(wlc(i,8), ForcePlateType5Test.fp5_dataout(i+7*samples), 1e-3);
                testCase.verifyEqual(wlc(i,9), ForcePlateType5Test.fp5_dataout(i+8*samples)-16.33887, 1e-3);
            end
        end
        
    end
    
end