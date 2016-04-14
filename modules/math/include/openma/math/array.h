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

#ifndef __openma_math_array_h
#define __openma_math_array_h

namespace ma
{
namespace math
{
  // ----------------------------------------------------------------------- //
  //                                   ARRAY
  // ----------------------------------------------------------------------- //
  
  template <int Cols>
  struct Traits<Array<Cols>>
  {
    using Values = Eigen::Array<double,Eigen::Dynamic,Cols>;
    using Residuals = Eigen::Array<double,Eigen::Dynamic,1>;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Cols;
    static _OPENMA_CONSTEXPR int Processing = None;
  };
  
  // ----------------------------------------------------------------------- //

  // NOTE: The documentation is inlined due to the use of the CRTP.

  /**
   * @class Array openma/math/array.h
   * @brief Class to store reconstructed data.
   * @tparam Cols The number of columns for the data (without the colum for the residuals)
   *
   * This class is usefull for any reconstructed data that can be stored as an array with any number of columns.
   * The values can be access by the method values() and the reconstruction residuals with the method residuals().
   * In general, this is not necessary to pass by these methods but directly use the operators and methods to do computations.
   * Indeed, internally, this class use template expression to manage invalid data (negative residuals). There is no need to loop through samples to check if it is valid or not.
   *
   * @todo Add example related to the automatic management of the residuals.
   *
   * @ingroup openma_math
   */
  template <int Cols>
  class Array : public ArrayBase<Array<Cols>>
  {
  public:
    using Values = typename Traits<Array<Cols>>::Values; ///< Type representing the data. 
    using Residuals = typename Traits<Array<Cols>>::Residuals; ///< Type representing the residuals associated with the data.
    using Index = typename Traits<Array<Cols>>::Index; ///< Type used to access elements in Values or Residuals.
    
    /**
     * Default constructor. Create an empty array (method isValue() returns false).
     */
    Array();
    
    /**
     * Create an array with a fixed number of @a Cols columns and a dynamic number of @a rows. 
     */
    Array(Index rows);
    
    /**
     * Create an array based on the given @a values. All the samples are set to valid (residuals set to 0).
     * @note The number of colums for the @a values must be the same than the number of @a Cols.
     */
    Array(const Values& values);
    
    /**
     * Create a constructor based on the given @a values and @b residuals.
     * @note The number of row for the @a values and teh @a residuals must be the same.
     */
    Array(const Values& values, const Residuals& residuals);
    
    /**
     * Constructor from an XprBase object (generaly used to store the result of a computation).
     * @code{.unparsed}
     * // a and b are other Array object.
     * auto c = a + b; // The assignment of c is realized with this constructor. 
     * @endcode
     */
    template <typename U> Array(const XprBase<U>& other);
    
    /**
     * Assignment operator from an XprBase object.
     */
    template <typename U> inline Array<Cols>& operator= (const XprBase<U>& other);
  };
  
  template <int Cols>
  inline Array<Cols>::Array()
  : ArrayBase<Array<Cols>>(Values(),Residuals())
  {};
  
  template <int Cols>
  inline Array<Cols>::Array(Index rows)
  : ArrayBase<Array<Cols>>(Values(rows,Cols),Residuals(rows,1))
  {};
  
  template <int Cols>
  inline Array<Cols>::Array(const Values& values)
  : ArrayBase<Array<Cols>>(values,Residuals::Zero(values.rows(),1))
  {};
  
  template <int Cols>
  inline Array<Cols>::Array(const Values& values, const Residuals& residuals)
  : ArrayBase<Array<Cols>>(values,residuals)
  {};
  
  template <int Cols>
  template <typename U>
  inline Array<Cols>::Array(const XprBase<U>& other)
  : Array()
  {
    Array::assign(*this, other);
  };
  
  template <int Cols>
  template <typename U>
  Array<Cols>& Array<Cols>::operator= (const XprBase<U>& other)
  {
    Array::assign(*this, other);
    return *this;
  };
  
  // ----------------------------------------------------------------------- //
  //                                   SCALAR
  // ----------------------------------------------------------------------- //
  
  /**
   * @headerfile openma/math/array.h
   * Define an Array with one column. This could represent a scalar value along a sequence.
   * @relates Array
   * @ingroup openma_math
   */
  using Scalar = Array<1>;
  
  // ----------------------------------------------------------------------- //
  //                                   VECTOR
  // ----------------------------------------------------------------------- //
  
  /**
   * @headerfile openma/math/array.h
   * Define an Array with three columns. This could represent a 3D vector along a sequence.
   * @relates Array
   * @ingroup openma_math
   */
  using Vector = Array<3>;
  
  // ----------------------------------------------------------------------- //
  //                                  POSITION
  // ----------------------------------------------------------------------- //
  
  /**
   * @headerfile openma/math/array.h
   * Define an Array with three columns. This could represent a 3D trajectory along a sequence.
   * @relates Array
   * @ingroup openma_math
   */
  using Position = Vector;
  
