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

#include "openma/body/descriptor.h"
#include "openma/body/enums.h"
#include "openma/body/eulerdescriptor.h"
#include "openma/body/joint.h"
#include "openma/body/landmarkstranslator.h"
#include "openma/body/model.h"
#include "openma/body/point.h"
#include "openma/body/plugingait.h"
#include "openma/body/referenceframe.h"
#include "openma/body/segment.h"
#include "openma/body/skeletonhelper.h"
#include "openma/body/utils.h"

#include "openma/base/node.h"
#include "openma/base/subject.h"
#include "openma/base/logger.h"

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
  inline bool calibrate(SkeletonHelper* helper, Node* trials, Subject* subject)
  {
    if (helper == nullptr)
    {
      error("A null pointer to a SkeletonHelper object was passed. Calibration aborted.");
      return false;
    }
    return helper->calibrate(trials, subject);
  };
  
  /**
   * Convenient function to create Model nodes and reconstrust their movement based on the @a helper and the @a trials.
   * For each trial, a model is created and added to the @a output.
   * @note The @a helper might need to be calibrated before the use of the reconstruct() function.
   */
  inline bool reconstruct(Node* root, SkeletonHelper* helper, Node* trials)
  {
    if (helper == nullptr)
    {
      error("A null pointer to a SkeletonHelper object was passed. Reconstruction aborted.");
      return false;
    }
    return helper->reconstruct(root, trials);
  }
  
  /**
   * Similar to the other reconstruct method but the models are added to the returned node.
   * @warning The returned node is allocated on the heap. The developer must care of the deletion of the object using the @c delete keyword.
   */
  inline Node* reconstruct(SkeletonHelper* helper, Node* trials)
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
   *  - enableScaleAdaptation is set to the value passed in @a anglesAdaptation
   * The use of these options means that computed Euler angles are expressed in degrees. The TimeSequence associated with each Segment will use both the corresponding segments' name concatenated with the suffix ".SCS". They are adapted or not depending of the value in @a anglesAdaptation.
   * The adaptation of the angles depends of the setting of each descriptor. You should refer to the model's definition (or helper) to know the descriptor used. For example with the PluginGait helper, the adaptation is used to be able to compare directly the left/right sides.
   */
  inline bool extract_joint_kinematics(Node* output, Node* input, bool anglesAdaptation = true)
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
      {"enableScaleAdaptation", anglesAdaptation}
    };
    for (auto& model: models)
    {
      auto analysis = new Node(model->name() + " - Joint Kinematics Analysis", output);
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
  inline Node* extract_joint_kinematics(Node* input, bool anglesAdaptation)
  {
    Node* root = new Node("root");
    if (!extract_joint_kinematics(root, input, anglesAdaptation))
    {
      delete root;
      root = nullptr;
    }
    return root;
  };
};
};

#endif // __openma_body_h