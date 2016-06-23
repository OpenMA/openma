import ma
import numpy as np
import unittest

class TimeSequenceTest(unittest.TestCase):
    
    def constructor_with_two_dims(self):
        a = ma.TimeSequence('ba',[3,3],10,200,0.0,-1,'foo');
        self.verifyEqual(a.elements(), 90);
        a = ma.TimeSequence('ba',[int(3),int(3)],10,200,0.0,-1,'foo');
        self.verifyEqual(a.elements(), 90);
        self.assertRaises(ValueError, ma.TimeSequence, 'ba',[3.4,3.1],10,200,0.0,-1,'foo')
    
    def range(self):
        a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        self.verifyEqual(a.range(), [-np.inf, np.inf]);
        a.setRange([-10,10]);
        self.verifyEqual(a.range(), [-10, 10]);
    
    def data(self):
        a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        temp = np.random.random(10,4);
        a.setData(temp);
        self.verifyEqual(a.data(), temp);
    
    def data_wrong_dims(self):
        a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        self.assertRaises(ValueError, a.setData, np.random.random(2,2))