import ma.instrument
import unittest

class ForcePlateType3Test(unittest.TestCase):
    def test_setget_sensor_offsets(self):
        fp3 = ma.instrument.ForcePlateType3('FP')
        fp3.setSensorOffsets([123.4, 456.7])
        so = fp3.sensorOffsets()
        self.assertAlmostEqual(so[0], 123.4, 15)
        self.assertAlmostEqual(so[1], 456.7, 15)
        