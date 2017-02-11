classdef TrialTest < matlab.unittest.TestCase
    methods (Test)
    
        function events(testCase)
            t = ma.Trial('trial');
            evts = t.events();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(evts.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(evts.name(), 'Events');
            testCase.verifyEqual(double(t.refcount()), 1);
            testCase.verifyEqual(double(evts.refcount()), 2);
            delete(t);
            testCase.verifyEqual(evts.hasParents(), false);
            testCase.verifyEqual(double(evts.refcount()), 1);
        end
        
        function timesequences(testCase)
            t = ma.Trial('trial');
            tss = t.timeSequences();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(tss.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(tss.name(), 'TimeSequences');
            testCase.verifyEqual(double(t.refcount()), 1);
            testCase.verifyEqual(double(tss.refcount()), 2);
            delete(t);
            testCase.verifyEqual(tss.hasParents(), false);
            testCase.verifyEqual(double(tss.refcount()), 1);
        end
        
        function hardwares(testCase)
            t = ma.Trial('trial');
            hws = t.hardwares();
            testCase.verifyEqual(t.hasChildren(), true);
            testCase.verifyEqual(hws.hasParents(), true);
            testCase.verifyEqual(t.name(), 'trial');
            testCase.verifyEqual(hws.name(), 'Hardwares');
            testCase.verifyEqual(double(t.refcount()), 1);
            testCase.verifyEqual(double(hws.refcount()), 2);
            delete(t);
            testCase.verifyEqual(hws.hasParents(), false);
            testCase.verifyEqual(double(hws.refcount()), 1);
        end
        
    end
end