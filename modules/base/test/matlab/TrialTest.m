classdef TrialTest < matlab.unittest.TestCase
    methods (Test)
    
        function events(testCase)
            t = ma.Trial('trial');
            e = t.events();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(e.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(e.name(), 'Events');
            testCase.verifyEqual(double(t.refcount()), 1);
            testCase.verifyEqual(double(e.refcount()), 2);
            delete(t);
            testCase.verifyEqual(e.hasParents(), false);
            testCase.verifyEqual(double(e.refcount()), 1);
        end
        
        function timesequences(testCase)
            t = ma.Trial('trial');
            ts = t.timeSequences();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(ts.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(ts.name(), 'TimeSequences');
            testCase.verifyEqual(double(t.refcount()), 1);
            testCase.verifyEqual(double(ts.refcount()), 2);
            delete(t);
            testCase.verifyEqual(ts.hasParents(), false);
            testCase.verifyEqual(double(ts.refcount()), 1);
        end
        
    end
end