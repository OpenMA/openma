import ma.instrument
import unittest

class ForcePlateType3Test(unittest.TestCase):
    def test_setget_sensor_offsets(self):
        fp3 = ma.instrument.ForcePlateType3('FP')
        fp3.setSensorOffsets([123.4, 456.7])
        so = fp3.sensorOffsets()
        self.assertAlmostEqual(so[0], 123.4, 15)
        self.assertAlmostEqual(so[1], 456.7, 15)
        
        
    def  test_setget_soft_reset(self):
        fp3 = ma.instrument.ForcePlateType3('FP')
        self.assertEqual(fp3.isSoftResetEnabled(), False)
        fp3.setSoftResetEnabled(True)
        self.assertEqual(fp3.isSoftResetEnabled(), True)
        srs = fp3.softResetSamples()
        self.assertEqual(srs[0], 0)
        self.assertEqual(srs[1], 0)
        fp3.setSoftResetSamples([0,9])
        srs = fp3.softResetSamples()
        self.assertEqual(srs[0], 0)
        self.assertEqual(srs[1], 9)
        fp3.setSoftResetSamples([9,19])
        srs = fp3.softResetSamples()
        self.assertEqual(srs[0], 9)
        self.assertEqual(srs[1], 19)