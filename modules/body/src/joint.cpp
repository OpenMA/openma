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

#include "openma/body/joint.h"
#include "openma/body/joint_p.h"
#include "openma/body/segment.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  // Note: The member Proximal and Distal are used to simplify the mangement of the segment in case one of them is set to null (to represent the global frame).
  
  JointPrivate::JointPrivate(Joint* pint, const std::string& name, Segment* proximal, Segment* distal)
  : NodePrivate(pint,name), Proximal(proximal), Distal(distal)
  {};
  
  JointPrivate::~JointPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace body
{
  /**
   * @class Joint openma/body/joint.h
   * @brief Representation of a connection between two segments.
   * The link between two segments is informative. This shall be used to compute joint kinematics and kinetics.
   * @ingroup openma_body
   */
  
  /**
   * The Joint object will be set as parent of the @a proximal and @a distal objects.
   * @warning The way @a proximal and @a distal objects is created is very important. It is possible to pass pointer from objects created on the stack or the heap. However, for the former, another parent (created before the Segment objects) must be assigned. The reason of this is related to the ownership of children by the parents.
   * @code{.unparsed}
   * // Creation of segments on the stack with prior parent 
   * ma::Node root("root");
   * ma::body::Segment prox("prox", 0, 0, &root);
   * ma::body::Segment dist("dist", 0, 0, &root);
   * ma::body::Joint joint("joint", &prox, &dist, &root);
   * // The code does not crash because 'root' is created first. In the order, 'joint' will be deleted first, then 'dist', 'prox' and 'root'.
   * @endcode
   * @code{.unparsed}
   * // WARNING: THE NEXT CODE WILL CRASH (Object deleted but not allocated)
   * // Creation of segments on the stack wihtout parent
   * ma::body::Segment prox("prox");
   * ma::body::Segment dist("dist");
   * ma::body::Joint joint("joint", &prox, &dist, &root);
   * // The code will crash because 'joint' is deleled first. This one will attempt to delete 'prox' and 'dist' in this destructor because they have no other parent.
   * @endcode
   * @code{.unparsed}
   * // Creation of segments on the heap with wihtout parent 
   * auto prox = new ma::body::Segment("prox");
   * auto dist = new ma::body::Segment("dist");
   * ma::body::Joint joint("joint", prox, dist,);
   * // No problem here because the Segment objects were allocated. The object 'joint' will delete them in its destructor.
   * @endcode
   */
  Joint::Joint(const std::string& name, Segment* proximal, Segment* distal, Node* parent)
  : Node(*new JointPrivate(this,name,proximal,distal), parent)
  {
    if (proximal != nullptr)
      proximal->addParent(this);
    if (distal != nullptr)
      distal->addParent(this);
  };
  
  /**
   * Destructor
   */
  Joint::~Joint() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the proximal segment associated with this joint
   */
  Segment* Joint::proximalSegment() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Proximal;
  };
  
  /**
   * Sets the proximal segment associated with this joint.
   * If the @a value is not the same than the stored one, the old proximal segment is replaced. If the previous proximal segment has no other parent, this one will be deleted.
   */
  void Joint::setProximalSegment(Segment* value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Proximal != value)
    {
      this->replaceChild(optr->Proximal,value);
      optr->Proximal = value;
      this->modified();
    }
  };
  
  /**
   * Returns the distal segment associated with this joint.
   */
  Segment* Joint::distalSegment() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Distal;
  };
  
  /**
   * Sets the distal segment associated with this joint.
   * If the @a value is not the same than the stored one, the old distal segment is replaced. If the previous distal segment has no other parent, this one will be deleted.
   */
  void Joint::setDistalSegment(Segment* value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Distal != value)
    {
      this->replaceChild(optr->Distal,value);
      optr->Distal = value;
      this->modified();
    }
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  Joint* Joint::clone(Node* parent) const
  {
    auto dest = new Joint(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void Joint::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Joint*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    if (optr_src->Proximal != nullptr)
      optr->Proximal = this->findChild<Segment*>(optr_src->Proximal->name());
    if (optr_src->Distal != nullptr)
      optr->Distal = this->findChild<Segment*>(optr_src->Distal->name());
  };
};
};