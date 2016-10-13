classdef ForcePlateType3Test < matlab.unittest.TestCase

    methods (Test)
    
        function setgetSensorOffsets(testCase)
            fp3 = ma.instrument.ForcePlateType3('FP');
            fp3.setSensorOffsets([123.4, 456.7]);
            so = fp3.sensorOffsets();
            testCase.verifyEqual(so(1), 123.4, 'AbsTol', 1e-15);
            testCase.verifyEqual(so(2), 456.7, 'AbsTol', 1e-15);
        end
    
    end
    
end