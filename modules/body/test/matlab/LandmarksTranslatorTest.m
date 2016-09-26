classdef LandmarksTranslatorTest < matlab.unittest.TestCase

    methods (Test)
    
        function convert(testCase)
            translator = ma.body.LandmarksTranslator('LandmarksTranslator',{ ...
              {'LFHD', 'L.HF'}, ...
              {'LBHD', 'L.HB'}, ...
              {'RFHD', 'R.HF'}, ...
              {'RBHD', 'R.HB'}, ...
              {'C7'  , 'C7'  }, ...
            });
            testCase.verifyEqual(translator.convert('LFHD'), 'L.HF');
            testCase.verifyEqual(translator.convert('LBHD'), 'L.HB');
            testCase.verifyEqual(translator.convert('RFHD'), 'R.HF');
            testCase.verifyEqual(translator.convert('RBHD'), 'R.HB');
            testCase.verifyEqual(translator.convert('C7'), 'C7');
            testCase.verifyEqual(translator.convert('RBHD_'), 'RBHD_');
            testCase.verifyEqual(translator.convert('T10'), 'T10');
        end
  
        function convertIfExists(testCase)
            translator = ma.body.LandmarksTranslator('LandmarksTranslator',{
              {'LFHD', 'L.HF'}, ...
              {'LBHD', 'L.HB'}, ...
              {'RFHD', 'R.HF'}, ...
              {'RBHD', 'R.HB'}, ...
              {'C7'  , 'C7'  }, ...
            });
            testCase.verifyEqual(translator.convertIfExists('LFHD'), 'L.HF');
            testCase.verifyEqual(translator.convertIfExists('LBHD'), 'L.HB');
            testCase.verifyEqual(translator.convertIfExists('RFHD'), 'R.HF');
            testCase.verifyEqual(translator.convertIfExists('RBHD'), 'R.HB');
            testCase.verifyEqual(translator.convertIfExists('C7'), 'C7');
            testCase.verifyEqual(isempty(translator.convertIfExists('RBHD_')), true);
            testCase.verifyEqual(isempty(translator.convertIfExists('T10')), true);
        end
        
    end
    
end