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

#ifndef __openma_math_binaryop_h
#define __openma_math_binaryop_h

namespace ma
{
namespace math
{
  // ----------------------------------------------------------------------- //
  //                                 BINARYOP
  // ----------------------------------------------------------------------- //
  
  template <typename Derived, typename XprOne, typename XprTwo>
  struct Traits<BinaryOp<Derived,XprOne,XprTwo>>
  {
    using Values = typename Traits<XprOne>::Values;
    using Residuals = typename Traits<XprOne>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int  ColsAtCompileTime = Traits<XprTwo>::ColsAtCompileTime; // XprTwo to have the good number of columns after some operation like TransformOp
    static _OPENMA_CONSTEXPR int Processing = Traits<Derived>::Processing;
  };
  
  template <typename Derived, typename XprOne, typename XprTwo>
  struct Nested<BinaryOp<Derived,XprOne,XprTwo>>
  {
    using type = Derived; // Plain object
  };
  
  // ----------------------------------------------------------------------- //
  
  // NOTE: The documentation is inlined due to the use of the CRTP.
  
  /**
   * @class BinaryOp openma/math/binaryop.h
   * @brief Base class for binary operations
   * @tparam Derived Class representing a binary operation
   * @tparam XprOne type of the left hand side operation
   * @tparam XprTwo type of the right hand side operation
   * All binary operations shall inherit of this class to correctly manage the generation of the associated residuals.
   * Inherit classes must implement also the following methods:
   *  - Index rows() const _OPENMA_NOEXCEPT
   *  - auto values() const _OPENMA_NOEXCEPT
   *  - auto residuals() const _OPENMA_NOEXCEPT
   * @ingroup openma_math
   */
  template <typename Derived, typename XprOne, typename XprTwo>
  class BinaryOp : public XprBase<BinaryOp<Derived, XprOne, XprTwo>>
  {
    using Index = typename Traits<BinaryOp<Derived, XprOne, XprTwo>>::Index; ///< Type used to access elements in Values or Residuals.
    
  protected:
    typename Nested<XprOne>::type m_Xpr1; ///< Storage (by reference or by value) of the expression used in the left hand side.
    typename Nested<XprTwo>::type m_Xpr2; ///< Storage (by reference or by value) of the expression used in the right hand side.
    
  public:
    BinaryOp() = delete;
    
    /**
     * Store the expressions @a x1 and @a x2 as plain objects or references depending of their type.
     */
    BinaryOp(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
    : m_Xpr1(static_cast<const XprOne&>(x1).derived()), m_Xpr2(static_cast<const XprTwo&>(x2).derived())
    {
      assert(this->m_Xpr1.rows() > 0);
      assert(this->m_Xpr2.rows() > 0);
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
    };
    
    /**
     * Returns the number of rows that shall have the result of the binary operation. Internaly, this method relies on the rows() method of the Derived class.
     */
    Index rows() const _OPENMA_NOEXCEPT {return static_cast<const Derived&>(*this).rows();};
    
    /**
     * Returns the number of columns that shall have the result of the binary operation. Internaly, this method relies on the template arguments of given expressions.
     */
    Index cols() const _OPENMA_NOEXCEPT {return Traits<BinaryOp<Derived,XprOne,XprTwo>>::ColsAtCompileTime;};
    
    /**
     * Cast this object as a const reference of the Derived class.
     */
    const Derived& derived() const _OPENMA_NOEXCEPT {return static_cast<const Derived&>(*this);};
    
    BinaryOp& operator=(const BinaryOp& ) = delete;
  };
  
  // ----------------------------------------------------------------------- //
  //                                  DIFFERENCEOP
  // ----------------------------------------------------------------------- //
  
  template <typename XprOne, typename XprTwo>
  struct Traits<DifferenceOp<XprOne,XprTwo>>
  {
    static _OPENMA_CONSTEXPR int Processing = Full;
  };

  // ----------------------------------------------------------------------- //
  
  /**
   * @class DifferenceOp openma/math/binaryop.h
   * @brief Compute difference between given expressions
   * @tparam XprOne type of the left hand side operation
   * @tparam XprTwo type of the right hand side operation
   * Template expression to compute an algebric difference. This class is used by the method XprBase::operator-().
   * @ingroup openma_math
   */
  template <typename XprOne, typename XprTwo>
  class DifferenceOp : public BinaryOp<DifferenceOp<XprOne, XprTwo>, XprOne, XprTwo>
  {
    static_assert(XprOne::ColsAtCompileTime == XprTwo::ColsAtCompileTime, "The number of columns must be the same.");
    
    using Index = typename Traits<BinaryOp<DifferenceOp<XprOne, XprTwo>, XprOne, XprTwo>>::Index; ///< * Type used to access elements in Values or Residuals.
    
