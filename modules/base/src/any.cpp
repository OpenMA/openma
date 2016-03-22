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

#include "openma/base/any.h"

namespace ma
{
  /**
   * @class Any openma/base/any.h
   * @brief Generic type to store and convert other types.
   *
   * As the C++ language is a strong typed language it is not possible to use the 
   * same variable to hold an integer, a real, or a string.
   * In most of the cases, this is not a problem as this is a feature of the C++ language.
   * However, some applications would require to store different kind of data in a same
   * variable (like a database's fields). There are already existing solutions like
   * union keyword in the C++ language, Boost::Variant, Boost::Any, or QVariant solution.
   * However, some of them are only definable at compile time (union, Boost::Variant) 
   * or cannot freely convert stored value between different types (e.g. string to integer)
   * like Boost::Any. In case the stored type is not known at run-time and the use of the
   * run-time type information (RTTI) mechanism is discouraged, only the Qt class 
   * QVariant gives the possibity to store a generic content and convert it (if possible)
   * in the desired type. However, the use of QVariant means you need to link your code
   * the QtCore module which could be very heavy if you have a lightweight project.
   * Moreover, QVariant could support more type than desired (QColor, QFont, QByteArray, etc.)
   *
   * The class ma::Any was implemented to have a similiar behaviour than QVariant using
   * the Boost naming convention. Indeed, the purpose of ma::Any is closer of 
   * Boost::Any than Boost::Variant.
   *
   * The implementation of the class ma::Any relies heavily on the features introduced
   * in the C++11 revision of the C++ standard (constexpr, variadic templates, 
   * initializer lists, static assertions, rvalue, move, etc.).
   *
   * The internal behaviour of the class is mostly composed of three elements:
   *  - A type-erasure storage containing the data
   *  - A cast() method implemented differently depending the type requested (using the SFINAE technique)
   *  - A static table (std::unordered_map) containing all the custom conversions between registered types.
   *
   * By default, all the arithmetic types (bool, integers, floating points) and 
   * the std::string type are convertible between them. The casting to const char* is only available if the stored content is originally a std::string
   *
   * @code{.unparsed}
   * ma::Any a(5); // store an integer with the value 5
   * int ifive = a; // will contain the value 5.
   * std::string str = a; // will contain the string "5".
   * bool active = a; // will be set to true.
   * float ffive = a; // will store the value 5.0f.
   * a = "something else"; // And now 'a' contains a string
   * ifive = a; // invalid conversion, the stored value is now 0.
   * str = a; // Will contain the string "something else".
   * active = a; // Still the value is equal to true.
   * ffive = a; // Invalid conversion, the stored value is set to 0.0f
   * @endcode
   *
   * It is possible to register new types verified at compile time.
   * For this, the type must answer two requirements:
   * - Have a valid default constructor
   * - Have an non-member equal operator
   *
   * For example, if you want to register the class @c Date defined as:
   *
   * @code{.unparsed}
   * struct Date
   * {
   *   int Year, Month, Day;
   *
   *   friend bool operator==(const Date& lhs,const Date& rhs)
   *   {
   *     return ((lhs.Year == rhs.Year) && (lhs.Month == rhs.Month) && (lhs.Day == rhs.Day));
   *   };
   * };
   * @endcode
   *
   * It is then possible to register it using the ma::Any::Register command.
   *
   * @code{.unparsed}
   * // Must be in a function. For example the main(). The scope of the registration
   * // is valid until the end of the program.
   * ma::Any::Register< Date, ma::converter<>, ma::converter<> >();
   * //                 ^^^^
   * //      Registered type
   * //                       ^^^^^^^^^^^^^^^
   * //     A Date stored in a ma::Any object
   * //       can be converted to these types
   * //      (none available in this example)
   * //                                        ^^^^^^^^^^^^^^^
   * //          A Date object can be created from another type
   * //                         (none available in this example)
   * @endcode
   *
   * When conversions are intended. the internal converter must be 
   * specialized in consequence. For example, if a @c Date can be converted to a
   * @c std::string or created from a @c std::string, two specializations are required.
   *
   * @code{.unparsed}
   * template<>
   * // From std::string to Date.
   * struct ma::Any::Converter::Helper<std::string,Date> : ma::Any::Converter::HelperBase<std::string,Date>
   * {
   *   // Fake conversion. The goal is only the show the specialization of the converter. 
   *   static inline Date convert(const std::string& )
   *   {
   *     return Date{2009,05,02};
   *   };
   * };
   *
   * // From Date to std::string.
   * template<>
   * struct ma::Any::Converter::Helper<Date,std::string> : ma::Any::Converter::HelperBase<Date,std::string>
   * {
   *   static inline std::string convert(const Date& val)
   *   {
   *     return std::to_string(val.Year) + "-" + std::to_string(val.Month) + "-" + std::to_string(val.Day);
   *   };
   * };
   * @endcode 
   * 
   * The registration in the conversion table would be then:
   * @code{.unparsed}
   * ma::Any::Register< Date, ma::converter< Date,std::string >, ma::converter< std::string > >();
   * @endcode
   *
   * In case a user type has to be unregistered (e.g. plugin unloaded), the command 
   * ma::Any::Unregister must be called.
   * @code{.unparsed}
   * // The next line unregisters all the conversions related to a Date object. 
   * // You can still store a Date object in ma::Any, but only a conversion to
   * // it its own type will be valid
   * ma::Any::Unregister<Date>();
   * ma::Any a(Date{2009,05,02});
   * ma::Date b = a; // 2009,05,02
   * std::string c = a; // empty string
   * @endcode
   *
   * @ingroup openma_base
   */
  
