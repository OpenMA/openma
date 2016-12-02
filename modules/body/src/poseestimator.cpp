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

#include "openma/body/poseestimator.h"
#include "openma/body/poseestimator_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  PoseEstimatorPrivate::PoseEstimatorPrivate(PoseEstimator* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  PoseEstimatorPrivate::~PoseEstimatorPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::PoseEstimator);

namespace ma
{
namespace body
{
  /**
   * @class PoseEstimator openma/body/poseestimator.h
   * @brief Abstract class to estimate segments' poses.
   * The goal of a oose estimator is to use a specific mathematical procedure to find the pose (i.e. a rigid body transformation) of a reference frame from a set of markers.
   * The paper of Cappozzo et al. (1997) [1] introduces two pose estimators:
   *  - nonoptimal pose estimator;
   *  - least-square pose estimator.
   *
   * @par References
   *  1. Cappozzo A., Cappello A. Della Croce U., Pensalfini F., <em>Surface-Marker Cluster Design Criteria for 3-D Bone Movement Reconstruction</em>, IEEE Transactions on biomedical engineering, 1997, 44
   *
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  PoseEstimator::PoseEstimator(const std::string& name, Node* parent)
  : Node(*new PoseEstimatorPrivate(this,name), parent)
  {};
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  PoseEstimator::PoseEstimator(PoseEstimatorPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
  /**
   * Destructor (default)
   */
  PoseEstimator::~PoseEstimator() _OPENMA_NOEXCEPT = default;
};
};