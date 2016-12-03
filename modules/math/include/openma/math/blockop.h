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

#ifndef __openma_math_blockop_h
#define __openma_math_blockop_h

namespace ma
{
namespace math
{
  template <typename Xpr, int Cols>
  struct Traits<BlockOp<Xpr,Cols>>
  {
    using Values = typename Traits<Array<Cols>>::Values;
    using Residuals = typename Traits<Array<Cols>>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Cols;
    static _OPENMA_CONSTEXPR int Processing = Traits<typename std::remove_const<Xpr>::type>::Processing;
  };
  
  template <typename Xpr, int Cols>
  struct Nested<BlockOp<Xpr,Cols>>
  {
    using type = const BlockOp<Xpr,Cols>; // Plain object
  };
  
  // ----------------------------------------------------------------------- //
  
  // NOTE: The documentation is inlined due to the use of the CRTP.
  
  /**
   * @class BlockOp
   * @brief Expression to extract a block of data. 
   * @tparam Xpr Type of the expression that will be used by the block operation.
   * @tparam Cols The number of columns to extract.
   * @todo Write a detailed description
   * @ingroup openma_math
   */
  template <typename Xpr, int Cols>
  class BlockOp : public XprBase<BlockOp<Xpr, Cols>>
  {
    static_assert(Xpr::ColsAtCompileTime >= Cols, "The requested number of columns is greater than the number of columns of the array.");
    
    using Index = typename Traits<BlockOp<Xpr,Cols>>::Index; ///< Type used to access elements in Values or Residuals.
    
    Xpr& mr_Xpr; ///< Store a reference of the the expression to use
    Index m_Index; ///< Store the index of the first column to use
    
  public:
    BlockOp() = delete;
    
    /**
     * Store a reference of the expression @a xpr and the @a index of the first column to extract.
     * @note The number of columns (@a Cols) plus the @a index must be lower or equal than the number of columns in the expression @a xpr.
     */
    BlockOp(Xpr& x, Index index)
    : mr_Xpr(x), m_Index(index)
    {
      assert(this->mr_Xpr.cols() >= (this->m_Index + Cols));
    };
    
    /**
     * Returns the number of values' rows .
     */
    Index rows() const _OPENMA_NOEXCEPT {return this->mr_Xpr.rows();};
    
    /**
     * Returns the number of values' columns.
     */
    Index cols() const _OPENMA_NOEXCEPT {return Cols;};
    
    /**
     * Return a const reference of this object
     */
    const BlockOp& derived() const _OPENMA_NOEXCEPT {return *this;};
    
    /**
     * Returns a block expression from the expression's values.
     */
    auto values() _OPENMA_NOEXCEPT -> Eigen::Block<typename std::remove_reference<decltype(std::declval<Xpr>().derived().values())>::type,Eigen::Dynamic,Cols>
    {
      return this->mr_Xpr.derived().values().template block<Eigen::Dynamic,Cols>(0,this->m_Index,this->mr_Xpr.derived().values().rows(),Cols);
    };
    
    /**
     * Returns a block expression from the expression's values.
     */
    auto values() const _OPENMA_NOEXCEPT -> Eigen::Block<typename std::remove_reference<decltype(std::declval<Xpr>().derived().values())>::type,Eigen::Dynamic,Cols>
    {
      return this->mr_Xpr.derived().values().template block<Eigen::Dynamic,Cols>(0,this->m_Index,this->mr_Xpr.derived().values().rows(),Cols);
    };

    /**
     * Returns a block expression from the expression's residuals.
     */
    auto residuals() _OPENMA_NOEXCEPT -> Eigen::Block<typename std::remove_reference<decltype(std::declval<Xpr>().derived().residuals())>::type,Eigen::Dynamic,1>
    {
      return this->mr_Xpr.derived().residuals().template block<Eigen::Dynamic,1>(0,0,this->mr_Xpr.derived().residuals().rows(),1);
    };
    