  public:
    
    /**
     * Constructor
     */
    DifferenceOp(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
    : BinaryOp<DifferenceOp<XprOne, XprTwo>, XprOne, XprTwo>(x1,x2)
    {
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
    };
    
   /**
    * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the first expresion.
    */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr1.rows();};
   
    /**
     * Returns the difference of the two expressions as a template expression.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(XprOne).values() - OPENMA_MATHS_DECLVAL_NESTED(XprTwo).values())
    {
      return this->m_Xpr1.values() - this->m_Xpr2.values();
    };
  
    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the ones of each input.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprTwo).residuals() >= 0.0)))
    {
      return generate_residuals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0));
    };
  };

  // ----------------------------------------------------------------------- //
  //                                  SUMOP
  // ----------------------------------------------------------------------- //

  template <typename XprOne, typename XprTwo>
  struct Traits<SumOp<XprOne,XprTwo>>
  {
    static _OPENMA_CONSTEXPR int Processing = Full;
  };
  
  // ----------------------------------------------------------------------- //

  /**
   * @class SumOp openma/math/binaryop.h
   * @brief Compute the sum of two expressions
   * @tparam XprOne type of the left hand side operation
   * @tparam XprTwo type of the right hand side operation
   * Template expression to compute an algebric sum. This class is used by the method XprBase::operator+().
   * @ingroup openma_math
   */
  template <typename XprOne, typename XprTwo>
  class SumOp : public BinaryOp<SumOp<XprOne, XprTwo>, XprOne, XprTwo>
  {
    static_assert(XprOne::ColsAtCompileTime == XprTwo::ColsAtCompileTime, "The number of columns must be the same.");
    
    using Index = typename Traits<BinaryOp<SumOp<XprOne, XprTwo>, XprOne, XprTwo>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:
    /**
     * Constructor
     */
    SumOp(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
    : BinaryOp<SumOp<XprOne, XprTwo>, XprOne, XprTwo>(x1,x2)
    {
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
    };
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the first expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr1.rows();};

    /**
     * Returns the sum of the two expressions as a template expression.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(XprOne).values() + OPENMA_MATHS_DECLVAL_NESTED(XprTwo).values())
    {
      return this->m_Xpr1.values() + this->m_Xpr2.values();
    };
  
    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the ones of each input.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprTwo).residuals() >= 0.0)))
    {
      return generate_residuals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0));
    };
  };

  // ----------------------------------------------------------------------- //
  //                                 CROSSOP
  // ----------------------------------------------------------------------- //
  
  template <typename XprOne, typename XprTwo>
  struct Traits<CrossOp<XprOne,XprTwo>>
  {
    static _OPENMA_CONSTEXPR int Processing = Full;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class CrossOp openma/math/binaryop.h
   * @brief Compute the cross product of two expressions
   * @tparam XprOne type of the left hand side operation
   * @tparam XprTwo type of the right hand side operation
   * Template expression to compute a geometric cross product.
   * @ingroup openma_math
   */
  template <typename XprOne, typename XprTwo>
  class CrossOp : public BinaryOp<CrossOp<XprOne, XprTwo>, XprOne, XprTwo>
  {
    static_assert(XprOne::ColsAtCompileTime == XprTwo::ColsAtCompileTime, "The number of columns must be the same.");
    static_assert(XprOne::ColsAtCompileTime == 3 && XprTwo::ColsAtCompileTime == 3, "The cross operation is only available for array with 3 columns.");
    
    using Index = typename Traits<BinaryOp<CrossOp<XprOne, XprTwo>, XprOne, XprTwo>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:    
    /**
     * Constructor
     */
    CrossOp(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
    : BinaryOp<CrossOp<XprOne, XprTwo>, XprOne, XprTwo>(x1,x2)
    {
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
    };
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the first expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr1.rows();};

