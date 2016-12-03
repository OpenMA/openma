classdef TimeSequenceTest < matlab.unittest.TestCase
    methods (Test)
    
        function constructorWithTwoDims(testCase)
            a = ma.TimeSequence('ba',[3,3],10,200,0.0,-1,'foo');
            testCase.verifyEqual(a.elements(), 90);
            a = ma.TimeSequence('ba',single([3,3]),10,200,0.0,-1,'foo');
            testCase.verifyEqual(a.elements(), 90);
            a = ma.TimeSequence('ba',int16([3,3]),10,200,0.0,-1,'foo');
            testCase.verifyEqual(a.elements(), 90);
            testCase.verifyError(@()ma.TimeSequence('ba',[3.4,3.1],10,200,0.0,-1,'foo'), 'SWIG:ValueError');
        end
    
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