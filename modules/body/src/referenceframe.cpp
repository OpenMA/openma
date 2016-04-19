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

#include "openma/body/referenceframe.h"
#include "openma/body/referenceframe_p.h"

#include <cassert>
#include <algorithm> // std::copy_n

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  ReferenceFramePrivate::ReferenceFramePrivate(ReferenceFrame* pint, const std::string& name, const double* data)
  : NodePrivate(pint,name)
  {
    if (data != nullptr)
    {
      std::copy_n(data, 12, this->Data);
    }
    else
    {
      this->Data[0] = 1.; this->Data[1] = 0.; this->Data[2] = 0.; // u
      this->Data[3] = 0.; this->Data[4] = 1.; this->Data[5] = 0.; // v
      this->Data[6] = 0.; this->Data[7] = 0.; this->Data[8] = 1.; // w
      this->Data[9] = 0.; this->Data[10] = 0.; this->Data[11] = 0.; // o
    }
  };
  
  ReferenceFramePrivate::ReferenceFramePrivate(ReferenceFrame* pint, const std::string& name, const double* orientation, const double* position)
  : NodePrivate(pint,name)
  {
    // Orientation
    if (orientation != nullptr)
    {
      std::copy_n(orientation, 9, this->Data);
    }
    else
    {
      this->Data[0] = 1.; this->Data[1] = 0.; this->Data[2] = 0.; // u
      this->Data[3] = 0.; this->Data[4] = 1.; this->Data[5] = 0.; // v
      this->Data[6] = 0.; this->Data[7] = 0.; this->Data[8] = 1.; // w
    }
    // Position
    if (position != nullptr)
    {
      std::copy_n(position, 3, this->Data+9);
    }
    else
    {
      this->Data[9] = 0.; this->Data[10] = 0.; this->Data[11] = 0.; // o
    }
  };
  
  ReferenceFramePrivate::~ReferenceFramePrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(body::ReferenceFrame);

namespace ma
{
namespace body
{
  /**
   * @class ReferenceFrame openma/body/referenceframe.h
   * @brief Create a fix/relative reference frame in a 3D euclidian space. 
   * The goal of this class is to store the 12 components used to represent the transformation of one reference frame to another. Alone, it might not be interesting. But, set as a child of a reference frame, it creates a relative reference frame. The association of a ReferenceFrame with a TimeSequence (representing a pose) can generate a motion. 
   * The way to parent a ReferenceFrame determines its meaning.
   * @ingroup openma_body
   */
  
  /**
   * Constructor. Create a new reference frame where @a data contains the u, v, w, and o vectors.
   * Internally, the content of @a data is copied.
   */
  ReferenceFrame::ReferenceFrame(const std::string& name, const double data[12], Node* parent)
  : Node(*new ReferenceFramePrivate(this, name, data), parent)
  {};
  
  /**
   * Constructor. Create a new reference frame where @a orientation contains the u, v, and w vectors. The @a position contains the @a o vector.
   * Internally, the content of @a orientation and @a position is copied.
   */
  ReferenceFrame::ReferenceFrame(const std::string& name, const double orientation[9], const double position[3], Node* parent)
  : Node(*new ReferenceFramePrivate(this, name, orientation, position), parent)
  {};
  
  /**
   * Destructor
   */
  ReferenceFrame::~ReferenceFrame() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns a const pointer to the 3d vector associated with the first axis of the orientation part of the reference frame.
   */
  const double* ReferenceFrame::u() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Returns a pointer to the 3d vector associated with the first axis of the orientation part of the reference frame.
   * @note You must call the modified() method after the use of this method if you set new values.
   */
  double* ReferenceFrame::u() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Returns a const pointer to the 3d vector associated with the second axis of the orientation part of the reference frame.
   */
  const double* ReferenceFrame::v() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data+3;
  };
  
  /**
   * Returns a const pointer to the 3d vector associated with the second axis of the orientation part of the reference frame.
   * @note You must call the modified() method after the use of this method if you set new values.
   */
  double* ReferenceFrame::v() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data+3;
  };
  
  /**
   * Returns a const pointer to the 3d vector associated with the third axis of the orientation part of the reference frame.
   */
  const double* ReferenceFrame::w() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data+6;
  };
  
  /**
   * Returns a pointer to the 3d vector associated with the third axis of the orientation part of the reference frame.
   * @note You must call the modified() method after the use of this method if you set new values.
   */
  double* ReferenceFrame::w() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data+6;
  };
  
  /**
   * Returns a const pointer to the 3d position part of the reference frame.
   */
  const double* ReferenceFrame::o() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data+9;
  };
  
  /**
   * Returns a pointer to the 3d position part of the reference frame.
   * @note You must call the modified() method after the use of this method if you set new values.
   */
  double* ReferenceFrame::o() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data+9;
  };
  
  /**
   * Returns a const pointer to the data.
   */
  const double* ReferenceFrame::data() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Returns a pointer to the data.
   * @note You must call the modified() method after the use of this method if you set new values.
   */
  double* ReferenceFrame::data() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Sets the matrix components stored in @a values.
   */
  void ReferenceFrame::setData(const double* values) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    _OPENMA_CONSTEXPR double eps = std::numeric_limits<double>::epsilon();
    if ((fabs(optr->Data[ 0] - values[ 0]) > eps)
     || (fabs(optr->Data[ 1] - values[ 1]) > eps)
     || (fabs(optr->Data[ 2] - values[ 2]) > eps)
     || (fabs(optr->Data[ 3] - values[ 3]) > eps)
     || (fabs(optr->Data[ 4] - values[ 4]) > eps)
     || (fabs(optr->Data[ 5] - values[ 5]) > eps)
     || (fabs(optr->Data[ 6] - values[ 6]) > eps)
     || (fabs(optr->Data[ 7] - values[ 7]) > eps)
     || (fabs(optr->Data[ 8] - values[ 8]) > eps)
     || (fabs(optr->Data[ 9] - values[ 9]) > eps)
     || (fabs(optr->Data[10] - values[10]) > eps)
     || (fabs(optr->Data[11] - values[11]) > eps))
    {
      std::copy_n(values, 12, optr->Data);
      this->modified();
    }
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  ReferenceFrame* ReferenceFrame::clone(Node* parent) const
  {
    auto dest = new ReferenceFrame(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void ReferenceFrame::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const ReferenceFrame*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    std::copy_n(optr_src->Data, 12, optr->Data);
  };
};
};