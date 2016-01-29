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

#ifndef __openma_maths_unaryop_h
#define __openma_maths_unaryop_h

namespace ma
{
namespace maths
{
  
  // ----------------------------------------------------------------------- //
  //                                  UNARYOP
  // ----------------------------------------------------------------------- //
  
  template <typename Derived, typename Xpr>
  struct Traits<UnaryOp<Derived,Xpr>>
  {
    using Values = typename Traits<Xpr>::Values;
    using Residuals = typename Traits<Xpr>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Traits<Xpr>::ColsAtCompileTime;
    static _OPENMA_CONSTEXPR int Processing = Traits<Derived>::Processing;
  };
  
  template <typename Derived, typename Xpr>
  struct Nested<UnaryOp<Derived,Xpr>>
  {
    using type = Derived; // Plain object
  };
  
  // ----------------------------------------------------------------------- //

  // NOTE: The documentation is inlined due to the use of the CRTP.

  /**
   * @class UnaryOp openma/maths/unaryop.h
   * @brief Base class for unary operations
   * @tparam Derived Class representing a binary operation
   * @tparam Xpr Type of the expression to transform
   * All unary operations shall inherit of this class to correctly manage the generation of the associated residuals.
   * Inherit classes must implement also the following methods:
   *  - Index rows() const _OPENMA_NOEXCEPT
   *  - auto values() const _OPENMA_NOEXCEPT
   *  - auto residuals() const _OPENMA_NOEXCEPT
   * @ingroup openma_maths
   */
  template <typename Derived, typename Xpr>
  class UnaryOp : public XprBase<UnaryOp<Derived, Xpr>>
  {
    using Index = typename Traits<UnaryOp<Derived, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
  protected:
    typename Nested<Xpr>::type m_Xpr; ///< Storage (by reference or by value) of the expression to transform.
    
  public:
    UnaryOp() = delete;
    
    /**
     * Store the expression @a x as plain object or reference depending of their type.
     */
    UnaryOp(const XprBase<Xpr>& x)
    : m_Xpr(static_cast<const Xpr&>(x).derived())
    {
      assert(this->m_Xpr.rows() > 0);
    };
    
    /**
     * Returns the number of rows that shall have the result of the unary operation. Internaly, this method relies on the rows() method of the Derived class.
     */
    Index rows() const _OPENMA_NOEXCEPT {return static_cast<const Derived&>(*this).rows();};
    
    /**
     * Returns the number of columns that shall have the result of the binary operation. Internaly, this method relies on the template arguments of given expression.
     */
    Index cols() const _OPENMA_NOEXCEPT {return Traits<UnaryOp<Derived,Xpr>>::ColsAtCompileTime;};
    
    /**
     * Cast this object as a const reference of the Derived class.
     */
    const Derived& derived() const _OPENMA_NOEXCEPT {return static_cast<const Derived&>(*this);};
    
    UnaryOp& operator=(const UnaryOp& ) = delete;
  };
  
  // ----------------------------------------------------------------------- //
  //                                  NORMOP
  // ----------------------------------------------------------------------- //
  
  template <typename Xpr>
  struct Traits<NormOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = ValuesOnly;
  };
  
