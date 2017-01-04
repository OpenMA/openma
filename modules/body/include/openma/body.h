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

#ifndef __openma_body_h
#define __openma_body_h

#include "openma/body_export.h"
#include "openma/body/descriptor.h"
#include "openma/body/dynamicdescriptor.h"
#include "openma/body/enums.h"
#include "openma/body/eulerdescriptor.h"
#include "openma/body/lyonwholebodymodel.h"
#include "openma/body/joint.h"
#include "openma/body/landmarksregistrar.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/body/model.h"
#include "openma/body/point.h"
#include "openma/body/poseestimator.h"
#include "openma/body/plugingait.h"
#include "openma/body/referenceframe.h"
#include "openma/body/segment.h"
#include "openma/body/skeletonhelper.h"
#include "openma/body/skeletonhelperposeestimator.h"
#include "openma/body/utils.h"
#include "openma/body/unitquaternionposeestimator.h"

namespace ma
{
namespace body
{
  OPENMA_BODY_EXPORT bool calibrate(SkeletonHelper* helper, Node* trials, Subject* subject = nullptr);
  OPENMA_BODY_EXPORT bool register_marker_cluster(SkeletonHelper* helper, Node* trials);
  OPENMA_BODY_EXPORT bool reconstruct(Node* root, SkeletonHelper* helper, Node* trials);
  OPENMA_BODY_EXPORT Node* reconstruct(SkeletonHelper* helper, Node* trials);
  OPENMA_BODY_EXPORT bool extract_joint_kinematics(Node* output, Node* input, bool sideAdaptation = true);
  OPENMA_BODY_EXPORT Node* extract_joint_kinematics(Node* input, bool sideAdaptation = true);
  OPENMA_BODY_EXPORT bool extract_joint_kinetics(Node* output, Node* input, bool sideAdaptation = true, bool massNormalization = true, RepresentationFrame frame = RepresentationFrame::Distal);
  OPENMA_BODY_EXPORT Node* extract_joint_kinetics(Node* input, bool sideAdaptation = true, bool massNormalization = true, RepresentationFrame frame = RepresentationFrame::Distal);
};
};

#endif // __openma_body_h