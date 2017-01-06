/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solutanalysisn Inc., all rights reserved.
 * 
 * Redistributanalysisn and use in source and binary forms, with or without
 * modificatanalysisn, are permitted provided that the following conditanalysisns
 * are met:
 * 
 *     * Redistributanalysisns of source code must retain the above
 *       copyright notice, this list of conditanalysisns and the following
 *       disclaimer.
 *     * Redistributanalysisns in binary form must reproduce the above
 *       copyright notice, this list of conditanalysisns and the following
 *       disclaimer in the documentatanalysisn and/or other materials
 *       provided with the distributanalysisn.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific pranalysisr written
 *       permissanalysisn.
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

#include "gaitzeni2008coordinate.h"
#include "openma/analysis/enums.h"
#include "openma/base/event.h"
#include "openma/base/logger.h"
#include "openma/base/subject.h"
#include "openma/base/timesequence.h"
#include "openma/base/utils.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/math.h"

#include <Eigen_openma/SignalProcessing/DetectPeaks.h>

void _gaitzeni2008coordinateDetectEvent(ma::Node* output, const ma::math::Position* SC, const ma::math::Map<ma::math::Position>* HEE, const ma::math::Map<ma::math::Position>* MTH2, const ma::math::Pose* progression, double sampleRate, double startTime, const std::string& context, const std::string& subjectName)
{
  if (!HEE->isValid() || !MTH2->isValid())
  {
    ma::warning("Missing 'HEE' or 'MTH2' markers for the %s side. Event detection not possible for this given side.", context.c_str());
    return;
  }
  _OPENMA_CONSTEXPR double mph = 50.; // Minimum height to detect events
  _OPENMA_CONSTEXPR int mpd = 20; // Distance minimum between 2 events
  using Index = typename ma::math::Scalar::Values::Index;
  ma::math::Vector temp;
  // Foot strike
  temp = progression->transform(*HEE - *SC);
  ma::math::Scalar heelDisplacement = temp.block<1>(0);
  heelDisplacement.values() = (heelDisplacement.residuals() >= 0.).select(heelDisplacement.values(), std::nan(""));
  ma::math::Scalar::Values footStrikeTimes = Eigen::detect_peaks(heelDisplacement.values(), mph, mpd).cast<double>() / sampleRate + startTime;
  if (footStrikeTimes.rows() != 0)
  {
    for (Index i = 0 ; i < footStrikeTimes.rows() ; ++i)
      new ma::Event("FootStrike", footStrikeTimes.coeff(i), subjectName, context, output);
  }
  // Foot off
  temp = progression->transform(*MTH2 - *SC);
  ma::math::Scalar toeDisplacement = temp.block<1>(0);
  toeDisplacement.values() = (toeDisplacement.residuals() >= 0.).select(toeDisplacement.values(), std::nan(""));
  ma::math::Scalar::Values footOffTimes = Eigen::detect_peaks(toeDisplacement.values(), mph, mpd).cast<double>() / sampleRate + startTime;
  if (footOffTimes.rows() != 0)
  {
    for (Index i = 0 ; i < footOffTimes.rows() ; ++i)
      new ma::Event("FootOff", footOffTimes.coeff(i), subjectName, context, output);
  }
};

namespace ma
{
namespace analysis
{
  GaitZeni2008Coordinate::GaitZeni2008Coordinate() = default;
  
  GaitZeni2008Coordinate::~GaitZeni2008Coordinate() _OPENMA_NOEXCEPT = default;

