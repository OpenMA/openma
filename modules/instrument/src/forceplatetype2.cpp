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

#include "openma/instrument/forceplatetype2.h"
#include "openma/base/timesequence.h"
#include "openma/math.h"

OPENMA_INSTANCE_STATIC_TYPEID(ma::instrument::ForcePlateType2)

namespace ma
{
namespace instrument
{

  /**
   * @class ForcePlateType2 "openma/instrument/forceplatetype2.h"
   * Force platform with 6 channels: Fx, Fy, Fz, Mx, My, Mz
   * A manufacturer known to use this force plate is AMTI, Inc.
   */

  /**
   * Constructor
   */
  ForcePlateType2::ForcePlateType2(const std::string& name, Node* parent)
  : ForcePlate(*new ForcePlatePrivate(this, name, 2, {{"Fx","Fy","Fz","Mx","My","Mz"}}), parent)
  {};
  
  /**
   * Destructor (default)
   */
  ForcePlateType2::~ForcePlateType2() _OPENMA_NOEXCEPT = default;
  
  /**
   * Compute forces and moments from analog channel data associated with this force plate.
   */
  bool ForcePlateType2::computeWrenchAtOrigin(TimeSequence* w, const std::vector<TimeSequence*>& cpts)
  {
    auto W = math::to_wrench(w);
    using MapAnalog = math::Map<const math::Array<1>>::Values;
    W.values().col(0) = MapAnalog(cpts[0]->data(), W.rows(), 1); // Fx
    W.values().col(1) = MapAnalog(cpts[1]->data(), W.rows(), 1); // Fy
    W.values().col(2) = MapAnalog(cpts[2]->data(), W.rows(), 1); // Fz
    W.values().col(3) = MapAnalog(cpts[3]->data(), W.rows(), 1); // Mx
    W.values().col(4) = MapAnalog(cpts[4]->data(), W.rows(), 1); // My
    W.values().col(5) = MapAnalog(cpts[5]->data(), W.rows(), 1); // Mz
    return true;
  };
  
  /**
   * Create a new ForcePlateType2 object on the heap
   */
  Node* ForcePlateType2::allocateNew() const
  {
    return new ForcePlateType2(this->name());
  };
};
};