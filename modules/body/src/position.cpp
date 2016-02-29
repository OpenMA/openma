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

#include "openma/body/position.h"
#include "openma/body/position_p.h"

#include <cassert>
#include <algorithm> // std::copy_n
#include <limits> // std::numeric_limits::epsilon

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  PositionPrivate::PositionPrivate(Position* pint, const std::string& name, const double* data)
  : NodePrivate(pint,name)
  {
    if (data != nullptr)
    {
      std::copy_n(data, 3, this->Data);
    }
    else
    {
      this->Data[0] = 0.; this->Data[1] = 0.; this->Data[2] = 0.;
    }
  };
  
  PositionPrivate::~PositionPrivate() = default;
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
   * @class Coordinates openma/body/position.h
   * @brief Create a fix/relative position in a 3D euclidian space. 
   * The goal of this class is to store 3D coordinates. Alone, it might not be interesting. But, set as a child of a reference frame, it creates a relative position. The association of a Position with a TimeSequence (representing a pose) can generate a trajectory. 
   * The way to parent a Position determine its meaning.
   * @ingroup openma_body
   */
  
  /**
   * Construct a new Position object with @a name and @a coordinates.
   */
  Position::Position(const std::string& name, const double coordinates[3], Node* parent)
  : Node(*new PositionPrivate(this,name,coordinates), parent)
  {};
  
  /**
   * Destructor
   */
  Position::~Position() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns a const pointer to the data.
   */
  const double* Position::data() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Returns a pointer to the data.
   * @note You must call the modified() method after the use of this method if you set new values.
   */
  double* Position::data() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Sets the coordinates stored in @a values.
   */
  void Position::setData(const double* values) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    _OPENMA_CONSTEXPR double eps = std::numeric_limits<double>::epsilon();
    if ((fabs(optr->Data[0] - values[0]) > eps)
     || (fabs(optr->Data[1] - values[1]) > eps)
     || (fabs(optr->Data[2] - values[2]) > eps))
    {
      std::copy_n(values, 3, optr->Data);
      this->modified();
    }
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  Position* Position::clone(Node* parent) const
  {
    auto dest = new Position(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
 
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void Position::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Position*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    std::copy_n(optr_src->Data, 3, optr->Data);
  };
};
};