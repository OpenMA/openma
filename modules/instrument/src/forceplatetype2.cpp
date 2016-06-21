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

namespace ma
{
namespace instrument
{

  /**
   * @class
   * 6 channels: Fx, Fy, Fz, Mx, My, Mz
   *
   */

  /**
   *
   */
  ForcePlateType2::ForcePlateType2(const std::string& name, Node* parent)
  : ForcePlate(*new ForcePlatePrivate(this, name, 2, {{"Fx","Fy","Fz","Mx","My","Mz"}}), parent)
  {};
  
  /**
   *
   */
  ForcePlateType2* ForcePlateType2::clone(Node* parent) const
  {
    auto dest = new ForcePlateType2(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };

  /**
   *
   */
  void ForcePlateType2::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const ForcePlateType2*>(source);
    if (src == nullptr)
      return;
    this->ForcePlate::copy(src);
  };
  
  /**
   *
   */
  bool ForcePlateType2::computeWrenchAtOrigin(TimeSequence* w)
  {
    auto W = math::to_wrench(w);
    W.values().col(0) = math::Map<const math::Array<1>>::Values(this->channel("Fx")->data(), W.rows(), 1);
    W.values().col(1) = math::Map<const math::Array<1>>::Values(this->channel("Fy")->data(), W.rows(), 1);
    W.values().col(2) = math::Map<const math::Array<1>>::Values(this->channel("Fz")->data(), W.rows(), 1);
    W.values().col(3) = math::Map<const math::Array<1>>::Values(this->channel("Mx")->data(), W.rows(), 1);
    W.values().col(4) = math::Map<const math::Array<1>>::Values(this->channel("My")->data(), W.rows(), 1);
    W.values().col(5) = math::Map<const math::Array<1>>::Values(this->channel("Mz")->data(), W.rows(), 1);
    return true;
  };
};
};