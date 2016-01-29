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

#ifndef __eigen_openma_interp1_linear_h
#define __eigen_openma_interp1_linear_h

#include "Interp1_base.h"

namespace Eigen
{
  namespace internal
  {
    template <typename VectorType>
    struct Interp1Linear : Interp1Base<VectorType>
    {
      typedef typename VectorType::Scalar Scalar;
      typedef typename VectorType::Index Index;
    
    	Interp1Linear(const VectorType* x, const VectorType* y)
    	: Interp1Base<VectorType>(x,y,2)
      {};
	
      Scalar rawinterp(Index j, Scalar x)
      {
        if (this->xx->coeff(j) == this->xx->coeff(j+1))
          return this->yy->coeff(j);
    		else
          return this->yy->coeff(j) + ((x - this->xx->coeff(j)) / (this->xx->coeff(j+1) - this->xx->coeff(j))) * (this->yy->coeff(j+1) - this->yy->coeff(j));
    	};
    };
  };
};

#endif // __eigen_openma_Interp1Linear_h