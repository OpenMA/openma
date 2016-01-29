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

#ifndef __openma_maths_arraybase_h
#define __openma_maths_arraybase_h

namespace ma
{
namespace maths
{
  // ----------------------------------------------------------------------- //
  
  template <typename Derived>
  struct Traits<ArrayBase<Derived>>
  {
    using Values = typename Traits<Derived>::Values;
    using Residuals = typename Traits<Derived>::Residuals;
    using Index = typename Traits<Derived>::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Traits<Derived>::ColsAtCompileTime;
    static _OPENMA_CONSTEXPR int Processing = Traits<Derived>::Processing;
  };
  
  template <typename Derived>
  struct Nested<ArrayBase<Derived>>
  {
    using type = const ArrayBase<Derived>&; // Reference
  };
  
  // ----------------------------------------------------------------------- //
  
  // NOTE: The documentation is inlined due to the use of the CRTP.
  
  /**
   * @class ArrayBase
   * @brief Base class for data storage
   * @tparam Derived Exact type used to store data
   * @todo Write a detailed description
   * @ingroup openma_maths
   */
  template <typename Derived>
  class ArrayBase : public XprBase<ArrayBase<Derived>>
  {
  public:
    using Values = typename Traits<ArrayBase<Derived>>::Values; ///< Type representing the data. Depending of the inheriting class (e.g Array or Map), the data are stored using specific Eigen (http://eigen.tuxfamily.org) object (i.e. Eigen::Array or Eigen::Map).
    using Residuals = typename Traits<ArrayBase<Derived>>::Residuals; ///< Type representing the residuals associated with the data. Depending of the inheriting class (e.g Array or Map), the residuals are stored using specific Eigen (http://eigen.tuxfamily.org) object (i.e. Eigen::Array or Eigen::Map).
    using Index = typename Traits<ArrayBase<Derived>>::Index; ///< Type used to access elements in Values or Residuals.
    
    /**
     * Default copy constructor
     */
    ArrayBase(const ArrayBase& other) = default;
    
    /**
     * Returns the number of values' rows .
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Values.rows();};
    
    /**
     * Returns the number of values' columns.
     */
    Index cols() const _OPENMA_NOEXCEPT {return this->m_Values.cols();};
    
    /**
     * Returns a reference for the values stored in this object.
     */
    Values& values() _OPENMA_NOEXCEPT {return this->m_Values;};
    
    /**
     * Returns a const reference for the values stored in this object.
     */
    const Values& values() const _OPENMA_NOEXCEPT {return this->m_Values;};
    
    /**
     * Returns a reference for the residuals associated with the stored values. The number of rows is equal to the number of values' rows. The number of columns is always 1.
     */
    Residuals& residuals() _OPENMA_NOEXCEPT {return this->m_Residuals;};
    
    /**
     * Returns a const reference for the residuals associated with the stored values. The number of rows is equal to the number of values' rows. The number of columns is always 1.
     */
    const Residuals& residuals() const _OPENMA_NOEXCEPT {return this->m_Residuals;};
    
    /**
     * Convient method to determine if the content of the object is valid. A valid object has at least 1 row. An object with no row is invalid.
     */
    bool isValid() const _OPENMA_NOEXCEPT {return (this->m_Values.rows() != 0) && (this->m_Residuals.rows() != 0);};
    
    /**
     * Convient method to determine if some value were not correctly reconstructed (i.e. occluded). Internaly, the associated residual is analyzed to find if some of them are negative.
     */
    bool isOccluded() const _OPENMA_NOEXCEPT {return (!this->isValid() || (this->m_Residuals < 0.0).all());};
    
    /**
     * Cast the object to a @a Derived type reference.
     */
    Derived& derived() _OPENMA_NOEXCEPT {return static_cast<Derived&>(*this);};
    
    /**
     * Cast the object to a @a Derived type consts reference.
     */
    const Derived& derived() const _OPENMA_NOEXCEPT {return static_cast<const Derived&>(*this);};
    
    // Next methods are defined after the declaration of the class BlockOp
    
