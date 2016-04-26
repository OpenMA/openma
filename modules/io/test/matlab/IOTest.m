classdef IOTest < matlab.unittest.TestCase
    methods (Test)
    
        function wrongPath(testCase)
            testCase.verifyError(@()ma.io.read('IDoNotExist.abc'), 'SWIG:IOError');
        end
    
    end
end