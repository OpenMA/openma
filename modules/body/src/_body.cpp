/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solution Inc., all rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "openma/body.h"

#include "openma/base/node.h"
#include "openma/base/subject.h"
#include "openma/base/logger.h"

#include "openma/base/logger.h"
#include "openma/base/node.h"
#include "openma/base/subject.h"
#include "openma/base/trial.h"

#include "openma/math.h"
#include <Eigen/SVD> // Eigen::JacobiSVD

namespace ma
{
namespace body
{
  /**
   * Convenient function to calibrate the @a helper from the @a trials and the @a subject data.
   * The number of required trials, the parameters required in the subject and the way to calibrate the helper is specific to it.
   * @relates SkeletonHelper
   * @ingroup openma_body
   */
  bool calibrate(SkeletonHelper* helper, Node* trials, Subject* subject)
  {
    if (helper == nullptr)
    {
      error("A null pointer to a SkeletonHelper object was passed. Calibration aborted.");
      return false;
    }
    return helper->calibrate(trials, subject);
  };
  
  /**
   * Convenient function to compute and register markers local coordinates in the TCS as well as to compute the segments TCS -> SCS transformation
   */
  bool register_marker_cluster(SkeletonHelper* helper, Node* trials)
  {
    if (helper == nullptr)
    {
      error("A null pointer to a SkeletonHelper object was passed. Registration aborted.");
      return false;
    }
    auto trial = trials->findChildren<Trial*>();
    if (trial.size() > 1)
    {
      error("The registration process supports only the use of one trial. Registration aborted.");
      return false;
    }
    Node tmrfsr("tmrfsr"); // Temporary Models Root for Segments Reconstruction
    Node ttrfsr("ttrfsr"); // Temporary Trials Root for Segments Reconstruction
    Trial ttfsr("ttfsr",&ttrfsr); // Temporary Trial for Segments Reconstruction
    auto markers = trial[0]->timeSequences()->findChildren<TimeSequence*>({},{{"type",TimeSequence::Marker}});
    // Average markers along the time
    for (const auto& m : markers)
      average_marker(m, ttfsr.timeSequences());
    // TODO DISABLE INVERSE DYNAMICS COMPUTATION
    // Delete the existing pose estimators (if any) to use the default one
    auto estimators = helper->findChildren<PoseEstimator*>({},{},false);
    for (auto e : estimators)
    {
      e->removeParent(helper);
      if (!e->hasParents())
        delete e;
    }
    // Reconstruct segments' SCS using the default pose estimator
    auto defaultEstimator = helper->defaultPoseEstimator();
    bool reconstructed = helper->reconstruct(&tmrfsr, &ttrfsr);
    delete defaultEstimator;
    if (!reconstructed || (tmrfsr.children().empty()))
    {
      error("An error occurred during the reconstruction of segments SCS used by the registration process. Registration aborted.");
      return false;
    }
    // Compute segments' TCS
    const auto& model = node_cast<Model*>(tmrfsr.child(0));
    if (model == nullptr)
    {
      error("Internal error, the first child is not a model! Registration aborted.");
      return false;
    }
    const auto& lt = helper->findChild<LandmarksTranslator*>({},{},false);
    auto mcr = helper->findChild("MarkerClusterRegistration",{},false);
    if (mcr == nullptr)
      mcr = new Node("MarkerClusterRegistration", helper);
    const auto& segments = model->segments()->findChildren<Segment*>({},{},false);
    for (const auto& segment : segments)
    {
      const auto& lr = segment->findChild<LandmarksRegistrar*>({},{},false);
      if (lr == nullptr)
        continue;
      auto markers = extract_landmark_positions(nullptr,lr->retrieveLandmarks(lt, ttfsr.timeSequences()));
      auto it = markers.begin();
      while (it != markers.end())
      {
        if (!it->second.isValid())
          it = markers.erase(it);
        else
          ++it;
      }
      if (markers.size() < 3)
      {
        error("Less than 3 valid markers was found for the segment '%s'. Impossible to compute the TCS. Registration aborted.", segment->name().c_str());
        return false;
      }
      auto segpose = segment->pose();
      const auto& scs = math::to_pose(segpose);
      if (!scs.isValid())
      {
        error("No SCS computed for the segment '%s'. Impossible to compute the TCS. Registration aborted.", segment->name().c_str());
        return false;
      }
      // Prepare data
      using VecNx3 = Eigen::Matrix<double,Eigen::Dynamic,3>;
      using Vec1x3 = Eigen::Matrix<double,1,3>;
      using Mat3x3 = Eigen::Matrix<double,3,3>;
      VecNx3 pts(markers.size(),3);
      int inc = 0;
      for (const auto& marker : markers)
        pts.row(inc++) = marker.second.values();
      // Compute the centroid
      Vec1x3 CoM = pts.colwise().mean();
      // Express marker' position relative to the CoM
      pts.rowwise() -= CoM;
      // Compute the tensor of inertia associated with the cluster of markers
      Mat3x3 temp = pts.transpose() * pts;
      Mat3x3 I; I << temp.coeff(1,1) + temp.coeff(2,2), -temp.coeff(0,1),                   -temp.coeff(0,2),
                    -temp.coeff(0,1),                    temp.coeff(0,0) + temp.coeff(2,2), -temp.coeff(1,2),
                    -temp.coeff(0,2),                   -temp.coeff(1,2),                    temp.coeff(0,0) + temp.coeff(1,1);
      // Compute the principal axes
      Mat3x3 U = I.jacobiSvd(Eigen::ComputeFullU).matrixU();
      // Express the SCS in the TCS and register the result
      const double res = 0.;
      math::Map<const math::Position> c(1,CoM.data(),&res);
      math::Map<const math::Vector> u(1,U.data(),&res);
      math::Map<const math::Vector> v(1,U.data()+3,&res);
      math::Pose tcsi = math::Pose(u,v,u.cross(v),c).inverse();
      math::Pose T = tcsi.transform(scs);
      std::string relscslabel = segpose->name();
      auto relscs = mcr->findChild<ReferenceFrame*>(relscslabel,{},false);
      if (relscs == nullptr)
        relscs = new ReferenceFrame(relscslabel,nullptr,mcr);
      std::copy_n(T.values().data(),12,relscs->data());
      // Express the markers in the SCS and register the result
      for (const auto& marker : markers)
      {
        math::Position p = tcsi.transform(marker.second);
        std::string relptlabel = segment->name() + "." + marker.first;
        auto relpt = mcr->findChild<Point*>(relptlabel,{},false);
        if (relpt == nullptr)
          relpt = new Point(relptlabel,nullptr,mcr);
        std::copy_n(p.values().data(),3,relpt->data());
      }
    }
    // Attach a least square pose estimator
    new UnitQuaternionPoseEstimator("LeastSquarePoseEstimator",helper);
    return true;
  };
  
