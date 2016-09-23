#include <cxxtest/TestDrive.h>

#define _TEST_BODY_USE_IO

#include "plugingaitTest_def.h"
#include "test_file_path.h"

#include <openma/body.h>

#include <openma/body/externalwrenchassigner.h>
#include <openma/body/inertialparametersestimator.h>
#include <openma/body/inversedynamicsmatrix.h>
#include <openma/body/joint.h>
#include <openma/body/chain.h>
#include <openma/body/segment.h>
#include <openma/base/logger.h>

#include <openma/instrument/forceplate.h>
#include <openma/instrument/enums.h>
#include <openma/io.h>

CXXTEST_SUITE(PluginGaitKineticsTest)
{
  CXXTEST_TEST(inverseDynamicsBothLowerBodyOneFrame)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setGravity(std::array<double,3>{{0,0,-9.81}}); // m/s^2
    helper.setMarkerDiameter(16.0); // mm
    helper.setLeftFootFlatEnabled(true);
    helper.setLeftLegLength(940.0); // mm
    helper.setLeftKneeWidth(110.0); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightFootFlatEnabled(true);
    helper.setRightLegLength(940.0); // mm
    helper.setRightKneeWidth(120.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    double mass = 66.5; // kg
    double height = 1790; // mm
    helper.setProperty("mass", mass);
    helper.setProperty("height", height);
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel"), kineticsanalyses("kineticsanalyses");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration-FlatFoot-One.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion-FlatFoot-One.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinetics(&kineticsanalyses, &rootModel), true);
    TS_ASSERT_EQUALS(kineticsanalyses.children().size(), 1u);
    
    auto trial = rootDynamic.child<ma::Trial*>(0);
    auto kinetic = kineticsanalyses.child(0);
    
    // Specific to the C3D file format that accepts only one unit by type
    for (const auto& c : kinetic->findChildren<ma::TimeSequence*>())
    {
      c->addParent(trial->timeSequences());
      if (c->type() == ma::TimeSequence::Force)
        c->setUnit("N");
      else if (c->type() == ma::TimeSequence::Moment)
        c->setUnit("Nmm");
      else if (c->type() == ma::TimeSequence::Power)
        c->setUnit("W");
    }
    ma::io::write(&rootDynamic, "test.c3d");
    
    /*
    
    Set the gravity
    const double g[3] = {0.,0.,-9810.0};
    auto model = rootModel.child<ma::body::Model*>(0);
    model->setGravity(g);
    double mass = 66.5; // kg
    double height = 1790; // mm
    model->setProperty("weight", mass * 9.81);
    model->setProperty("height", height);
    
    // Associate FP wrench to feet
    auto ewa = helper.defaultExternalWrenchAssigner();
    ewa->run(&rootModel);
    // Compute BSIPs
    auto ipe = helper.defaultInertialParametersEstimator();
    ipe->generate(&rootModel);
    // Compute inverse dynamics in the global frame
    auto idp = helper.defaultInverseDynamicProcessor();
    idp->run(&rootModel);
    // Transform joint forces and moments in the distal segmental frame
    auto models = rootModel.findChildren<ma::body::Model*>({},{},false);
    for (auto& model : models)
    {
      auto chains = model->chains()->findChildren<ma::body::Chain*>({},{},false);
      for (auto& chain : chains)
      {
        auto joints = chain->findChildren<ma::body::Joint*>({},{},false);
        for (auto& joint : joints)
        {
          auto seg = joint->distalSegment();
          auto pose = seg->findChild<ma::TimeSequence*>(seg->name() + ".SCS");
          auto force = joint->findChild<ma::TimeSequence*>({},{{"type",ma::TimeSequence::Force}},false);
          auto moment = joint->findChild<ma::TimeSequence*>({},{{"type",ma::TimeSequence::Moment}},false);
          if ((pose == nullptr) || (force == nullptr) || (moment == nullptr))
          {
            ma::error("Missing force/moment/pose");
            continue;
          }
          ma::math::Pose P = ma::math::to_pose(pose).inverse();
          ma::math::Vector F = P.block<9>(0).transform(ma::math::to_vector(force));
          ma::math::Vector M = P.block<9>(0).transform(ma::math::to_vector(moment));
          ma::math::to_timesequence(F, joint->name() + ".Force.Local.Distal", force->sampleRate(), force->startTime(), ma::TimeSequence::Force, "N", joint);
          ma::math::to_timesequence(M, joint->name() + ".Moment.Local.Distal", moment->sampleRate(), moment->startTime(), ma::TimeSequence::Moment, "Nmm", joint);
        }
      }
#if 1
    }
#else      
      std::cout << std::endl;
      std::string str;

      str = "R.Ankle.Force.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() / mass << std::endl;
      str = "RAnkleForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "R.Ankle.Moment.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() / mass << std::endl;
      str = "RAnkleMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;

      str = "R.Knee.Force.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() / mass << std::endl;
      str = "RKneeForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "R.Knee.Moment.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() / mass << std::endl;
      str = "RKneeMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;

      str = "R.Hip.Force.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() / mass << std::endl;
      str = "RHipForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "R.Hip.Moment.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() / mass << std::endl;
      str = "RHipMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;

      auto fps = rootDynamic.findChildren<ma::instrument::ForcePlate*>();
      for (const auto& fp : fps)
        fp->wrench(ma::instrument::Location::SurfaceOrigin, true, 10.0, 100.0);

      str = "FP1.Wrench.Global.SO";
      std::cout << str << "\n" << ma::math::to_wrench(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Wrench}})).block<3>(0).values() / mass << std::endl;
      str = "RGroundReactionForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "FP1.Wrench.Global.SO";
      std::cout << str << "\n" << ma::math::to_wrench(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Wrench}})).block<3>(3).values() / mass << std::endl;
      str = "RGroundReactionMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;
    }
    
    const std::string str = "FP1.Wrench.Global.COP";
    auto cop = model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Wrench}});
    ma::math::to_timesequence(ma::math::to_vector(cop,6),str,cop->sampleRate(),cop->startTime(),ma::TimeSequence::Marker,"mm",rootDynamic.child<ma::Trial*>(0)->timeSequences());
    ma::io::write(&rootDynamic,"/Users/alzathar/Downloads/test.c3d");
#endif
    */
  };
  
  CXXTEST_TEST(inverseDynamicsBothLowerBodyFullFramesHeadOffsetDisabled)
  {
    ma::body::PluginGait helper(ma::body::Region::Lower, ma::body::Side::Both, ma::body::PluginGait::Basic);
    helper.setGravity(std::array<double,3>{{0,0,-9.81}}); // m/s^2
    helper.setMarkerDiameter(14.0); // mm
    helper.setLeftLegLength(780.0); // mm
    helper.setLeftKneeWidth(90); // mm
    helper.setLeftAnkleWidth(70.0); // mm
    helper.setRightLegLength(780.0); // mm
    helper.setRightKneeWidth(95.0); // mm
    helper.setRightAnkleWidth(70.0); // mm
    helper.setProperty("mass",33.0); // kg
    helper.setProperty("height", 1465.0); // mm
    
    ma::Node rootCalibration("rootCalibration"), rootDynamic("rootDynamic"), rootModel("rootModel"), kineticsanalyses("kineticsanalyses");
    generate_trial_from_file(&rootCalibration, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Calibration4.c3d"));
    TS_ASSERT(helper.calibrate(&rootCalibration, nullptr));
    generate_trial_from_file(&rootDynamic, OPENMA_TDD_PATH_IN("c3d/plugingait/PiG_Motion4_noFF_noHO.c3d"));
    TS_ASSERT(helper.reconstruct(&rootModel, &rootDynamic));
    TS_ASSERT_EQUALS(ma::body::extract_joint_kinetics(&kineticsanalyses, &rootModel), true);
    TS_ASSERT_EQUALS(kineticsanalyses.children().size(), 1u);
    
    auto trial = rootDynamic.child<ma::Trial*>(0);
    auto kinetic = kineticsanalyses.child(0);
    
    for (const auto& c : kinetic->findChildren<ma::TimeSequence*>())
    {
      // Specific to the C3D file format that accepts only one unit by type
      c->addParent(trial->timeSequences());
      if (c->type() == ma::TimeSequence::Force)
        c->setUnit("N");
      else if (c->type() == ma::TimeSequence::Moment)
        c->setUnit("Nmm");
      else if (c->type() == ma::TimeSequence::Power)
        c->setUnit("W");
    }
    ma::io::write(&rootDynamic, "test2.c3d");
    
    /*
    // Set the gravity
    // const double g[3] = {0.,0.,-9810.0};
    // auto model = rootModel.child<ma::body::Model*>(0);
    // model->setGravity(g);
    // double mass = 33; // kg
    // double height = 1465; // mm
    // model->setProperty("weight", mass * 9.81);
    // model->setProperty("height", height);
    
    // Associate FP wrench to feet
    auto ewa = helper.defaultExternalWrenchAssigner();
    ewa->run(&rootModel);
    // Compute BSIPs
    auto ipe = helper.defaultInertialParametersEstimator();
    ipe->generate(&rootModel);
    // Compute inverse dynamics in the global frame
    auto idp = helper.defaultInverseDynamicProcessor();
    idp->run(&rootModel);
    // Transform joint forces and moments in the distal segmental frame
    auto models = rootModel.findChildren<ma::body::Model*>({},{},false);
    for (auto& model : models)
    {
      auto chains = model->chains()->findChildren<ma::body::Chain*>({},{},false);
      for (auto& chain : chains)
      {
        auto joints = chain->findChildren<ma::body::Joint*>({},{},false);
        for (auto& joint : joints)
        {
          auto seg = joint->distalSegment();
          auto pose = seg->findChild<ma::TimeSequence*>(seg->name() + ".SCS");
          auto force = joint->findChild<ma::TimeSequence*>({},{{"type",ma::TimeSequence::Force}},false);
          auto moment = joint->findChild<ma::TimeSequence*>({},{{"type",ma::TimeSequence::Moment}},false);
          if ((pose == nullptr) || (force == nullptr) || (moment == nullptr))
          {
            ma::error("Missing force/moment/pose");
            continue;
          }
          ma::math::Pose P = ma::math::to_pose(pose).inverse();
          ma::math::Vector F = P.block<9>(0).transform(ma::math::to_vector(force));
          ma::math::Vector M = P.block<9>(0).transform(ma::math::to_vector(moment));
          ma::math::to_timesequence(F, joint->name() + ".Force.Local.Distal", force->sampleRate(), force->startTime(), ma::TimeSequence::Force, "N", joint);
          ma::math::to_timesequence(M, joint->name() + ".Moment.Local.Distal", moment->sampleRate(), moment->startTime(), ma::TimeSequence::Moment, "Nmm", joint);
        }
      }
#if 1
    }
#else
      std::cout << std::endl;
      std::string str;

      str = "R.Ankle.Force.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() / mass << std::endl;
      str = "RViconAnkleForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "R.Ankle.Moment.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() / mass << std::endl;
      str = "RViconAnkleMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;

      str = "R.Knee.Force.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() / mass << std::endl;
      str = "RViconKneeForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "R.Knee.Moment.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() / mass << std::endl;
      str = "RViconKneeMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;

      str = "R.Hip.Force.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() / mass << std::endl;
      str = "RViconHipForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "R.Hip.Moment.Local.Distal";
      std::cout << str << "\n" << ma::math::to_vector(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() / mass << std::endl;
      str = "RViconHipMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;

      auto fps = rootDynamic.findChildren<ma::instrument::ForcePlate*>();
      for (const auto& fp : fps)
        fp->wrench(ma::instrument::Location::SurfaceOrigin, true, 10.0, 100.0);

      str = "FP1.Wrench.Global.SO";
      std::cout << str << "\n" << ma::math::to_wrench(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Wrench}})).block<3>(0).values() / mass << std::endl;
      str = "RViconGroundReactionForce";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Force}})).values() << std::endl;
      str = "FP1.Wrench.Global.SO";
      std::cout << str << "\n" << ma::math::to_wrench(model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Wrench}})).block<3>(3).values() / mass << std::endl;
      str = "RViconGroundReactionMoment";
      std::cout << str << "\n" << ma::math::to_vector(rootDynamic.findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Moment}})).values() << std::endl;
    }
    
    const std::string str = "FP1.Wrench.Global.COP";
    auto cop = model->findChild<ma::TimeSequence*>(str,{{"type",ma::TimeSequence::Wrench}});
    ma::math::to_timesequence(ma::math::to_vector(cop,6),str,cop->sampleRate(),cop->startTime(),ma::TimeSequence::Marker,"mm",rootDynamic.child<ma::Trial*>(0)->timeSequences());
    ma::io::write(&rootDynamic,"/Users/alzathar/Downloads/test2.c3d");
#endif
    */
  };
};

CXXTEST_SUITE_REGISTRATION(PluginGaitKineticsTest)
CXXTEST_TEST_REGISTRATION(PluginGaitKineticsTest, inverseDynamicsBothLowerBodyOneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitKineticsTest, inverseDynamicsBothLowerBodyFullFramesHeadOffsetDisabled)