  bool GaitZeni2008Coordinate::detect(Node* output, Node* input, const std::unordered_map<std::string,Any>& arguments)
  {
    // 0. Verify the data and arguments
    // 0.a. progression option
    auto progressionArg = this->extractArgument(arguments, "progression");
    int progressionAxisIndex = -1;
    math::Pose progressionTpl(1);
    progressionTpl.residuals().setZero();
    if (progressionArg.isValid())
    {
      auto progressionVal = progressionArg.cast<std::string>();
      if (strcmpi(progressionVal, "X"))
      {
        progressionAxisIndex = 0;
        progressionTpl.values() << 1., 0., 0., 0., 1., 0., 0., 0., 1., 0., 0., 0.;
      }
      else if (strcmpi(progressionVal, "Y"))
      {
        progressionAxisIndex = 1;
        progressionTpl.values() << 0., 0., 1., 1., 0., 0., 0., 1., 0., 0., 0., 0.;
      }
      else if (strcmpi(progressionVal, "Z"))
      {
        progressionAxisIndex = 2;
        progressionTpl.values() << 0., 1., 0., 0., 0., 1., 1., 0., 0., 0., 0., 0.;
      }
      else
      {
        this->setFailure(Failure::InvalidArgument, "The values for the argument 'progression' are limited to 'X', 'Y' and 'Z'. Detection aborted");
        return false;
      }
    }
    // 0.b Check required data
    std::vector<std::string> labels = {"SC","L.PSIS","R.PSIS","L.HEE","R.HEE","L.MTH2","R.MTH2"};
    auto lt = input->findChild<body::LandmarksTranslator*>();
    if (lt != nullptr)
    {
      for (size_t i = 0 ; i < labels.size() ; ++i)
        labels[i] = lt->convertReverse(labels[i]);
    }
    std::vector<TimeSequence*> tss(7,nullptr);
    tss[0] = input->findChild<TimeSequence*>(labels[0]);
    tss[1] = input->findChild<TimeSequence*>(labels[1]);
    tss[2] = input->findChild<TimeSequence*>(labels[2]);
    tss[3] = input->findChild<TimeSequence*>(labels[3]);
    tss[4] = input->findChild<TimeSequence*>(labels[4]);
    tss[5] = input->findChild<TimeSequence*>(labels[5]);
    tss[6] = input->findChild<TimeSequence*>(labels[6]);
    double rate = 0.0, start = 0.0;
    unsigned samples = 0;
    if (!compare_timesequences_properties(tss, rate, start, samples))
    {
      this->setFailure(Failure::InvalidData, "The markers needed for the event detection do not have the same features(sample rate, start time, etc). Detection aborted");
      return false;
    }
    // Check if the sacrum marker is available or the Posterior Superior Iliac Spine markers
    if ((tss[0] == nullptr) && ((tss[1] == nullptr) || (tss[2] == nullptr)))
    {
      this->setFailure(Failure::InvalidData, "Missing markers to compute the sacrum trajectory. Detection aborted");
      return false;
    }
    // 0.c Check for the progression frame
    auto progressionSCS = input->findChild<TimeSequence*>("Progression.SCS");
    math::Pose progression;
    if (progressionSCS == nullptr)
    {
      progression = math::to_pose(progressionSCS);
    }
    else
    {
      if (progressionAxisIndex == -1)
      {
        this->setFailure(Failure::InvalidData, "Missing progression frame. Detection aborted");
        return false;
      }
      else
      {
        progression = progressionTpl.replicate(samples);
      }
    }
    // 0.d Check for the subject name
    auto subject = input->findChild<Subject*>();
    std::string subjectName = ((subject == nullptr) ? std::string{"Anonymous"} : subject->name());
    // 1. Prepare the data
    math::Position SC =  math::to_position(tss[0]);
    const auto& L_PSIS = math::to_position(tss[1]);
    const auto& R_PSIS = math::to_position(tss[2]);
    const auto& L_HEE =  math::to_position(tss[3]);
    const auto& R_HEE =  math::to_position(tss[4]);
    const auto& L_MTH2 = math::to_position(tss[5]);
    const auto& R_MTH2 = math::to_position(tss[6]);
    if (!SC.isValid())
      SC = (L_PSIS + R_PSIS) / 2.0;
    // 2. Do the computation
    _gaitzeni2008coordinateDetectEvent(output, &SC, &L_HEE, &L_MTH2, &progression, rate, start, "Left", subjectName);
    _gaitzeni2008coordinateDetectEvent(output, &SC, &R_HEE, &R_MTH2, &progression, rate, start, "Right", subjectName);
    return true;
  };
  
};
};