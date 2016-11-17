import ma.body
import ma.io
import unittest

from test_file_path import openma_tdd_path_in

class PluginGaitCalibrationTest(unittest.TestCase):
  def test_calibrateLowerBodyKADMed(self):
    helper = ma.body.PluginGait(ma.body.Region_Lower, ma.body.Side_Both, ma.body.PluginGait.Variant_KAD)
    subject = ma.Subject('Anonymous', [
      ['markerDiameter', 14],
      ['leftLegLength', 860.0],
      ['leftKneeWidth', 102.0],
      ['leftAnkleWidth', 75.3],
      ['leftFootFlatEnabled', True],
      ['rightLegLength', 865.0],
      ['rightKneeWidth', 103.4],
      ['rightAnkleWidth', 72.9],
      ['rightFootFlatEnabled', True],
    ])
    statictrials = ma.io.read(openma_tdd_path_in("c3d/plugingait/PiGKad_Calibration_FootFlat.c3d"))
    self.assertEqual(helper.calibrate(statictrials, subject), True)
    self.assertAlmostEqual(helper.interAsisDistance(), 211.162, 3);
    self.assertAlmostEqual(helper.leftAsisTrochanterAPDistance(), 62.208, 3);
    self.assertAlmostEqual(helper.leftThighRotationOffset(), 0.156355, 4);
    self.assertAlmostEqual(helper.leftShankRotationOffset(), 0.242123, 4);
    self.assertAlmostEqual(helper.leftStaticPlantarFlexionOffset(), 0.0629524, 4);
    self.assertAlmostEqual(helper.leftStaticRotationOffset(), 0.00504156, 4);
    self.assertAlmostEqual(helper.rightAsisTrochanterAPDistance(), 62.852, 4);
    self.assertAlmostEqual(helper.rightThighRotationOffset(), -0.175377, 4);
    self.assertAlmostEqual(helper.rightShankRotationOffset(), 0.268151, 4);
    self.assertAlmostEqual(helper.rightStaticPlantarFlexionOffset(), 0.12429, 4);
    self.assertAlmostEqual(helper.rightStaticRotationOffset(), 0.0345916, 4);
  
  def test_calibrateLowerBodyKADMed2(self):
    helper = ma.body.PluginGait(ma.body.Region_Lower, ma.body.Side_Both, ma.body.PluginGait.Variant_KADMed)
    subject = ma.Subject('Anonymous', [
      ['markerDiameter', 25],
      ['leftLegLength', 775.0],
      ['leftKneeWidth', 105.1],
      ['leftAnkleWidth', 68.4],
      ['rightLegLength', 770.0],
      ['rightKneeWidth', 107.0],
      ['rightAnkleWidth', 68.6]
    ])
    statictrials = ma.io.read(openma_tdd_path_in("c3d/plugingait/PiGKadMed_Calibration2.c3d"))
    self.assertEqual(helper.calibrate(statictrials, subject), True)
    self.assertAlmostEqual(helper.interAsisDistance(), 213.74600219726562, 3)
    self.assertAlmostEqual(helper.leftAsisTrochanterAPDistance(), 51.259998321533203, 3)
    self.assertAlmostEqual(helper.leftThighRotationOffset(), -0.19650661945343018, 4)
    self.assertAlmostEqual(helper.leftShankRotationOffset(), 0.036574419587850571, 4)
    self.assertAlmostEqual(helper.leftTibialTorsionOffset(), 0.18572920560836792, 4)
    self.assertAlmostEqual(helper.leftStaticPlantarFlexionOffset(), 0.082310251891613007, 4)
    self.assertAlmostEqual(helper.leftStaticRotationOffset(), 0.08043140172958374, 4)
    self.assertAlmostEqual(helper.leftAnkleAbAddOffset(), 0.13921844959259033, 4)
    self.assertAlmostEqual(helper.rightAsisTrochanterAPDistance(), 50.616001129150391, 3)
    self.assertAlmostEqual(helper.rightThighRotationOffset(), 0.230163544416427610, 4)
    self.assertAlmostEqual(helper.rightShankRotationOffset(), -0.11728926748037338, 4)
    self.assertAlmostEqual(helper.rightTibialTorsionOffset(), -0.34318757057189941, 4)
    self.assertAlmostEqual(helper.rightStaticPlantarFlexionOffset(), 0.032737139612436295, 4)
    self.assertAlmostEqual(helper.rightStaticRotationOffset(), 0.28456372022628784, 4)
    self.assertAlmostEqual(helper.rightAnkleAbAddOffset(), 0.13163727521896362, 4)