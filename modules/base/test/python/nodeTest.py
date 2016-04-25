import ma
import unittest

class NodeTest(unittest.TestCase):
    def test_ctor(self):
        n = ma.Node('test')
        self.assertEqual(n.name(), 'test')