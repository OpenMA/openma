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

#include "openma/body/chain.h"
#include "openma/body/chain_p.h"
#include "openma/body/joint.h"
#include "openma/body/segment.h"
#include "openma/base/logger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  ChainPrivate::ChainPrivate(Chain* pint, const std::string& name, const std::vector<Joint*>& joints)
  : NodePrivate(pint,name),
    Joints(), Segments(), LastGenerated(0ul)
  {};
  
  ChainPrivate::~ChainPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(body::Chain);

namespace ma
{
namespace body
{ 
  /**
   * @class Chain "openma/body/chain.h"
   * @brief Node specialization to represent a set of joints linked together by segments
   * 
   * @sa Joint Segment
   * @ingroup openma_body
   */
  
  /**
   * Construct a chain without definition. Use the method setDefinition to store related joints.
   */
  Chain::Chain(const std::string& name, Node* parent)
  : Chain(name, std::vector<Joint*>{}, parent)
  {};
  
  /**
   * Construct a chain defined by the set of @a joints passed.
   * This method verify that each joint are connected together. More exaclty, the distal segment associated with the joint J must correspond to the proximal of the joint J+1.
   * If this is not the case, the new definition is not stored.
   */
  Chain::Chain(const std::string& name, const std::vector<Joint*>& joints, Node* parent)
  : Node(*new ChainPrivate(this, name, joints), parent)
  {
    auto optr = this->pimpl();
    // Check if the joints are given in the correct order:
    // - from proximal to distal
    // - distal_seg_jnt == proximal_seg_jnt+1
    bool valid = true;
    for (size_t i = 0, len = (joints.size()-1) ; i < len ; ++i)
    {
      if (joints[i]->distalSegment() != joints[i+1]->proximalSegment())
      {
        error("The joints passed to the chain '%s' are not given in the correct order.", name.c_str());
        valid = false;
      }
    }
    if (valid)
    {
      optr->Joints = joints;
      for (auto& joint : optr->Joints)
        joint->addParent(this);
    }
  };
  
  /**
   * Destructor (default)
   */
  Chain::~Chain() _OPENMA_NOEXCEPT = default;
  
  /*
   * Replace the current definition of the chain by the one stored in @a joints.
   * This method verify that each joint are connected together. More exaclty, the distal segment associated with the joint J must correspond to the proximal of the joint J+1.
   * If this is not the case, the new definition is not stored.
   */
  /*
  void Chain::setDefinition(const std::vector<Joint*>& joints)
  {
    TODO Implement this method. The default constructor could be then public.
  };
  */
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  Chain* Chain::clone(Node* parent) const
  {
    auto dest = new Chain(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void Chain::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Chain*>(source);
    if (src == nullptr)
      return;
    // auto optr = this->pimpl();
    // auto optr_src = src->pimpl();
    this->Node::copy(src);
#if !defined(_MSC_VER)
#warning FINALIZE THE CHAIN::COPY METHOD. WHAT ABOUT SHARED CLONED CHILDREN?
#endif
  };
};
};