  /**
   * Convenient function to create Model nodes and reconstrust their movement based on the @a helper and the @a trials.
   * For each trial, a model is created and added to the @a output.
   * @note The @a helper might need to be calibrated before the use of the reconstruct() function.
   */
  bool reconstruct(Node* root, SkeletonHelper* helper, Node* trials)
  {
    if (helper == nullptr)
    {
      error("A null pointer to a SkeletonHelper object was passed. Reconstruction aborted.");
      return false;
    }
    return helper->reconstruct(root, trials);
  };
  
  /**
   * Similar to the other reconstruct method but the models are added to the returned node.
   * @warning The returned node is allocated on the heap. The developer must care of the deletion of the object using the @c delete keyword.
   */
  Node* reconstruct(SkeletonHelper* helper, Node* trials)
  {
    Node* root = new Node("root");
    if (!reconstruct(root, helper, trials))
    {
      delete root;
      root = nullptr;
    }
    return root;
  };
  
  /**
   * Convenient function to extract kinematics described by EulerDescriptor objects found in Model objects passed as the children of the @a input.
   * Internally, a node is added to the @a output. TimeSequence objects representing joint kinematics are added to this node.
   * Several options are passed to each descriptor found:
   *  - suffixProximal is set to ".SCS"
   *  - suffixDistal is set to ".SCS"
   *  - enableDegreeConversion is enabled
   *  - adaptForInterpretation is set to the value passed in @a sideAdaptation
   * The use of these options means that computed Euler angles are expressed in degrees. The TimeSequence associated with each Segment will use both the corresponding segments' name concatenated with the suffix ".SCS". They are adapted or not depending of the value in @a sideAdaptation.
   * The adaptation of the angles depends of the setting of each descriptor. You should refer to the model's definition (or helper) to know the descriptor used. For example with the PluginGait helper, the adaptation is used to be able to compare directly the left/right sides.
   */
  bool extract_joint_kinematics(Node* output, Node* input, bool sideAdaptation)
  {
    if (output == nullptr)
    {
      error("Invalid null output node. Joint kinematics computation aborted.");
      return false;
    }
    auto models = input->findChildren<Model*>({},{},false);
    std::unordered_map<std::string, Any> options{
      {"suffixProximal", ".SCS"},
      {"suffixDistal", ".SCS"},
      {"enableDegreeConversion", true},
      {"adaptForInterpretation", sideAdaptation}
    };
    for (auto& model: models)
    {
      auto analysis = new Node(model->name() + "_JointKinematics", output);
      auto joints = model->joints()->findChildren<Joint*>({},{},false);
      for (auto& joint: joints)
      {
        auto descriptors = joint->findChildren<EulerDescriptor*>({},{},false);
        for (auto& descriptor: descriptors)
        {
          descriptor->evaluate(analysis, joint, options);
        }
      }
    }
    return true;
  };
  
