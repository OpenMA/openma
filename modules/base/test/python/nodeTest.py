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
        self.assertEqual(root.property('name').cast('string'), 'root')
        root.setProperty('foo',1)
        self.assertEqual(root.property('foo').cast('int'), 1)
        root.setProperty('bar',ma.Any(10.0))
        self.assertEqual(root.property('bar').cast('float'), 10.0)
        
    def test_copy_variable_in_cell(self):
        nodes = [];
        for i in xrange(0,3):
            node = ma.Node('Node'+str(i+1));
            nodes.append(node);
        self.assertEqual(nodes[0].name(), 'Node1');
        self.assertEqual(nodes[1].name(), 'Node2');
        self.assertEqual(nodes[2].name(), 'Node3');
    
    def test_find_children(self):
        root = ma.Node('root');
        node = ma.Node('node',root);
        node.setProperty('foo','bar');
        node.setProperty('hello',True);
        children = root.findChildren(ma.T_Node,'.*',[['name','node']]);
        self.assertEqual(len(children),1);
        self.assertEqual(children[0].name(),'node');
        children = root.findChildren(ma.T_Node,'.*',[['foo','bar']]);
        self.assertEqual(len(children),1);
        self.assertEqual(children[0].name(),'node');
        children = root.findChildren(ma.T_Node,'.*',[['hello',True]]);
        self.assertEqual(len(children),1);
        self.assertEqual(children[0].name(),'node');
        
    def test_find_child(self):
        root = ma.Node('root');
        node = ma.Node('node',root);
        node.setProperty('foo','bar');
        node.setProperty('hello',True);
        child = root.findChild(ma.T_Node,'node');
        self.assertEqual(child == None, False);
        self.assertEqual(child.name(), 'node');
        child = root.findChild(ma.T_Node,'anything');
        self.assertEqual(child == None, True);
        child = root.findChild(ma.T_Node,'node',[['foo','bar']]);
        self.assertEqual(child == None, False);
        self.assertEqual(child.name(),'node');
        child = root.findChild(ma.T_Node,'node',[['hello',False]]);
        self.assertEqual(child == None, True);
        child = root.findChild(ma.T_Node,'',[['foo','bar']]);
        self.assertEqual(child == None, False);
        self.assertEqual(child.name(),'node');