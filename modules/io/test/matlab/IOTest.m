classdef IOTest < matlab.unittest.TestCase
    methods (Test)
    
        function wrongPath(testCase)
            testCase.verifyError(@()ma.io.read('IDoNotExist.abc'), 'SWIG:IOError');
        end
        
        function forcePlateExtraction(testCase)
            root = ma.io.read(OpenMATDDPathIn('c3d/standard/sample01/Eb015vi.c3d'));
            forceplates = root.findChildren(ma.instrument.T_ForcePlate);
            testCase.verifyEqual(length(forceplates),2);
            for i = 1:2
                fp = forceplates{i};
                testCase.verifyEqual(double(fp.type), 2);
                testCase.verifyEqual(fp.type, ma.instrument.ForcePlate.Type_Type2);
                testCase.verifyEqual(fp.type, ma.instrument.ForcePlate.Type_Raw6x6);
                channels = fp.channels().findChildren(ma.T_TimeSequence);
                testCase.verifyEqual(length(channels),6);
                testCase.verifyEqual(channels{1}.name, ['FX',num2str(i)]);
                testCase.verifyEqual(channels{2}.name, ['FY',num2str(i)]);
                testCase.verifyEqual(channels{3}.name, ['FZ',num2str(i)]);
                testCase.verifyEqual(channels{4}.name, ['MX',num2str(i)]);
                testCase.verifyEqual(channels{5}.name, ['MY',num2str(i)]);
                testCase.verifyEqual(channels{6}.name, ['MZ',num2str(i)]);
            end
        end
    
    end
end