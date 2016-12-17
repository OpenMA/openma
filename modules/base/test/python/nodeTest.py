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
        self.assertEqual(root.refcount(), 1)
        self.assertEqual(child.refcount(), 2)
        del root
        self.assertEqual(child.hasParents(), False)
        self.assertEqual(child.refcount(), 1)

    def test_child0(self):
        root = ma.Node('root')
        self.assertRaises(IndexError, root.child, 0)

    def test_set_property(self):
        root = ma.Node('root')
        root.setProperty('foo',1)
        self.assertEqual(root.property('foo').cast('int'), 1)
        root.setProperty('bar',ma.Any(10.0))
        self.assertEqual(root.property('bar').cast('float'), 10.0)

    def test_several_children(self):
        root = ma.Node('root')
        child1 = ma.Node('child1', root)
        child2 = ma.Node('child2', root)
        self.assertEqual(root.refcount(), 1)
        self.assertEqual(child1.refcount(), 2)
        self.assertEqual(child2.refcount(), 2)
        del root
        self.assertEqual(child1.refcount(), 1)
        self.assertEqual(child2.refcount(), 1)

    def test_children(self):
        root = ma.Node('root')
        child1 = ma.Node('child1', root)
        child2 = ma.Node('child2', root)
        children = root.children()
        self.assertEqual(len(children), 2)
        self.assertEqual(children[0].name(), child1.name())
        self.assertEqual(children[1].name(), child2.name())
        del root
        self.assertEqual(children[0].hasParents(), False)
        self.assertEqual(children[1].hasParents(), False)
        del child1
        del child2
        # Still exist?
        self.assertEqual(children[0].name(), 'child1')
        self.assertEqual(children[1].name(), 'child2')
        self.assertEqual(children[0].refcount(), 1)
        self.assertEqual(children[1].refcount(), 1)

    def test_clear_node(self):
        root = ma.Node('root')
        child1 = ma.Node('child1', root)
        child2 = ma.Node('child2', root)
        self.assertEqual(root.refcount(), 1)
        self.assertEqual(child1.refcount(), 2)
        self.assertEqual(child2.refcount(), 2)
        root.clear()
        self.assertEqual(root.refcount(), 1)
        self.assertEqual(child1.refcount(), 1)
        self.assertEqual(child2.refcount(), 1)

    def test_copy_node(self):
        root = ma.Node('root')
        child1 = ma.Node('child1', root)
        child2 = ma.Node('child2', child1)
        other = ma.Node('other')
        sub1 = ma.Node('sub1', other)
        other.copy(root)
        self.assertEqual(sub1.hasParents(), False)
        self.assertEqual(root.refcount(), 1)
        self.assertEqual(child1.refcount(), 2)
        self.assertEqual(child2.refcount(), 2)
        self.assertEqual(other.refcount(), 1)
        self.assertEqual(sub1.refcount(), 1)
        other.child(0).setName('Goal')
        other.child(0).child(0).setName('Goal2')
        self.assertEqual(child1.name(),'child1')
        self.assertEqual(child2.name(),'child2')

    def test_clone_node(self):
        root = ma.Node('root')
        ma.Node('child1', root)
        other = root.clone()
        other.setName('Foo')
        other.child(0).setName('Bar')
        self.assertEqual(root.hasChildren(), True)
        self.assertEqual(other.hasChildren(), True)
        self.assertEqual(root.refcount(), 1)
        self.assertEqual(other.refcount(), 1)
        self.assertEqual(root.name(),'root')
        self.assertEqual(other.name(),'Foo')
        self.assertEqual(root.child(0).name(),'child1')
        self.assertEqual(other.child(0).name(),'Bar')

    def test_add_parent(self):
        root = ma.Node('root')
        child1 = ma.Node('child1')
        child1.addParent(root)
        self.assertEqual(child1.refcount(), 2)
        self.assertEqual(root.hasChildren(), True)
        self.assertEqual(child1.hasParents(), True)
        self.assertEqual(root.child(0).name(), 'child1')
        del root
        self.assertEqual(child1.refcount(), 1)

    def test_remove_parent(self):
        root = ma.Node('root')
        child1 = ma.Node('child1', root)
        child1.removeParent(root)
        self.assertEqual(child1.refcount(), 1)
        self.assertEqual(root.hasChildren(), False)
        self.assertEqual(child1.hasParents(), False)