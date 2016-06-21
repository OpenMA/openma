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

#include "openma/body/externalwrenchassigner.h"
#include "openma/body/externalwrenchassigner_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  ExternalWrenchAssignerPrivate::ExternalWrenchAssignerPrivate(ExternalWrenchAssigner* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  ExternalWrenchAssignerPrivate::~ExternalWrenchAssignerPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::ExternalWrenchAssigner);

namespace ma
{
namespace body
{
  /**
   * @class ExternalWrenchAssigner openma/body/externalwrenchassigner.h
   * @brief 
   * 
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  ExternalWrenchAssigner::ExternalWrenchAssigner(const std::string& name, Node* parent)
  : Node(*new ExternalWrenchAssignerPrivate(this,name), parent)
  {};
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) into the private implementation.
   */
  ExternalWrenchAssigner::ExternalWrenchAssigner(ExternalWrenchAssignerPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
  /**
   * Destructor
   */
  ExternalWrenchAssigner::~ExternalWrenchAssigner() _OPENMA_NOEXCEPT = default;
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void ExternalWrenchAssigner::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const ExternalWrenchAssigner*>(source);
    if (src == nullptr)
      return;
    this->Node::copy(src);
  };

 };
};