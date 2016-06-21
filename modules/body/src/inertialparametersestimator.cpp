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

#include "openma/body/inertialparametersestimator.h"

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::InertialParametersEstimator);

namespace ma
{
namespace body
{
  /**
   * @class InertialParametersEstimator openma/body/inertialparametersestimator.h
   * @brief Abstract class to generate body segment inertial parameters
   * To generalize the way to compute body segment inertial parameters (BSIP), this class proposes a generate() method to overload. Each inheriting class must overload this method.
   *
   * @TODO Give an example on the implementation of the method generate().
   *
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  InertialParametersEstimator::InertialParametersEstimator(const std::string& name, Node* parent)
  : Node(name, parent)
  {};

  /**
   * Destructor
   */
  InertialParametersEstimator::~InertialParametersEstimator() _OPENMA_NOEXCEPT = default;

  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void InertialParametersEstimator::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const InertialParametersEstimator*>(source);
    if (src == nullptr)
      return;
    this->Node::copy(src);
  };
  
  /**
   * @fn virtual bool InertialParametersEstimator::generate(Node* input) = 0;
   * Compute BSIP for known segments
   * @note The implementation of this method is up to the choice of the developer.
   * However, it is important to verify if the required reference(s) frame(s) exists.
   * More exaclty, to create a compatibility format between inertial parameters and models, the formers must be expressed in the body inertial coordinate system (BCS).
   * @TODO Finalize the explanation
   */
 };
};