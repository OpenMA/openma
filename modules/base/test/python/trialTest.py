import ma
import unittest

class TrialTest(unittest.TestCase):
    
    def test_events(self):
        t = ma.Trial('trial')
        evts = t.events()
        self.assertEqual(t.hasChildren(), True)
        self.assertEqual(evts.hasParents(), True)
        self.assertEqual(t.name(), 'trial')
        self.assertEqual(evts.name(), 'Events')
        self.assertEqual(t.refcount(), 1)
        self.assertEqual(evts.refcount(), 2)
        del t
        self.assertEqual(evts.hasParents(), False)
        self.assertEqual(evts.refcount(), 1)
    
    def test_timesequences(self):
        t = ma.Trial('trial')
        tss = t.timeSequences()
        self.assertEqual(t.hasChildren(), True)
        self.assertEqual(tss.hasParents(), True)
        self.assertEqual(t.name(), 'trial')
        self.assertEqual(tss.name(), 'TimeSequences')
        self.assertEqual(t.refcount(), 1)
        self.assertEqual(tss.refcount(), 2)
        del t
        self.assertEqual(tss.hasParents(), False)
        self.assertEqual(tss.refcount(), 1)
        
    def test_hardwares(self):
        t = ma.Trial('trial')
        hws = t.hardwares()
        self.assertEqual(t.hasChildren(), True)
        self.assertEqual(hws.hasParents(), True)
        self.assertEqual(t.name(), 'trial')
        self.assertEqual(hws.name(), 'Hardwares')
        self.assertEqual(t.refcount(), 1)
        self.assertEqual(hws.refcount(), 2)
        del t
        self.assertEqual(hws.hasParents(), False)
        self.assertEqual(hws.refcount(), 1)
