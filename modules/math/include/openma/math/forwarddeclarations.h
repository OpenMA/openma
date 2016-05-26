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

#ifndef __openma_math_forwarddeclarations_h
#define __openma_math_forwarddeclarations_h

// ----------------------------------------------------------------------- //
//                           FORWARD DECLARATIONS
// ----------------------------------------------------------------------- //

namespace ma
{
namespace math
{
  template <typename Derived> class ArrayBase;
  template <int Cols> class Array;
  class Pose;
  template <typename Derived> class Map;
  
  template <typename Xpr, int Cols> class BlockOp;
  template <typename Derived, typename Xpr> class UnaryOp;
  template <typename Derived, typename XprOne, typename XprTwo> class BinaryOp;  
  
  template <typename Xpr> class NormOp;
  template <typename Xpr> class MeanOp;
  template <typename XprOne, typename XprTwo> class DifferenceOp;
  template <typename XprOne, typename XprTwo> class SumOp;
  template <typename Xpr> class ScaleOp;
  template <typename Xpr> class NormalizedOp;
  template <typename XprOne, typename XprTwo> class CrossOp;
  template <typename Xpr> class TransposeOp;
  template <typename XprOne, typename XprTwo> class TransformOp;
  template <typename U> class ReplicateOp;
  template <typename Xpr> class InverseOp;
  template <typename Xpr> class EulerAnglesOp;
};
};

#endif // __openma_math_forwarddeclarations_h