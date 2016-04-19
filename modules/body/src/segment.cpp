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

#include "openma/body/segment.h"
#include "openma/body/segment_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  SegmentPrivate::SegmentPrivate(Segment* pint, const std::string& name, int part, int side)
  : NodePrivate(pint,name),
    Part(part), Side(side)
  {};
  
  SegmentPrivate::~SegmentPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(body::Segment);

namespace ma
{
namespace body
{ 
  /**
   * @class Segment "openma/body/segment.h"
   * @brief Node specialization to represent a body segment
   * A Segment is simply a Node with specific method to retrieve information on it (i.e. its identify part and its side).
   * All other contents (e.g body segment parameters, relative reference frane, relative postion) shall be set as its children.
   * This gives the possibility to model a body as needed (e.g. only one anatomical frame, or a technical frame with a rigid transformation to a relative anatomical frame, etc.).
   */
  
  /**
   * Construct a segment
   */
  Segment::Segment(const std::string& name, int part, int side, Node* parent)
  : Node(*new SegmentPrivate(this,name,part,side), parent)
  {};
  
  /**
   * Destructor (default)
   */
  Segment::~Segment() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the part identifying the segment
   */
  int Segment::part() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Part;
  };
  
  /**
   * Set the part identifying the segment.
   * See the enumeration Part for some predefined values.
   */
  void Segment::setPart(int value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Part == value)
      return;
    optr->Part = value;
    this->modified();
  };
  
  /**
   * Return the segment's side.
   */
  int Segment::side() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Side;
  };
  
  /**
   * Sets the segment's side.
   * See the enumeration Side for some predefined values.
   */
  void Segment::setSide(int value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Side == value)
      return;
    optr->Side = value;
    this->modified();
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  Segment* Segment::clone(Node* parent) const
  {
    auto dest = new Segment(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void Segment::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Segment*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    optr->Part = optr_src->Part;
    optr->Side = optr_src->Side;
  };
};
};