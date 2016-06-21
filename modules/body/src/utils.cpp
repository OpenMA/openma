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

#include "openma/body/utils.h"

#include "openma/body/landmarkstranslator.h"
#include "openma/body/point.h"
#include "openma/body/referenceframe.h"
#include "openma/body/inertialparameters.h"
#include "openma/body/segment.h"
#include "openma/body/skeletonhelper.h"
#include "openma/base/trial.h"

namespace ma
{
namespace body
{
  /**
   * Returns a collection of mapped time sequences based on the stored LandmarksTranslator in the given @a helper.
   * If no translator was set in the helper, try to use its default translator.
   * If no translator was found, the keys used in the collection are directly the name of each time sequence.
   * If a translator was found, only the registered landmarks will be extracted, their name converted and stored in the output collection.
   *
   * There is also extra outputs to determine the common sample rate and the common start time in all extracted landmarks.
   * If all the landmarks have the same sample rate, and if the output @a rate is given, its value will be assigned to the found common sample rate (-1.0 otherwise).
   * If all the landmarks have the same start time, and if the output @a start is given, its value will be assigned to the found common start time (-1.0 otherwise).
   * If all the landmarks have the same sample rate and start time, and if the output @a ok is given, its value will be assigned to true (false otherwise).
   */
  std::unordered_map<std::string,math::Map<math::Vector>> extract_landmark_positions(SkeletonHelper* helper, Trial* trial, double* rate, double* start, bool* ok) _OPENMA_NOEXCEPT
  {
    std::unordered_map<std::string,math::Map<math::Vector>> positions;
    
    double sampleRate = -1.0, startTime = -1.0;
    bool common = true;

    auto lt = helper->findChild<LandmarksTranslator*>({},{},false);
    // No defined translator? Let's use the one embedded within the helper (if any)
    if (lt == nullptr)
      lt = helper->defaultLandmarksTranslator();
    auto markers = trial->timeSequences()->findChildren<TimeSequence*>({},{{"type",TimeSequence::Marker},{"components",4}},false);
    decltype(markers) landmarks;
    // No translator found? Create positions for all the markers found
    if (lt == nullptr)
    {
      for (auto it = markers.cbegin() ; it != markers.cend() ; ++it)
      {
        positions.insert(std::make_pair((*it)->name(),math::to_position(*it)));
        landmarks.push_back(*it);
      }
    }
    // Convert only the markers found in the translator
    else
    {
      for (auto it = markers.cbegin() ; it != markers.cend() ; ++it)
      {
        std::string name = lt->convertIfExists((*it)->name());
        if (!name.empty())
        {
          positions.insert(std::make_pair(name,math::to_position(*it)));
          landmarks.push_back(*it);
        }
      }
    }
    // Detect the common sampling information
    auto it = landmarks.cbegin();
    if (it != landmarks.cend())
    {
      sampleRate = (*it)->sampleRate();
      startTime = (*it)->startTime();
      ++it;
    }
    for ( ; it != landmarks.cend() ; ++it)
    {
      if ((fabs(sampleRate - (*it)->sampleRate()) > std::numeric_limits<float>::epsilon())
       || (fabs(startTime - (*it)->startTime()) > std::numeric_limits<float>::epsilon()))
      {
        common = false;
        sampleRate = -1.0;
        startTime = -1.0;
        break;
      }
    }
    // Assign the sampling information (if the inout parameters are given) 
    if (rate != nullptr)
      *rate = sampleRate;
    if (start != nullptr)
      *start = startTime;
    if (ok != nullptr)
      *ok = common;
    
    return positions;
  };
  
  /**
   * Returns the transformation of the relative ReferenceFrame @a relframe expressed in the Segment @a seg. The Pose associated with the Segment is given in @a segpose.
   * @warning The ReferenceFrame node @a relframe must be a (non-)direct child of the Segment @a seg. Moreover, this function does not check if the input variables (@a relpoint, @a seg, @a pose) are null or not.
   * @relates ReferenceFrame
   * @ingroup openma_body
   */
  math::Pose transform_relative_frame(const ReferenceFrame* relframe, const Segment* seg, const math::Pose& segpose) _OPENMA_NOEXCEPT
  {
    assert(seg->findChild<const ReferenceFrame*>(relframe->name()) == relframe);
    auto path = seg->retrievePath(relframe);
    // No path found?
    if (path.empty())
      return math::Pose();
    // Compute the pose
    const double res[1] = {0.};
    math::Pose temp, mot(1);
    mot.residuals().setZero();
    std::copy_n(relframe->data(), 12, mot.values().data());
    for (size_t i = path.size()-2 ; i > 0 ; --i)
    {
      auto relrefframe = node_cast<const ReferenceFrame*>(path[i]);
      if (relrefframe == nullptr)
        continue;
      temp = math::Map<const math::Pose>(1,relrefframe->data(),res).transform(mot);
      mot = temp;
    }
    temp = segpose.transform(mot.replicate(segpose.rows()));
    return temp;
  };
  
  /**
   * Returns the transformation of the relative Point @a relpoint expressed in the Segment @a seg. The Pose associated with the Segment is given in @a segpose.
   * @warning The Point node @a relpoint must be a (non-)direct child of the Segment @a seg. Moreover, this function does not check if the input variables (@a relpoint, @a seg, @a pose) are null or not.
   * @relates Point
   * @ingroup openma_body
   */
  math::Position transform_relative_point(const Point* relpoint, const Segment* seg, const math::Pose& segpose) _OPENMA_NOEXCEPT
  {
    assert(seg->findChild<const Point*>(relpoint->name()) == relpoint);
    auto path = seg->retrievePath(relpoint);
    // No path found?
    if (path.empty())
      return math::Position();
    // Compute the position
    const double res[1] = {0.};
    math::Position temp, traj(1);
    traj.residuals().setZero();
    std::copy_n(relpoint->data(), 3, traj.values().data());
    for (size_t i = path.size()-2 ; i > 0 ; --i)
    {
      auto relrefframe = node_cast<const ReferenceFrame*>(path[i]);
      if (relrefframe == nullptr)
        continue;
      temp = math::Map<const math::Pose>(1,relrefframe->data(),res).transform(traj);
      traj = temp;
    }
    temp = segpose.transform(traj.replicate(segpose.rows()));
    return temp;
  };
  
  math::Array<9> transform_relative_inertia(InertialParameters* relbsip, const Segment* seg, const math::Pose& pose) _OPENMA_NOEXCEPT
  {
    ReferenceFrame rInertia("rInertia", relbsip->inertia(), relbsip);
    return transform_relative_frame(&rInertia, seg, pose).block<9>(0).transform(pose.block<9>(0).transpose());
  };
  
  math::Position transform_relative_com(InertialParameters* relbsip, const Segment* seg, const math::Pose& pose) _OPENMA_NOEXCEPT
  {
    Point rCoM("rCoM", relbsip->centerOfMass(), relbsip);
    return transform_relative_point(&rCoM, seg, pose);
  };
};
};