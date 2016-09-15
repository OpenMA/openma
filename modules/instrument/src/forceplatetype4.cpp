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

#include "openma/instrument/forceplatetype4.h"
#include "openma/base/timesequence.h"
#include "openma/math.h"

namespace ma
{
namespace instrument
{

  /**
   * @class ForcePlateType4 "openma/instrument/forceplatetype4.h"
   * Force platform with 6 channels (Fx, Fy, Fz, Mx, My, Mz) and a calibration matrix 6x6
   * A manufacturer known to use this force plate is AMTI, Inc.
   */

  /**
   * Constructor
   */
  ForcePlateType4::ForcePlateType4(const std::string& name, Node* parent)
  : ForcePlate(*new ForcePlatePrivate(this, name, 4, {{"Fx","Fy","Fz","Mx","My","Mz"}}, 6, 6), parent)
  {};
  
  /**
   * Destructor (default)
   */
  ForcePlateType4::~ForcePlateType4() _OPENMA_NOEXCEPT = default;
  
  /**
   * Compute forces and moments from analog channel data associated with this force plate.
   */
  bool ForcePlateType4::computeWrenchAtOrigin(TimeSequence* w)
  {
    // Compute the force at the FP origin
    auto cFx = this->channel("Fx")->data();
    auto cFy = this->channel("Fy")->data();
    auto cFz = this->channel("Fz")->data();
    auto cMx = this->channel("Mx")->data();
    auto cMy = this->channel("My")->data();
    auto cMz = this->channel("Mz")->data();
    auto W = math::to_wrench(w);
    Eigen::Map<Eigen::Matrix<double,6,6>> X(this->calibrationMatrixData());
    Eigen::Matrix<double,6,1> c;
    for (unsigned i = 0 ; i < W.rows() ; ++i)
    {
      c.coeffRef(0) = cFx[i];
      c.coeffRef(1) = cFy[i];
      c.coeffRef(2) = cFz[i];
      c.coeffRef(3) = cMx[i];
      c.coeffRef(4) = cMy[i];
      c.coeffRef(5) = cMz[i];
      W.values().block<1,6>(i,0) = X * c;
    }
    return true;
  };
   
  /**
   * Create a new ForcePlateType4 object on the heap
   */
  Node* ForcePlateType4::allocateNew() const
  {
    return new ForcePlateType4(this->name());
  };
};
};