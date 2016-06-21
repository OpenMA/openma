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

#include "openma/body/simplegaitforceplatetofeetassigner.h"

#include "openma/body/model.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/body/segment.h"
#include "openma/base/trial.h"
#include "openma/base/logger.h"
#include "openma/instrument/forceplate.h"
#include "openma/math.h"

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::SimpleGaitForcePlateToFeetAssigner);

namespace ma
{
namespace body
{
  /**
   * @class SimpleGaitForcePlateToFeetAssigner openma/body/simplegaitforceplatetofeetassigner.h
   * @brief Assign ground reaction wrench to feet based on a distance criterion.
   * 
   * @important This algorithm is currently limited for a simple gait task (straight walk).
   * @todo Add an option regarding the threshold to use for the computation of the wrenches at the COP.
   * @todo Suggest to use a threshold based on the weight of the subject (e.g 2%)
   * @todo Improve the algorithm to manage trial with rounding or treadmil (i.e. Need to check by window instead of the entire signal)
   * 
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  SimpleGaitForcePlateToFeetAssigner::SimpleGaitForcePlateToFeetAssigner(Node* parent)
  : ExternalWrenchAssigner("SimpleGaitForcePlateToFeetAssigner", parent)
  {};
  
  /**
   * Destructor
   */
  SimpleGaitForcePlateToFeetAssigner::~SimpleGaitForcePlateToFeetAssigner() _OPENMA_NOEXCEPT = default;
  
  /**
   * Run the algorithm on the given data. Internally, the algorithm does the following:
   *  - Look for direct Model descendant of @a inout
   *  - For each model found:
   *    - Look for a Trial node attached as a direct descendant
   *    - Look for ForcePlate nodes in this trial
   *    - Look for markers corresponding to the internal names "L.HEE", "L.MTH2", "R.HEE", and "R.MTH2"
   *    - Look for the "L.Foot" and "R.Foot" segments in the model
   *    - Compute the wrench associated with each force plate expressed at the center of pressure (COP)
   *    - Compute virtual markers corresponding to the middle of *.HEE and *.MTH2.
   *    - Compute the distance betwen the COPs and the virtual markers
   *    - Assign the wrench to the segment where the distance is minimum
   */
  bool SimpleGaitForcePlateToFeetAssigner::run(Node* inout)
  {
    // if ((output == nullptr) || (input == nullptr))
    // {
    //   error("Null input or output passed. Automatic force plates to feet assignment (simple gait) aborted.");
    //   return false;
    // }
    // auto models = output->findChildren<Model*>({},{},false);
    // auto trials = input->findChildren<Trial*>({},{},false);
    // if (models.size() != trials.size())
    // {
    //   error("The number of found models and trials is not the same. Automatic force plates to feet assignment (simple gait) aborted.");
    //   return false;
    // }
    
    if (inout == nullptr)
    {  
      error("Null argument passed. Automatic force plates to feet assignment (simple gait) aborted.");
      return false;
    }
    auto models = inout->findChildren<Model*>({},{},false);
    if (models.empty())
    {
      error("Mo model found. Automatic force plates to feet assignment (simple gait) aborted.");
      return false;
    }
    for (size_t i = 0, ilen = models.size() ; i < ilen ; ++i)
    {
      auto model = models[i];
      auto trial = model->findChild<Trial*>({},{},false);
      if (trial == nullptr)
      {
        error("No trial attached to the model '%s'. This algorithm requires a trial where markers of interest are stored. Automatic force plates to feet assignment (simple gait) aborted for this model.", model->name().c_str());
        continue;
      }
      auto forceplates = trial->hardwares()->findChildren<instrument::ForcePlate*>({},{},false);
      auto L_Foot = model->segments()->findChild<Segment*>("L.Foot",{},false);
      auto R_Foot = model->segments()->findChild<Segment*>("R.Foot",{},false);
      if ((L_Foot == nullptr) || (R_Foot == nullptr))
      {
        error("Missing the left or right foot segment for the model '%s'. Automatic force plates to feet assignment (simple gait) aborted for this model.", model->name().c_str());
        continue;
      }
      std::vector<std::string> markerLabels = {{"L.HEE","L.MTH2","R.HEE","R.MTH2"}};
      auto lt = model->findChild<const LandmarksTranslator*>({},{},false);
      if (lt != nullptr)
      {
        for (size_t j = 0 ; j < 4 ; ++j)
        {
          std::string temp = markerLabels[j];
          markerLabels[j] = lt->convertReverse(markerLabels[j]);
        }
      }
      std::vector<TimeSequence*> tss(4);
      std::vector<math::Map<math::Position>> markers; markers.reserve(4);
      bool missingMarker = false;
      for (size_t j = 0 ; j < 4 ; ++j)
      {
        tss[j] = trial->timeSequences()->findChild<TimeSequence*>(markerLabels[j],{},false);
        markers.push_back(math::to_position(tss[j]));
        if (!markers[j].isValid())
        {
          missingMarker = true;
          break;
        }
      }
      if (missingMarker)
      {
        error("At least one marker associated with the model '%s'. Automatic force plates to feet assignment (simple gait) aborted for this model.", model->name().c_str());
        continue;
      }
      double sampleRate = 0.0; double startTime = 0.0; unsigned samples = 0;
      if (!compare_timesequences_properties(tss, sampleRate, startTime, samples))
      {
        error("At least one marker associated with the model '%s' does not have the same sample rate, start time, or number of samples than the others. Automatic force plates to feet assignment (simple gait) aborted for this model.", model->name().c_str());
        continue;
      }
      // Compute the middle of the HEEL - MTH2 markers
      math::Position L_Middle = (markers[0] + markers[1]) * 0.5;
      math::Position R_Middle = (markers[2] + markers[3]) * 0.5;
      for (size_t j = 0, jlen = forceplates.size() ; j < jlen ; ++j)
      {
        auto forceplate = forceplates[j];
        auto wrench = forceplate->wrench(instrument::Location::CentreOfPressure, true, 10.0, sampleRate);
        math::Map<math::Position> cop(samples, wrench->data()+6*samples, wrench->data()+9*samples);
        math::Scalar L_diff = (L_Middle - cop).norm().min();
        math::Scalar R_diff = (R_Middle - cop).norm().min();
        if (L_diff.isOccluded() || R_diff.isOccluded())
        {
          warning("No valid sample found for the force plate '%s' associated with the model '%s'. You may need to assign it manually to a foot.", forceplate->name().c_str(), model->name().c_str());
          continue;
        }
        if (static_cast<double>(L_diff) < static_cast<double>(R_diff))
        {
          // Assign the wrench to the left foot
          wrench->addParent(L_Foot);
        }
        else
        {
          // Assign the wrench to the right foot
          wrench->addParent(R_Foot);
        }
      }
    }
    return true;
  };
  
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  SimpleGaitForcePlateToFeetAssigner* SimpleGaitForcePlateToFeetAssigner::clone(Node* parent) const
  {
    auto dest = new SimpleGaitForcePlateToFeetAssigner;
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void SimpleGaitForcePlateToFeetAssigner::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const SimpleGaitForcePlateToFeetAssigner*>(source);
    if (src == nullptr)
      return;
    this->ExternalWrenchAssigner::copy(src);
  };

 };
};