  /**
   * Default constructor.
   * This kind of Any object is defined as null (method Any::isValid() returns true).
   */
  Any::Any() _OPENMA_NOEXCEPT
  : mp_Storage(nullptr)
  {};

  /**
   * Copy constructor.
   * If the content of the copied object (@c other) is not null, it will be cloned (deep copy) in the copy
   */
  Any::Any(const Any& other)
  : mp_Storage(other.mp_Storage ? other.mp_Storage->clone() : nullptr)
  {};

  /**
   * Move constructor
   * The content of @c other is moved to this object. The content of @c other is then defined as null (method Any::isValid() returns true).
   */
  Any::Any(Any&& other) _OPENMA_NOEXCEPT
  : mp_Storage(std::move(other.mp_Storage))
  {
    other.mp_Storage = nullptr;
  };

  /**
   * @fn template <typename U, typename D, typename > Any::Any(U&& value, D&& dimensions = D{});
   * Constructor which store the given @a value and @a dimensions (optional). When no dimension is given, this one is set automatically based on the given @a value.
   *
   * In case the number of elements in the @a value is not the same than the number of @a dimension, the later is chosen. Data can be truncated or expanded with default value.
   */
  
  /**
   * @fn template <typename U, typename > Any::Any(std::initializer_list<U> values, std::initializer_list<size_t> dimensions = {});
   * Constructor which store the given @a value and @a dimensions (optional). When no dimension is given, this one is set automatically based on the given @a value.
   *
   * In case the number of elements in the @a value is not the same than the number of @a dimension, the later is chosen. Data can be truncated or expanded with default value.
   */
    
  /**
   * Destrutor
   * Delete the internal data.
   */
  Any::~Any()
  {
    delete this->mp_Storage;
  };
  
  /**
   * Return a vector with the dimensions associated with the data stored.
   */
  std::vector<size_t> Any::dimensions() const _OPENMA_NOEXCEPT
  {
    return (this->mp_Storage != nullptr) ? this->mp_Storage->dimensions() : std::vector<size_t>{};
  };

  /**
   * Returns the number of elements stored.
   */
  size_t Any::size() const _OPENMA_NOEXCEPT
  {
    return (this->mp_Storage != nullptr) ? this->mp_Storage->size() : size_t(0);
  };
  
  /**
   * Return true if the object has a content, otherwise false.
   */
  bool Any::isValid() const _OPENMA_NOEXCEPT
  {
    return (this->mp_Storage != nullptr);
  };
  
  /**
   * Return true if the object as a non-empty content, otherwise false.
   * In case the object is not valid, this method returns false too.
   */
  bool Any::isEmpty() const _OPENMA_NOEXCEPT
  {
    return (!this->isValid() || this->mp_Storage->size() <= 0) ? true : false;
  };
 
  /**
   * Swap the content of two Any object.
   */
  void Any::swap(Any& other) _OPENMA_NOEXCEPT
  {
    std::swap(this->mp_Storage, other.mp_Storage);
  };
  
  
  /**
   * @fn template <typename U, typename> inline bool Any::isEqual(U&& value) const _OPENMA_NOEXCEPT
   * Convenient method to compare the content of an Any object with the given value.
   */
  
  /**
   * @fn template <typename U, typename D, typename> inline void Any::assign(U&& value, D&& dimensions) _OPENMA_NOEXCEPT
   * Convenient method to assign a @a value with optional @a dimensions to an Any object.
   */
  
  /**
   * @fn template <typename U, typename> inline void Any::assign(std::initializer_list<U> values, std::initializer_list<size_t> dimensions = {}) _OPENMA_NOEXCEPT
   * Convenient method to assign a @a value with optional @a dimensions to an Any object.
   */
  