    /**
     * Returns a block expression from the expression's residuals.
     */
    auto residuals() const _OPENMA_NOEXCEPT -> Eigen::Block<typename std::remove_reference<decltype(std::declval<Xpr>().derived().residuals())>::type,Eigen::Dynamic,1>
    {
      return this->mr_Xpr.derived().residuals().template block<Eigen::Dynamic,1>(0,0,this->mr_Xpr.derived().residuals().rows(),1);
    };
    
    /**
     * Assignment operator from another block object. This will assign the content of @a other to the expression stored in this Block object.
     */    
    BlockOp& operator= (const BlockOp& other)
    {
      this->values().lazyAssign(other.values());
      this->residuals().lazyAssign(generate_residuals((this->residuals() >= 0.0) && (other.residuals() >= 0.0)));
      return *this;
    };

     /**
      * Assignment operator from another block object. This will assign the content of @a other to the expression stored in this Block object.
      */
    template <typename U> BlockOp& operator= (const XprBase<U>& other)
    {
      static_assert(U::ColsAtCompileTime == Cols, "The number of columns must be the same.");
      auto o = static_cast<const typename XprBase<U>::DerivedType&>(other).derived();
      this->values() = o.values();
      this->residuals() = generate_residuals((this->residuals() >= 0.0) && (o.residuals() >= 0.0));
      return *this;
    };
    
    /**
     * Addition of the scalar @a other with the values of this object.
     */
    BlockOp& operator+=(double other) {this->values() += other; return *this;};
    
    /**
     * Substraction of the scalar @a other with the values of this object.
     */
    BlockOp& operator-=(double other) {this->values() -= other; return *this;};
    
    /**
     * Multiply this object by the scalar @a other
     */
    BlockOp& operator*=(double other) {this->values() *= other; return *this;};
    
    /**
     * Divide this object by the scalar @a other
     */
    BlockOp& operator/=(double other) {this->values() /= other; return *this;};
    
    /**
     * Addition of the @a other template expression to this object.
     */
    template<typename OtherDerived> BlockOp& operator+=(const XprBase<OtherDerived>& other);
    
    /**
     * Addition of the @a other template expression to this object.
     */
    template<typename OtherDerived> BlockOp& operator-=(const XprBase<OtherDerived>& other);
    
    /**
     * Addition of the @a other template expression to this object.
     */
    template<typename OtherDerived> BlockOp& operator*=(const XprBase<OtherDerived>& other);
    
    /**
     * Addition of the @a other template expression to this object.
     */
    template<typename OtherDerived> BlockOp& operator/=(const XprBase<OtherDerived>& other);
  };
  
  template <typename Xpr, int Cols>
  template<typename OtherDerived>
  inline BlockOp<Xpr,Cols>& BlockOp<Xpr,Cols>::operator+=(const XprBase<OtherDerived>& other)
  {
    static_assert(Cols == Traits<OtherDerived>::ColsAtCompileTime, "The number of columns is not the same.");
    auto& otherderived = static_cast<const OtherDerived&>(other).derived();
    assert(this->rows() == otherderived.rows());
    this->values() += otherderived.values();
    this->residuals() = generate_residuals((this->residuals() >= 0.0) && (otherderived.residuals() >= 0.0));
    return *this;
  };
  
  template <typename Xpr, int Cols>
  template<typename OtherDerived>
  inline BlockOp<Xpr,Cols>& BlockOp<Xpr,Cols>::operator-=(const XprBase<OtherDerived>& other)
  {
    static_assert(Cols == Traits<OtherDerived>::ColsAtCompileTime, "The number of columns is not the same.");
    auto& otherderived = static_cast<const OtherDerived&>(other).derived();
    assert(this->rows() == otherderived.rows());
    this->values() -= otherderived.values();
    this->residuals() = generate_residuals((this->residuals() >= 0.0) && (otherderived.residuals() >= 0.0));
    return *this;
  };
  
