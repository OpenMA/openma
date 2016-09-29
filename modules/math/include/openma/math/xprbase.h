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

#ifndef __openma_math_xprbase_h
#define __openma_math_xprbase_h

namespace ma
{
namespace math
{
  // NOTE: The documentation is inlined due to the use of the CRTP.
  
  /**
   * @class XprBase openma/math/xprbase.h
   * @brief Base class for all template expressions used in the math module.
   * @tparam Derived Class representing any kind of expression
   * @todo Write a detailed description 
   * ingroup openma_math
   */
  template <typename Derived>
  class XprBase
  {
  public:
    
    using DerivedType = typename std::remove_const<Derived>::type; ///< True type used for the template expression
    
    using Index = typename Traits<DerivedType>::Index; ///< Type used to access elements in Values or Residuals.
    
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Traits<DerivedType>::ColsAtCompileTime; ///< Number of columns known at compile time
    
    static _OPENMA_CONSTEXPR int Processing = Traits<DerivedType>::Processing; ///< Determine the type of processing realized by the derived template expression regarding the computation of the values and the associated residuals. @note This information is only for internal purpose.
    
    /**
     * Returns a casted version of @a this object as a reference to a @a Derived type.
     */
    operator Derived& () _OPENMA_NOEXCEPT {return static_cast<Derived&>(*this);};
    
    /**
     * Returns a casted version of @a this object as a const reference to a @a Derived type.
     */
    operator const Derived& () const _OPENMA_NOEXCEPT {return static_cast<const Derived&>(*this);};
    
    /**
     * Convenient method to extract the first value if valid (i.e the associated residial is null or positive), otherwise returns 0.0.
     * @warning No assertion is realized to know if the expression is empty or not.
     */
    operator double () const _OPENMA_NOEXCEPT {return (static_cast<const Derived&>(*this).derived().residuals().eval().coeff(0) >= 0.0) ? static_cast<const Derived&>(*this).derived().values().eval().coeff(0) : 0.0;};
    
    // Next methods are defined after the declaration of the class BlockOp
    
    /**
     * Returns a block of the current expression with @a Cols columns and starting at the column @a index (0-based).
     */
    template <int Cols> BlockOp<Derived,Cols> block(Index index) _OPENMA_NOEXCEPT;
    
    /**
     * Returns a const block of the current expression with @a Cols columns and starting at the column @a index (0-based).
     */
    template <int Cols> const BlockOp<const Derived,Cols> block(Index index) const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class MeanOp
    
    /**
     * Returns an object representing an average of each row of this object.
     */
    const MeanOp<Derived> mean() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class NormOp
    
    /**
     * Returns an object representing the Euclidian norm of each row of this object.
     */
    const NormOp<Derived> norm() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class NormalizedOp

    /**
     * Returns an object representing this object with all this rows normalized.
     */ 
    const NormalizedOp<Derived> normalized() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class CrossOp
    
    /**
     * Returns an object representing a cross product operator between @a this object and the @a other object.
     */
    template <typename OtherDerived> const CrossOp<Derived, OtherDerived> cross(const XprBase<OtherDerived>& other) const _OPENMA_NOEXCEPT;
    
   // Next method is defined after the declaration of the class TransposeOp
   
   /**
    * Returns an object representing this object but where column were transposed.
    */
   const TransposeOp<Derived> transpose() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class InverseOp
    
    /**
     * Returns an object representing this object with all this rows inversed.
     */
    const InverseOp<Derived> inverse() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class ReplicateOp
    
    /**
     * Returns an object representing a replicated version of this @a rows time.
     */
    template <int U = 1> const ReplicateOp<Derived,U> replicate(Index rows = 1) const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class TransformOp
    
    /**
     * Returns an object representing a transformation between @a this object and the @a other object.
     */
    template <typename OtherDerived> const TransformOp<Derived,OtherDerived> transform(const XprBase<OtherDerived>& other) const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class SkewReduxOp
    
    /**
     * Returns an object representing the extraction of non null elements of a skew symmetric matrix.
     */
    const SkewReduxOp<Derived> skewRedux() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class DownsampleOp
    
    /**
     * Returns an object representing a version of @a this object reduced (decimated) by the factor @a f.
     * Each column of @a this object is treated independently. It is assumed that each column correspond to a discret uniformly spaced signal.
     * The type used by @c U can be @c int, @c float, or @c double. Other type will throw a static assertion.
     * If @a f is an integer factor (e.g. 8, 5.0f, or 10.0), only the @a f -th sample is kept.
     * In case of a rational factor, a linear interpolation method is used to compute 
     */
    template <typename U> const DownsampleOp<Derived,U> downsample(U f) const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class DerivativeOp
    
