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

#include "openma/body/plugingait.h"
#include "openma/body/plugingait_p.h"
#include "openma/body/anchor.h"
#include "openma/body/chain.h"
#include "openma/body/dempstertable.h"
#include "openma/body/dynamicdescriptor.h"
#include "openma/body/enums.h"
#include "openma/body/eulerdescriptor_p.h"
#include "openma/body/simplegaitforceplatetofeetassigner.h"
#include "openma/body/joint.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/body/model.h"
#include "openma/body/referenceframe.h"
#include "openma/body/segment.h"
#include "openma/body/utils.h"
#include "openma/body/inversedynamicsmatrix.h"
#include "openma/body/skeletonhelperposeestimator.h"
#include "openma/base/logger.h"
#include "openma/base/subject.h"
#include "openma/base/trial.h"

#include "Eigen_openma/Utils/sign.h"

#include <algorithm> // std::copy_n

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  PluginGaitPrivate::PluginGaitPrivate(PluginGait* pint, const std::string& name, int region, int side)
  : SkeletonHelperPrivate(pint,name,region,side),
    // All the options are null by default
    MarkerDiameter(0.0),
    HeadOffsetEnabled(false),
    RightShoulderOffset(0.0),
    LeftShoulderOffset(0.0),
    RightElbowWidth(0.0),
    LeftElbowWidth(0.0),
    RightWristWidth(0.0),
    LeftWristWidth(0.0),
    RightHandThickness(0.0),
    LeftHandThickness(0.0),
    InterAsisDistance(0.0),
    RightLegLength(0.0),
    LeftLegLength(0.0),
    RightAsisTrochanterAPDistance(0.0),
    LeftAsisTrochanterAPDistance(0.0),
    RightKneeWidth(0.0),
    LeftKneeWidth(0.0),
    RightAnkleWidth(0.0),
    LeftAnkleWidth(0.0),
    RightFootFlatEnabled(false), 
    LeftFootFlatEnabled(false),
    HeadOffset(0.0),
    RightStaticPlantarFlexionOffset(0.0),
    RightStaticRotationOffset(0.0),
    LeftStaticPlantarFlexionOffset(0.0),
    LeftStaticRotationOffset(0.0)
  {};
    
  PluginGaitPrivate::~PluginGaitPrivate() _OPENMA_NOEXCEPT = default;
  
  bool PluginGaitPrivate::calibrateLowerLimb(int side, const math::Position* HJC, TaggedMappedPositions* landmarks) _OPENMA_NOEXCEPT
  {
    auto pptr = this->pint();
    std::string prefix;
    double s = 0.0, ankleWidth = 0.0, kneeWidth = 0.0, seglength = 0.0;
    double *staticPlantarFlexionOffset = nullptr, *staticRotationOffset = nullptr;
    bool footFlat = false;
    if (side == Side::Left)
    {
      prefix = "L.";
      s = -1.0;
      ankleWidth = this->LeftAnkleWidth;
      kneeWidth = this->LeftKneeWidth;
      footFlat = this->LeftFootFlatEnabled;
      staticPlantarFlexionOffset = &(this->LeftStaticPlantarFlexionOffset);
      staticRotationOffset = &(this->LeftStaticRotationOffset);
    }
    else if (side == Side::Right)
    {
      prefix = "R.";
      s = 1.0;
      ankleWidth = this->RightAnkleWidth;
      kneeWidth = this->RightKneeWidth;
      footFlat = this->RightFootFlatEnabled;
      staticPlantarFlexionOffset = &(this->RightStaticPlantarFlexionOffset);
      staticRotationOffset = &(this->RightStaticRotationOffset);
    }
    else
    {
      error("PluginGait - Unknown side for the lower limb. Calibration aborted.");
      return false;
    }
    // -----------------------------------------
    // Thigh
    // -----------------------------------------
    // Required landmarks: *.ITB, *.LFE
    const auto& ITB = (*landmarks)[prefix+"ITB"];
    const auto& LFE = (*landmarks)[prefix+"LFE"];
    if (!ITB.isValid() || !LFE.isValid())
    {
      error("PluginGait - Missing landmarks to define the thigh. Calibration aborted.");
      return false;
    }
    // Compute the knee joint centre (KJC)
    const math::Position KJC = compute_chord((this->MarkerDiameter + kneeWidth) / 2.0, LFE, *HJC, ITB);
    // Set the segment length
    seglength = (KJC - *HJC).norm().mean();
    pptr->setProperty(prefix+"Thigh.length", seglength);
    // Set the body inertial coordinate system (relative to the SCS)
    const double relOriBcsFromScs[9] = {
      0., -1., 0., // BCS u axis has to point to the right. SCS v axis is along the ML axis but pointing to the left (LM)
      1.,  0., 0., // BCS v axis has to point forward. SCS u axis is pointing forward too
      0.,  0., 1.  // BCS w axis has to point upward. SCS w axis is going updward too
    };
    double relPosBcsFromScs[3] = {0.,0.,seglength};
    new ReferenceFrame(prefix+"Thigh.BCS", relOriBcsFromScs, relPosBcsFromScs, pptr);
    // -----------------------------------------
    // Shank
    // -----------------------------------------
    // Required landmarks: *.LTM, *.LS
    const auto& LTM = (*landmarks)[prefix+"LTM"];
    const auto& LS = (*landmarks)[prefix+"LS"];
    if (!LTM.isValid() || !LS.isValid())
    {
      error("PluginGait - Missing landmarks to define the shank. Calibration aborted.");
      return false;
    }
    // Compute the ankle joint centre (AJC)
    const math::Position AJC = compute_chord((this->MarkerDiameter + ankleWidth) / 2.0, LTM, KJC, LS);
    // Set the segment length
    seglength = (AJC - KJC).norm().mean();
    pptr->setProperty(prefix+"Shank.length", seglength);
    // Set the body inertial coordinate system (relative to the SCS)
    //  - Same relative orientation than for the thigh
    relPosBcsFromScs[2] = seglength;
    new ReferenceFrame(prefix+"Shank.BCS", relOriBcsFromScs, relPosBcsFromScs, pptr);
    // -----------------------------------------
    // Foot
    // -----------------------------------------
    // Required landmarks: *.MTH2, *.HEE
    const auto& MTH2 = (*landmarks)[prefix+"MTH2"];
    math::Position HEE = (*landmarks)[prefix+"HEE"]; // Copy instead of a map due to possible modification on its coordinates if the foot flat option is activated
    if (!MTH2.isValid() || !HEE.isValid())
    {
      error("PluginGait - Missing landmarks to define the foot. Calibration aborted.");
      return false;
    }
    // Set the segment length
    seglength = (MTH2 - HEE).norm().mean();
    pptr->setProperty(prefix+"Foot.length", seglength);
    // Set the body inertial coordinate system (relative to the SCS)
    //  - The BCS origin is the same than the SCS. That's why the relative posiiton is set to nullptr (which internaly is equal to 0,0,0)
    const double relOriBcsFromScsFoot[9] = {
      0., -1.,  0., // BCS u axis (right) corresponds to SCS -v axis
      0.,  0., -1., // BCS v axis (forward) corresponds to SCS -w axis
      1.,  0.,  0.  // BCS w axis (upward) corresponds to SCS -u axis
    };
    new ReferenceFrame(prefix+"Foot.BCS", relOriBcsFromScsFoot, nullptr, pptr);
    // Compute foot offset angles
    if (footFlat)
    {
      // Copy the Z-coordinates of MTH2 into HEE
      // FIXME: This assumes that the vertical axis is always Z!
      HEE.block<1>(2) = MTH2.block<1>(2);
    }
    // - Shank axes
    math::Vector w = (KJC - AJC).normalized();
    math::Vector u = s * w.cross(LS - AJC).normalized();
    math::Vector v_shank = w.cross(u);
    // - Foot reference
    w = (HEE - MTH2).normalized();
    u = v_shank.cross(w).normalized();
    math::Pose foot(u,w.cross(u),w,AJC);
    // - Uncorrected foot reference
    w = (AJC - MTH2).normalized();
    u = v_shank.cross(w).normalized();
    math::Pose uncorrected_foot(u,w.cross(u),w,AJC);
    // - Offset angles
    if (foot.isOccluded() || uncorrected_foot.isOccluded())
    {
      error("PluginGait - Impossible to find a least one valid frame for the foot motion. Calibration aborted.");
      return false;
    }
    math::Vector::Values offsetAngles = uncorrected_foot.inverse().transform(foot).eulerAngles(1,0,2).mean().values();
    *staticPlantarFlexionOffset = -1.0 * offsetAngles.coeff(0);
    *staticRotationOffset = s * offsetAngles.coeff(1);
    return true;
  };
  
  bool PluginGaitPrivate::reconstructUpperLimb(Model* model, Trial* trial, int side, const math::Vector* u_torso, const math::Vector* o_torso, TaggedMappedPositions* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT
  {
    auto pptr = this->pint();
    std::string prefix;
    double s = 0.0, shoulderOffset = 0.0, elbowWidth = 0.0, wristWidth = 0.0, handThickness = 0.0;
    if (side == Side::Left)
    {
      prefix = "L.";
      s = -1.0;
      shoulderOffset = this->LeftShoulderOffset;
      elbowWidth = this->LeftElbowWidth;
      wristWidth = this->LeftWristWidth;
      handThickness = this->LeftHandThickness;
    }
    else if (side == Side::Right)
    {
      prefix = "R.";
      s = 1.0;
      shoulderOffset = this->RightShoulderOffset;
      elbowWidth = this->RightElbowWidth;
      wristWidth = this->RightWristWidth;
      handThickness = this->RightHandThickness;
    }
    else
    {
      error("PluginGait - Unknown side for the upper limb. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // Temporary variable use to construct segments' motion
    Segment* seg;
    math::Vector u, v, w, o;
    // -----------------------------------------
    // Arm & clavicle (optional)
    // -----------------------------------------
    // Required landmarks: *.AC, *.US, *.RS, *.LHE
    const auto& AC = (*landmarks)[prefix+"AC"];
    const auto& US = (*landmarks)[prefix+"US"];
    const auto& RS = (*landmarks)[prefix+"RS"];
    const auto& LHE = (*landmarks)[prefix+"LHE"];
    if (!AC.isValid() || !US.isValid() || !RS.isValid() || !LHE.isValid())
    {
      error("PluginGait - Missing landmarks to define the arm. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // VWM: virtual "wand" marker
    const math::Position VWM = (AC - *o_torso).cross(*u_torso) + AC;
    // Compute the shoulder joint centre (SJC)
    const math::Position SJC = compute_chord(s * (shoulderOffset + this->MarkerDiameter / 2.0), AC, *o_torso, VWM);
    // Clavicle
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Clavicle}},false);
    w = (*o_torso - SJC).normalized();
    u = w.cross(VWM - SJC).normalized();
    v = w.cross(u);
    math::to_timesequence(u, v, w, SJC, seg->name()+".SCS", sampleRate, startTime, seg);
    // Construction of the vector marker (CVM) for the elbow
    const math::Position MWP = (US + RS) / 2.0; // Middle Wrist Point
    const math::Position CVM = s * (MWP - LHE).cross(SJC - LHE).normalized() + LHE;
    const math::Position EJC = compute_chord((elbowWidth + this->MarkerDiameter) / 2.0, LHE, SJC, CVM);
    const math::Position WJC = s * (US - RS).cross(EJC - MWP).normalized() * (wristWidth + this->MarkerDiameter) / 2.0 + MWP;
    // Arm frame
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Arm}},false);
    w = (SJC - EJC).normalized();
    v = (WJC - EJC).cross(w).normalized();
    u = v.cross(w);
    math::to_timesequence(u, v, w, EJC, seg->name()+".SCS", sampleRate, startTime, seg);
    seg->setProperty("length", pptr->property(seg->name()+".length"));
    // -----------------------------------------
    // Forearm
    // -----------------------------------------
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Forearm}},false);
    w = (EJC - WJC).normalized();
    u = v.cross(w); // The 'v' axis is the same than the one defined for the arm
    math::to_timesequence(u, v, w, WJC, seg->name()+".SCS", sampleRate, startTime, seg);
    seg->setProperty("length", pptr->property(seg->name()+".length"));
    // -----------------------------------------
    // Hand
    // -----------------------------------------
    // Required landmark : *.MH2
    const auto& MH2 = (*landmarks)[prefix+"MH2"];
    if (!MH2.isValid())
    {
      error("PluginGait - Missing landmark 'MH2' to define the hand. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Hand}},false);
    // HO: Hand Origin
    const math::Position HO = compute_chord((handThickness + this->MarkerDiameter) / 2.0, MH2, WJC, MWP);
    o =  2.0 * (HO - WJC) + WJC;
    w = (WJC - HO).normalized();
    u = s * w.cross(US - RS).normalized();
    v = w.cross(u);
    math::to_timesequence(u, v, w, o, seg->name()+".SCS", sampleRate, startTime, seg);
    seg->setProperty("length", pptr->property(seg->name()+".length"));
    return true;
  };
  
  bool PluginGaitPrivate::reconstructLowerLimb(Model* model, Trial* trial, int side, const math::Position* HJC, TaggedMappedPositions* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT
  {
    auto pptr = this->pint();
    std::string prefix;
    double s = 0.0, ankleWidth = 0.0, kneeWidth = 0.0,
           staticPlantarFlexionOffset = 0.0, staticRotationOffset = 0.0;
    if (side == Side::Left)
    {
      prefix = "L.";
      s = -1.0;
      ankleWidth = this->LeftAnkleWidth;
      kneeWidth = this->LeftKneeWidth;
      staticPlantarFlexionOffset = -this->LeftStaticPlantarFlexionOffset;
      staticRotationOffset = -this->LeftStaticRotationOffset;
    }
    else if (side == Side::Right)
    {
      prefix = "R.";
      s = 1.0;
      ankleWidth = this->RightAnkleWidth;
      kneeWidth = this->RightKneeWidth;
      staticPlantarFlexionOffset = -this->RightStaticPlantarFlexionOffset;
      staticRotationOffset = this->RightStaticRotationOffset;
    }
    else
    {
      error("PluginGait - Unknown side for the lower limb. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // Temporary variables used to construct segments' motion
    Segment* seg = nullptr;
    math::Vector u,v,w,o;
    ReferenceFrame* bcs = nullptr;
    // -----------------------------------------
    // Thigh
    // -----------------------------------------
    // Required landmarks: *.ITB, *.LFE
    const auto& ITB = (*landmarks)[prefix+"ITB"];
    const auto& LFE = (*landmarks)[prefix+"LFE"];
    if (!ITB.isValid() || !LFE.isValid())
    {
      error("PluginGait - Missing landmarks to define the thigh. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Thigh}},false);
    const math::Position KJC = compute_chord((this->MarkerDiameter + kneeWidth) / 2.0, LFE, *HJC, ITB);
    u = s * (*HJC - LFE).cross(ITB - LFE).normalized();
    w = (*HJC - KJC).normalized();
    v = w.cross(u);
    math::to_timesequence(u, v, w, KJC, seg->name()+".SCS", sampleRate, startTime, seg);
    seg->setProperty("length", pptr->property(seg->name()+".length"));
    if ((bcs = pptr->findChild<ReferenceFrame*>(seg->name()+".BCS")) != nullptr) bcs->addParent(seg);
    // -----------------------------------------
    // Shank
    // -----------------------------------------
    // Required landmarks: *.LTM, *.LS
    const auto& LTM = (*landmarks)[prefix+"LTM"];
    const auto& LS = (*landmarks)[prefix+"LS"];
    if (!LTM.isValid() || !LS.isValid())
    {
      error("PluginGait - Missing landmarks to define the shank. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Shank}},false);
    // Compute the ankle joint centre (AJC)
    const math::Position AJC = compute_chord((this->MarkerDiameter + ankleWidth) / 2.0, LTM, KJC, LS);
    w = (KJC - AJC).normalized();
    u = s * w.cross(LS - AJC).normalized();
    math::Vector v_shank = w.cross(u);
    math::to_timesequence(u, v_shank, w, AJC, seg->name()+".SCS", sampleRate, startTime, seg);
    seg->setProperty("length", pptr->property(seg->name()+".length"));
    if ((bcs = pptr->findChild<ReferenceFrame*>(seg->name()+".BCS")) != nullptr) bcs->addParent(seg);
    // -----------------------------------------
    // Foot
    // -----------------------------------------
    // Required landmarks: *.MTH2
    const auto& MTH2 = (*landmarks)[prefix+"MTH2"];
    if (!MTH2.isValid())
    {
      error("PluginGait - Missing landmark to define the foot. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    seg = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Foot}},false);
    w = (AJC - MTH2).normalized();
    u = v_shank.cross(w).normalized();
    v = w.cross(u);
    const double cx = cos(staticRotationOffset), sx = sin(staticRotationOffset),
                 cy = cos(staticPlantarFlexionOffset), sy = sin(staticPlantarFlexionOffset);
    const math::Vector ur = u * cy - w * sy;
    const math::Vector vr = u * sx*sy + v * cx + w * cy*sx;
    const math::Vector wr = u * cx*sy - v * sx + w * cx*cy;
    // FIXME: This is not the good origin. None anatomical meaning was discovered to explain this position...
    math::to_timesequence(ur, vr, wr, AJC, seg->name()+".SCS", sampleRate, startTime, seg);
    seg->setProperty("length", pptr->property(seg->name()+".length"));
    if ((bcs = pptr->findChild<ReferenceFrame*>(seg->name()+".BCS")) != nullptr) bcs->addParent(seg);
    return true;
  };
  
  // ----------------------------------------------------------------------- //
  
  PluginGaitLeftShoulderDescriptor::PluginGaitLeftShoulderDescriptor(Node* parent)
  : EulerDescriptor("L.Shoulder.Angle", EulerDescriptor::XYZ, parent)
  {};

  bool PluginGaitLeftShoulderDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double range = M_PI;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      range = 180.0;
    auto optr = this->pimpl();
    math::Scalar::Values temp = -1.0 * optr->OutputData.values().col(1);
    math::Scalar mult = optr->OutputData.block<1>(0);
    optr->OutputData.values().col(1) = optr->OutputData.values().col(0) - mult.normalized().values() * range;
    mult = optr->OutputData.block<1>(2);
    optr->OutputData.values().col(2) -= mult.normalized().values() * range;
    optr->OutputData.values().col(2) *= -1.0;
    optr->OutputData.values().col(0) = temp;
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };

  // ----------------------------------------------------------------------- //


  PluginGaitRightShoulderDescriptor::PluginGaitRightShoulderDescriptor(Node* parent)
  : EulerDescriptor("R.Shoulder.Angle", EulerDescriptor::XYZ, parent)
  {};

  bool PluginGaitRightShoulderDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double range = M_PI;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      range = 180.0;
    auto optr = this->pimpl();
    math::Scalar::Values temp = -1.0 * optr->OutputData.values().col(1);
    optr->OutputData.values().col(1) = range - optr->OutputData.values().col(0);
    math::Scalar mult = optr->OutputData.block<1>(2);
    optr->OutputData.values().col(2) -= mult.normalized().values() * range;
    optr->OutputData.values().col(0) = temp;
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };

  // ----------------------------------------------------------------------- //

  PluginGaitNeckDescriptor::PluginGaitNeckDescriptor(Node* parent)
  : EulerDescriptor("Neck.Angle", EulerDescriptor::YXZ, {{-1.0,-1.0,1.0}}, parent)
  {};

  bool PluginGaitNeckDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double range = M_PI;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      range = 180.0;
    auto optr = this->pimpl();
    math::Scalar mult = optr->OutputData.block<1>(0);
    optr->OutputData.values().col(0) -= mult.normalized().values() * range;
    mult = optr->OutputData.block<1>(2);
    optr->OutputData.values().col(2) -= mult.normalized().values() * range;
    math::to_timesequence(optr->OutputData, "L."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    optr->OutputData.values().col(1) *= -1.0;
    optr->OutputData.values().col(2) *= -1.0;
    math::to_timesequence(optr->OutputData, "R."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };

  // ----------------------------------------------------------------------- //

  PluginGaitLeftAnkleDescriptor::PluginGaitLeftAnkleDescriptor(Node* parent)
  : EulerDescriptor("L.Ankle.Angle", EulerDescriptor::YXZ, {{-1.,-1.,-1.}}, parent)
  {};

  bool PluginGaitLeftAnkleDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double offset = M_PI / 2.0;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      offset = 90.0;
    auto optr = this->pimpl();
    optr->OutputData.values().col(0) -= offset;
    math::Scalar::Values temp = optr->OutputData.values().col(1);
    optr->OutputData.values().col(1) = optr->OutputData.values().col(2);
    optr->OutputData.values().col(2) = temp;
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };

  // ----------------------------------------------------------------------- //

  PluginGaitRightAnkleDescriptor::PluginGaitRightAnkleDescriptor(Node* parent)
  : EulerDescriptor("R.Ankle.Angle", EulerDescriptor::YXZ, {{-1.0,1.0,1.0}}, parent)
  {};

  bool PluginGaitRightAnkleDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double offset = M_PI / 2.0;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      offset = 90.0;
    auto optr = this->pimpl();
    optr->OutputData.values().col(0) -= offset;
    math::Scalar::Values temp = optr->OutputData.values().col(1);
    optr->OutputData.values().col(1) = optr->OutputData.values().col(2);
    optr->OutputData.values().col(2) = temp;
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };

  // ----------------------------------------------------------------------- //

  PluginGaitSpineDescriptor::PluginGaitSpineDescriptor(Node* parent)
  : EulerDescriptor("Spine.Angle", EulerDescriptor::YXZ, parent)
  {};

  bool PluginGaitSpineDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double range = M_PI;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      range = 180.0;
    auto optr = this->pimpl();
    math::Scalar mult = optr->OutputData.block<1>(0);
    optr->OutputData.values().col(0) -= mult.normalized().values() * range;
    mult = optr->OutputData.block<1>(2);
    optr->OutputData.values().col(2) -= mult.normalized().values() * range;
    math::to_timesequence(optr->OutputData, "R."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    optr->OutputData.values().col(1) *= -1.0;
    optr->OutputData.values().col(2) *= -1.0;
    math::to_timesequence(optr->OutputData, "L."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
  
  // ----------------------------------------------------------------------- //
  
  PluginGaitPelvisDescriptor::PluginGaitPelvisDescriptor(Node* parent)
  : EulerDescriptor("Pelvis.Progress.Angle", EulerDescriptor::YXZ, parent)
  {};
  
  bool PluginGaitPelvisDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    OPENMA_UNUSED(options);
    auto optr = this->pimpl();
    optr->OutputData.values().col(1) *= -1.0;
    math::to_timesequence(optr->OutputData, "R."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    optr->OutputData.values().col(1) *= -1.0;
    optr->OutputData.values().col(2) *= -1.0;
    math::to_timesequence(optr->OutputData, "L."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
  
  // ----------------------------------------------------------------------- //
  
  PluginGaitThoraxDescriptor::PluginGaitThoraxDescriptor(Node* parent)
  : EulerDescriptor("Thorax.Progress.Angle", EulerDescriptor::YXZ, parent)
  {};
  
  bool PluginGaitThoraxDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double range = M_PI;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      range = 180.0;
    auto optr = this->pimpl();
    math::Scalar mult = optr->OutputData.block<1>(0);
    optr->OutputData.values().col(0) -= mult.normalized().values() * range;
    math::Scalar::Values temp = optr->OutputData.values().col(2);
    mult = optr->OutputData.block<1>(2);
    optr->OutputData.values().col(2) = mult.normalized().values() * range - temp;
    math::to_timesequence(optr->OutputData, "R."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    optr->OutputData.values().col(1) *= -1.0;
    optr->OutputData.values().col(2) *= -1.0;
    math::to_timesequence(optr->OutputData, "L."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
  
  // ----------------------------------------------------------------------- //
  
  PluginGaitHeadDescriptor::PluginGaitHeadDescriptor(Node* parent)
  : EulerDescriptor("Head.Progress.Angle", EulerDescriptor::YXZ, parent)
  {};
  
  bool PluginGaitHeadDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    double range = M_PI;
    // Option enableDegreeConversion activated?
    auto it = options.cend();
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
      range = 180.0;
    auto optr = this->pimpl();
    optr->OutputData.values().col(0) *= -1.0;
    optr->OutputData.values().col(1) *= -1.0;
    math::to_timesequence(optr->OutputData, "R."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    optr->OutputData.values().col(1) *= -1.0;
    optr->OutputData.values().col(2) *= -1.0;
    math::to_timesequence(optr->OutputData, "L."+this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
  
  // ----------------------------------------------------------------------- //

  PluginGaitRightFootDescriptor::PluginGaitRightFootDescriptor(Node* parent)
  : EulerDescriptor("R.Foot.Progress.Angle", EulerDescriptor::YXZ, parent)
  {};

  bool PluginGaitRightFootDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    OPENMA_UNUSED(options)
    auto optr = this->pimpl();
    math::Scalar::Values temp = optr->OutputData.values().col(1);
    optr->OutputData.values().col(1) = -1.0 * optr->OutputData.values().col(2);
    optr->OutputData.values().col(2) = temp;
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
  
  // ----------------------------------------------------------------------- //

  PluginGaitLeftFootDescriptor::PluginGaitLeftFootDescriptor(Node* parent)
  : EulerDescriptor("L.Foot.Progress.Angle", EulerDescriptor::YXZ, parent)
  {};

  bool PluginGaitLeftFootDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    OPENMA_UNUSED(options)
    auto optr = this->pimpl();
    math::Scalar::Values temp = -1.0 * optr->OutputData.values().col(1);
    optr->OutputData.values().col(1) = optr->OutputData.values().col(2);
    optr->OutputData.values().col(2) = temp;
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::PluginGait);

namespace ma
{
namespace body
{
  /**
   * @class PluginGait openma/body/plugingait.h
   * @brief Helper class to create and reconstruct a model based on the Plug-in Gait markers set.
   * 
   * @todo Add a detailed description on possible configurations, segments, joints, and coordinate systems definition. 
   * @todo Add the required and optional parameters used by this helper.
   * @todo Explain how to set custom hip joint centre
   *
   * @ingroup openma_body
   */
  
#ifdef DOXYGEN_SHOULD_TAKE_THIS
  /** * @brief Fake structure to create node's properties */
  struct PluginGait::__Doxygen_Properties
  {
  /**
   * [Required] This property holds the marker diameter used in the computation. By default, this property contains the value 0.0.
   * @sa markerDiameter() setMarkerDiameter()
   */
  double MarkerDiameter;
  /**
   * [Required] This property holds the confirmation the subject has the head not oriented along the longitudinal horizontal axis. By default, this property is set to false.
   * @sa headOffsetEnabled() setLHeadOffsetEnabled()
   */
  bool HeadOffsetEnabled;
  /**
   * [Required] This property holds the vertical distance from the centre of the glenohumeral joint to the marker on the acromion calivicular joint. By default, this property contains the value 0.0.
   * @sa rightShoulderOffset() setRightShoulderOffset()
   */
  double RightShoulderOffset;
  /**
   * [Required] This property holds the vertical distance from the centre of the glenohumeral joint to the marker on the acromion calivicular joint. By default, this property contains the value 0.0.
   * @sa rightShoulderOffset() setRightShoulderOffset()
   */
  double LeftShoulderOffset;
  /**
   *[Required] This property holds the distance between the medial and lateral epicondyles of the humerus. By default, this property contains the value 0.0.
   * @sa rightElbowWidth() setRightElbowWidth()
   */
  double RightElbowWidth;
  /**
   * [Required] This property holds the distance between the medial and lateral epicondyles of the humerus. By default, this property contains the value 0.0.
   * @sa leftElbowWidth() setLeftElbowWidth()
   */
  double LeftElbowWidth;
  /**
   * [Required] This property holds the distance between the ulnar and radial styloids. By default, this property contains the value 0.0.
   * @sa rightWristWidth() setRightWristWidth()
   */
  double RightWristWidth;
  /**
   * [Required] This property holds the distance between the ulnar and radial styloids. By default, this property contains the value 0.0.
   * @sa leftWristWidth() setLeftWristWidth()
   */
  double LeftWristWidth;
  /**
   * [Required] This property holds the distance between the dorsal and palmar surfaces of the hand. By default, this property contains the value 0.0.
   * @sa rightHandThickness() setRightHandThickness()
   */
  double RightHandThickness;
  /**
   * [Required] This property holds the distance between the dorsal and palmar surfaces of the hand. By default, this property contains the value 0.0.
   * @sa leftHandThickness() setLeftHandThickness()
   */
  double LeftHandThickness;
  /**
   * [Optional] This property holds the distance based on the position of the L.ASIS and R.ASIS markers. By default, this property contains the value 0.0.
   * @sa interAsisDistance() setInterAsisDistance()
   */
  double InterAsisDistance;
  /**
   * [Required] This property holds the leg length measured from the ASIS to the medial malleolus. By default, this property contains the value 0.0.
   * @sa rightLegLength() setRightLegLength()
   */
  double RightLegLength;
  /**
   * [Required] This property holds the leg length measured from the ASIS to the medial malleolus. By default, this property contains the value 0.0.
   * @sa leftLegLength() setLeftLegLength()
   */
  double LeftLegLength;
  /**
   * [Optional] This property holds the perpendicular distance from the trochanter to the ASIS point. By default, this property contains the value 0.0.
   * @sa rightAsisTrochanterAPDistance() setRightAsisTrochanterAPDistance()
   */
  double RightAsisTrochanterAPDistance; 
  /**
   * [Optional] This property holds the perpendicular distance from the trochanter to the ASIS point. By default, this property contains the value 0.0.
   * @sa leftAsisTrochanterAPDistance() setLeftAsisTrochanterAPDistance()
   */
  double LeftAsisTrochanterAPDistance;
  /**
   * [Required] This property holds the distance between femoral epycondyles. By default, this property contains the value 0.0.
   * @sa rightKneeWidth() setRightKneeWidth()
   */
  double RightKneeWidth;
  /**
   * [Required] This property holds the distance between femoral epycondyles. By default, this property contains the value 0.0.
   * @sa leftKneeWidth() setLeftKneeWidth()
   */
  double LeftKneeWidth;
  /**
   * [Required] This property holds the distance between malleoli. By default, this property contains the value 0.0.
   * @sa rightAnkleWidth() setRightAnkleWidth()
   */
  double RightAnkleWidth;
  /**
   * [Required] This property holds the distance between malleoli. By default, this property contains the value 0.0.
   * @sa leftAnkleWidth() setLeftAnkleWidth()
   */
  double LeftAnkleWidth;
  /**
   * [Required] This property holds the confirmation the subject is able to stand with the foot on the ground. By default, this property is set to false.
   * @sa rightFootFlatEnabled() setRightFootFlatEnabled()
   */
  bool RightFootFlatEnabled;
  /**
   * [Required] This property holds the confirmation the subject is able to stand with the foot on the ground.  By default, this property is set to false.
   * @sa leftFootFlatEnabled() setLeftFootFlatEnabled()
   */
  bool LeftFootFlatEnabled;
  /**
   * [Calculated] This property holds the head offset angle. By default, this property contains the value 0.0.
   * @sa headOffset()
   */
  double HeadOffset;
  /**
   * [Calculated] This property holds the rotation about the foot rotation axis, which is perpendicular to the foot vector (after applying the foot plantar flexion offset) and the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa rightStaticPlantarFlexionOffset()
   */
  double RightStaticPlantarFlexionOffset;
  /**
   * [Calculated] This property holds the rotation angle about the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa rightStaticRotationOffset()
   */
  double RightStaticRotationOffset;
  /**
   * [Calculated] This property holds the  rotation about the foot rotation axis, which is perpendicular to the foot vector (after applying the foot plantar flexion offset) and the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa leftStaticPlantarFlexionOffset()
   */
  double LeftStaticPlantarFlexionOffset;
  /**
   * [Calculated] This property holds the rotation angle about the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa leftStaticRotationOffset()
   */
  double LeftStaticRotationOffset;
  };
#endif
  
  /**
   * Constructor. The name is set to "PluginGait".
   */
  PluginGait::PluginGait(int region, int side, Node* parent)
  : SkeletonHelper(*new PluginGaitPrivate(this, "PluginGait", region, side), parent)
  {};

  /**
   * Create segments and joints according to the region and side given to the constructor.
   */
  bool PluginGait::setupModel(Model* model) const
  {
    if (model == nullptr)
    {
      error("Null model passed. Setup aborted.");
      return false;
    }
    auto optr = this->pimpl();
    if (optr->Side == Side::Center)
    {
      error("Impossible to have a 'centre' side for the PluginGait helper. Only the enumerators Side::Right, Side::Left, and Side::Both are available for this helper. Setup aborted.");
      return false;
    }
    Node* segments = model->segments();
    Node* joints = model->joints();
    Node* chains = model->chains();
    Segment *torso = nullptr, *pelvis = nullptr,
            *progression = new Segment("Progression", Part::User, Side::Center, segments);
    Joint* jnt;
    if ((optr->Region & Region::Upper) == Region::Upper)
    {
      model->setName(this->name() + "_UpperLimb");
      Segment* head = new Segment("Head", Part::Head, Side::Center, segments);
      torso = new Segment("Torso", Part::Torso, Side::Center, segments);
      if (optr->Side & Side::Left)
      {
        Segment* leftClavicle = new Segment("L.Clavicle", Part::Clavicle, Side::Left, segments);
        Segment* leftArm = new Segment("L.Arm", Part::Arm, Side::Left, segments);
        Segment* leftForearm = new Segment("L.Forearm", Part::Forearm, Side::Left, segments);
        Segment* leftHand = new Segment("L.Hand", Part::Hand, Side::Left, segments);
        std::vector<Joint*> leftUpperLimbJoints(3);
        jnt = new Joint("L.Shoulder", torso, leftArm, Anchor::origin(leftClavicle), joints);
        leftUpperLimbJoints[0] = jnt;
        new PluginGaitLeftShoulderDescriptor(jnt);
        jnt = new Joint("L.Elbow", leftArm, leftForearm, Anchor::origin(leftArm), joints);
        leftUpperLimbJoints[1] = jnt;
        new EulerDescriptor("L.Elbow.Angle", EulerDescriptor::YXZ, jnt);
        jnt = new Joint("L.Wrist", leftForearm, leftHand, Anchor::origin(leftForearm), joints);
        leftUpperLimbJoints[2] = jnt;
        new EulerDescriptor("L.Wrist.Angle", EulerDescriptor::YXZ, {{1.0, -1.0, -1.0}}, jnt);
        // new Chain("L.UpperLimb", leftUpperLimbJoints, chains);
      }
      if (optr->Side & Side::Right)
      {
        Segment* rightClavicle = new Segment("R.Clavicle", Part::Clavicle, Side::Right, segments);
        Segment* rightArm = new Segment("R.Arm", Part::Arm, Side::Right, segments);
        Segment* rightForearm = new Segment("R.Forearm", Part::Forearm, Side::Right, segments);
        Segment* rightHand = new Segment("R.Hand", Part::Hand, Side::Right, segments);
        std::vector<Joint*> rightUpperLimbJoints(3);
        jnt = new Joint("R.Shoulder", torso, rightArm, Anchor::origin(rightClavicle), joints);
        rightUpperLimbJoints[0] = jnt;
        new PluginGaitRightShoulderDescriptor(jnt);
        jnt = new Joint("R.Elbow", rightArm, rightForearm, Anchor::origin(rightArm), joints);
        rightUpperLimbJoints[1] = jnt;
        new EulerDescriptor("R.Elbow.Angle", EulerDescriptor::YXZ, jnt);
        jnt = new Joint("R.Wrist", rightForearm, rightHand, Anchor::origin(rightForearm), joints);
        rightUpperLimbJoints[2] = jnt;
        new EulerDescriptor("R.Wrist.Angle", EulerDescriptor::YXZ, jnt);
        // new Chain("R.UpperLimb", rightUpperLimbJoints, chains);
      }
      jnt = new Joint("Neck", head, torso, joints);
      new PluginGaitNeckDescriptor(jnt);
      jnt = new Joint("Thorax.Progress", progression, torso, joints);
      jnt->setDescription("Torso relative to progression frame");
      new PluginGaitThoraxDescriptor(jnt);
      jnt = new Joint("Head.Progress", progression, head, joints);
      jnt->setDescription("Head relative to progression frame");
      new PluginGaitHeadDescriptor(jnt);
    }
    if ((optr->Region & Region::Lower) == Region::Lower)
    {
      model->setName(this->name() + "_LowerLimb");
      pelvis = new Segment("Pelvis", Part::Pelvis, Side::Center, segments);
      if (optr->Side & Side::Left)
      {
        Segment* leftThigh = new Segment("L.Thigh", Part::Thigh, Side::Left, segments);
        Segment* leftShank = new Segment("L.Shank", Part::Shank, Side::Left, segments);
        Segment* leftFoot = new Segment("L.Foot", Part::Foot, Side::Left, segments);
        std::vector<Joint*> leftLowerLimbJoints(3);
        jnt = new Joint("L.Hip", pelvis, Anchor::point("L.HJC"), leftThigh, Anchor::point("L.HJC", pelvis), joints);
        leftLowerLimbJoints[0] = jnt;
        new EulerDescriptor("L.Hip.Angle", EulerDescriptor::YXZ, {{-1.,-1.,-1.}}, jnt);
        new DynamicDescriptor({{0,1,2,1,0,2,0,1,2}}, {{1.,1.,1.,1.,1.,1.,1.,1.,-1.}}, jnt);
        jnt = new Joint("L.Knee", leftThigh, leftShank, Anchor::origin(leftThigh), joints);
        leftLowerLimbJoints[1] = jnt;
        new EulerDescriptor("L.Knee.Angle", EulerDescriptor::YXZ, {{1.0, -1.0, -1.0}}, jnt);
        new DynamicDescriptor({{0,1,2,1,0,2,0,1,2}}, {{1.,1.,1.,-1.,1.,1.,1.,1.,-1.}}, jnt);
        jnt = new Joint("L.Ankle", leftShank, leftFoot, Anchor::origin(leftShank), joints);
        leftLowerLimbJoints[2] = jnt;
        new PluginGaitLeftAnkleDescriptor(jnt);
        new DynamicDescriptor({{0,1,2,1,2,0,0,1,2}}, {{-1.,1.,-1.,1.,-1.,1.,1.,1.,-1.}}, jnt);
        jnt = new Joint("L.Foot.Progress", progression, leftFoot, joints);
        jnt->setDescription("Left foot relative to progression frame");
        new PluginGaitLeftFootDescriptor(jnt);
        new Chain("L.LowerLimb", leftLowerLimbJoints, chains);
      }
      if (optr->Side & Side::Right)
      {
        Segment* rightThigh = new Segment("R.Thigh", Part::Thigh, Side::Right, segments);
        Segment* rightShank = new Segment("R.Shank", Part::Shank, Side::Right, segments);
        Segment* rightFoot = new Segment("R.Foot", Part::Foot, Side::Right,segments);
        std::vector<Joint*> rightLowerLimbJoints(3);
        jnt = new Joint("R.Hip", pelvis, Anchor::point("R.HJC"), rightThigh, Anchor::point("R.HJC", pelvis), joints);
        rightLowerLimbJoints[0] = jnt;
        new EulerDescriptor("R.Hip.Angle", EulerDescriptor::YXZ, {{-1.0, 1.0, 1.0}}, jnt);
        new DynamicDescriptor({{0,1,2,1,0,2,0,1,2}}, {{1.,1.,1.,1.,-1.,-1.,1.,1.,-1.}}, jnt);
        jnt = new Joint("R.Knee", rightThigh, rightShank, Anchor::origin(rightThigh), joints);
        rightLowerLimbJoints[1] = jnt;
        new EulerDescriptor("R.Knee.Angle", EulerDescriptor::YXZ, jnt);
        new DynamicDescriptor({{0,1,2,1,0,2,0,1,2}}, {{1.,1.,1.,-1.,-1.,-1.,1.,1.,-1.}}, jnt);
        jnt = new Joint("R.Ankle", rightShank, rightFoot, Anchor::origin(rightShank), joints);
        rightLowerLimbJoints[2] = jnt;
        new PluginGaitRightAnkleDescriptor(jnt);
        new DynamicDescriptor({{0,1,2,1,2,0,0,1,2}}, {{-1.,1.,-1.,1.,1.,-1.,1.,1.,-1.}}, jnt);
        jnt = new Joint("R.Foot.Progress", progression, rightFoot, joints);
        jnt->setDescription("Right foot relative to progression frame");
        new PluginGaitRightFootDescriptor(jnt);
        new Chain("R.LowerLimb", rightLowerLimbJoints, chains);
      }
      jnt = new Joint("Pelvis.Progress", progression, pelvis, joints);
      new PluginGaitPelvisDescriptor(jnt);
    }
    if ((optr->Region & Region::Full) == Region::Full)
    {
      model->setName(this->name() + "_FullBody");
      jnt = new Joint("Spine", torso, pelvis, joints);
      jnt->setDescription("Torso relative to pelvis");
      new PluginGaitSpineDescriptor(jnt);
    }
    return true;
  };

  /**
   * Calibrate this helper based on the first Trial object found in @a trials. If @a subject is not a null pointer, its dynamic properties are copied to this object.
   * @todo Explain how to set custom hip joint centre
   */
  bool PluginGait::calibrate(Node* trials, Subject* subject)
  {
    if (trials == nullptr)
    {
      error("PluginGait - Null trials passed. Calibration aborted.");
      return false;
    }
    auto trial = trials->findChild<Trial*>();
    if (trial == nullptr)
    {
       error("PluginGait - No trial found. Calibration aborted.");
      return false;
    }
    bool ok = false;
    auto landmarks = extract_landmark_positions(this, trial, nullptr, nullptr, &ok);
    if (!ok)
    {
      error("PluginGait - The sampling information is not consistent between required landmarks (sampling rates or start times are not the same). Calibration aborted.");
      return false;
    }
    
    auto optr = this->pimpl();
    // Copy the content of subject into the helper
    if (subject != nullptr)
    {
      const auto& props = subject->dynamicProperties();
      for (const auto& prop : props)
        this->setProperty(prop.first, prop.second);
    }
    
    // Calibrate the helper
    
    // --------------------------------------------------
    // LOWER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Lower)
    {
      math::Position L_HJC(1); L_HJC.residuals().setZero();
      math::Position R_HJC(1); R_HJC.residuals().setZero();
      auto& _R_HJC = R_HJC.values();
      auto& _L_HJC = L_HJC.values();
      const auto& L_ASIS = landmarks["L.ASIS"];
      const auto& R_ASIS = landmarks["R.ASIS"];
      
      //  - inter ASIS distance
      if (optr->InterAsisDistance == 0.0)
      {
        if (!L_ASIS.isValid() || !R_ASIS.isValid())
        {
          error("PluginGait - Missing landmarks to compute the inter ASIS distance. Calibration aborted.");
          return false;
        }
        else
          optr->InterAsisDistance = (L_ASIS - R_ASIS).mean().norm();
      }
      //  - leg length
      double leftLegLength = optr->LeftLegLength;
      double rightLegLength = optr->RightLegLength;
      if ((leftLegLength == 0.0) && (rightLegLength > 0.0))
      {
        warning("PluginGait - Missing model's parameter: unknown left leg length. The algorithm uses only the right leg length.");
        leftLegLength = rightLegLength;
      }
      else if ((leftLegLength > 0.0) && (rightLegLength == 0.0))
      {
        warning("PluginGait - Missing model's parameter: unknown right leg length. The algorithm uses only the left leg length.");
        rightLegLength = leftLegLength;
      }
      else if ((leftLegLength <= 0.0) && (rightLegLength <= 0.0))
      {
        error("PluginGait - Missing model's parameter. Unknown left and right leg lengths. Calibration aborted.");
        return false;
      }
      //  - x_dis parameter
      if (optr->LeftAsisTrochanterAPDistance <= 0.0)
        optr->LeftAsisTrochanterAPDistance = 0.1288 * leftLegLength - 48.56;
      if (optr->RightAsisTrochanterAPDistance <= 0.0)
        optr->RightAsisTrochanterAPDistance = 0.1288 * rightLegLength - 48.56;
      // - hip joint centre
      Point *leftHJCH = nullptr, *rightHJCH = nullptr;
      //   - Left
      if ((leftHJCH = this->findChild<Point*>("L.HJC",{},false)) == nullptr)
      {
        optr->computeHipJointCenter(_L_HJC.data(), -1.0,  leftLegLength * 0.115 - 15.3, optr->LeftAsisTrochanterAPDistance);
        new Point("L.HJC", _L_HJC.data(), this);
      }
      else
        std::copy_n(leftHJCH->data(), 3, _L_HJC.data());
      //   - Right
      if ((rightHJCH = this->findChild<Point*>("R.HJC",{},false)) == nullptr)
      {
        optr->computeHipJointCenter(_R_HJC.data(),  1.0, rightLegLength * 0.115 - 15.3, optr->RightAsisTrochanterAPDistance);
        new Point("R.HJC", _R_HJC.data(), this);
      }
      else
        std::copy_n(rightHJCH->data(), 3, _R_HJC.data());
      // -----------------------------------------
      // Pelvis
      // -----------------------------------------
      // Required landmarks: *.ASIS, SC or *.PSIS
      const auto& L_PSIS = landmarks["L.PSIS"];
      const auto& R_PSIS = landmarks["R.PSIS"];
      math::Position SC = landmarks["SC"];
      if (!L_ASIS.isValid() || !R_ASIS.isValid())
      {
        error("PluginGait - Missing landmarks (ASISs) to define the pelvis. Calibration aborted.");
        return false;
      }
      if (!SC.isValid() && (!L_PSIS.isValid() || !R_PSIS.isValid()))
      {
        error("PluginGait - Missing landmarks (SC or PSISs) to define the pelvis. Calibration aborted.");
        return false;
      }
      if (!SC.isValid())
        SC = (L_PSIS + R_PSIS) / 2.0;
      
      const math::Vector v = (L_ASIS - R_ASIS).normalized();
      const math::Vector w = ((R_ASIS - SC).cross(L_ASIS - SC)).normalized();
      const math::Pose pelvis(v.cross(w), v, w, (L_ASIS + R_ASIS) / 2.0);
      // Set the segment length
      // NOTE: the coefficient 0.828 comes from Vicon
      this->setProperty("Pelvis.length", 0.828 * (_L_HJC - _R_HJC).matrix().norm());
      // -----------------------------------------
      // Other lower limbs (dependant of the hip joint centres)
      // -----------------------------------------
      if ((optr->Side & Side::Left) == Side::Left)
      {
        const math::Position HJC = pelvis.transform(L_HJC.replicate(pelvis.rows()));
        if (!optr->calibrateLowerLimb(Side::Left, &HJC, &landmarks))
          return false;
      }
      if ((optr->Side & Side::Right) == Side::Right)
      {
        const math::Position HJC = pelvis.transform(R_HJC.replicate(pelvis.rows()));
        if (!optr->calibrateLowerLimb(Side::Right, &HJC, &landmarks))
          return false;
       }
    }
    // --------------------------------------------------
    // UPPER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Upper)
    {
#if !defined(_MSC_VER)
#warning LENGTHS OF UPPERLIMB SEGMENTS ARE NULL
#endif
      this->setProperty("L.Arm.length", 0);
      this->setProperty("L.Forearm.length", 0);
      this->setProperty("L.Hand.length", 0);
      this->setProperty("R.Arm.length", 0);
      this->setProperty("R.Forearm.length", 0);
      this->setProperty("R.Hand.length", 0);
      this->setProperty("Torso.length", 0);
      this->setProperty("Head.length", 0);
      if (optr->HeadOffsetEnabled)
      {
        // -----------------------------------------
        // Head
        // -----------------------------------------
        // Required landmarks: L.HF, L.HB, R.HF and R.HB
        const auto& L_HF = landmarks["L.HF"];
        const auto& L_HB = landmarks["L.HB"];
        const auto& R_HF = landmarks["R.HF"];
        const auto& R_HB = landmarks["R.HB"];
        if (!L_HF.isValid() || !L_HB.isValid() || !R_HF.isValid() || !R_HB.isValid())
        {
          error("PluginGait - Missing landmarks to define the head. Calibration aborted.");
          return false;
        }
        // NOTE : The markers are first averaged before the computation of the offset!
        const math::Position _L_HF = L_HF.mean();
        const math::Position _L_HB = L_HB.mean();
        const math::Position _R_HF = R_HF.mean();
        const math::Position _R_HB = R_HB.mean();        
        // WARNING: The origin (set to the middle of the four points) is not the same than Vicon!
        const math::Vector u = ((_L_HF + _R_HF) / 2.0 - (_L_HB + _R_HB) / 2.0).normalized();
        const math::Vector w = u.cross((_L_HF + _L_HB) / 2.0 - (_R_HF + _R_HB) / 2.0).normalized();
        const math::Pose head(u, w.cross(u), w, (_L_HF + _R_HF + _L_HB + _R_HB) / 4.0);
        if (head.isOccluded())
        {
          error("PluginGait - Impossible to find a least one valid frame for the head motion. Calibration aborted.");
          return false;
        }
        optr->HeadOffset = -1.0 * head.eulerAngles(2,0,1).mean().values().z();
      }
    }
    return true;
  };
  
  /**
   * Reconstruct @a model segments' movement based on the content of the given @a trial.
   * @a todo Explain that exported segment frame corresponds to the segmental coordinate system (SCS).
   * @a todo Explain that hip joint centre stored in this helper are cloned in the model. Like that, the helper can modify theses HJC for different tests, while the model store data used for its reconstruction.
   */
  bool PluginGait::reconstructModel(Model* model, Trial* trial)
  {
    if (model == nullptr)
    {
      error("PluginGait - Null model passed. Movement reconstruction aborted.");
      return false;
    }
    if (trial == nullptr)
    {
      error("PluginGait - Null trial passed. Movement reconstruction aborted.");
      return false;
    }
    model->setName(model->name() + "_" + trial->name());
    double sampleRate = 0.0;
    double startTime = 0.0;
    bool ok = false;
    auto landmarks = extract_landmark_positions(this, trial, &sampleRate, &startTime, &ok);
    if (!ok)
    {
      error("PluginGait - The sampling information are no consistent (sampling rate or start time not the same) between required landmarks. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    auto optr = this->pimpl();
    Segment* seg = nullptr;
    TimeSequence *torsoSCS = nullptr, *pelvisSCS = nullptr;
    // --------------------------------------------------
    // UPPER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Upper)
    {
      // -----------------------------------------
      // Torso
      // -----------------------------------------
      seg = model->segments()->findChild<Segment*>({},{{"part",Part::Torso}},false);
      // Required landmarks: C7, T10, SS and XP
      const auto& C7 = landmarks["C7"];
      const auto& T10 = landmarks["T10"];
      const auto& SS = landmarks["SS"];
      const auto& XP = landmarks["XP"];
      if (!C7.isValid() || !T10.isValid() || !SS.isValid() || !XP.isValid())
      {
        error("PluginGait - Missing landmarks to define the torso. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
        return false;
      }
      const math::Vector w = ((XP + T10) / 2.0 - (SS + C7) / 2.0).normalized();
      const math::Vector v = w.cross((XP + SS) / 2.0 - (T10 + C7) / 2.0).normalized();
      const math::Vector u = v.cross(w);
      const math::Vector o = SS - (optr->MarkerDiameter / 2.0 * u);
      torsoSCS = math::to_timesequence(u, v, w, o, seg->name()+".SCS", sampleRate, startTime, seg);
      seg->setProperty("length", this->property(seg->name()+".length"));
      // -----------------------------------------
      // Other upper limbs (dependant of the torso)
      // -----------------------------------------
      if ((optr->Side & Side::Left) && !optr->reconstructUpperLimb(model, trial, Side::Left, &u,  &o, &landmarks, sampleRate, startTime))
        return false;
      if ((optr->Side & Side::Right) && !optr->reconstructUpperLimb(model, trial, Side::Right, &u, &o, &landmarks, sampleRate, startTime))
        return false;
    }
    // --------------------------------------------------
    // LOWER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Lower)
    {
      // -----------------------------------------
      // Pelvis
      // -----------------------------------------
      seg = model->segments()->findChild<Segment*>({},{{"part",Part::Pelvis}},false);
      // - Hip joint centre
      Point *leftHJCH = nullptr, *rightHJCH = nullptr;
      //   - Left
      if ((leftHJCH = this->findChild<Point*>("L.HJC",{},false)) == nullptr)
      {
        error("PluginGait - Left relative hip joint centre not found. Did you calibrate first the helper? Movement reconstruction aborted.");
        return false;
      }
      //   - Right
      if ((rightHJCH = this->findChild<Point*>("R.HJC",{},false)) == nullptr)
      {
        error("PluginGait - Right relative hip joint centre not found. Did you calibrate first the helper? Movement reconstruction aborted.");
        return false;
      }
      // - Construct the relative segment coordinate system for the pelvis
      auto relframe = new ReferenceFrame("SCS", nullptr, seg);
      relframe->o()[0] = (leftHJCH->data()[0] + rightHJCH->data()[0]) / 2.0;
      relframe->o()[1] = (leftHJCH->data()[1] + rightHJCH->data()[1]) / 2.0;
      relframe->o()[2] = (leftHJCH->data()[2] + rightHJCH->data()[2]) / 2.0;
      // Add (L|R).HJC nodes to the model
      auto leftHipJointCenter = leftHJCH->clone(seg);
      auto rightHipJointCenter = rightHJCH->clone(seg);

      // Required landmarks: *.ASIS, SC or *.PSIS
      const auto& L_ASIS = landmarks["L.ASIS"];
      const auto& R_ASIS = landmarks["R.ASIS"];
      const auto& L_PSIS = landmarks["L.PSIS"];
      const auto& R_PSIS = landmarks["R.PSIS"];
      math::Position SC = landmarks["SC"];
      if (!L_ASIS.isValid() || !R_ASIS.isValid())
      {
        error("PluginGait - Missing landmarks (ASISs) to define the pelvis. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
        return false;
      }
      if (!SC.isValid() && (!L_PSIS.isValid() || !R_PSIS.isValid()))
      {
        error("PluginGait - Missing landmarks (SC or PSISs) to define the pelvis. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
        return false;
      }
      if (!SC.isValid())
        SC = (L_PSIS + R_PSIS) / 2.0;
      const math::Vector v = (L_ASIS - R_ASIS).normalized();
      const math::Vector w = ((R_ASIS - SC).cross(L_ASIS - SC)).normalized();
      const math::Pose pelvis(v.cross(w), v, w, (L_ASIS + R_ASIS) / 2.0);
      math::to_timesequence(pelvis, seg->name()+".TCS", sampleRate, startTime, TimeSequence::Pose, "", seg);
      seg->setProperty("length", this->property(seg->name()+".length"));
      pelvisSCS = math::to_timesequence(transform_relative_frame(relframe, seg, pelvis), seg->name()+".SCS", sampleRate, startTime, TimeSequence::Pose, "", relframe);
      // -----------------------------------------
      // Thigh, shank, foot
      // -----------------------------------------
      if ((optr->Side & Side::Left) == Side::Left)
      {
        const math::Position HJC = transform_relative_point(leftHipJointCenter, seg, pelvis);
        if (!optr->reconstructLowerLimb(model, trial, Side::Left, &HJC, &landmarks, sampleRate, startTime))
          return false;
      }
      if ((optr->Side & Side::Right) == Side::Right)
      {
        const math::Position HJC = transform_relative_point(rightHipJointCenter, seg, pelvis);
        if (!optr->reconstructLowerLimb(model, trial, Side::Right, &HJC, &landmarks, sampleRate, startTime))
          return false;
      }
    }
    
    // --------------------------------------------------
    // Other Segment
    // --------------------------------------------------
    
    // -----------------------------------------
    // Progression frame
    // -----------------------------------------
    // Try to find a trajectory can inform us on the direction of progression of the movement
    // WARNING: This method is not robust in the case where there is a return!
    seg = model->segments()->findChild<Segment*>("Progression",{{"part", Part::User}},false);
    {
      // Keep in local scope
      TimeSequence* scs = nullptr;
      if (optr->Region & Region::Lower)
        scs = pelvisSCS;
      else if (optr->Region & Region::Upper)
        scs = torsoSCS;
      if ((scs == nullptr) || (scs->samples() == 0))
      {
        error("Missing data to compute the progression frame. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
        return false;
      }
      // Valid first and last samples
      unsigned fs = 0, ls = scs->samples()-1;
      while(scs->data(fs,12) < 0.0) ++fs;
      while(scs->data(ls,12) < 0.0) --ls;
      if (ls <= fs)
      {
        error("Not enough samples to compute the progression frame. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
        return false;
      }
      // Find the main axis
      double udiff[3] = { scs->data(ls,9)  - scs->data(fs,9),
                          scs->data(ls,10) - scs->data(fs,10),
                          scs->data(ls,11) - scs->data(fs,11) };
      unsigned uidx = 0;
      if (fabs(udiff[1]) > fabs(udiff[uidx])) uidx = 1;
      if (fabs(udiff[2]) > fabs(udiff[uidx])) uidx = 2;
      math::Vector::Values u(1,3);
      switch(uidx)
      {
      case 0:
        u << sign(udiff[uidx]), 0., 0.;
        break;
      case 1:
        u << 0., sign(udiff[uidx]), 0.;
        break;
      case 2:
        u << 0., 0., sign(udiff[uidx]);
        break;
      default:
        error("Internal error during the computation of the progression frame (u axis). Please contact the developers.");
        return false;
      }
      // Find the vertical axis
      double wsum[3] = { (scs->data(ls,6) + scs->data(fs,6)) / 2.0,
                         (scs->data(ls,7) + scs->data(fs,7)) / 2.0,
                         (scs->data(ls,8) + scs->data(fs,8)) / 2.0 };
      unsigned widx = 0;
      if (fabs(wsum[1]) > fabs(wsum[widx])) widx = 1;
      if (fabs(wsum[2]) > fabs(wsum[widx])) widx = 2;
      math::Vector::Values w(1,3);
      switch(widx)
      {
      case 0:
        w << 1., 0., 0.;
        break;
      case 1:
        w << 0., 1., 0.;
        break;
      case 2:
        w << 0., 0., 1.;
        break;
      default:
        error("Internal error during the computation of the progression frame (w axis). Please contact the developers.");
        return false;
      }
      // Create the TimeSequence representing the pose of the progression frame
      math::Vector::Values v(1,3);
      v << w.coeff(1) * u.coeff(2) - u.coeff(1) * w.coeff(2),
           w.coeff(2) * u.coeff(0) - u.coeff(2) * w.coeff(0),
           w.coeff(0) * u.coeff(1) - u.coeff(0) * w.coeff(1);
      auto progression = new TimeSequence(seg->name()+".SCS", 13, scs->samples(), sampleRate, startTime, TimeSequence::Pose, "", seg);
      double* data = progression->data();
      for (size_t i = 0, len = scs->samples() ; i < len ; ++i)
      {
        data[i]        = u.coeff(0); data[i+   len] = u.coeff(1); data[i+ 2*len] = u.coeff(2);
        data[i+ 3*len] = v.coeff(0); data[i+ 4*len] = v.coeff(1); data[i+ 5*len] = v.coeff(2);
        data[i+ 6*len] = w.coeff(0); data[i+ 7*len] = w.coeff(1); data[i+ 8*len] = w.coeff(2);
        data[i+ 9*len] = 0.        ; data[i+10*len] = 0.        ; data[i+11*len] = 0.;
        data[i+12*len] = 0.;
      }
    }
    
    // -----------------------------------------
    // Head
    // -----------------------------------------
    seg = model->segments()->findChild<Segment*>({},{{"part", Part::Head}},false);
    if (seg != nullptr)
    {
      // Required landmarks: L.HF, L.HB, R.HF and R.HB
      const auto& L_HF = landmarks["L.HF"];
      const auto& L_HB = landmarks["L.HB"];
      const auto& R_HF = landmarks["R.HF"];
      const auto& R_HB = landmarks["R.HB"];
      if (!L_HF.isValid() || !L_HB.isValid() || !R_HF.isValid() || !R_HB.isValid())
      {
        error("PluginGait - Missing landmarks to define the head. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
        return false;
      }
      const math::Vector u = ((L_HF + R_HF) / 2.0 - (L_HB + R_HB) / 2.0).normalized();
      const math::Vector w = u.cross((L_HF + L_HB) / 2.0 - (R_HF + R_HB) / 2.0).normalized();
      const math::Vector v = w.cross(u);
      const math::Vector o = (L_HF + R_HF + L_HB + R_HB) / 4.0;
      // Rotate the head frame along its axis Y using the offset "HeadOffset"
      const double cy = cos(optr->HeadOffset), sy = sin(optr->HeadOffset);
      const math::Vector ur = u * cy - w * sy;
      const math::Vector wr = u * sy + w * cy;
      math::to_timesequence(ur, v, wr, o, seg->name()+".SCS", sampleRate, startTime, seg);
      seg->setProperty("length", this->property(seg->name()+".length"));
    }
    return true;
  };

  // ----------------------------------------------------------------------- //
  
  /**
   * Returns the marker diameter used in the calibration and reconstructor steps.
   */
  double PluginGait::markerDiameter() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->MarkerDiameter;
  };
  
  /**
   * Sets the markers diameter used in the calibration and reconstructor steps. 
   */
  void PluginGait::setMarkerDiameter(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->MarkerDiameter) < std::numeric_limits<double>::epsilon())
      return;
    optr->MarkerDiameter = value;
    this->modified();
  };
  
  /**
   * Return the internal parameter HeadOffsetEnabled
   */
  bool PluginGait::headOffsetEnabled() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->HeadOffsetEnabled;
  };
  
  /**
   * Sets the internal parameter HeadOffsetEnabled
   */
  void PluginGait::setHeadOffsetEnabled(bool value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->HeadOffsetEnabled)
      return;
    optr->HeadOffsetEnabled = value;
    this->modified();
  };

  /**
   * Returns the internal parameter HeadOffset.
   */
  double PluginGait::headOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->HeadOffset;
  };
  
  /**
   * Returns the internal parameter RightShoulderOffset.
   */
  double PluginGait::rightShoulderOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightShoulderOffset;
  };
  
  /**
   * Set the internal parameter RightShoulderOffset.
   */
  void PluginGait::setRightShoulderOffset(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightShoulderOffset) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightShoulderOffset = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftShoulderOffset.
   */
  double PluginGait::leftShoulderOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftShoulderOffset;
  };
  
  /**
   * Set the internal parameter LeftShoulderOffset.
   */
  void PluginGait::setLeftShoulderOffset(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftShoulderOffset) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftShoulderOffset = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightElbowWidth.
   */
  double PluginGait::rightElbowWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightElbowWidth;
  };
  
  /**
   * Set the internal parameter RightElbowWidth.
   */
  void PluginGait::setRightElbowWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightElbowWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightElbowWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftElbowWidth.
   */
  double PluginGait::leftElbowWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftElbowWidth;
  };
  
  /**
   * Set the internal parameter LeftElbowWidth.
   */
  void PluginGait::setLeftElbowWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftElbowWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftElbowWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightWristWidth.
   */
  double PluginGait::rightWristWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightWristWidth;
  };
  
  /**
   * Set the internal parameter RightWristWidth.
   */
  void PluginGait::setRightWristWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightWristWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightWristWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftWristWidth.
   */
  double PluginGait::leftWristWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftWristWidth;
  };
  
  /**
   * Set the internal parameter LeftWristWidth.
   */
  void PluginGait::setLeftWristWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftWristWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftWristWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightHandThickness.
   */
  double PluginGait::rightHandThickness() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightHandThickness;
  };
  
  /**
   * Set the internal parameter RightHandThickness.
   */
  void PluginGait::setRightHandThickness(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightHandThickness) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightHandThickness = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftHandThickness.
   */
  double PluginGait::leftHandThickness() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftHandThickness;
  };
  
  /**
   * Set the internal parameter LeftHandThickness.
   */
  void PluginGait::setLeftHandThickness(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftHandThickness) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftHandThickness = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter InterAsisDistance.
   */
  double PluginGait::interAsisDistance() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->InterAsisDistance;
  };
  
  /**
   * Set the internal parameter InterAsisDistance.
   */
  void PluginGait::setInterAsisDistance(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->InterAsisDistance) < std::numeric_limits<double>::epsilon())
      return;
    optr->InterAsisDistance = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightLegLength.
   */
  double PluginGait::rightLegLength() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightLegLength;
  };
  
  /**
   * Set the internal parameter RightLegLength.
   */
  void PluginGait::setRightLegLength(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightLegLength) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightLegLength = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftLegLength.
   */
  double PluginGait::leftLegLength() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftLegLength;
  };
  
  /**
   * Set the internal parameter LeftLegLength.
   */
  void PluginGait::setLeftLegLength(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftLegLength) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftLegLength = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightAsisTrochanterAPDistance.
   */
  double PluginGait::rightAsisTrochanterAPDistance() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightAsisTrochanterAPDistance;
  };
  
  /**
   * Set the internal parameter RightAsisTrochanterAPDistance.
   */
  void PluginGait::setRightAsisTrochanterAPDistance(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightAsisTrochanterAPDistance) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightAsisTrochanterAPDistance = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftAsisTrochanterAPDistance.
   */
  double PluginGait::leftAsisTrochanterAPDistance() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftAsisTrochanterAPDistance;
  };
  
  /**
   * Set the internal parameter LeftAsisTrochanterAPDistance.
   */
  void PluginGait::setLeftAsisTrochanterAPDistance(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftAsisTrochanterAPDistance) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftAsisTrochanterAPDistance = value;
    this->modified();
  };
  
 /**
   * Returns the internal parameter RightKneeWidth.
  */
  double PluginGait::rightKneeWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightKneeWidth;
  };
  
  /**
   * Set the internal parameter RightKneeWidth.
   */
  void PluginGait::setRightKneeWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightKneeWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightKneeWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftKneeWidth.
   */
  double PluginGait::leftKneeWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftKneeWidth;
  };
  
  /**
   * Set the internal parameter LeftKneeWidth.
   */
  void PluginGait::setLeftKneeWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftKneeWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftKneeWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightAnkleWidth.
   */
  double PluginGait::rightAnkleWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightAnkleWidth;
  };
  
  /**
   * Set the internal parameter RightAnkleWidth.
   */
  void PluginGait::setRightAnkleWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightAnkleWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightAnkleWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftAnkleWidth.
   */
  double PluginGait::leftAnkleWidth() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftAnkleWidth;
  };
  
  /**
   * Set the internal parameter LeftAnkleWidth.
   */
  void PluginGait::setLeftAnkleWidth(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftAnkleWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftAnkleWidth = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightFootFlatEnabled.
   */
  bool PluginGait::rightFootFlatEnabled() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightFootFlatEnabled;
  };
  
  /**
   * Set the internal parameter RightFootFlatEnabled.
   */
  void PluginGait::setRightFootFlatEnabled(bool value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->RightFootFlatEnabled)
      return;
    optr->RightFootFlatEnabled = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftFootFlatEnabled.
   */
  bool PluginGait::leftFootFlatEnabled() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftFootFlatEnabled;
  };
  
  /**
   * Set the internal parameter LeftFootFlatEnabled.
   */
  void PluginGait::setLeftFootFlatEnabled(bool value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->LeftFootFlatEnabled)
      return;
    optr->LeftFootFlatEnabled = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter RightStaticPlantarFlexionOffset.
   */
  double PluginGait::rightStaticPlantarFlexionOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightStaticPlantarFlexionOffset;
  };
  
  /**
   * Returns the internal parameter RightStaticRotationOffset.
   */
  double PluginGait::rightStaticRotationOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightStaticRotationOffset;
  };
  
  /**
   * Returns the internal parameter LeftStaticPlantarFlexionOffset.
   */
  double PluginGait::leftStaticPlantarFlexionOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftStaticPlantarFlexionOffset;
  };
  
  /**
   * Returns the internal parameter LeftStaticRotationOffset.
   */
  double PluginGait::leftStaticRotationOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftStaticRotationOffset;
  };
  
  // ----------------------------------------------------------------------- //
  
  /*
   * Generate a landmarks translator for the PiG model.
   * The following list presents the label used in the PluginGait helper and its translation in OpenMA:
   *  - LFHD: L.HF   (Left front head: approximately over the left)
   *  - LBHD: L.HB   (Left back head: on the back of the head)
   *  - RFHD: R.HF   (Right front head: approximately over the)
   *  - RBHD: R.HB   (Right back head: on the back of the head)
   *  - C7:   C7     (Seventh cervical vertebra: also named vertebra prominens)
   *  - T10:  T10    (Spinous Process of the 10th thoracic vertebrae)
   *  - CLAV: SS     (Suprasternal notch: superior border of the manubrium of the sternum, also known as the jugular notch)
   *  - STRN: XP     (Xiphoid process: extension to the lower part of the sternum)
   *  - LSHO: L.AC   (Left acromion - Acromio-clavicular joint)
   *  - RSHO: R.AC   (Right acromion (Acromio-clavicular joint))
   *  - LELB: L.LHE  (Left lateral humerus epicondyle)
   *  - LWRB: L.US   (Left ulnar styloid process - wrist bar pinkie)
   *  - LWRA: L.RS   (Left radius styloid process - wrist bar thumb)
   *  - RELB: R.LHE  (Right lateral humerus epicondyle)
   *  - RWRB: R.US   (Right ulnar styloid process)
   *  - RWRA: R.RS   (Right radius styloid process)
   *  - LFIN: L.MH2  (Left head of the second metacarpus)
   *  - RFIN: R.MH2  (Right head of the second metacarpus)
   *  - LASI: L.ASIS (Left Anterior Superior Iliac Spine)
   *  - RASI: R.ASIS (Right Anterior Superior Iliac Spine)
   *  - LPSI: L.PSIS (Left Posterior Superior Iliac Spine)
   *  - RPSI: R.PSIS (Right Posterior Superior Iliac Spine)
   *  - SACR: SC     (Midpoint between Left and right PSIS)
   *  - LTHI: L.ITB  (Left Iliotibial Band)
   *  - RTHI: R.ITB  (Right Iliotibial Band)
   *  - LKNE: L.LFE  (Left lateral epicondyle of the femur)
   *  - RKNE: R.LFE  (Right lateral epicondyle of the femur)
   *  - LTIB: L.LS   (Left lateral shank marker)
   *  - RTIB: R.LS   (Right lateral shank marker)
   *  - LANK: L.LTM  (Left lateral malleolus of the tibia)
   *  - RANK: R.LTM  (Right lateral malleolus of the tibia)
   *  - LTOE: L.MTH2 (Head of the second metatarsal for the left)
   *  - RTOE: R.MTH2 (Head of the second metatarsal for the right)
   *  - LHEE: L.HEE  (Left heel marker)
   *  - RHEE: R.HEE  (Right heel marker)
   */
  LandmarksTranslator* PluginGait::defaultLandmarksTranslator()
  {
    return new LandmarksTranslator(
      "LandmarksTranslator", 
      {
        {"LFHD", "L.HF"},
        {"LBHD", "L.HB"},
        {"RFHD", "R.HF"},
        {"RBHD", "R.HB"},
        {"C7"  , "C7"}, // same
        {"T10" , "T10"}, // same
        {"CLAV", "SS"},
        {"STRN", "XP"},
        {"LSHO", "L.AC"},
        {"RSHO", "R.AC"},
        {"LELB", "L.LHE"},
        {"LWRB", "L.US"},
        {"LWRA", "L.RS"},
        {"RELB", "R.LHE"},
        {"RWRB", "R.US"},
        {"RWRA", "R.RS"},
        {"LFIN", "L.MH2"},
        {"RFIN", "R.MH2"},
        {"LASI", "L.ASIS"},
        {"RASI", "R.ASIS"},
        {"LPSI", "L.PSIS"},
        {"RPSI", "R.PSIS"},
        {"SACR", "SC"},
        {"LTHI", "L.ITB"},
        {"RTHI", "R.ITB"},
        {"LKNE", "L.LFE"},
        {"RKNE", "R.LFE"},
        {"LTIB", "L.LS"},
        {"RTIB", "R.LS"},
        {"LANK", "L.LTM"},
        {"RANK", "R.LTM"},
        {"LTOE", "L.MTH2"},
        {"RTOE", "R.MTH2"},
        {"LHEE", "L.HEE"},
        {"RHEE", "R.HEE"},
      }, this);
  };
  
  /*
   * Returns a SkeletonHelperPoseEstimator node
   */
  PoseEstimator* PluginGait::defaultPoseEstimator()
  {
    return new SkeletonHelperPoseEstimator("PluginGaitPoseEstimator", this);
  };

  /*
   * Returns a DempsterTable node
   */
  InertialParametersEstimator* PluginGait::defaultInertialParametersEstimator()
  {
    return new DempsterTable(this);
  };
  
  /*
   * Returns a SimpleGaitForcePlateToFeetAssigner node
   */
  ExternalWrenchAssigner* PluginGait::defaultExternalWrenchAssigner()
  {
    return new SimpleGaitForcePlateToFeetAssigner(this);
  };
  
  /*
   * Returns a InverseDynamicMatrix node
   */
  InverseDynamicProcessor* PluginGait::defaultInverseDynamicProcessor()
  {
    return new InverseDynamicMatrix(this);
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  PluginGait* PluginGait::clone(Node* parent) const
  {
    auto dest = new PluginGait(0,0);
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void PluginGait::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const PluginGait*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->SkeletonHelper::copy(src);
    optr->MarkerDiameter = optr_src->MarkerDiameter;
    optr->HeadOffsetEnabled = optr_src->HeadOffsetEnabled;
    optr->RightShoulderOffset = optr_src->RightShoulderOffset;
    optr->LeftShoulderOffset = optr_src->LeftShoulderOffset;
    optr->RightElbowWidth = optr_src->RightElbowWidth;
    optr->LeftElbowWidth = optr_src->LeftElbowWidth;
    optr->RightWristWidth = optr_src->RightWristWidth;
    optr->LeftWristWidth = optr_src->LeftWristWidth;
    optr->RightHandThickness = optr_src->RightHandThickness;
    optr->LeftHandThickness = optr_src->LeftHandThickness;
    optr->InterAsisDistance = optr_src->InterAsisDistance;
    optr->RightLegLength = optr_src->RightLegLength;
    optr->LeftLegLength = optr_src->LeftLegLength;
    optr->RightAsisTrochanterAPDistance = optr_src->RightAsisTrochanterAPDistance;
    optr->LeftAsisTrochanterAPDistance = optr_src->LeftAsisTrochanterAPDistance;
    optr->RightKneeWidth = optr_src->RightKneeWidth;
    optr->LeftKneeWidth = optr_src->LeftKneeWidth;
    optr->RightAnkleWidth = optr_src->RightAnkleWidth;
    optr->LeftAnkleWidth = optr_src->LeftAnkleWidth;
    optr->RightFootFlatEnabled = optr_src->RightFootFlatEnabled;
    optr->LeftFootFlatEnabled = optr_src->LeftFootFlatEnabled;
    optr->HeadOffset = optr_src->HeadOffset;
    optr->RightStaticPlantarFlexionOffset = optr_src->RightStaticPlantarFlexionOffset;
    optr->RightStaticRotationOffset = optr_src->RightStaticRotationOffset;
    optr->LeftStaticPlantarFlexionOffset = optr_src->LeftStaticPlantarFlexionOffset;
    optr->LeftStaticRotationOffset = optr_src->LeftStaticRotationOffset;
  };
};
};