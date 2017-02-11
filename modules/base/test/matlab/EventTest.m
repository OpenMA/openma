classdef EventTest < matlab.unittest.TestCase
    methods (Test)
    
        function constructor(testCase)
            evt = ma.Event('Foot Strike', 0., 'Right', 'Anonymous');
            testCase.verifyEqual(evt.name, 'Foot Strike');
            testCase.verifyEqual(evt.time, 0.);
            testCase.verifyEqual(evt.context, 'Right');
            testCase.verifyEqual(evt.subject, 'Anonymous');
            testCase.verifyEqual(evt.property('name').cast('char'), 'Foot Strike');
            testCase.verifyEqual(evt.property('time').cast(), 0.);
            testCase.verifyEqual(evt.property('context').cast('char'), 'Right');
            testCase.verifyEqual(evt.property('subject').cast('char'), 'Anonymous');
        end
    
        function findChildren(testCase)
            root = ma.Node('Root');
            evt01 = ma.Event('Foot Strike', 0.,   'Right', 'Anonymous', root);
            evt02 = ma.Event('Foot Strike', 1.05, 'Right', 'Anonymous', root);
            evt03 = ma.Event('Foot Strike', 2.11, 'Right', 'Anonymous', root);
            evt04 = ma.Event('Foot Strike', 0.5,  'Left',  'Anonymous', root);
            evt05 = ma.Event('Foot Strike', 1.60, 'Left',  'Anonymous', root);
            evt06 = ma.Event('Foot Strike', 2.73, 'Left',  'Anonymous', root);
            evt07 = ma.Event('Foot Off',    0.84, 'Right', 'Anonymous', root);
            evt08 = ma.Event('Foot Off',    1.90, 'Right', 'Anonymous', root);
            evt09 = ma.Event('Foot Off',    2.85, 'Right', 'Anonymous', root);
            evt10 = ma.Event('Foot Off',    0.75, 'Left',  'Anonymous', root);
            evt11 = ma.Event('Foot Off',    2.10, 'Left',  'Anonymous', root);
            evt12 = ma.Event('Foot Off',    3.09, 'Left',  'Anonymous', root);
            evts = root.findChildren(ma.T_Event,'Foot Strike');
            testCase.verifyEqual(length(evts), 6);
            evts = root.findChildren(ma.T_Event,'Foot Strike',{{'context','Right'}});
            testCase.verifyEqual(length(evts), 3);
            evts = root.findChildren(ma.T_Event,'.*',{{'context','Right'}});
            testCase.verifyEqual(length(evts), 6);
            evts = root.findChildren(ma.T_Event,'Foot Off');
            testCase.verifyEqual(length(evts), 6);
            evts = root.findChildren(ma.T_Event,'Foot Off',{{'context','Left'}});
            testCase.verifyEqual(length(evts), 3);
            evts = root.findChildren(ma.T_Event,'.*',{{'context','Left'}});
            testCase.verifyEqual(length(evts), 6);
        end
    
    end
end