    /**
     * Returns an object representing the finite derivative of this template expression for the given order.
     * Boundaries (begin and end of the signal, samples before and after an occlusion) use forward and backward finite difference methods.
     * Other parts of the signal used the central difference method.
     * @note To reduce the computational time, the order of accuracy used for each method is equal to 2.
     */
    template <unsigned U> const DerivativeOp<Derived,U> derivative(double h) const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class MinOp
   
    /**
     * Returns an object representing the minimum on each column.
     */
    const MinOp<Derived> min() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class MaxOp
   
    /**
     * Returns an object representing the maximum on each column.
     */
    const MaxOp<Derived> max() const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class EulerAnglesOp
  
    /**
     * Returns an object representing an euler angles operation using the given order @a a0, @a a1, @a a2 for the sequence order.
     */
    const EulerAnglesOp<Derived> eulerAngles(Index a0, Index a1, Index a2) const _OPENMA_NOEXCEPT;
    
    // Next method is defined after the declaration of the class DotOp
    
    /**
     * Returns an object representing a dot product operator between @a this object and the @a other object.
     */
    template <typename OtherDerived> const DotOp<Derived,OtherDerived> dot(const XprBase<OtherDerived>& other) const _OPENMA_NOEXCEPT;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Returns a column vector representing the result of the given @a condition.
   * For elements where the condition is respected, the value is set to 0. Otherwise the value is set to -1.0.
   * This represent valid (>= 0) and invalid (< 0) reconstructed data.
   * @relates XprBase
   * @ingroup openma_math
   */
  template <typename U> 
  inline auto generate_residuals(const U& condition) -> decltype(condition.select(Traits<Array<U::ColsAtCompileTime>>::Residuals::Zero(condition.rows(),1),-1.0))
  {
    return condition.select(Traits<Array<U::ColsAtCompileTime>>::Residuals::Zero(condition.rows(),1),-1.0);
  };
  
  // ----------------------------------------------------------------------- //

  /**
   * Convenient minus operator to compute the difference between two template expressions
   * @relates XprBase
   * @relates ArrayBase
   */
  template <typename XprOne, typename XprTwo>
  const DifferenceOp<XprOne,XprTwo> operator-(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2) _OPENMA_NOEXCEPT
  {
    return DifferenceOp<XprOne,XprTwo>(x1,x2);
  };

  /**
   * Convenient plus operator to compute the sum between two template expressions
   * @relates XprBase
   * @relates ArrayBase
   */
  template <typename XprOne, typename XprTwo>
  const SumOp<XprOne,XprTwo> operator+(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2) _OPENMA_NOEXCEPT
  {
    return SumOp<XprOne,XprTwo>(x1,x2);
  };

  /**
  * Convenient multiplication operator to scale the template expressions @a x2 by the scalara@a x1.
   * @relates XprBase
   * @relates ArrayBase
   */
  template <typename Xpr>
  const ScaleOp<Xpr> operator* (double x1, const XprBase<Xpr>& x2)
  {
    return ScaleOp<Xpr>(x1,x2);
  };

  /**
   * Convenient multiplication operator to scale the template expressions @a x1 by @a x2.
   * @relates XprBase
   * @relates ArrayBase
   */
  template <typename Xpr>
  const ScaleOp<Xpr> operator* (const XprBase<Xpr>& x1, double x2)
  {
    return ScaleOp<Xpr>(x2,x1);
  };

  /**
   * Convenient division operator to scale the template expressions @a x1 by @a x2.
   * @relates XprBase
   * @relates ArrayBase
   */
  template <typename Xpr>
  const ScaleOp<Xpr> operator/ (const XprBase<Xpr>& x1, double x2)
  {
    assert(fabs(x2) >= std::numeric_limits<double>::epsilon());
    return ScaleOp<Xpr>(1.0/x2,x1);
  };
  
  /**
   * Convenient multiplication operator to create a coefficient-wise product of @a x1 and @a x2.
   * @relates XprBase
   * @relates ArrayBase
   */
  template <typename XprOne, typename XprTwo>
  const CoefficientProductOp<XprOne,XprTwo> operator* (const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
  {
    return CoefficientProductOp<XprOne,XprTwo>(x1,x2);
  };
};
};

#endif // __openma_math_xprbase_h
