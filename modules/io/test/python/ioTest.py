import ma.io
import ma.instrument
import unittest
from test_file_path import openma_tdd_path_in

class IOTest(unittest.TestCase):
    def test_wrong_path(self):
        self.assertRaises(IOError, ma.io.read, 'IDoNotExist.abc')
    
    def test_force_plate_extraction(self):
        root = ma.io.read(openma_tdd_path_in('c3d/standard/sample01/Eb015vi.c3d'))
        forceplates = root.findChildren(ma.instrument.T_ForcePlate)
        self.assertEqual(len(forceplates),2)
        for i in xrange(0,2):
            fp = forceplates[i]
            self.assertEqual(fp.type(), 2)
            self.assertEqual(fp.type(), ma.instrument.ForcePlate.Type_Type2)
            self.assertEqual(fp.type(), ma.instrument.ForcePlate.Type_Raw6x6)
            channels = fp.channels().findChildren(ma.T_TimeSequence)
            self.assertEqual(len(channels),6)
            self.assertEqual(channels[0].name(), 'FX'+str(i+1))
            self.assertEqual(channels[1].name(), 'FY'+str(i+1))
            self.assertEqual(channels[2].name(), 'FZ'+str(i+1))
            self.assertEqual(channels[3].name(), 'MX'+str(i+1))
            self.assertEqual(channels[4].name(), 'MY'+str(i+1))
            self.assertEqual(channels[5].name(), 'MZ'+str(i+1))