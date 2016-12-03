classdef TrialTest < matlab.unittest.TestCase
    methods (Test)
    
        function events(testCase)
            t = ma.Trial('trial');
            e = t.events();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(e.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(e.name(), 'Events');
            testCase.verifyEqual(t.property('_MA_REF_COUNTER').cast, 0);
            testCase.verifyEqual(e.property('_MA_REF_COUNTER').cast, 1);
            delete(t);
            testCase.verifyEqual(e.hasParents(), false);
            testCase.verifyEqual(e.property('_MA_REF_COUNTER').cast, 0);
        end
        
        function timesequences(testCase)
            t = ma.Trial('trial');
            ts = t.timeSequences();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(ts.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(ts.name(), 'TimeSequences');
            testCase.verifyEqual(t.property('_MA_REF_COUNTER').cast, 0);
            testCase.verifyEqual(ts.property('_MA_REF_COUNTER').cast, 1);
            delete(t);
            testCase.verifyEqual(ts.hasParents(), false);
            testCase.verifyEqual(ts.property('_MA_REF_COUNTER').cast, 0);
        end
        
    end
end