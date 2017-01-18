classdef ForcePlateType3Test < matlab.unittest.TestCase

    methods (Test)
    
        function setgetSensorOffsets(testCase)
            fp3 = ma.instrument.ForcePlateType3('FP');
            fp3.setSensorOffsets([123.4, 456.7]);
            so = fp3.sensorOffsets();
            testCase.verifyEqual(so(1), 123.4, 'AbsTol', 1e-15);
            testCase.verifyEqual(so(2), 456.7, 'AbsTol', 1e-15);
        end
        
        function setgetSoftReset(testCase)
            fp3 = ma.instrument.ForcePlateType3('FP');
            testCase.verifyEqual(fp3.isSoftResetEnabled(), false);
            fp3.setSoftResetEnabled(true);
            testCase.verifyEqual(fp3.isSoftResetEnabled(), true);
            srs = fp3.softResetSamples();
            testCase.verifyEqual(srs(1), 0);
            testCase.verifyEqual(srs(2), 0);
            fp3.setSoftResetSamples([0,9]); % WARNING : 0-based indices
            srs = fp3.softResetSamples();
            testCase.verifyEqual(srs(1), 0);
            testCase.verifyEqual(srs(2), 9);
            fp3.setSoftResetSamples([9,19]); % WARNING : 0-based indices
            srs = fp3.softResetSamples();
            testCase.verifyEqual(srs(1), 9);
            testCase.verifyEqual(srs(2), 19);
        end
    
    end
    
end