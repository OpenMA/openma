import ma
import unittest

class AnyTest(unittest.TestCase):
    def test_default_ctor(self):
        a = ma.Any()
        self.assertEqual(a.isEmpty(), True)
        self.assertEqual(a.isValid(), False)
        self.assertEqual(a.dimensions(),[])
        self.assertEqual(a.size(), 0)
        self.assertEqual(a.cast(), 0.)
        self.assertEqual(a.cast('bool'), False)
        self.assertEqual(a.cast('int'), 0)
        self.assertEqual(a.cast('long'), 0)
        self.assertEqual(a.cast('string'), '')
        
    def test_other_constructor_single(self):
        a = ma.Any(1)
        self.assertEqual(a.cast(),1.)
        self.assertEqual(a.dimensions(), [])
        self.assertEqual(a.size(), 1)
        self.assertEqual(a.cast('int'),1)
        self.assertEqual(a.cast('float'),1.)

    def test_other_constructor_array(self):
        a = ma.Any([[0, 10], [12309127, 999999]])
        self.assertEqual(a.dimensions(), [2,2])
        self.assertEqual(a.size(), 4)
        self.assertEqual(a.cast(),[[0., 10.], [12309127., 999999.]])
        self.assertEqual(a.cast('long'), [[0, 10], [12309127, 999999]])
        self.assertEqual(a.cast('string'), [['0', '10'], ['12309127', '999999']])

    def test_other_constructor_logical(self):
        a = ma.Any([[True, False, False], [False, True, True]]);
        self.assertEqual(a.dimensions(), [2,3]);
        self.assertEqual(a.size(), 6);
        self.assertEqual(a.cast(),[[1., 0., 0.], [0, 1, 1]]);
        self.assertEqual(a.cast('bool'),[[True, False, False], [False, True, True]]);
        self.assertEqual(a.cast('string'), [['1','0','0'], ['0', '1', '1']]);

    def test_other_constructor_List(self):
        a = ma.Any(['foo','bar']);
        self.assertEqual(a.dimensions(), [2]);
        self.assertEqual(a.size(), 2);
        self.assertEqual(a.cast(),[0, 0]);
        self.assertEqual(a.cast('string'), ['foo','bar']);
    
    def test_unsupported_type(self):
        self.assertRaises(TypeError, ma.Any, 1j)
        a = ma.Any()
        self.assertRaises(TypeError, a.assign, 1j)
        
    def test_mix_type(self):
        self.assertRaises(TypeError, ma.Any, [])
        self.assertRaises(TypeError, ma.Any, [[],[]])
        self.assertRaises(TypeError, ma.Any, [[],''])
        self.assertRaises(TypeError, ma.Any, ['',[]])
        self.assertRaises(TypeError, ma.Any, [1.,1])
        self.assertRaises(TypeError, ma.Any, [1, 1j])
        self.assertRaises(TypeError, ma.Any, [1, 1, 1j])
        self.assertRaises(TypeError, ma.Any, [1,'1'])
        self.assertRaises(TypeError, ma.Any, [[1],['1']])
        self.assertRaises(TypeError, ma.Any, [[1, 1.],['1','1']])
        self.assertRaises(TypeError, ma.Any, [1,[1,1]])
        self.assertRaises(TypeError, ma.Any, [[1, 1],'1'])
        self.assertRaises(TypeError, ma.Any, [[1, 1],[1.,'1']])
        self.assertRaises(TypeError, ma.Any, [[[1],[1]],[[1.],['1']]])
        
    def test_mix_dims(self):
        self.assertRaises(ValueError, ma.Any, [[1,1],[1,1,1]])