  // ----------------------------------------------------------------------- //
  //                                    POSE
  // ----------------------------------------------------------------------- //

  template <>
  struct Traits<Pose> : Traits<Array<12>>
  {};

  // ----------------------------------------------------------------------- //
  
  /**
   * @class Pose openma/math/array.h
   * @brief Define an Array with 12 columns to represent a 3D orientation and a 3D position along a sequence.
   * Convenient class with dedicated methods to construct/access poses.
   * @relates Array
   * @ingroup openma_math
   */
  class Pose : public Array<12>
  {
  public:
    using Orientations = Array<9>::Values; ///< Type used for the orientation part of a pose
    using Positions = Array<3>::Values; ///< Type used for the position part of a pose
    
    Pose();
    Pose(Index rows);
    template <typename U, typename V, typename W, typename O> Pose(const XprBase<U>& u, const XprBase<V>& v, const XprBase<W>& w, const XprBase<O>& o);
    template <typename U> Pose(const XprBase<U>& other);
    
    /**
     * Returns the nine first columns to access to the 3D orientation part.
     */
    const Eigen::Block<const Array<12>::Values> orientations() const _OPENMA_NOEXCEPT {return this->m_Values.topLeftCorner(this->rows(),Orientations::ColsAtCompileTime);};
    
    /**
     * Returns the three last columns to access to the 3D orientation part.
     */
    const Eigen::Block<const Array<12>::Values> positions() const _OPENMA_NOEXCEPT {return this->m_Values.topRightCorner(this->rows(),Positions::ColsAtCompileTime);};
  };
  
  /**
   * Default constructor
   */
  inline Pose::Pose()
  : Array<12>()
  {};
 
  /**
   * Initialize a motion with a number of samples (lines) equals to @a rows 
   */
  inline Pose::Pose(Index rows)
  : Array<12>(rows)
  {};
 
  /**
   * Constructor from vectors (u,v,w,o).
   * Internally, these vectors are joined into an array with 12 components. They represents a compact affine transformation.
   * The storage order is the following u_x, u_y, u_z, v_x, v_x, v_y, v_z, w_x, w_y, w_z, o_x, o_y, o_z.
   */
  template <typename U, typename V, typename W, typename O>
  inline Pose::Pose(const XprBase<U>& u, const XprBase<V>& v, const XprBase<W>& w, const XprBase<O>& o)
  : Array<12>(static_cast<const U&>(u).rows())
  {
    static_assert(U::ColsAtCompileTime == 3, "The input 'u' must be an array with 3 columns (i.e. a Vector)");
    static_assert(V::ColsAtCompileTime == 3, "The input 'v' must be an array with 3 columns (i.e. a Vector)");
    static_assert(W::ColsAtCompileTime == 3, "The input 'w' must be an array with 3 columns (i.e. a Vector)");
    static_assert(O::ColsAtCompileTime == 3, "The input 'o' must be an array with 3 columns (i.e. a Vector)");
    const auto& xpr1 = static_cast<const U&>(u).derived();
    const Vector::Values v1 = xpr1.values();
    const Vector::Residuals r1 = xpr1.residuals();
    const auto& xpr2 = static_cast<const V&>(v).derived();
    const Vector::Values v2 = xpr2.values();
    const Vector::Residuals r2 = xpr2.residuals();
    const auto& xpr3 = static_cast<const W&>(w).derived();
    const Vector::Values v3 = xpr3.values();
    const Vector::Residuals r3 = xpr3.residuals();
    const auto& xpr4 = static_cast<const O&>(o).derived();
    const Vector::Values v4 = xpr4.values();
    const Vector::Residuals r4 = xpr4.residuals();
    assert(v1.rows() == v2.rows());
    assert(v2.rows() == v3.rows());
    assert(v3.rows() == v4.rows());
    assert(r1.rows() == r2.rows());
    assert(r2.rows() == r3.rows());
    assert(r3.rows() == r4.rows());
    const Eigen::Array<bool,Eigen::Dynamic,1> cond = (r1 >= 0.0) && (r2 >= 0.0) && (r3 >= 0.0) && (r4 >= 0.0);
    for (Index i = 0 ; i < cond.rows() ; ++i)
    {
      if (cond.coeff(i))
      {
        this->m_Values.block<1,3>(i,0) = v1.row(i);
        this->m_Values.block<1,3>(i,3) = v2.row(i);
        this->m_Values.block<1,3>(i,6) = v3.row(i);
        this->m_Values.block<1,3>(i,9) = v4.row(i);
        this->m_Residuals.coeffRef(i) = 0.0;
      }
      else
      {
        this->m_Values.row(i).setZero();
        this->m_Residuals.coeffRef(i) = -1.0;  
      }
    }
  };
  
  /**
   * Convenient constructor to store the result of an expression.
   */
  template <typename U>
  Pose::Pose(const XprBase<U>& other)
  : Array<12>(other)
  {};
  
};
};

#endif // __openma_math_array_h