  template <typename Xpr, int Cols>
  template<typename OtherDerived>
  inline BlockOp<Xpr,Cols>& BlockOp<Xpr,Cols>::operator*=(const XprBase<OtherDerived>& other)
  {
    static_assert(Cols == Traits<OtherDerived>::ColsAtCompileTime, "The number of columns is not the same.");
    auto& otherderived = static_cast<const OtherDerived&>(other).derived();
    assert(this->rows() == otherderived.rows());
    this->values() *= otherderived.values();
    this->residuals() = generate_residuals((this->residuals() >= 0.0) && (otherderived.residuals() >= 0.0));
    return *this;
  };
  
  template <typename Xpr, int Cols>
  template<typename OtherDerived>
  inline BlockOp<Xpr,Cols>& BlockOp<Xpr,Cols>::operator/=(const XprBase<OtherDerived>& other)
  {
    static_assert(Cols == Traits<OtherDerived>::ColsAtCompileTime, "The number of columns is not the same.");
    auto& otherderived = static_cast<const OtherDerived&>(other).derived();
    assert(this->rows() == otherderived.rows());
    this->values() /= otherderived.values();
    this->residuals() = generate_residuals((this->residuals() >= 0.0) && (otherderived.residuals() >= 0.0));
    return *this;
  };

  // ----------------------------------------------------------------------- //
  
  // The following methods are defined here due to the declaration order of the classes. The associated documentation is in the header of the XprBase class.
  
  template <typename Derived>
  template <int Cols>
  inline BlockOp<Derived,Cols> XprBase<Derived>::block(Index index) _OPENMA_NOEXCEPT
  {
    return BlockOp<Derived,Cols>(*this,index);
  };

  template <typename Derived>
  template <int Cols>
  inline const BlockOp<const Derived,Cols> XprBase<Derived>::block(Index index) const _OPENMA_NOEXCEPT
  {
    return BlockOp<const Derived,Cols>(*this,index);
  };
  
  // ----------------------------------------------------------------------- //
  
  // The following methods are defined here due to the declaration order of the classes. The associated documentation is in the header of the ArrayBase class.
  
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,1> ArrayBase<Derived>::x() _OPENMA_NOEXCEPT {return this->template block<1>(0);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,1> ArrayBase<Derived>::x() const _OPENMA_NOEXCEPT {return this->template block<1>(0);};
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,1> ArrayBase<Derived>::y() _OPENMA_NOEXCEPT {return this->template block<1>(1);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,1> ArrayBase<Derived>::y() const _OPENMA_NOEXCEPT {return this->template block<1>(1);};
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,1> ArrayBase<Derived>::z() _OPENMA_NOEXCEPT {return this->template block<1>(2);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,1> ArrayBase<Derived>::z() const _OPENMA_NOEXCEPT {return this->template block<1>(2);};
  
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,3> ArrayBase<Derived>::u() _OPENMA_NOEXCEPT {return this->template block<3>(0);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,3> ArrayBase<Derived>::u() const _OPENMA_NOEXCEPT {return this->template block<3>(0);};
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,3> ArrayBase<Derived>::v() _OPENMA_NOEXCEPT {return this->template block<3>(3);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,3> ArrayBase<Derived>::v() const _OPENMA_NOEXCEPT {return this->template block<3>(3);};
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,3> ArrayBase<Derived>::w() _OPENMA_NOEXCEPT {return this->template block<3>(6);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,3> ArrayBase<Derived>::w() const _OPENMA_NOEXCEPT {return this->template block<3>(6);};
  template <typename Derived> inline BlockOp<ArrayBase<Derived>,3> ArrayBase<Derived>::o() _OPENMA_NOEXCEPT {return this->template block<3>(9);};
  template <typename Derived> inline const BlockOp<const ArrayBase<Derived>,3> ArrayBase<Derived>::o() const _OPENMA_NOEXCEPT {return this->template block<3>(9);};
  
};
};

#endif // __openma_math_blockop_h