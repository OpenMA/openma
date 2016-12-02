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

#include "openma/body/unitquaternionposeestimator.h"

#include "openma/body/landmarksregistrar.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/body/model.h"
#include "openma/body/point.h"
#include "openma/body/referenceframe.h"
#include "openma/body/segment.h"
#include "openma/body/skeletonhelper.h"
#include "openma/body/utils.h"
#include "openma/base/trial.h"
#include "openma/base/logger.h"
#include "openma/math.h"

#include <Eigen/Eigenvalues> // Eigen::SelfAdjointEigenSolver

#include <iostream>

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::UnitQuaternionPoseEstimator);

namespace ma
{
namespace body
{
  /**
   * @class UnitQuaternionPoseEstimator openma/body/unitquaternionposeestimator.h
   * @brief Least square pose estimator based on unit quaternions
   *
   * This estimator is based on the paper of Horn [1] that use unit quaternions to find the rigid body transformation betwen two group of markers.
   *
   * @par References
   *  1. Horn B.K.P., <em>Closed-form solution of absolute orientation using unit quaternions</em>, Journal of the Optical Society of America, 1987, 4
   *
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  UnitQuaternionPoseEstimator::UnitQuaternionPoseEstimator(const std::string& name, Node* parent)
  : PoseEstimator(name,parent)
  {};
  
  /**
   * Destructor (default)
   */
  UnitQuaternionPoseEstimator::~UnitQuaternionPoseEstimator() _OPENMA_NOEXCEPT = default;
  
