import ma
import unittest

class NodeTest(unittest.TestCase):
    def test_ctor(self):
        n = ma.Node('test')
        self.assertEqual(n.name(), 'test')
        
    def test_ctor_with_parent(self):
        root = ma.Node('root')
        child = ma.Node('child', root)
        self.assertEqual(root.hasChildren(), True)
        self.assertEqual(child.hasParents(), True)
        self.assertEqual(root.name(), 'root')
        self.assertEqual(child.name(), 'child')
        del root
        self.assertEqual(child.hasParents(), False)
    
    def test_child0(self):
        root = ma.Node('root');
        self.assertRaises(IndexError, root.child, 0);