  template <typename Xpr>
  struct Traits<UnaryOp<NormOp<Xpr>,Xpr>>
  {
    using Values = typename Traits<Xpr>::Values;
    using Residuals = typename Traits<Xpr>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = 1;
    static _OPENMA_CONSTEXPR int Processing = Traits<NormOp<Xpr>>::Processing;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class NormOp openma/maths/unaryop.h
   * @brief Compute the Euclidean norm for each row 
   * @tparam Xpr Type of the expression to transform
   * Template expression to compute the Euclidean norm for each row and the associated residuals.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class NormOp : public UnaryOp<NormOp<Xpr>,Xpr>
  {
    using Index = typename Traits<UnaryOp<NormOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:
    
    /**
     * Constructor
     */
    NormOp(const XprBase<Xpr>& x)
    : UnaryOp<NormOp<Xpr>,Xpr>(x)
    {};

    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr.rows();};

    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values().square().rowwise().sum().sqrt())
    {
      return this->m_Xpr.values().square().rowwise().sum().sqrt();
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals())
    {
      return this->m_Xpr.residuals();
    };
  };
  
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  inline const NormOp<Derived> XprBase<Derived>::norm() const _OPENMA_NOEXCEPT
  {
    return NormOp<Derived>(*this);
  };
  
  // ----------------------------------------------------------------------- //
  //                                  MEANOP
  // ----------------------------------------------------------------------- //
  
  template <typename Xpr>
  struct Traits<MeanOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = Full;
  };
  
  // ----------------------------------------------------------------------- //

  /**
   * @class MeanOp openma/maths/unaryop.h
   * @brief Compute the average for each row
   * @tparam Xpr Type of the expression to transform
   * Template expression to compute the average for each row and the associated residuals.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class MeanOp : public UnaryOp<MeanOp<Xpr>,Xpr>
  {
    using Index = typename Traits<UnaryOp<MeanOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:
    /**
     * Constructor
     */
    MeanOp(const XprBase<Xpr>& x)
    : UnaryOp<MeanOp<Xpr>,Xpr>(x)
    {}
      
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr.rows();};

    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
#ifndef DOXYGEN_SHOULD_SKIP_THIS // Doxygen does not like the use of '.template'
    auto values() const _OPENMA_NOEXCEPT -> decltype((OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals() >= 0.0).template replicate<1,Xpr::ColsAtCompileTime>().select(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values(), 0.0).colwise().sum() / ((OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals() >= 0.0).any() ? (OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals() >= 0.0).count() : 1.0))
#else
    auto values() const _OPENMA_NOEXCEPT
#endif
    {
      const auto& cond = (this->m_Xpr.residuals() >= 0.0);
      return cond.template replicate<1,Xpr::ColsAtCompileTime>().select(this->m_Xpr.values(), 0.0).colwise().sum() / (cond.any() ? cond.count() : 1.0);
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
#ifndef DOXYGEN_SHOULD_SKIP_THIS // Doxygen does not like the use of '.template'
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals() >= 0.0).colwise().sum()))
#else
    auto residuals() const _OPENMA_NOEXCEPT  
