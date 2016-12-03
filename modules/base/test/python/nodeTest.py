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
        
    def test_set_property(self):
        root = ma.Node('root');
        root.setProperty('foo',1)
        self.assertEqual(root.property('foo').cast('int'), 1)
        root.setProperty('bar',ma.Any(10.0))
        self.assertEqual(root.property('bar').cast('float'), 10.0)
        
    def test_several_children(self):
        root = ma.Node('root');
        child1 = ma.Node('child1', root);
        child2 = ma.Node('child2', root);
        self.assertEqual(root.property('_MA_REF_COUNTER').cast('int'), 0)
        self.assertEqual(child1.property('_MA_REF_COUNTER').cast('int'), 1)
        self.assertEqual(child2.property('_MA_REF_COUNTER').cast('int'), 1)