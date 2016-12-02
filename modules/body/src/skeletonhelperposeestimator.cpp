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

#include "openma/body/skeletonhelperposeestimator.h"

#include "openma/body/model.h"
#include "openma/body/skeletonhelper.h"
#include "openma/base/trial.h"
#include "openma/base/logger.h"

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::SkeletonHelperPoseEstimator);

namespace ma
{
namespace body
{
  /**
   * @class SkeletonHelperPoseEstimator openma/body/skeletonhelperposeestimator.h
   * @brief Segments' pose estimator using an implementation proposed in a SkeletonHelper
   *
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  SkeletonHelperPoseEstimator::SkeletonHelperPoseEstimator(const std::string& name, Node* parent)
  : PoseEstimator(name,parent)
  {};
  
  /**
   * Destructor (default)
   */
  SkeletonHelperPoseEstimator::~SkeletonHelperPoseEstimator() _OPENMA_NOEXCEPT = default;
  
  /**
   * Internaly call the method SkeletonHelper::reconstruct().
   */
  bool SkeletonHelperPoseEstimator::run(Model* output, SkeletonHelper* helper, Trial* trial)
  {
    if (helper == nullptr)
    {
      error("You try to use a skeleton helper pose estimator but a null skeleton helper was passed. Reconstruction aborted.");
      return false;
    }
    return helper->reconstructModel(output,trial);
  }
};
};