  /**
   * Similar to the other extract_joint_kinematics() method but the computed joint kinematics are added to a returned node.
   * @warning The returned node is allocated on the heap. The developer must care of the deletion of the object using the @c delete keyword.
   */
  Node* extract_joint_kinematics(Node* input, bool sideAdaptation)
  {
    Node* root = new Node("root");
    if (!extract_joint_kinematics(root, input, sideAdaptation))
    {
      delete root;
      root = nullptr;
    }
    return root;
  };
  
  /**
   * Convenient function to extract joint kinetics described by DynamicDescriptor objects found in Model objects passed as the children of the @a input.
   * Internally, a node is added to the @a output. TimeSequence objects representing joint kinetics (force, moment, and power) are added to this node.
   * Several options are passed to each descriptor found:
   *  - adaptForInterpretation is set to the value passed in @a sideAdaptation
   *  - massNormalization is set to model's mass property if @a massNormalization is set to true
   *  - representationFrame is set to the value passed in @a frame
   *  - representationFrameSuffix is set to ".SCS"
   * The default values passed to these options means that computed joint forces and moment are expressed int the reference frame associated with the distal segment and normalized by subject's mass. They are also adapted to interpret values for the left and right side.
   */
  bool extract_joint_kinetics(Node* output, Node* input, bool sideAdaptation, bool massNormalization, RepresentationFrame frame)
  {
    if (output == nullptr)
    {
      error("Invalid null output node. Joint kinetics computation aborted.");
      return false;
    }
    auto models = input->findChildren<Model*>({},{},false);
    std::unordered_map<std::string, Any> options{
      {"representationFrame", frame},
      {"representationFrameSuffix", ".SCS"},
      {"adaptForInterpretation", sideAdaptation}
    };
    for (auto& model: models)
    {
      if (massNormalization)
      {
        auto massProp = model->property("mass");
        auto massVal = massProp.cast<double>();
        if (massProp.isValid() && (massVal > 0.) && !std::isnan(massVal))
          options["massNormalization"] = model->property("mass");
        else
          warning("The model '%s' has no property 'mass' or is set to a non positive value or a not a number (NaN). It is not possible to normalize the data.", model->name().c_str());
      }
      auto analysis = new Node(model->name() + "_JointKinetics", output);
      auto joints = model->joints()->findChildren<Joint*>({},{},false);
      for (auto& joint: joints)
      {
        auto descriptors = joint->findChildren<DynamicDescriptor*>({},{},false);
        for (auto& descriptor: descriptors)
        {
          descriptor->evaluate(analysis, joint, options);
        }
      }
    }
    return true;
  };
  
  /**
   * Similar to the other extract_joint_kinetics() method but the computed joint kinetics are added to a returned node.
   * @important The returned node is allocated on the heap. The developer must care of the deletion of the object using the @c delete keyword.
   */
  Node* extract_joint_kinetics(Node* input, bool sideAdaptation, bool massNormalization, RepresentationFrame frame)
  {
    Node* root = new Node("root");
    if (!extract_joint_kinetics(root, input, sideAdaptation, massNormalization, frame))
    {
      delete root;
      root = nullptr;
    }
    return root;
  };
};
};