    /**
     * Returns the cross product of the two expressions as a template expression.
     */
    auto values() const _OPENMA_NOEXCEPT -> Eigen::internal::CrossOpValues<decltype(OPENMA_MATHS_DECLVAL_NESTED(XprOne).values()),decltype(OPENMA_MATHS_DECLVAL_NESTED(XprTwo).values())>
    {
      using V1 = decltype(this->m_Xpr1.values());
      using V2 = decltype(this->m_Xpr2.values());
      return Eigen::internal::CrossOpValues<V1,V2>(this->m_Xpr1.values(), this->m_Xpr2.values());
    };
  
    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the ones of each input.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprTwo).residuals() >= 0.0)))
    {
      return generate_residuals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0));
    };
  };
  
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  template <typename U>
  inline const CrossOp<Derived,U> XprBase<Derived>::cross(const XprBase<U>& other) const _OPENMA_NOEXCEPT
  {
    return CrossOp<Derived,U>(*this,other);
  };

  // ----------------------------------------------------------------------- //
  //                               TRANSFORMOP
  // ----------------------------------------------------------------------- //
  
  template <typename XprOne, typename XprTwo>
  struct Traits<TransformOp<XprOne,XprTwo>>
  {
    static _OPENMA_CONSTEXPR int Processing = Full;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class TransformOp openma/math/binaryop.h
   * @brief Compute the transformation of two expressions
   * @tparam XprOne type of the left hand side operation
   * @tparam XprTwo type of the right hand side operation
   * Template expression to compute a geometric transformation like A * B (Pose x Pose) or A * b (Pose x Vector).
   * @ingroup openma_math
   */
  template <typename XprOne, typename XprTwo>
  class TransformOp : public BinaryOp<TransformOp<XprOne, XprTwo>, XprOne, XprTwo>
  { 
    using Index = typename Traits<BinaryOp<TransformOp<XprOne, XprTwo>, XprOne, XprTwo>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:
    /**
     * Constructor
     */
    TransformOp(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
    : BinaryOp<TransformOp<XprOne, XprTwo>, XprOne, XprTwo>(x1,x2)
    {};
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the first expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr1.rows();};

    /**
     * Returns the transformation of the two expressions as a template expression.
     */
    auto values() const _OPENMA_NOEXCEPT -> Eigen::internal::TransformOpValues<decltype(OPENMA_MATHS_DECLVAL_NESTED(XprOne).values()),decltype(OPENMA_MATHS_DECLVAL_NESTED(XprTwo).values())>
    {
      using V1 = decltype(this->m_Xpr1.values());
      using V2 = decltype(this->m_Xpr2.values()); 
      return Eigen::internal::TransformOpValues<V1,V2>(this->m_Xpr1.values(), this->m_Xpr2.values());
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the ones of each input.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprTwo).residuals() >= 0.0)))
    {
      return generate_residuals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0));
    };
  };

  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  template <typename U>
  inline const TransformOp<Derived,U> XprBase<Derived>::transform(const XprBase<U>& other) const _OPENMA_NOEXCEPT
  {
    return TransformOp<Derived,U>(*this,other);
  };
  
  // ----------------------------------------------------------------------- //
  //                              DOTPRODUCTOP
  // ----------------------------------------------------------------------- //
  
  template <typename XprOne, typename XprTwo>
  struct Traits<DotOp<XprOne,XprTwo>>
  {
    static _OPENMA_CONSTEXPR int Processing = Full;
  };
  
  template <typename XprOne, typename XprTwo>
  struct Traits<BinaryOp<DotOp<XprOne,XprTwo>,XprOne,XprTwo>>
  {
    using Values = typename Traits<XprOne>::Values;
    using Residuals = typename Traits<XprOne>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = 1;
    static _OPENMA_CONSTEXPR int Processing = Traits<DotOp<XprOne,XprTwo>>::Processing;
  }; 
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class DotOp openma/math/binaryop.h
   * @brief Compute the dot product of two expressions
   * @tparam XprOne type of the left hand side operation
   * @tparam XprTwo type of the right hand side operation
   * Template expression to compute a geometric dot product.
   * @ingroup openma_math
   */
  template <typename XprOne, typename XprTwo>
  class DotOp : public BinaryOp<DotOp<XprOne, XprTwo>, XprOne, XprTwo>
  {
    static_assert(XprOne::ColsAtCompileTime == XprTwo::ColsAtCompileTime, "The number of columns must be the same.");
    static_assert(XprOne::ColsAtCompileTime == 3 && XprTwo::ColsAtCompileTime == 3, "The cross operation is only available for array with 3 columns.");
    
    using Index = typename Traits<BinaryOp<DotOp<XprOne, XprTwo>, XprOne, XprTwo>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:    
    /**
     * Constructor
     */
    DotOp(const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2)
    : BinaryOp<DotOp<XprOne, XprTwo>, XprOne, XprTwo>(x1,x2)
    {
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
    };
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the first expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr1.rows();};

    /**
     * Returns the dot product of the two expressions as a template expression.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype((OPENMA_MATHS_DECLVAL_NESTED(XprOne).values() * OPENMA_MATHS_DECLVAL_NESTED(XprTwo).values()).rowwise().sum())
    {
      return (this->m_Xpr1.values() * this->m_Xpr2.values()).rowwise().sum();
    };
  
    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the ones of each input.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprTwo).residuals() >= 0.0)))
    {
      return generate_residuals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0));
    };
  };

  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  template <typename U>
  inline const DotOp<Derived,U> XprBase<Derived>::dot(const XprBase<U>& other) const _OPENMA_NOEXCEPT
  {
    return DotOp<Derived,U>(*this,other);
  };
};
};

#endif // __openma_math_binaryop_h