  /**
   * Internaly call the method UnitQuaternion::reconstruct().
   */
  bool UnitQuaternionPoseEstimator::run(Model* output, SkeletonHelper* helper, Trial* trial)
  {
    // 0. Check
    if (output == nullptr)
    {
      error("UnitQuaternionPoseEstimator - Null output passed. Pose estimator aborted.");
      return false;
    }
    if (helper == nullptr)
    {
      error("UnitQuaternionPoseEstimator - Null helper passed. Pose estimator aborted.");
      return false;
    }
    if (trial == nullptr)
    {
      error("UnitQuaternionPoseEstimator - Null trial passed. Pose estimator aborted.");
      return false;
    }
    // 1. Look for the child node MarkerClusterRegistration
    auto mcr = helper->findChild("MarkerClusterRegistration",{},false);
    if (mcr == nullptr)
    {
      error("UnitQuaternionPoseEstimator - No marker cluster registration found. Pose estimator aborted.");
      return false;
    }
    const auto& lt = helper->findChild<LandmarksTranslator*>({},{},false);
    const auto& segments = output->segments()->findChildren<Segment*>({},{},false);
    double startTime = 0.0, sampleRate = 0.0;
    bool ok = false;
    for (const auto& segment : segments)
    {
      const auto& lr = segment->findChild<LandmarksRegistrar*>({},{},false);
      if (lr == nullptr)
        continue;
      // Look for the markers in the trial and the marker cluster registration
      auto globalMarkers = extract_landmark_positions(nullptr, lr->retrieveLandmarks(lt, trial->timeSequences()), &sampleRate, &startTime, &ok);
      if (!ok)
      {
        error("UnitQuaternionPoseEstimator - The sampling information is not consistent between required landmarks (sampling rates or start times are not the same). Calibration aborted.");
        return false;
      }
      std::vector<std::pair<Eigen::Map<Eigen::Matrix<double,3,1>>,const math::Map<math::Position>&>> mappedMarkers;
      mappedMarkers.reserve(globalMarkers.size());
      auto it = globalMarkers.begin();
      while (it != globalMarkers.end())
      {
        Point* localMarker = nullptr;
        if (!it->second.isValid() || ((localMarker = mcr->findChild<Point*>(segment->name()+"."+it->first,{},false)) == nullptr))
          it = globalMarkers.erase(it);
        else
        {
          mappedMarkers.push_back({localMarker->data(),it->second});
          ++it;
        }
      }
      assert(globalMarkers.size() == mappedMarkers.size());
      if (mappedMarkers.size() < 3)
      {
        error("Less than 3 valid markers was found for the segment '%s'. Impossible to compute the TCS. Pose estimator aborted.", segment->name().c_str());
        return false;
      }
      // Reconstruct for each sample
      long numSamples = std::numeric_limits<long>::max();
      for (const auto& marker : globalMarkers)
      {
        numSamples = std::min(numSamples, marker.second.rows());
        if (marker.second.rows() != numSamples)
        {
          error("The number of samples for the markers used by the cluster '%s.Cluster' is not the same. Impossible to compute the TCS. Pose estimator aborted.", segment->name().c_str());
          return false;
        }
      }
      ma::math::Pose tcs(numSamples);
      Eigen::Matrix<double,3,3> M;
      Eigen::Matrix<double,4,4> N;
      Eigen::Matrix<double,3,Eigen::Dynamic> ps1, ps2;
      for (int i = 0 ; i < numSamples ; ++i)
      {
        int inc = 0;
        ps1.setZero(3,static_cast<int>(globalMarkers.size()));
        ps2.setZero(3,static_cast<int>(globalMarkers.size()));
        for (const auto& m : mappedMarkers)
        {
          if (m.second.residuals().coeff(i) >= 0.0)
          {
            ps1.col(inc) = m.first; // Local
            ps2.col(inc) = m.second.values().row(i); // Global
            ++inc;
          }
        }
        if (inc < 3) // Not enough landmark to create the least square fitting.
        {
          tcs.residuals().coeffRef(i) = -1.0;
          continue;
        }
        ps1.resize(Eigen::NoChange,inc);
        ps2.resize(Eigen::NoChange,inc);
        // Express the point sets regarding to their respective center
        Eigen::Matrix<double,3,1> p1 = (ps1.rowwise().sum() / static_cast<double>(inc));
        ps1 -= p1.replicate(1,inc);
        // ps1.row(0).array() -= p1.x();
        // ps1.row(1).array() -= p1.y();
        // ps1.row(2).array() -= p1.z();
        Eigen::Matrix<double,3,1> p2 = ps2.rowwise().sum() / static_cast<double>(inc);
        ps2 -= p2.replicate(1,inc);
        // ps2.row(0).array() -= p2.x();
        // ps2.row(1).array() -= p2.y();
        // ps2.row(2).array() -= p2.z();
        // Build the matrice N
        M.setZero();
        for (int j = 0 ; j < inc ; ++j)
          M += ps1.col(j) * ps2.col(j).transpose();
        N.setZero();
        N.coeffRef(0,1) = N.coeffRef(1,0) = M.coeff(1,2) - M.coeff(2,1);
        N.coeffRef(0,2) = N.coeffRef(2,0) = M.coeff(2,0) - M.coeff(0,2);
        N.coeffRef(0,3) = N.coeffRef(3,0) = M.coeff(0,1) - M.coeff(1,0);
        N.coeffRef(1,2) = N.coeffRef(2,1) = M.coeff(0,1) + M.coeff(1,0);
        N.coeffRef(1,3) = N.coeffRef(3,1) = M.coeff(2,0) + M.coeff(0,2);
        N.coeffRef(2,3) = N.coeffRef(3,2) = M.coeff(1,2) + M.coeff(2,1);
        N.coeffRef(0,0) =  M.coeff(0,0) + M.coeff(1,1) + M.coeff(2,2);
        N.coeffRef(1,1) =  M.coeff(0,0) - M.coeff(1,1) - M.coeff(2,2);
        N.coeffRef(2,2) = -M.coeff(0,0) + M.coeff(1,1) - M.coeff(2,2);
        N.coeffRef(3,3) = -M.coeff(0,0) - M.coeff(1,1) + M.coeff(2,2);
        // Extract the eigen vector associated with the most positive eigen value and compute the rotation matrix (the extracted eigen vector is a quaternion)
        Eigen::SelfAdjointEigenSolver< Eigen::Matrix<double,4,4> > eig(N);
        int idx; eig.eigenvalues().maxCoeff(&idx);
        // Q2R need to be done in 2 steps as the eigen vector is formatted as WXYZ and the internal storage in Eigen is XYZW
        Eigen::Matrix<double,4,1> q = eig.eigenvectors().col(idx);
        Eigen::Matrix<double,3,3> R = Eigen::Quaternion<double>(q.coeff(0),q.coeff(1),q.coeff(2),q.coeff(3)).toRotationMatrix();
        // Set the pose
        auto row = tcs.values().row(i);
        row.segment<3>(0) = R.col(0);    // u
        row.segment<3>(3) = R.col(1);    // v
        row.segment<3>(6) = R.col(2);    // w
        row.segment<3>(9) = p2 - R * p1; // o
        tcs.residuals().coeffRef(i) = 0.0;
      }
      // Reconstruction of the SCS
      // Look for Reference frame in the node MarkerClusterRegistration
      auto relframe = mcr->findChild<ReferenceFrame*>(segment->name() + ".SCS", {}, false);
      math::to_timesequence(tcs, segment->name() + ".TCS", sampleRate, startTime, TimeSequence::Pose, "", segment);
      if (relframe != nullptr)
      {
        relframe->addParent(segment);
        math::to_timesequence(transform_relative_frame(relframe, segment, tcs), segment->name() + ".SCS", sampleRate, startTime, TimeSequence::Pose, "", segment);
      }
    }
    return true;
  }
};
};