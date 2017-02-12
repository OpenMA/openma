import ma
import unittest

class EventTest(unittest.TestCase):
    
    def test_constructor(self):
        evt = ma.Event('Foot Strike', 0., 'Right', 'Anonymous')
        self.assertEqual(evt.name(), 'Foot Strike')
        self.assertEqual(evt.time(), 0.)
        self.assertEqual(evt.context(), 'Right')
        self.assertEqual(evt.subject(), 'Anonymous')
        self.assertEqual(evt.property('name').cast('string'), 'Foot Strike')
        self.assertEqual(evt.property('time').cast(), 0.)
        self.assertEqual(evt.property('context').cast('string'), 'Right')
        self.assertEqual(evt.property('subject').cast('string'), 'Anonymous')
    
    def test_find_Children(self):
        root = ma.Node('Root')
        evt01 = ma.Event('Foot Strike', 0.,   'Right', 'Anonymous', root)
        evt02 = ma.Event('Foot Strike', 1.05, 'Right', 'Anonymous', root)
        evt03 = ma.Event('Foot Strike', 2.11, 'Right', 'Anonymous', root)
        evt04 = ma.Event('Foot Strike', 0.5,  'Left',  'Anonymous', root)
        evt05 = ma.Event('Foot Strike', 1.60, 'Left',  'Anonymous', root)
        evt06 = ma.Event('Foot Strike', 2.73, 'Left',  'Anonymous', root)
        evt07 = ma.Event('Foot Off',    0.84, 'Right', 'Anonymous', root)
        evt08 = ma.Event('Foot Off',    1.90, 'Right', 'Anonymous', root)
        evt09 = ma.Event('Foot Off',    2.85, 'Right', 'Anonymous', root)
        evt10 = ma.Event('Foot Off',    0.75, 'Left',  'Anonymous', root)
        evt11 = ma.Event('Foot Off',    2.10, 'Left',  'Anonymous', root)
        evt12 = ma.Event('Foot Off',    3.09, 'Left',  'Anonymous', root)
        evts = root.findChildren(ma.T_Event,'Foot Strike')
        self.assertEqual(len(evts), 6)
        evts = root.findChildren(ma.T_Event,'Foot Strike',[['context','Right']])
        self.assertEqual(len(evts), 3)
        evts = root.findChildren(ma.T_Event,'.*',[['context','Right']])
        self.assertEqual(len(evts), 6)
        evts = root.findChildren(ma.T_Event,'Foot Off')
        self.assertEqual(len(evts), 6)
        evts = root.findChildren(ma.T_Event,'Foot Off',[['context','Left']])
        self.assertEqual(len(evts), 3)
        evts = root.findChildren(ma.T_Event,'.*',[['context','Left']])
        self.assertEqual(len(evts), 6)
        
    def test_node_cast(self):
        root = ma.Node('Root')
        evt01 = ma.Event('Foot Strike', 0.,'Right','Anonymous',root)
        evt01_ = ma.node_cast(ma.T_Event, root.child(0))
        self.assertEqual(root.hasChildren(), True)
        self.assertEqual(evt01.refcount(), 3)
        self.assertEqual(evt01_.refcount(), 3)
        self.assertEqual(evt01.hasParents(), True)
        self.assertEqual(evt01_.hasParents(), True)
        self.assertEqual(evt01.name(), evt01_.name())
        del evt01
        self.assertEqual(root.hasChildren(), True)
        self.assertEqual(evt01_.refcount(), 2)
        del root
        self.assertEqual(evt01_.hasParents(), False)
        self.assertEqual(evt01_.refcount(), 1)