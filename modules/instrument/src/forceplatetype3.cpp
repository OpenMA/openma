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

#include "openma/instrument/forceplatetype3.h"
#include "openma/instrument/forceplatetype3_p.h"
#include "openma/base/timesequence.h"
#include "openma/math.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace instrument
{
  ForcePlateType3Private::ForcePlateType3Private(ForcePlate* pint, const std::string& name, int type, std::vector<std::string>&& labels)
  : ForcePlatePrivate(pint, name, type, std::move(labels)), SensorOffsets{{0.,0}}
  {};
  
  ForcePlateType3Private::~ForcePlateType3Private() _OPENMA_NOEXCEPT = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::instrument::ForcePlateType3)

namespace ma
{
namespace instrument
{

  /**
   * @class ForcePlateType3 "openma/instrument/forceplatetype3.h"
   * Force platform with 8 channels: Fx12, Fx34, Fy14, Fy23, Fz1, Fz2, Fz3, Fz4
   * A manufacturer known to use this force plate is Kistler
   * @important It is important to set sensor offsets with the method setSensorOffsets(). */

  /**
   * Constructor
   */
  ForcePlateType3::ForcePlateType3(const std::string& name, Node* parent)
  : ForcePlate(*new ForcePlateType3Private(this, name, 3, {{"Fx12","Fx34","Fy14","Fy23","Fz1","Fz2","Fz3","Fz4"}}), parent)
  {};
  
  /**
   * Destructor (default)
   */
  ForcePlateType3::~ForcePlateType3() _OPENMA_NOEXCEPT = default;
  
  /**
   * Return an array of 2 sensor offsets used by the force plate to compute moments.
   * The first offset represents the distance between the transducer axes and the y-axis of the force plate.
   * The second offset represents the distance between the transducer axes and the x-axis of the force plate.
   */
  const std::array<double,2>& ForcePlateType3::sensorOffsets() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->SensorOffsets;
  };
  
  /**
   * The input @a value[0] represents the distance between the transducer axes and the y-axis of the force plate.
   * The input @a value[1] represents the distance between the transducer axes and the x-axis of the force plate.
   */
  void ForcePlateType3::setSensorOffsets(const std::array<double,2>& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (std::fabs(optr->SensorOffsets[0] - value[0]) < std::numeric_limits<double>::epsilon()
      && std::fabs(optr->SensorOffsets[1] - value[1]) < std::numeric_limits<double>::epsilon())
      return;
    optr->SensorOffsets = value;
    this->modified();
  };
  
  /**
   * Compute forces and moments from analog channel data associated with this force plate.
   */
  bool ForcePlateType3::computeWrenchAtOrigin(TimeSequence* w)
  {
    auto optr = this->pimpl();
    const auto& offsets = optr->SensorOffsets;
    auto W = math::to_wrench(w);
    auto Fx12 = math::Map<const math::Array<1>>::Values(this->channel("Fx12")->data(), W.rows(), 1);
    auto Fx34 = math::Map<const math::Array<1>>::Values(this->channel("Fx34")->data(), W.rows(), 1);
    auto Fy14 = math::Map<const math::Array<1>>::Values(this->channel("Fy14")->data(), W.rows(), 1);
    auto Fy23 = math::Map<const math::Array<1>>::Values(this->channel("Fy23")->data(), W.rows(), 1);
    auto Fz1 =  math::Map<const math::Array<1>>::Values(this->channel("Fz1")->data(),  W.rows(), 1);
    auto Fz2 =  math::Map<const math::Array<1>>::Values(this->channel("Fz2")->data(),  W.rows(), 1);
    auto Fz3 =  math::Map<const math::Array<1>>::Values(this->channel("Fz3")->data(),  W.rows(), 1);
    auto Fz4 =  math::Map<const math::Array<1>>::Values(this->channel("Fz4")->data(),  W.rows(), 1);
    W.values().col(0) = Fx12 + Fx34;
    W.values().col(1) = Fy14 + Fy23;
    W.values().col(2) = Fz1 + Fz2 + Fz3 + Fz4;
    W.values().col(3) = offsets[1] * (Fz1 + Fz2 - Fz3 - Fz4);
    W.values().col(4) = offsets[0] * (Fz2 + Fz3 - Fz1 - Fz4);
    W.values().col(5) = offsets[1] * (Fx34 - Fx12) + offsets[0] * (Fy14 - Fy23);
    return true;
  };
  
  /**
   * Create a new ForcePlateType3 object on the heap
   */
  Node* ForcePlateType3::allocateNew() const
  {
    return new ForcePlateType3(this->name());
  };
  
  /**
   * Copy the content of the @a source
   */
  void ForcePlateType3::copyContents(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const ForcePlateType3*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->ForcePlate::copyContents(src);
    optr->SensorOffsets = optr_src->SensorOffsets;
  };
};
};