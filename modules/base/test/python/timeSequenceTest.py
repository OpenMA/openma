import ma
import numpy as np
import unittest

class TimeSequenceTest(unittest.TestCase):
    
    def test_constructor_with_one_dim(self):
        a = ma.TimeSequence('ba',1,10,200,0.0,-1,'foo');
        self.assertEqual(a.elements(), 10);
        
    def test_constructor_with_two_dims(self):
        a = ma.TimeSequence('ba',[3,3],10,200,0.0,-1,'foo');
        self.assertEqual(a.elements(), 90);
        a = ma.TimeSequence('ba',[int(3),int(3)],10,200,0.0,-1,'foo');
        self.assertEqual(a.elements(), 90);
        self.assertRaises(ValueError, ma.TimeSequence, 'ba',[3.4,3.1],10,200,0.0,-1,'foo')
    
    def test_range(self):
        a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        self.assertEqual(a.range(), [-np.inf, np.inf]);
        a.setRange([-10,10]);
        self.assertEqual(a.range(), [-10, 10]);

    def test_data(self):
        a = ma.TimeSequence('ba',1,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        temp = np.random.rand(10,1);
        a.setData(temp);
        self.assertEqual(np.allclose(a.data(), temp), True)
        
    def test_data_bis(self):
        a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        temp = np.random.rand(10,4);
        a.setData(temp);
        self.assertEqual(np.allclose(a.data(), temp), True)

    def test_data_wrong_dims(self):
        a = ma.TimeSequence('ba',4,10,200,0.0,ma.TimeSequence.Type_Marker,'mm');
        self.assertRaises(ValueError, a.setData, np.random.rand(2,2))