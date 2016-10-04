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

#include "openma/instrument/forceplatetype5.h"
#include "openma/base/timesequence.h"
#include "openma/math.h"

OPENMA_INSTANCE_STATIC_TYPEID(ma::instrument::ForcePlateType5)

namespace ma
{
namespace instrument
{

  /**
   * @class ForcePlateType5 "openma/instrument/forceplatetype5.h"
   * Force platform with 8 channels (Fz1, Fz2, Fz3, Fz4, Fx12, Fx34, Fy14, Fy23) and a calibration matrix 6 (columns) by 8 (rows)
   * A manufacturer known to use this force plate is AMTI, Inc.
   */

  /**
   * Constructor
   */
  ForcePlateType5::ForcePlateType5(const std::string& name, Node* parent)
  : ForcePlate(*new ForcePlatePrivate(this, name, 5, {{"Fz1","Fz2","Fz3","Fz4","Fx12","Fx34","Fy14","Fy23"}}, 6, 8), parent)
  {};
  
  /**
   * Destructor (default)
   */
  ForcePlateType5::~ForcePlateType5() _OPENMA_NOEXCEPT = default;
  
  /**
   * Compute forces and moments from analog channel data associated with this force plate.
   */
  bool ForcePlateType5::computeWrenchAtOrigin(TimeSequence* w)
  {
    // Compute the force at the FP origin
    auto cFz1  = this->channel("Fz1")->data();
    auto cFz2  = this->channel("Fz2")->data();
    auto cFz3  = this->channel("Fz3")->data();
    auto cFz4  = this->channel("Fz4")->data();
    auto cFx12 = this->channel("Fx12")->data();
    auto cFx34 = this->channel("Fx34")->data();
    auto cFy14 = this->channel("Fy14")->data();
    auto cFy23 = this->channel("Fy23")->data();
    auto W = math::to_wrench(w);
    Eigen::Map<Eigen::Matrix<double,6,8>> X(this->calibrationMatrixData());
    Eigen::Matrix<double,8,1> c;
    for (unsigned i = 0 ; i < W.rows() ; ++i)
    {
      c.coeffRef(0) = cFz1[i];
      c.coeffRef(1) = cFz2[i];
      c.coeffRef(2) = cFz3[i];
      c.coeffRef(3) = cFz4[i];
      c.coeffRef(4) = cFx12[i];
      c.coeffRef(5) = cFx34[i];
      c.coeffRef(6) = cFy14[i];
      c.coeffRef(7) = cFy23[i];
      W.values().block<1,6>(i,0) = X * c;
    }
    return true;
  };
  
  /**
   * Create a new ForcePlateType5 object on the heap
   */
  Node* ForcePlateType5::allocateNew() const
  {
    return new ForcePlateType5(this->name());
  };
};
};