#endif
    {
      return generate_residuals((this->m_Xpr.residuals() >= 0.0).colwise().sum());
    };
  };
  
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  inline const MeanOp<Derived> XprBase<Derived>::mean() const _OPENMA_NOEXCEPT
  {
    return MeanOp<Derived>(*this);
  };
  
  // ----------------------------------------------------------------------- //
  //                                 SCALEOP
  // ----------------------------------------------------------------------- //

  template <typename Xpr>
  struct Traits<ScaleOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = ValuesOnly;
  };
  
  // ----------------------------------------------------------------------- //

  /**
   * @class ScaleOp openma/maths/unaryop.h
   * @brief Scale the data
   * @tparam Xpr Type of the expression to transform
   * Template expression to scale the content of each row and the associated residuals.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class ScaleOp : public UnaryOp<ScaleOp<Xpr>,Xpr>
  {
    using Index = typename Traits<UnaryOp<ScaleOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
    double m_Scale; ///< Scaling factor

  public:
    /**
     * Constructor
     */
    ScaleOp(double scale, const XprBase<Xpr>& x)
    : UnaryOp<ScaleOp<Xpr>,Xpr>(x), m_Scale(scale)
    {}
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr.rows();};

    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype(std::declval<double>() * OPENMA_MATHS_DECLVAL_NESTED(Xpr).values())
    {
      return this->m_Scale * this->m_Xpr.values();
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals())
    {
      return this->m_Xpr.residuals();
    };
  };
  
  // ----------------------------------------------------------------------- //
  //                               NORMALIZEDOP
  // ----------------------------------------------------------------------- //

  template <typename Xpr>
  struct Traits<NormalizedOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = ValuesOnly;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class NormalizedOp openma/maths/unaryop.h
   * @brief Normalize the data
   * @tparam Xpr Type of the expression to transform
   * Template expression to normalize each row and the associated residuals.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class NormalizedOp : public UnaryOp<NormalizedOp<Xpr>,Xpr>
  {
    using Index = typename Traits<UnaryOp<NormalizedOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
  public:
    /**
     * Constructor
     */
    NormalizedOp(const XprBase<Xpr>& x)
    : UnaryOp<NormalizedOp<Xpr>,Xpr>(x)
    {};

    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr.rows();};

    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values().cwiseQuotient((OPENMA_MATHS_DECLVAL_NESTED(Xpr).values().square().rowwise().sum() >= std::numeric_limits<double>::epsilon()).select(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values().square().rowwise().sum().sqrt(),1.0).replicate(1,3)))
    {
      const auto temp = this->m_Xpr.values().square().rowwise().sum();
      const auto norm = (temp >= std::numeric_limits<double>::epsilon()).select(temp.sqrt(),1.0).replicate(1,this->m_Xpr.cols());
      return this->m_Xpr.values().cwiseQuotient(norm);
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals())
    {
      return this->m_Xpr.residuals();
    };
  };
   
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  inline const NormalizedOp<Derived> XprBase<Derived>::normalized() const _OPENMA_NOEXCEPT
  {
    return NormalizedOp<Derived>(*this);
  };
  
  // ----------------------------------------------------------------------- //
  //                               REPLICATEOP
  // ----------------------------------------------------------------------- //
  
  template <typename Xpr>
  struct Traits<ReplicateOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = None;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class ReplicateOp openma/maths/unaryop.h
   * @brief Replicate the rows vertically.
   * @tparam Xpr Type of the expression to transform
   * Template expression to replicate the rows and generate the associated residuals.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class ReplicateOp : public UnaryOp<ReplicateOp<Xpr>,Xpr>
  {
    using Index = typename Traits<UnaryOp<ReplicateOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
    Index m_Rows;
    
  public:
    /**
     * Constructor
     */
    ReplicateOp(const XprBase<Xpr>& x, Index rows)
    : UnaryOp<ReplicateOp<Xpr>,Xpr>(x), m_Rows(rows)
    {};
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT
    {
      return this->m_Xpr.rows() * this->m_Rows;
    };

    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
    auto values() const _OPENMA_NOEXCEPT -> Eigen::Replicate<typename std::decay<decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values())>::type, Eigen::Dynamic, Eigen::Dynamic>
    {
      return this->m_Xpr.values().replicate(this->m_Rows,1);
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> Eigen::Replicate<typename std::decay<decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals())>::type, Eigen::Dynamic, Eigen::Dynamic>
    {
      return this->m_Xpr.residuals().replicate(this->m_Rows,1);
    };
  };
  
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  inline const ReplicateOp<Derived> XprBase<Derived>::replicate(Index rows) const _OPENMA_NOEXCEPT
  {
    return ReplicateOp<Derived>(*this,rows);
  };
  
  // ----------------------------------------------------------------------- //
  //                                 INVERSEOP
  // ----------------------------------------------------------------------- //
  
  template <typename Xpr>
  struct Traits<InverseOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = ValuesOnly;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class InverseOp openma/maths/unaryop.h
   * @brief Compute the inverse
   * @tparam Xpr Type of the expression to transform
   * Template expression to compute the inverse for each row and the associated residuals.
   * @note this operator would be used with Pose object or Array with 12 columns.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class InverseOp : public UnaryOp<InverseOp<Xpr>,Xpr>
  {
    using Index = typename Traits<UnaryOp<InverseOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
    static_assert(Xpr::ColsAtCompileTime == 12, "The inverse operation is only available for array with 12 columns.");
    
  public:
    /**
     * Constructor
     */
    InverseOp(const XprBase<Xpr>& x)
    : UnaryOp<InverseOp<Xpr>,Xpr>(x)
    {};
    
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr.rows();};

    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
    auto values() const _OPENMA_NOEXCEPT -> decltype(Eigen::internal::InverseOpValues<decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values())>(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values()))
    {
      using V = decltype(this->m_Xpr.values());
      return Eigen::internal::InverseOpValues<V>(this->m_Xpr.values());
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals())
    {
      return this->m_Xpr.residuals();
    };
  };
  
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  inline const InverseOp<Derived> XprBase<Derived>::inverse() const _OPENMA_NOEXCEPT
  {
    return InverseOp<Derived>(*this);
  };
  
  // ----------------------------------------------------------------------- //
  //                              EULERANGLESOP
  // ----------------------------------------------------------------------- //
  
  template <typename Xpr>
  struct Traits<EulerAnglesOp<Xpr>>
  {
    static _OPENMA_CONSTEXPR int Processing = ValuesOnly;
  };
  
  template <typename Xpr>
  struct Traits<UnaryOp<EulerAnglesOp<Xpr>,Xpr>>
  {
    using Values = typename Traits<Xpr>::Values;
    using Residuals = typename Traits<Xpr>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = 3;
    static _OPENMA_CONSTEXPR int Processing = Traits<EulerAnglesOp<Xpr>>::Processing;
  }; 
    
  // ----------------------------------------------------------------------- //

  /**
   * @class EulerAnglesOp openma/maths/unaryop.h
   * @brief Compute euler angles.
   * @tparam Xpr Type of the expression to transform
   * Template expression to compute euler angles for each row and the associated residuals.
   * 
   * @note This operator would be used with an array with at least 9 columns representing an orientation. It can be for example a Pose object.
   * @ingroup openma_maths
   */
  template <typename Xpr>
  class EulerAnglesOp : public UnaryOp<EulerAnglesOp<Xpr>,Xpr>
  {
    static_assert(Xpr::ColsAtCompileTime >= 9, "The Euler angles operation is only available for array with at least 9 columns.");
    
    using Index = typename Traits<UnaryOp<EulerAnglesOp<Xpr>, Xpr>>::Index; ///< Type used to access elements in Values or Residuals.
    
    Index m_Axis0;
    Index m_Axis1;
    Index m_Axis2;
    
  public:
    /**
     * Constructor. The Index @a a0, @a a1, @a a2 reprensent the sequence order.
     *  - The rotation along the axis X will be given with the value 0.
     *  - The rotation along the axis Y will be given with the value 1.
     *  - The rotation along the axis Z will be given with the value 2.
     * @code
     * auto eao1 = EulerAnglesOp(pose,0,1,2); // Extract eurler angles using axes X, Y', and Z".
     * auto eao2 = EulerAnglesOp(pose,2,0,1); // Extract eurler angles using axes Z, X', and Y".
     * @endcode
     */
    EulerAnglesOp(const XprBase<Xpr>& x, Index a0, Index a1, Index a2)
    : UnaryOp<EulerAnglesOp<Xpr>,Xpr>(x),
      m_Axis0(a0), m_Axis1(a1), m_Axis2(a2)
    {};
      
    /**
     * Returns the number of rows that shall have the result of this operation. Internaly, this method relies on the number of rows of the given expresion.
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr.rows();};
      
    /**
     * Returns a template expression corresponding to the calculation of this operation.
     */
    auto values() const _OPENMA_NOEXCEPT -> Eigen::internal::EulerAnglesOpValues<decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).values())>
    {
      using V = decltype(this->m_Xpr.values());
      return Eigen::internal::EulerAnglesOpValues<V>(this->m_Xpr.values(), this->m_Axis0, this->m_Axis1, this->m_Axis2);
    };

    /**
     * Returns the residuals associated with this operation. The residuals is generated based on the input one.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(OPENMA_MATHS_DECLVAL_NESTED(Xpr).residuals())
    {
      return this->m_Xpr.residuals();
    };
  };
  
  // Defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  template <typename Derived>
  inline const EulerAnglesOp<Derived> XprBase<Derived>::eulerAngles(Index a0, Index a1, Index a2) const _OPENMA_NOEXCEPT
  {
    return EulerAnglesOp<Derived>(*this,a0,a1,a2);
  };

};
};

#endif // __openma_maths_unaryop_h