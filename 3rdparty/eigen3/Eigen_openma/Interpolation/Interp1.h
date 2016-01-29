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

#ifndef __eigen_openma_Interp1_h
#define __eigen_openma_Interp1_h

#include "Interp1_linear.h"
#include "Interp1_cubic.h"

namespace Eigen
{
  struct Interp1
  {
    Interp1() = delete;
    ~Interp1() = delete;
    Interp1(const Interp1& ) = delete;
    Interp1(Interp1&& other) = delete;
    Interp1& operator=(const Interp1& ) = delete;
    Interp1& operator=(Interp1&& other) = delete;
    
    template <typename VectorType, typename OtherVectorType>
    static inline void linear(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi);
    
    template <typename VectorType, typename OtherVectorType>
    static inline void cubic(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi);
  };
  
  template <typename VectorType, typename OtherVectorType>
  void Interp1::linear(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi)
  {
    typedef typename VectorType::Index Index;
    internal::Interp1Linear<VectorType> li(&x, &y);
    yi->resize(xi.rows(), xi.cols());
    for (Index i = 0 ; i < (xi.rows() * xi.cols()) ; ++i)
      yi->coeffRef(i) = li.interp(xi.coeff(i));
  };
  
  template <typename VectorType, typename OtherVectorType>
  void Interp1::cubic(OtherVectorType* yi, const VectorType& x, const VectorType& y, const OtherVectorType& xi)
  {
    typedef typename VectorType::Index Index;
    internal::Interp1Cubic<VectorType> pi(&x, &y);
    yi->resize(xi.rows(), xi.cols());
    for (Index i = 0 ; i < (xi.rows() * xi.cols()) ; ++i)
      yi->coeffRef(i) = pi.interp(xi.coeff(i));
  };
};

#endif // __eigen_openma_Interp1_h