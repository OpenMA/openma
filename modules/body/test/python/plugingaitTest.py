import ma.body
import ma.io
import unittest
from test_file_path import openma_tdd_path_in

class PluginGaitTest(unittest.TestCase):
  def test_fix_crash_old_refcount_mechanism(self):
    # Note: The following code crashed with the old reference count mechanism
    #       The problem was related to the inclusion of trial and translator   
    #       object in a model but the counter was not incremented.
    calibration_filename = openma_tdd_path_in('c3d/plugingait/PiG_Calibration-FlatFoot-Hole.c3d')
    dynamic_filename = openma_tdd_path_in('c3d/plugingait/PiG_Motion-FlatFoot-Hole.c3d')
    subject = ma.Subject('Anonymous',
        [['markerDiameter', 16.0],
         ['leftFootFlatEnabled', True],
         ['leftLegLength', 940.0],
         ['leftKneeWidth', 110.0],
         ['leftAnkleWidth', 70.0],
         ['rightFootFlatEnabled', True],
         ['rightLegLength', 940.0],
         ['rightKneeWidth', 120.0],
         ['rightAnkleWidth', 70.0]])
    skeletonhelper = ma.body.PluginGait(ma.body.Region_Lower, ma.body.Side_Both)
    ma.body.LandmarksTranslator('CGM',
           [['LASI', 'L.ASIS'],
            ['RASI', 'R.ASIS'],
            ['LPSI', 'L.PSIS'],
            ['RPSI', 'R.PSIS'],
            ['SACR', 'SC'],
            ['LTHI', 'L.ITB'],
            ['RTHI', 'R.ITB'],
            ['LKNE', 'L.LFE'],
            ['RKNE', 'R.LFE'],
            ['LTIB', 'L.LS'],
            ['RTIB', 'R.LS'],
            ['LANK', 'L.LTM'],
            ['RANK', 'R.LTM'],
            ['LTOE', 'L.MTH2'],
            ['RTOE', 'R.MTH2'],
            ['LHEE', 'L.HEE'],
            ['RHEE', 'R.HEE']], skeletonhelper)
    trials = ma.io.read(calibration_filename)
    ma.body.calibrate(skeletonhelper, trials, subject)
    models = ma.body.reconstruct(skeletonhelper, trials)
    jointkinematics = ma.body.extract_joint_kinematics(models)
    self.assertEqual(trials.child(0).refcount(),3)
    self.assertEqual(models.findChild(ma.T_Trial).refcount(),3)
    self.assertEqual(models.findChild(ma.body.T_LandmarksTranslator).refcount(),3)
    trials = ma.io.read(dynamic_filename)
    models = ma.body.reconstruct(skeletonhelper, trials)
    jointkinematics = ma.body.extract_joint_kinematics(models)
    self.assertEqual(trials.child(0).refcount(),3)
    self.assertEqual(models.findChild(ma.T_Trial).refcount(),3)
    self.assertEqual(models.findChild(ma.body.T_LandmarksTranslator).refcount(),3)