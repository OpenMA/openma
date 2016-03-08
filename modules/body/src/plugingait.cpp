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

#include "openma/body/enums.h"
#include "openma/body/joint.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/body/model.h"
#include "openma/body/referenceframe.h"
#include "openma/body/segment.h"
#include "openma/body/utils.h"
#include "openma/base/subject.h"
#include "openma/base/trial.h"
#include "openma/base/logger.h"

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
    HeadOffset(0.0),
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
    RightFootFlat(false), 
    LeftFootFlat(false),
    RightStaticPlantarFlexionOffset(0.0),
    RightStaticRotationOffset(0.0),
    LeftStaticPlantarFlexionOffset(0.0),
    LeftStaticRotationOffset(0.0)
  {};
    
  PluginGaitPrivate::~PluginGaitPrivate() _OPENMA_NOEXCEPT = default;
  
  bool PluginGaitPrivate::calibrateLowerLimb(int side, const maths::Position* HJC, ummp* landmarks) _OPENMA_NOEXCEPT
  {
    std::string prefix;
    double s = 0.0, ankleWidth = 0.0, kneeWidth = 0.0;
    double *staticPlantarFlexionOffset = nullptr, *staticRotationOffset = nullptr;
    bool footFlat = false;
    if (side == Side::Left)
    {
      prefix = "L.";
      s = 1.0;
      ankleWidth = this->LeftAnkleWidth;
      kneeWidth = this->LeftKneeWidth;
      footFlat = this->LeftFootFlat;
      staticPlantarFlexionOffset = &(this->LeftStaticPlantarFlexionOffset);
      staticRotationOffset = &(this->LeftStaticRotationOffset);
    }
    else if (side == Side::Right)
    {
      prefix = "R.";
      s = -1.0;
      ankleWidth = this->RightAnkleWidth;
      kneeWidth = this->RightKneeWidth;
      footFlat = this->RightFootFlat;
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
    const maths::Position KJC = compute_chord((this->MarkerDiameter + kneeWidth) / 2.0, LFE, *HJC, ITB);
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
    const maths::Position AJC = compute_chord((this->MarkerDiameter + ankleWidth) / 2.0, LTM, KJC, LS);
    
    // -----------------------------------------
    // Foot
    // -----------------------------------------
    // Required landmarks: *.MTH2, *.HEE
    const auto& MTH2 = (*landmarks)[prefix+"MTH2"];
    maths::Position HEE = (*landmarks)[prefix+"HEE"]; // Copy instead of a map due to possible modification on its coordinates if the foot flat option is activated
    if (!MTH2.isValid() || !HEE.isValid())
    {
      error("PluginGait - Missing landmarks to define the foot. Calibration aborted.");
      return false;
    }
    if (footFlat)
    {
      // Copy the Z-coordinates of MTH2 into HEE
      // FIXME: This assume that the vertical axis is always Z!
      HEE.block<1>(2) = MTH2.block<1>(2);
    }
    // Shank axes
    maths::Vector w = (KJC - AJC).normalized();
    maths::Vector u = s * (KJC - AJC).cross(LS - AJC).normalized();
    maths::Vector v_shank = w.cross(u);
    // Foot reference
    w = (MTH2 - HEE).normalized();
    u = v_shank.cross(w).normalized();
    maths::Pose foot(u,w.cross(u),w,AJC);
    // Uncorrected foot reference
    w = (MTH2 - AJC).normalized();
    u = v_shank.cross(w).normalized();
    maths::Pose uncorrected_foot(u,w.cross(u),w,AJC);

    // Offset angles
    if (foot.isOccluded() || uncorrected_foot.isOccluded())
    {
      error("PluginGait - Impossible to find a least one valid frame for the foot motion. Calibration aborted.");
      return false;
    }
    maths::Vector::Values offsetAngles = uncorrected_foot.inverse().transform(foot).eulerAngles(1,0,2).mean().values();
    *staticPlantarFlexionOffset = 1.0 * offsetAngles.coeff(0);
    *staticRotationOffset = -1.0 * s * offsetAngles.coeff(1);
    return true;
  };
  
  bool PluginGaitPrivate::reconstructUpperLimb(Model* model, Trial* trial, int side, const maths::Vector* u_torso, const maths::Vector* o_torso, ummp* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT
  {
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
    maths::Vector u, v, w, o;
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
    const maths::Position VWM = (AC - *o_torso).cross(*u_torso) + AC;
    // Compute the shoulder joint centre (SJC)
    const maths::Position SJC = compute_chord(s * (shoulderOffset + this->MarkerDiameter / 2.0), AC, *o_torso, VWM);
    // Clavicle
    Segment* clavicle = model->segments()->findChild<Segment*>({},{{"side",side},{"part",Part::Clavicle}},false);
    if (clavicle != nullptr)
    {
      w = (*o_torso - SJC).normalized();
      u = w.cross(VWM - SJC).normalized();
      v = w.cross(u);
      maths::to_timesequence(u, v, w, SJC, prefix+"Clavicle.SCS", sampleRate, startTime, trial->timeSequences());
    }
    // Construction of the vector marker (CVM) for the elbow
    const maths::Position MWP = (US + RS) / 2.0; // Middle Wrist Point
    const maths::Position CVM = s * (MWP - LHE).cross(SJC - LHE).normalized() + LHE;
    const maths::Position EJC = compute_chord((elbowWidth + this->MarkerDiameter) / 2.0, LHE, SJC, CVM);
    // Compute the wrist joint centre (WJC)
    // NOTE: The scale factor was found experimentaly when comparing with the Vicon data.
    const maths::Position WJC = s * (US - RS).cross(EJC - MWP).normalized() * wristWidth * 0.7 + MWP;
    // Arm frame
    w = (SJC - EJC).normalized();
    v = (WJC - EJC).cross(w).normalized();
    u = v.cross(w);
    maths::to_timesequence(u, v, w, EJC, prefix+"Arm.SCS", sampleRate, startTime, trial->timeSequences());
    // -----------------------------------------
    // Forearm
    // -----------------------------------------
    w = (EJC - WJC).normalized();
    u = v.cross(w); // The 'v' axis is the same than the one defined for the arm
    maths::to_timesequence(u, v, w, WJC, prefix+"Forearm.SCS", sampleRate, startTime, trial->timeSequences());
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
    // HO: Hand Origin
    const maths::Position HO = compute_chord((handThickness + this->MarkerDiameter) / 2.0, MH2, WJC, MWP);
    o =  2.0 * (HO - WJC) + WJC;
    w = (WJC - HO).normalized();
    u = s * w.cross(US - RS).normalized();
    v = w.cross(u);
    maths::to_timesequence(u, v, w, o, prefix+"Hand.SCS", sampleRate, startTime, trial->timeSequences());
    return true;
  };
  
  bool PluginGaitPrivate::reconstructLowerLimb(Trial* trial, int side, const maths::Position* HJC, ummp* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT
  {
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
    // Temporary variable use to construct segments' motion
    maths::Vector u,v,w,o;
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
    const maths::Position KJC = compute_chord((this->MarkerDiameter + kneeWidth) / 2.0, LFE, *HJC, ITB);
    u = s * (*HJC - LFE).cross(ITB - LFE).normalized();
    w = (*HJC - KJC).normalized();
    v = w.cross(u);
    maths::to_timesequence(u, v, w, KJC, prefix+"Thigh.SCS", sampleRate, startTime, trial->timeSequences());
    
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
    // Compute the ankle joint centre (AJC)
    const maths::Position AJC = compute_chord((this->MarkerDiameter + ankleWidth) / 2.0, LTM, KJC, LS);
    w = (KJC - AJC).normalized();
    u = s * w.cross(LS - AJC).normalized();
    maths::Vector v_shank = w.cross(u);
    maths::to_timesequence(u, v_shank, w, AJC, prefix+"Shank.SCS", sampleRate, startTime, trial->timeSequences());
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
    
    w = -1.0 * (MTH2 - AJC).normalized();
    u = v_shank.cross(w).normalized();
    v = w.cross(u);
    const double cx = cos(staticRotationOffset), sx = sin(staticRotationOffset),
                 cy = cos(staticPlantarFlexionOffset), sy = sin(staticPlantarFlexionOffset);
    
    const maths::Vector ur = u * cy - w * sy;
    const maths::Vector vr = u * sx*sy + v * cx + w * cy*sx;
    const maths::Vector wr = u * cx*sy - v * sx + w * cx*cy;
    // FIXME: This is not the good origin. None anatomical meaning was discovered to explain this position...
    maths::to_timesequence(ur, vr, wr, AJC, prefix+"Foot.SCS", sampleRate, startTime, trial->timeSequences());
    return true;
  };
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace body
{
  /**
   * @class PluginGait openma/body/plugingait.h
   * @brief Helper class to create and reconstruct a model based on the Plug in Gait marker set.
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
   * [Calculated] This property holds the head offset angle. By default, this property contains the value 0.0.
   * @sa headOffset() setHeadOffset()
   */
  double HeadOffset;
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
   * @sa rightFootFlat() setRightFootFlat()
   */
  bool RightFootFlat;
  /**
   * [Required] This property holds the confirmation the subject is able to stand with the foot on the ground.  By default, this property is set to false.
   * @sa leftFootFlat() setLeftFootFlat()
   */
  bool LeftFootFlat;
  /**
   * [Calculated] This property holds the rotation about the foot rotation axis, which is perpendicular to the foot vector (after applying the foot plantar flexion offset) and the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa rightStaticPlantarFlexionOffset() setRightStaticPlantarFlexionOffset()
   */
  double RightStaticPlantarFlexionOffset;
  /**
   * [Calculated] This property holds the rotation angle about the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa rightStaticRotationOffset() setRightStaticRotationOffset()
   */
  double RightStaticRotationOffset;
  /**
   * [Calculated] This property holds the  rotation about the foot rotation axis, which is perpendicular to the foot vector (after applying the foot plantar flexion offset) and the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa leftStaticPlantarFlexionOffset() setLeftStaticPlantarFlexionOffset()
   */
  double LeftStaticPlantarFlexionOffset;
  /**
   * [Calculated] This property holds the rotation angle about the ankle flexion axis. By default, this property contains the value 0.0.
   * @sa leftStaticRotationOffset() setLeftStaticRotationOffset()
   */
  double LeftStaticRotationOffset;
  };
#endif
  
  /**
   * Constructor. The name is set by to "PluginGait".
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
    Segment *torso = nullptr, *pelvis = nullptr;
    if (optr->Region & Region::Upper)
    {
      model->setName(this->name() + "Upper Limb");
      Segment* head = new Segment("Head", Part::Head, Side::Center, segments);
      torso = new Segment("Torso", Part::Torso, Side::Center, segments);
      if (optr->Side & Side::Left)
      {
        new Segment("L.Clavicle", Part::Clavicle, Side::Left, segments);
        Segment* leftArm = new Segment("L.Arm", Part::Arm, Side::Left, segments);
        Segment* leftForearm = new Segment("L.Forearm", Part::Forearm, Side::Left, segments);
        Segment* leftHand = new Segment("L.Hand", Part::Hand, Side::Left, segments);
        new Joint("L.Shoulder", torso, leftArm, joints);
        new Joint("L.Elbow", leftArm, leftForearm, joints);
        new Joint("L.Wrist", leftForearm, leftHand, joints);
      }
      if (optr->Side & Side::Right)
      {
        new Segment("R.Clavicle", Part::Clavicle, Side::Right, segments);
        Segment* rightArm = new Segment("R.Arm", Part::Arm, Side::Right, segments);
        Segment* rightForearm = new Segment("R.Forearm", Part::Forearm, Side::Right, segments);
        Segment* rightHand = new Segment("R.Hand", Part::Hand, Side::Right, segments);
        new Joint("R.Shoulder", torso, rightArm, joints);
        new Joint("R.Elbow", rightArm, rightForearm, joints);
        new Joint("R.Wrist", rightForearm, rightHand, joints);
      }
      new Joint("Neck", head, torso, joints);
      Joint* globalThorax = new Joint("Thorax", nullptr, torso, joints);
      globalThorax->setDescription("Torso relative to global frame");
      Joint* globalHead = new Joint("Head", nullptr, head, joints);
      globalHead->setDescription("Head relative to global frame");
    }
    if (optr->Region & Region::Lower)
    {
      model->setName(this->name() + "Lower Limb");
      pelvis = new Segment("Pelvis", Part::Pelvis, Side::Center, segments);
      if (optr->Side & Side::Left)
      {
        Segment* leftThigh = new Segment("L.Thigh", Part::Thigh, Side::Left, segments);
        Segment* leftShank = new Segment("L.Shank", Part::Shank, Side::Left, segments);
        Segment* leftFoot = new Segment("L.Foot", Part::Foot, Side::Left, segments);
        new Joint("L.Hip", pelvis, leftThigh, joints);
        new Joint("L.Knee", leftThigh, leftShank, joints);
        new Joint("L.Ankle", leftShank, leftFoot, joints);
        Joint* lfp = new Joint("L.FootProgress", nullptr, leftFoot, joints);
        lfp->setDescription("Left foot relative to global frame");
      }
      if (optr->Side & Side::Right)
      {
        Segment* rightThigh = new Segment("R.Thigh", Part::Thigh, Side::Right, segments);
        Segment* rightShank = new Segment("R.Shank", Part::Shank, Side::Right, segments);
        Segment* rightFoot = new Segment("R.Foot", Part::Foot, Side::Right,segments);
        new Joint("R.Hip", pelvis, rightThigh, joints);
        new Joint("R.Knee", rightThigh, rightShank, joints);
        new Joint("R.Ankle", rightShank, rightFoot, joints);
        Joint* rfp = new Joint("R.FootProgress", nullptr, rightFoot, joints);
        rfp->setDescription("Right foot relative to global frame");
      }
      new Joint("Pelvis", nullptr, pelvis, joints);
    }
    if ((optr->Region & Region::Full) == Region::Full)
    {
      model->setName(this->name() + "Full Body");
      Joint* spine = new Joint("Spine", torso, pelvis, joints);
      spine->setDescription("Torso relative to pelvis");
    }
    return true;
  };

  /**
   * Calibrate this helper based on the first Trial object found in @a trials. If @a subject is not a null pointer, the required parameters are copied from subject's properties.
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
      optr->MarkerDiameter = subject->property("markerDiameter");
      optr->HeadOffset = subject->property("headOffset");
      optr->RightShoulderOffset = subject->property("rightShoulderOffset");
      optr->LeftShoulderOffset = subject->property("leftShoulderOffset");
      optr->RightElbowWidth = subject->property("rightElbowWidth");
      optr->LeftElbowWidth = subject->property("leftElbowWidth");
      optr->RightWristWidth = subject->property("rightWristWidth");
      optr->LeftWristWidth = subject->property("leftWristWidth");
      optr->RightHandThickness = subject->property("rightHandThickness");
      optr->LeftHandThickness = subject->property("leftHandThickness");
      optr->InterAsisDistance = subject->property("interAsisDistance");
      optr->RightLegLength = subject->property("rightLegLength");
      optr->LeftLegLength = subject->property("leftLegLength");
      optr->RightAsisTrochanterAPDistance = subject->property("rightAsisTrochanterAPDistance");
      optr->LeftAsisTrochanterAPDistance = subject->property("leftAsisTrochanterAPDistance");
      optr->RightKneeWidth = subject->property("rightKneeWidth");
      optr->LeftKneeWidth = subject->property("leftKneeWidth");
      optr->RightAnkleWidth = subject->property("rightAnkleWidth");
      optr->LeftAnkleWidth = subject->property("leftAnkleWidth");
      optr->RightFootFlat = subject->property("rightFootFlat");
      optr->LeftFootFlat = subject->property("leftFootFlat");
    }
    
    // Calibrate the helper
    
    // --------------------------------------------------
    // LOWER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Lower)
    {
      maths::Position L_HJC(1); L_HJC.residuals().setZero();
      maths::Position R_HJC(1); R_HJC.residuals().setZero();
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
      maths::Position SC = landmarks["SC"];
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
      
      const maths::Vector v = (L_ASIS - R_ASIS).normalized();
      const maths::Vector w = ((R_ASIS - SC).cross(L_ASIS - SC)).normalized();
      const maths::Pose pelvis(v.cross(w), v, w, (L_ASIS + R_ASIS) / 2.0);
      // -----------------------------------------
      // Other lower limbs (dependant of the hip joint centres)
      // -----------------------------------------
      if ((optr->Side & Side::Left) == Side::Left)
      {
        const maths::Position HJC = pelvis.transform(L_HJC.replicate(pelvis.rows()));
        if (!optr->calibrateLowerLimb(Side::Left, &HJC, &landmarks))
          return false;
      }
      if ((optr->Side & Side::Right) == Side::Right)
      {
        const maths::Position HJC = pelvis.transform(R_HJC.replicate(pelvis.rows()));
        if (!optr->calibrateLowerLimb(Side::Right, &HJC, &landmarks))
          return false;
       }
    }
    // --------------------------------------------------
    // UPPER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Upper)
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
      const maths::Position _L_HF = L_HF.mean();
      const maths::Position _L_HB = L_HB.mean();
      const maths::Position _R_HF = R_HF.mean();
      const maths::Position _R_HB = R_HB.mean();        
      // WARNING: The origin (set to the middle of the four points) is not the same than Vicon!
      const maths::Vector u = ((_L_HF + _R_HF) / 2.0 - (_L_HB + _R_HB) / 2.0).normalized();
      const maths::Vector w = u.cross((_L_HF + _L_HB) / 2.0 - (_R_HF + _R_HB) / 2.0).normalized();
      const maths::Pose head(u, w.cross(u), w, (_L_HF + _R_HF + _L_HB + _R_HB) / 4.0);
      if (head.isOccluded())
      {
        error("PluginGait - Impossible to find a least one valid frame for the head motion. Calibration aborted.");
        return false;
      }
      optr->HeadOffset = M_PI - head.eulerAngles(2,0,1).mean().values().z();
      
    }
    return true;
  };
  
  /**
   * Reconstruct @a model segments' movement based on the content of the given @a trial.
   * @a todo Explain that exported segment frame corresponds to the segmental coordinate system (SCS).
   * @a todo Explain thant hip joint centre stored in this helper are cloned in the model. Like that, the helper can modify theses HJC for different tests, while the model store date used for its reconstruction.
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
    model->setName(model->name() + " - " + trial->name());
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
    
    // --------------------------------------------------
    // UPPER LIMB
    // --------------------------------------------------
    if (optr->Region & Region::Upper)
    {
      // -----------------------------------------
      // Torso
      // -----------------------------------------
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
      const maths::Vector w = ((XP + T10) / 2.0 - (SS + C7) / 2.0).normalized();
      const maths::Vector v = w.cross((XP + SS) / 2.0 - (T10 + C7) / 2.0).normalized();
      const maths::Vector u = v.cross(w);
      const maths::Vector o = SS - (optr->MarkerDiameter / 2.0 * u);
      maths::to_timesequence(u, v, w, o, "Torso.SCS", sampleRate, startTime, trial->timeSequences());
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
      seg = model->segments()->findChild<Segment*>("Pelvis",{},false);
      if (seg == nullptr)
      {
        error("PluginGait - Missing pelvis segment in the model. Movement reconstruction aborted.");
        return false;
      }
      
      // - Hip joint centre
      Point *leftHJCH = nullptr, *rightHJCH = nullptr;
      //   - Left
      if ((leftHJCH = this->findChild<Point*>("L.HJC",{},false)) == nullptr)
      {
        error("PluginGait - Left hip joint centre not found. Did you calibrate first the helper? Movement reconstruction aborted.");
        return false;
      }
      //   - Right
      if ((rightHJCH = this->findChild<Point*>("R.HJC",{},false)) == nullptr)
      {
        error("PluginGait - Right hip joint centre not found. Did you calibrate first the helper? Movement reconstruction aborted.");
        return false;
      }
      // - Construct the relative segment coordinate systme for the pelvis
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
      maths::Position SC = landmarks["SC"];
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
      const maths::Vector v = (L_ASIS - R_ASIS).normalized();
      const maths::Vector w = ((R_ASIS - SC).cross(L_ASIS - SC)).normalized();
      const maths::Pose pelvis(v.cross(w), v, w, (L_ASIS + R_ASIS) / 2.0);
      maths::to_timesequence(transform_relative_frame(relframe, seg, pelvis), "Pelvis.SCS", sampleRate, startTime, TimeSequence::Pose, "", trial->timeSequences());
      // -----------------------------------------
      // Thigh, shank, foot
      // -----------------------------------------
      if ((optr->Side & Side::Left) == Side::Left)
      {
        const maths::Position HJC = transform_relative_point(leftHipJointCenter, seg, pelvis);
        if (!optr->reconstructLowerLimb(trial, Side::Left, &HJC, &landmarks, sampleRate, startTime))
          return false;
      }
      if ((optr->Side & Side::Right) == Side::Right)
      {
        const maths::Position HJC = transform_relative_point(rightHipJointCenter, seg, pelvis);
        if (!optr->reconstructLowerLimb(trial, Side::Right, &HJC, &landmarks, sampleRate, startTime))
          return false;
      }
    }
    // --------------------------------------------------
    // Other Segment
    // --------------------------------------------------
    auto segments = model->segments()->findChildren<Segment*>();
    for (auto it = segments.cbegin() ; it != segments.cend() ; ++it)
    {
      if ((*it)->part() == Part::Head)
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
          error("PluginGait - Missing landmarks to define the head. Movement reconstruction aborted for trial '%s'.", trial->name().c_str());
          return false;
        }
        const maths::Vector u = ((L_HF + R_HF) / 2.0 - (L_HB + R_HB) / 2.0).normalized();
        const maths::Vector w = u.cross((L_HF + L_HB) / 2.0 - (R_HF + R_HB) / 2.0).normalized();
        const maths::Vector v = w.cross(u);
        const maths::Vector o = (L_HF + R_HF + L_HB + R_HB) / 4.0;
        // Rotate the head frame along its axis Y using the offset "HeadOffset"
        const double cy = cos(optr->HeadOffset), sy = sin(optr->HeadOffset);
        const maths::Vector ur = u * cy - w * sy;
        const maths::Vector wr = u * sy + w * cy;
        maths::to_timesequence(ur, v, wr, o, "Head.SCS", sampleRate, startTime, trial->timeSequences());
      }
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
   * Returns the internal parameter HeadOffset.
   */
  double PluginGait::headOffset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->HeadOffset;
  };
  
  /**
   * Set the internal parameter HeadOffset.
   */
  void PluginGait::setHeadOffset(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->HeadOffset) < std::numeric_limits<double>::epsilon())
      return;
    optr->HeadOffset = value;
    this->modified();
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
   * Returns the internal parameter RightFootFlat.
   */
  bool PluginGait::rightFootFlat() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RightFootFlat;
  };
  
  /**
   * Set the internal parameter RightFootFlat.
   */
  void PluginGait::setRightFootFlat(bool value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->RightFootFlat)
      return;
    optr->RightFootFlat = value;
    this->modified();
  };
  
  /**
   * Returns the internal parameter LeftFootFlat.
   */
  bool PluginGait::leftFootFlat() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->LeftFootFlat;
  };
  
  /**
   * Set the internal parameter LeftFootFlat.
   */
  void PluginGait::setLeftFootFlat(bool value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->LeftFootFlat)
      return;
    optr->LeftFootFlat = value;
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
   *  - LSHO: L.AC   (Left acromion (Acromio-clavicular joint))
   *  - RSHO: R.AC   (Right acromion (Acromio-clavicular joint))
   *  - LELB: L.LHE  (Left lateral humerus epicondyle)
   *  - LWRB: L.US   (Left ulnar styloid process (wrist bar pinkie)
   *  - LWRA: L.RS   (Left radius styloid process (wrist bar thumb)
   *  - RELB: R.LHE  (Right lateral humerus epicondyle)
   *  - RWRB: R.US   (Right ulnar styloid process)
   *  - RWRA: R.RS   (Right radius styloid process)
   *  - LFIN: L.MH2  (Left head of the second metacarpus)
   *  - RFIN: R.MH2  (Right head of the second metacarpus)
   *  - LASI: L.ASIS (Left Anterior Superior Iliac Spine)
   *  - RASI: R.ASIS (Right Anterior Superior Iliac Spine)
   *  - LPSI: L.PSIS (Left Posterior Superior Iliac Spine)
   *  - RPSI: R.PSIS (Right Posterior Superior Iliac Spine)
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
    auto translator = this->findChild<LandmarksTranslator*>("LandmarksTranslator",{},false);
    if (translator == nullptr)
    {  
      translator = new LandmarksTranslator("LandmarksTranslator",{
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
      },this);
    }
    return translator;
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
    optr->HeadOffset = optr_src->HeadOffset;
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
    optr->RightFootFlat = optr_src->RightFootFlat;
    optr->LeftFootFlat = optr_src->LeftFootFlat;
    optr->RightStaticPlantarFlexionOffset = optr_src->RightStaticPlantarFlexionOffset;
    optr->RightStaticRotationOffset = optr_src->RightStaticRotationOffset;
    optr->LeftStaticPlantarFlexionOffset = optr_src->LeftStaticPlantarFlexionOffset;
    optr->LeftStaticRotationOffset = optr_src->LeftStaticRotationOffset;
  };
};
};