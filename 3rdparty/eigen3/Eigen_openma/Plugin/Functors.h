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

#ifndef __eigen_openma_Functors_h
#define __eigen_openma_Functors_h

#define OPENMA_EIGEN_VECTOROP_MEMBER_FUNCTOR(MEMBER,COST)                  \
  template <typename ResultType>                                        \
  struct openma_eigen_vectorop_member_##MEMBER {                           \
    EIGEN_EMPTY_STRUCT_CTOR(openma_eigen_vectorop_member_##MEMBER)         \
    typedef ResultType result_type;                                     \
    template<typename Scalar, int Size> struct Cost                     \
    { enum { value = COST }; };                                         \
    template<typename XprType>                                          \
    EIGEN_STRONG_INLINE ResultType operator()(const XprType& mat) const \
    { return mat.MEMBER(); } \
  };
  
// NOTE: The given cost is the upper bound: O(n*n) + addition + multiplication
OPENMA_EIGEN_VECTOROP_MEMBER_FUNCTOR(median, (Size*Size)*Eigen::NumTraits<Scalar>::ReadCost + Eigen::NumTraits<Scalar>::AddCost + Eigen::NumTraits<Scalar>::MulCost);

// NOTE: The detailed cost is the following:
// -  Computation of the mean: Size * AddCost + 1 * MulCost
//  - Difference of each element with the mean: Size * AddCost
//  - Computation of the squared sum: Size * AddCost + Size * MulCost
//  - Average: 1 * AddCost + 1 * MulCost
//  The total is then: (3 * Size + 1) * AddCost + (Size + 2) * MulCost
OPENMA_EIGEN_VECTOROP_MEMBER_FUNCTOR(std, (3*Size+1)*Eigen::NumTraits<Scalar>::MulCost + (Size+2)*Eigen::NumTraits<Scalar>::AddCost);

#endif // __eigen_openma_Functors_h