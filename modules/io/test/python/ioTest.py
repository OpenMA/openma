import ma.io
import ma.instrument
import unittest
import numpy as np
from test_file_path import openma_tdd_path_in, openma_tdd_path_out

class IOTest(unittest.TestCase):
    def test_wrong_path(self):
        self.assertRaises(RuntimeError, ma.io.read, 'IDoNotExist.abc')
    
    def test_force_plate_extraction(self):
        root = ma.io.read(openma_tdd_path_in('c3d/standard/sample01/Eb015vi.c3d'))
        forceplates = root.findChildren(ma.instrument.T_ForcePlate)
        self.assertEqual(len(forceplates),2)
        for i in range(0,2):
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
        marker = root.findChild(ma.T_TimeSequence,'RTH4');
        data = marker.data();
        self.assertAlmostEqual(data[16,0], 285.0, 3)
        self.assertAlmostEqual(data[16,1], 222.4166, 3)
        self.assertAlmostEqual(data[16,2], 564.4166, 3)
        
    def test_write_c3d_from_scratch(self):
        root = ma.Node('root')
        trial = ma.Trial('my_analysis',root)
        trial.setProperty('MY_GROUP:MY_PARAMETER',10.0)
        angle = ma.TimeSequence('LHipAngles.Left', 4, 101, 1.0, 0.0, ma.TimeSequence.Type_Angle, 'deg', trial.timeSequences())
        angle.setData(np.random.rand(101,4))
        self.assertEqual(ma.io.write(root,openma_tdd_path_out('c3d/my_analysis.c3d')),True)
        root_ = ma.io.read(openma_tdd_path_out('c3d/my_analysis.c3d'))
        self.assertEqual(len(root_.children()),1)
        self.assertEqual(root_.child(0).property('MY_GROUP:MY_PARAMETER').cast(), 10.0)
        self.assertNotEqual(root_.child(0).property('POINT:SCALE').cast(), 0.0)