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

#ifndef __openma_math_map_h
#define __openma_math_map_h

namespace ma
{
namespace math
{
  template <typename Derived>
  struct Traits<Map<Derived>>
  {
    using Values = Eigen::Map<typename Traits<Derived>::Values>;
    using Residuals = Eigen::Map<typename Traits<Derived>::Residuals>;
    using Index = typename Values::Index;
    using Scalar = typename Values::Scalar;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Traits<Derived>::ColsAtCompileTime;
    static _OPENMA_CONSTEXPR int Processing = None;
  };
  
  template <typename Derived>
  struct Traits<Map<const Derived>>
  {
    using Values = Eigen::Map<const typename Traits<Derived>::Values>;
    using Residuals = Eigen::Map<const typename Traits<Derived>::Residuals>;
    using Index = typename Values::Index;
    using Scalar = const typename Values::Scalar;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = Traits<Derived>::ColsAtCompileTime;
    static _OPENMA_CONSTEXPR int Processing = None;
  };
  
  // ----------------------------------------------------------------------- //

  // NOTE: The documentation is inlined due to the use of the CRTP.

  template <typename Derived>
  class Map : public ArrayBase<Map<Derived>>
  {
  public:
    using Values = typename Traits<Map<Derived>>::Values;
    using Residuals = typename Traits<Map<Derived>>::Residuals;
    using Index = typename Traits<Map<Derived>>::Index;
    using Scalar = typename Traits<Map<Derived>>::Scalar;
    
    Map();
    Map(Index rows, Scalar* values, Scalar* residuals);
    template <typename U> Map(const Map<U>& other);
    
    Map& operator= (const Map& other) = delete;
    template <typename U> Map<Derived>& operator= (const XprBase<U>& other);
  };
  
  /**
   * @class Map openma/math/map.h
   * @brief An array expression mapping an existing array of data.
   * @tparam Derived Declaration of the mapping array expression
   * @todo Write a detailed description
   * @ingroup openma_math
   */
  /**
   * @var Map::Values
   * Type representing the data. Depending of the inheriting class (e.g Array or Map), the data are stored using specific Eigen (http://eigen.tuxfamily.org) object (i.e. Eigen::Array or Eigen::Map).
   */
  /**
   * @var Map::Residuals
   * Type representing the residuals associated with the data. Depending of the inheriting class (e.g Array or Map), the residuals are stored using specific Eigen (http://eigen.tuxfamily.org) object (i.e. Eigen::Array or Eigen::Map).
   */
  
  /**
   * @var Map::Index
   * Type used to access elements in Values or Residuals.
   */
  /**
   * @var Map::Scalar
   * Type used for each element in Values and Residuals.
   */
  
  /**
   * Default constructor.
   * This create a null map. The method isValid() will return false.
   * The use of this constructor is interesting only if you want to reuse a variable.
   */
  template <typename Derived>
  inline Map<Derived>::Map()
  : ArrayBase<Map<Derived>>(Values(nullptr,0,Values::ColsAtCompileTime),Residuals(nullptr,0,Residuals::ColsAtCompileTime))
  {};
  
  /**
   * Constructor where @a rows must correspond to the number of rows provided in @a values and @a residuals.
   * The number of elements in @a values must correspond to @a rows multiplied by the number of columns used by the @a Derived class.
   * The number of elements in @a residuals must correspond to @a rows elements.
   */
  template <typename Derived>
  inline Map<Derived>::Map(Index rows, Scalar* values, Scalar* residuals)
  : ArrayBase<Map<Derived>>(Values(values,rows,Derived::Values::ColsAtCompileTime), Residuals(residuals,rows,Derived::Residuals::ColsAtCompileTime))
  {};
  
  /**
   * Copy constructor for const Map only
   */
  template <typename Derived>
  template <typename U>
  inline Map<Derived>::Map(const Map<U>& other)
  : ArrayBase<Map<Derived>>(Values(other.values().data(),other.rows(),Derived::Values::ColsAtCompileTime),Residuals(other.residuals().data(),other.rows(),Derived::Residuals::ColsAtCompileTime))
  {
    static_assert(std::is_same<Derived, typename std::add_const<U>::type>::value, "You can only copy to a read-only (const) Map.");
    static_assert(Derived::Values::ColsAtCompileTime == U::Values::ColsAtCompileTime, "The number of columns must be the same.");
  };
  
  /**
   * Assignment operator from any other template expression
   * @warning You should be sur to have a Map object able to store the result of the template expression @a other. Otherwise, you should have a crash (buffer overflow). Do not use the default constructor.
   */
  template <typename Derived>
  template <typename U>
  inline Map<Derived>& Map<Derived>::operator= (const XprBase<U>& other)
  {
    Map::assign(*this, other);
    return *this;
  };
};
};

#endif // __openma_math_map_h