  /** 
   * @fn template <typename U, typename> U Any::cast() const _OPENMA_NOEXCEPT
   * Method to explicitly convert the content of this object to the given type.
   *
   * @code{.unparsed}
   * ma::Any a("5");
   * std::string str = a.cast<std::string>();
   * @endcode
   *
   * @see Any::operator U()
   */
  
  /** 
   * @fn template <typename U, typename> U Any::cast(size_t idx) const _OPENMA_NOEXCEPT
   * Method to explicitely convert one element of this object to the given type.
   *
   * @code{.unparsed}
   * ma::Any a({"5","7"});
   * std::string str = a.cast<std::string>(1);
   * @endcode
   *
   * @see Any::operator U()
   */
  
  /**
   * @fn template<class U> Any::operator U() const _OPENMA_NOEXCEPT
   * Type conversion operator. Internally this operator uses the Any::cast() method.
   */

  /**
   * Copy assignement operator.
   * In case the assigned object is not this one, the previous content is deleted and replaced by a deep copy of the content of the @c other object.
   */
  Any& Any::operator=(const Any& other)
  {
    if (this != &other)
    {
      delete this->mp_Storage;
      this->mp_Storage = other.mp_Storage ? other.mp_Storage->clone() : nullptr;
    }
    return *this;
  };

  /**
   * Move assignement operator.
   * In case the assigned object is not this one, the previous content is deleted and replaced by the content of the @c other object. The @c other object is then defined as null (method Any::isValid() returns true).
   */
  Any& Any::operator=(Any&& other) _OPENMA_NOEXCEPT
  {
    if (this != &other)
    {
      delete this->mp_Storage;
      this->mp_Storage = other.mp_Storage;
      other.mp_Storage = nullptr;
    }
    return *this;
  };

  /**
   * Equal operator. Compare the content of two Any objects.
   */
  bool operator==(const Any& lhs, const Any& rhs) _OPENMA_NOEXCEPT
  {
    return ((lhs.mp_Storage != nullptr) && (rhs.mp_Storage != nullptr) && lhs.mp_Storage->compare(rhs.mp_Storage));
  };
  
 /**
  * Inequal operator. Returns the opposite of equal operator.
  */
  bool operator!=(const Any& lhs, const Any& rhs) _OPENMA_NOEXCEPT
  {
    return !(lhs == rhs);
  };
  
  /**
   * @fn template <typename U, typename A, typename> friend inline bool Any::operator==(const A& lhs, const U& rhs)
   * Convenient equal operator to compare an Any object (@c lhs) with another object of type @c U (@c rhs).
   */  
    
  /**
   * @fn template <typename U, typename A, typename> friend inline bool Any::operator==(const U& lhs, const A& rhs)
   * Convenient equal operator to compare an object of type @c U (@c lhs) with an Any object (@c rhs). Internally this operator does (rhs == lhs).
   */
  
  /**
   * @fn template <typename U, typename A, typename> friend inline bool Any::operator!=(const A& lhs, const U& rhs)
   * Convenient inequal operator to compare an Any object (@c lhs) with another object of type @c U (@c rhs). Internally this operator take the opposite of the equal operator.
   */
  
  /**
   * @fn template <typename U, typename A, typename> friend inline bool Any::operator!=(const U& lhs, const A& rhs)
   * Convenient inequal operator to compare an object of type @c U (@c lhs) with an Any object (@c rhs). Internally this operator take the opposite of the equal operator.
   */

  // ----------------------------------------------------------------------- //

  namespace __details
  {
    /**
     * Returns the conversion table used for all the Any object.
     *
     * @note The returned object is a singleton as proposed by Scott Meyers in C++11.
     */
    _Any_converter_map& _any_converter_map() _OPENMA_NOEXCEPT
    {
      static _Any_converter_map table;
      return table;
    };
  
    /**
     * Extract the convertion function pointer based on the ID used from the type source (@c sid) and the returned type (@c rid)
     * In case no function pointer was found, the returned value is set to nullptr.
     */
    _Any_convert_t _any_extract_converter(typeid_t sid, typeid_t rid) _OPENMA_NOEXCEPT
    {
      auto it = _any_converter_map().find(_any_hash(static_cast<size_t>(sid),static_cast<size_t>(rid)));
      return (it != _any_converter_map().end()) ? it->second : nullptr;
    };
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct Any::Conversion openma/base/any.h
   * @brief Utilitary to define conversion from/to a Any object.
   * The role of this class is to facilitate the registration of a type using Any::Register. 
   */
  
  /**
   * @struct Any::Register openma/base/any.h
   * @brief Utilitary to register a new type in an Any object.
   */
  
 /**
  * @struct Any::Unregister openma/base/any.h
  * @brief Utilitary to unregister a new type in an Any object.
  */
};