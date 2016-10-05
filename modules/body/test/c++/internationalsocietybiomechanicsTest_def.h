#ifndef internationalsocietybiomechanicsTest_def_h
#define internationalsocietybiomechanicsTest_def_h

#include <openma/body/internationalsocietybiomechanics.h>
#include <openma/body/enums.h>
#include <openma/body/landmarkstranslator.h>
#include <openma/body/model.h>
#include <openma/base/timesequence.h>
#include <openma/base/trial.h>

#include <openma/io/handlerreader.h>
#include <openma/io/file.h>
#include <openma/math.h>

#include <cassert>

void set_isb_custom_landmarks_translator(ma::body::InternationalSocietyBiomechanics* helper)
{
  new ma::body::LandmarksTranslator("LandmarksTranslator",{
    {"C7"              , "C7"},
    {"T8"              , "T8"},
    {"IJ"              , "SS"},
    {"PX"              , "XP"},
    {"L.Acromion"      , "L.AC"},
    {"L.AngulusAcromio", "L.AA"},
    {"L.Biceps"        , "L.GH"},
    {"L.Epi.Lat"       , "L.LHE"},
    {"L.Epi.Med"       , "L.MHE"},
    {"L.Wrist.Ulna"    , "L.US"},
    {"L.Wrist.Rad"     , "L.RS"},
    {"L.D5M1"          , "L.MH5"},
    {"L.D2M1"          , "L.MH2"},
    {"R.Acromion"      , "R.AC"},
    {"R.AngulusAcromio", "R.AA"},
    {"R.Biceps"        , "R.GH"},
    {"R.Epi.Lat"       , "R.LHE"},
    {"R.Epi.Med"       , "R.MHE"},
    {"R.Wrist.Ulna"    , "R.US"},
    {"R.Wrist.Rad"     , "R.RS"},
    {"R.D5M1"          , "R.MH5"},
    {"R.D2M1"          , "R.MH2"},
  }, helper);
};

void compare_segment_motion(ma::body::Model* model, const std::string& frame, int idx, const double* refdata)
{
  const auto cptpose = ma::math::to_pose(model->segments()->findChild<const ma::TimeSequence*>(frame));
  for (unsigned i = 0 ; i < 9 ; ++i)
    TS_ASSERT_DELTA(cptpose.values()(idx,i), refdata[i], 1e-7);
  for (unsigned i = 9 ; i < 12 ; ++i)
    TS_ASSERT_DELTA(cptpose.values()(idx,i), refdata[i], 5e-6);
};

#endif // internationalsocietybiomechanicsTest_def_h