#ifndef lyonwholebodymodelTest_def_h
#define lyonwholebodymodelTest_def_h

#include <openma/body.h>
#include <openma/base.h>
#include <openma/io.h>
#include <openma/math.h>

void set_isb_custom_landmarks_translator(ma::body::LyonWholeBodyModel* helper)
{
  new ma::body::LandmarksTranslator("LandmarksTranslator",{
    {"C7"          , "C7"},
    {"T8"          , "T8"},
    {"IJ"          , "SS"},
    {"PX"          , "XP"},
    {"L.Acromion"  , "L.AC"},
    {"L.Biceps"    , "L.GH"},
    {"L.Epi.Lat"   , "L.LHE"},
    {"L.Epi.Med"   , "L.MHE"},
    {"L.Wrist.Ulna", "L.US"},
    {"L.Wrist.Rad" , "L.RS"},
    {"L.D5M1"      , "L.MH5"},
    {"L.D2M1"      , "L.MH2"},
    {"R.Acromion"  , "R.AC"},
    {"R.Biceps"    , "R.GH"},
    {"R.Epi.Lat"   , "R.LHE"},
    {"R.Epi.Med"   , "R.MHE"},
    {"R.Wrist.Ulna", "R.US"},
    {"R.Wrist.Rad" , "R.RS"},
    {"R.D5M1"      , "R.MH5"},
    {"R.D2M1"      , "R.MH2"},
  }, helper);
};

void set_isb_custom_landmarks_translator2(ma::body::LyonWholeBodyModel* helper)
{
  new ma::body::LandmarksTranslator("LandmarksTranslator",{
    {"C7"         , "C7"},
    {"T8"         , "T8"},
    {"IJ"         , "SS"},
    {"PX"         , "XP"},
    {"L.Scapula"  , "L.AC"},
    {"L.HeadHum"  , "L.GH"},
    {"L.Epi.Lat"  , "L.LHE"},
    {"L.Epi.Med"  , "L.MHE"},
    {"L.Wrist.Uln", "L.US"},
    {"L.Wrist.Rad", "L.RS"},
    {"L.Hand5"    , "L.MH5"},
    {"L.Hand2"    , "L.MH2"},
    {"R.Scapula"  , "R.AC"},
    {"R.HeadHum"  , "R.GH"},
    {"R.Epi.Lat"  , "R.LHE"},
    {"R.Epi.Med"  , "R.MHE"},
    {"R.Wrist.Uln", "R.US"},
    {"R.Wrist.Rad", "R.RS"},
    {"R.Hand5"    , "R.MH5"},
    {"R.Hand2"    , "R.MH2"},
    {"R.EIAS"     , "R.ASIS"},
    {"LEIAS"      , "L.ASIS"},
    {"Sacral"     , "SC"},
    {"R.GT"       , "R.GT"},
    {"R.Knee.Lat" , "R.LFE"},
    {"R.Knee.Med" , "R.MFE"},
    {"R.Fibula"   , "R.FH"},
    {"R.Ankle.Lat", "R.LTM"},
    {"R.Ankle.Med", "R.MTM"},
    {"R.Calca"    , "R.HEE"},
    {"R.Foot.Lat" , "R.MTH5"},
    {"R.Foot.Med" , "R.MTH1"},
    {"L.GT"       , "L.GT"},
    {"L.Knee.Lat" , "L.LFE"},
    {"L.Knee.Med" , "L.MFE"},
    {"L.Fibula"   , "L.FH"},
    {"L.Ankle.Lat", "L.LTM"},
    {"L.Ankle.Med", "L.MTM"},
    {"L.Calca"    , "L.HEE"},
    {"L.Foot.Lat" , "L.MTH5"},
    {"L.Foot.Med" , "L.MTH1"},
  }, helper);
};

void compare_segment_relscs(ma::body::SkeletonHelper* helper, const std::string& frame, const double* refdata)
{
  auto segment_relscs_node = helper->findChild<const ma::body::ReferenceFrame*>(frame);
  TSM_ASSERT_DIFFERS(frame.c_str(), segment_relscs_node, nullptr);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->u()[0], refdata[ 0], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->u()[1], refdata[ 1], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->u()[2], refdata[ 2], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->v()[0], refdata[ 3], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->v()[1], refdata[ 4], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->v()[2], refdata[ 5], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->w()[0], refdata[ 6], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->w()[1], refdata[ 7], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->w()[2], refdata[ 8], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->o()[0], refdata[ 9], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->o()[1], refdata[10], 1e-5);
  TSM_ASSERT_DELTA(frame.c_str(), segment_relscs_node->o()[2], refdata[11], 1e-5);
};

void compare_segment_motion(ma::body::Model* model, const std::string& frame, int idx, const double* refdata, bool rotcmp = true, bool poscmp = true)
{
  const auto cptpose = ma::math::to_pose(model->segments()->findChild<const ma::TimeSequence*>(frame));
  if (rotcmp)
  {
    for (unsigned i = 0 ; i < 9 ; ++i)
    {
      auto s = frame + "(" + std::to_string(i) + ")";
      TSM_ASSERT_DELTA(s.c_str(), cptpose.values()(idx,i), refdata[i], 1e-5);
    }
  }
  if (poscmp)
  {
    for (unsigned i = 9 ; i < 12 ; ++i)
    {
      auto s = frame + "(" + std::to_string(i) + ")";
      TSM_ASSERT_DELTA(s.c_str(), cptpose.values()(idx,i), refdata[i], 1e-5);
    }
  }
};

void compare_joint_kinematics(ma::Node* kinematics, const std::string& name, int idx, const double* refdata)
{
  auto angle = kinematics->findChild<ma::TimeSequence*>(name,{{"type",ma::TimeSequence::Angle}},false);
  TSM_ASSERT_DIFFERS(name.c_str(), angle, nullptr);
  auto num = angle->samples();
  auto data = angle->data();
  TSM_ASSERT_DELTA(name.c_str(), data[0*num+idx], refdata[0], 1e-5);
  TSM_ASSERT_DELTA(name.c_str(), data[1*num+idx], refdata[1], 1e-5);
  TSM_ASSERT_DELTA(name.c_str(), data[2*num+idx], refdata[2], 1e-5);
};

#endif // lyonwholebodymodelTest_def_h