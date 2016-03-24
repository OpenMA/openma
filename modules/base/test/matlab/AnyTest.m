classdef AnyTest < matlab.unittest.TestCase
    methods (Test)
    
        function defaultConstructor(testCase)
            a = ma.Any;
            testCase.verifyEqual(a.isEmpty(),true);
            testCase.verifyEqual(a.isValid(),false);
            testCase.verifyEqual(a.dimensions(),[]);
            testCase.verifyEqual(a.size(), 0);
            testCase.verifyEqual(a.cast(), 0);
            testCase.verifyEqual(a.cast('int64'), int64(0));
            testCase.verifyEqual(a.cast('char'), '');
            testCase.verifyEqual(a.cast('uint8'), uint8(0));
        end
        
        function otherConstructorSingle(testCase)
            a = ma.Any(1);
            testCase.verifyEqual(a.cast(),1);
            testCase.verifyEqual(a.dimensions(), [1,1]);
            testCase.verifyEqual(a.size(), 1);
            testCase.verifyEqual(a.cast('int64'),int64(1));
            testCase.verifyEqual(a.cast('single'),single(1));
        end
        
        function otherConstructorArray(testCase)
            a = ma.Any([uint64(0) uint64(10) ; uint64(123091278957) uint64(999999)]);
            testCase.verifyEqual(a.cast(),[0, 10 ; 123091278957 999999]);
            testCase.verifyEqual(a.dimensions(), [2,2]);
            testCase.verifyEqual(a.size(), 4);
            testCase.verifyEqual(a.cast('int64'),[int64(0) int64(10) ; int64(123091278957) int64(999999)]);
            testCase.verifyEqual(a.cast('single'),[single(0) single(10) ; single(123091278957) single(999999)]);
            testCase.verifyEqual(a.cast('char'), {'0' '10' ; '123091278957' '999999'});
        end
        
        function otherConstructorLogical(testCase)
            a = ma.Any([true, false, false; false, true, true]);
            testCase.verifyEqual(a.cast(),[1, 0, 0 ; 0, 1, 1]);
            testCase.verifyEqual(a.dimensions(), [2,3]);
            testCase.verifyEqual(a.size(), 6);
            testCase.verifyEqual(a.cast('char'), {'1','0','0' ; '0', '1', '1'});
        end
        
        function otherConstructorCell(testCase)
            a = ma.Any({'foo','bar'});
            testCase.verifyEqual(a.cast(),[0, 0]);
            testCase.verifyEqual(a.dimensions(), [1,2]);
            testCase.verifyEqual(a.size(), 2);
            testCase.verifyEqual(a.cast('char'), {'foo','bar'});
        end
        
    end
end