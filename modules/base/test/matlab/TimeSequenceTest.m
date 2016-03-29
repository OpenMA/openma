classdef TimeSequenceTest < matlab.unittest.TestCase
    methods (Test)
    
        function range(testCase)
            a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
            testCase.verifyEqual(a.range(), [-Inf Inf]);
            a.setRange([-10,10]);
            testCase.verifyEqual(a.range(), [-10 10]);
        end
        
        function data(testCase)
            a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
            temp = rand(10,4);
            a.setData(temp);
            testCase.verifyEqual(a.data(), temp);
        end
        
    end
end