    /**
     * Convenient method to extract the first column of the values.
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one(s) requested.
     */
    BlockOp<ArrayBase<Derived>,1> x() _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the first column of the values.
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one(s) requested.
     */
    const BlockOp<const ArrayBase<Derived>,1> x() const _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the second column of the values.
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one(s) requested.
     */
    BlockOp<ArrayBase<Derived>,1> y() _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the second column of the values.
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one(s) requested.
     */
    const BlockOp<const ArrayBase<Derived>,1> y() const _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the third column of the values.
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one(s) requested.
     */
    BlockOp<ArrayBase<Derived>,1> z() _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the third column of the values.
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one(s) requested.
     */
    const BlockOp<const ArrayBase<Derived>,1> z() const _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the columns #1-3 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */
    BlockOp<ArrayBase<Derived>,3> u() _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the columns #1-3 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */
    const BlockOp<const ArrayBase<Derived>,3> u() const _OPENMA_NOEXCEPT;
    
   /**
    * Convenient method to extract the columns #4-6 of the values (could be usefull with Pose object).
    * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
    */
    BlockOp<ArrayBase<Derived>,3> v() _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the columns #4-6 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */
    const BlockOp<const ArrayBase<Derived>,3> v() const _OPENMA_NOEXCEPT;
    
    /**
     * Convenient method to extract the columns #7-9 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */
    BlockOp<ArrayBase<Derived>,3> w() _OPENMA_NOEXCEPT;
   
    /**
     * Convenient method to extract the columns #7-9 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */
    const BlockOp<const ArrayBase<Derived>,3> w() const _OPENMA_NOEXCEPT;

    /**
     * Convenient method to extract the columns #10-12 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */ 
    BlockOp<ArrayBase<Derived>,3> o() _OPENMA_NOEXCEPT;

    /**
     * Convenient method to extract the columns #10-12 of the values (could be usefull with Pose object).
     * @warning This method does not check the number of existing columns. Unknown behaviour will happen if the number of columns is less than the one requested.
     */
    const BlockOp<const ArrayBase<Derived>,3> o() const _OPENMA_NOEXCEPT;
    
  protected:
    ArrayBase() = delete;
    
    /**
     * Constructor from data
     */
    ArrayBase(const Values& values, const Residuals& residuals);
    
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    template <typename U, typename V>
    static inline typename std::enable_if<Traits<V>::Processing == None>::type assign(ArrayBase<U>& lhs, const XprBase<V>& rhs)
    {
      static_assert(Traits<U>::ColsAtCompileTime == Traits<V>::ColsAtCompileTime, "The number of columns is not the same.");
      auto& self = static_cast<U&>(lhs).derived();
      const auto& other = static_cast<const V&>(rhs).derived();
      self.values().lazyAssign(other.values());
      self.residuals().lazyAssign(other.residuals());
    };
    
    template <typename U, typename V>
    static inline typename std::enable_if<Traits<V>::Processing == ValuesOnly>::type assign(ArrayBase<U>& lhs, const XprBase<V>& rhs)
    {
      static_assert(Traits<U>::ColsAtCompileTime == Traits<V>::ColsAtCompileTime, "The number of columns is not the same.");
      auto& self = static_cast<U&>(lhs).derived();
      const auto& other = static_cast<const V&>(rhs).derived();
      self.residuals().lazyAssign(generate_residuals(other.residuals() >= 0.0));
      self.values().lazyAssign((self.residuals().template replicate<1,Values::ColsAtCompileTime>() >= 0.0).select(other.values(),0.0));
    };
    
    template <typename U, typename V>
    static inline typename std::enable_if<Traits<V>::Processing == Full>::type assign(ArrayBase<U>& lhs, const XprBase<V>& rhs)
    {
      static_assert(Traits<U>::ColsAtCompileTime == Traits<V>::ColsAtCompileTime, "The number of columns is not the same.");
      auto& self = static_cast<U&>(lhs).derived();
      const auto& other = static_cast<const V&>(rhs).derived();
      self.residuals().lazyAssign(other.residuals());
      self.values().lazyAssign((self.residuals().template replicate<1,Values::ColsAtCompileTime>() >= 0.0).select(other.values(),0.0));
    };
#endif
    
    Values m_Values; ///< Storage of the values
    Residuals m_Residuals; ///< Storage of the associated residuals
  };
   
  template <typename Derived>
  inline ArrayBase<Derived>::ArrayBase(const Values& values, const Residuals& residuals)
  : m_Values(values), m_Residuals(residuals)
  {
    assert(values.rows() == residuals.rows());
  };
  
};
};

#endif // __openma_maths_arraybase_h