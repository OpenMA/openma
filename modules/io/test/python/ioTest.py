import ma.io
import unittest

class IOTest(unittest.TestCase):
    def wrong_path(self):
        self.assertRaises(IOError, ma.io.read, 'IDoNotExist.abc');
        