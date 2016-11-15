#ifndef registermarkerclusterTest_def_h
#define registermarkerclusterTest_def_h

#include <openma/body.h>

// Util functions

inline void set_pt_data(ma::TimeSequence* ts, double x, double y, double z)
{
  ts->data()[0] = x;
  ts->data()[1] = y;
  ts->data()[2] = z;
  ts->data()[3] = 0.;
};

inline void set_pose_data_fake(const std::string& name, ma::Node* parent)
{
  auto pose = new ma::TimeSequence(name,13,1,100.0,0.0,ma::TimeSequence::Pose,"",parent);
  pose->data()[0] = 1.0; pose->data()[1] = 0.0; pose->data()[2] = 0.0;
  pose->data()[3] = 0.0; pose->data()[4] = 1.0; pose->data()[5] = 0.0;
  pose->data()[6] = 0.0; pose->data()[7] = 0.0; pose->data()[8] = 1.0;
  pose->data()[9] = 0.0; pose->data()[10] = 0.0; pose->data()[11] = 0.0;
  pose->data()[12] = 0.0;
};

inline void compare_pt_data(ma::Node* mcr, const std::string& name, double x, double y, double z)
{
  auto pt = mcr->findChild<const ma::body::Point*>(name,{},false);
  TSM_ASSERT_DIFFERS(name, pt, nullptr);
  TSM_ASSERT_DELTA(name, pt->data()[0], x, 1e-12);
  TSM_ASSERT_DELTA(name, pt->data()[1], y, 1e-12);
  TSM_ASSERT_DELTA(name, pt->data()[2], z, 1e-12);
};

inline void compare_pose_data(ma::Node* mcr, const std::string& name, const std::array<double,12>& ref)
{
  auto pose = mcr->findChild<const ma::body::ReferenceFrame*>(name,{},false);
  TSM_ASSERT_DIFFERS(name, pose, nullptr);
  TSM_ASSERT_DELTA(name, pose->data()[0], ref[0], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[1], ref[1], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[2], ref[2], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[3], ref[3], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[4], ref[4], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[5], ref[5], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[6], ref[6], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[7], ref[7], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[8], ref[8], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[9], ref[9], 1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[10],ref[10],1e-12);
  TSM_ASSERT_DELTA(name, pose->data()[11],ref[11],1e-12);
};

// Cube test

class CubeHelper : public ma::body::SkeletonHelper
{
public:
  CubeHelper() : ma::body::SkeletonHelper("CubeHelper") {};
  virtual bool calibrate(ma::Node* /*trials*/, ma::Subject* /*subject*/) override {return true;};
  virtual ma::body::LandmarksTranslator* defaultLandmarksTranslator() override {return nullptr;};
  virtual ma::body::InertialParametersEstimator* defaultInertialParametersEstimator() override {return nullptr;};
  virtual ma::body::ExternalWrenchAssigner* defaultExternalWrenchAssigner() override {return nullptr;};
  virtual ma::body::InverseDynamicProcessor* defaultInverseDynamicProcessor() override {return nullptr;};
protected:
  virtual bool setupModel(ma::body::Model* model) const override;
  virtual bool reconstructModel(ma::body::Model* model, ma::Trial* trial) override;
};

bool CubeHelper::setupModel(ma::body::Model* model) const
{
  auto cube = new ma::body::Segment("Cube",0,0,model->segments());
  new ma::body::LandmarksRegistrar("Cube.Cluster", {"uname*1","uname*2","uname*3","uname*4","uname*5","uname*6","uname*7","uname*8"}, cube);
  return true;
};

bool CubeHelper::reconstructModel(ma::body::Model* model, ma::Trial* /*trial*/)
{
  set_pose_data_fake("Cube.SCS",model->segments()->findChild<ma::body::Segment*>("Cube"));
  return true;
};

// Lower limbs test

class LowerLimbsHelper : public ma::body::SkeletonHelper
{
public:
  LowerLimbsHelper() : ma::body::SkeletonHelper("LowerLimbsHelper") {};
  virtual bool calibrate(ma::Node* /*trials*/, ma::Subject* /*subject*/) override {return true;};
  virtual ma::body::LandmarksTranslator* defaultLandmarksTranslator() override {return nullptr;};
  virtual ma::body::InertialParametersEstimator* defaultInertialParametersEstimator() override {return nullptr;};
  virtual ma::body::ExternalWrenchAssigner* defaultExternalWrenchAssigner() override {return nullptr;};
  virtual ma::body::InverseDynamicProcessor* defaultInverseDynamicProcessor() override {return nullptr;};
protected:
  virtual bool setupModel(ma::body::Model* model) const override;
  virtual bool reconstructModel(ma::body::Model* model, ma::Trial* trial) override;
};

bool LowerLimbsHelper::setupModel(ma::body::Model* model) const
{
  ma::body::Segment* seg = nullptr;
  seg = new ma::body::Segment("L.Thigh",ma::body::Part::Thigh,ma::body::Side::Left,model->segments());
  new ma::body::LandmarksRegistrar(seg->name()+".Cluster", {"LF01","LF02","LF03","LF04","LF05"}, seg);
  seg = new ma::body::Segment("R.Thigh",ma::body::Part::Thigh,ma::body::Side::Right,model->segments());
  new ma::body::LandmarksRegistrar(seg->name()+".Cluster", {"RF01","RF02","RF03","RF04","RF05"}, seg);
  seg = new ma::body::Segment("L.Shank",ma::body::Part::Shank,ma::body::Side::Left,model->segments());
  new ma::body::LandmarksRegistrar(seg->name()+".Cluster", {"LT01","LT02","LT03","LT04","LT05"}, seg);
  seg = new ma::body::Segment("R.Shank",ma::body::Part::Shank,ma::body::Side::Right,model->segments());
  new ma::body::LandmarksRegistrar(seg->name()+".Cluster", {"RT01","RT02","RT03","RT04","RT05"}, seg);
  return true;
};

bool LowerLimbsHelper::reconstructModel(ma::body::Model* model, ma::Trial* /*trial*/)
{
  set_pose_data_fake("L.Thigh.SCS",model->segments()->findChild<ma::body::Segment*>("L.Thigh"));
  set_pose_data_fake("R.Thigh.SCS",model->segments()->findChild<ma::body::Segment*>("R.Thigh"));
  set_pose_data_fake("L.Shank.SCS",model->segments()->findChild<ma::body::Segment*>("L.Shank"));
  set_pose_data_fake("R.Shank.SCS",model->segments()->findChild<ma::body::Segment*>("R.Shank"));
  return true;
};

#endif // registermarkerclusterTest_def_h