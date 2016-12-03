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

#ifndef __openma_base_typeid_h
#define __openma_base_typeid_h

#include "openma/base_export.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT

#include <cstddef> // size_t
#include <utility> // std::move
#include <string> // Used for the required export with MSVC

namespace ma
{
  class typeid_t
  {
  public:
    typeid_t();
    ~typeid_t() _OPENMA_NOEXCEPT = default;
    typeid_t(const typeid_t& ) = default;
    typeid_t(typeid_t&& other) _OPENMA_NOEXCEPT;
    typeid_t& operator=(const typeid_t& ) = default;
    typeid_t& operator=(typeid_t&& other) _OPENMA_NOEXCEPT;
    
    explicit operator size_t() const _OPENMA_NOEXCEPT {return reinterpret_cast<size_t>(this->id);};
    
    friend _OPENMA_CONSTEXPR bool operator==(typeid_t lhs, typeid_t rhs) _OPENMA_NOEXCEPT {return (lhs.id == rhs.id);};
    friend _OPENMA_CONSTEXPR bool operator!=(typeid_t lhs, typeid_t rhs) _OPENMA_NOEXCEPT {return (lhs.id != rhs.id);};
    
  private:
    template<typename T> friend typeid_t static_typeid() _OPENMA_NOEXCEPT;
    
    using sig = typeid_t(*)();
    sig id;
    
    _OPENMA_CONSTEXPR typeid_t(sig id) : id{id} {};
  };
  
  inline typeid_t::typeid_t()
  : id(nullptr)
  {};
  
  inline typeid_t::typeid_t(typeid_t&& other) _OPENMA_NOEXCEPT
  : id(std::move(other.id))
  {
    other.id = nullptr;
  };
  
  inline typeid_t& typeid_t::operator=(typeid_t&& other) _OPENMA_NOEXCEPT
  {
    if (this != &other)
    {
      this->id = other.id;
      other.id = nullptr;
    }
    return *this;
  };

  template <typename T>
  typeid_t static_typeid() _OPENMA_NOEXCEPT
  {
#if defined(_MSC_VER)
    static auto odr = &static_typeid<T>;
    return reinterpret_cast<typeid_t::sig>(odr);
#else
    return &static_typeid<T>;
#endif
  };
  
  /**
   * @class typeid_t openma/base/typeid.h
   * @brief Unique identifier for each type without using runtime type identifier
   *
   * Internally, and compared to the RTTI mechanism, this class use the so-called one-definition rule (ODR).
   * <blockquote>
   * [...] If an identifier declared with external linkage is used in an expression [...], somewhere in the entire program there shall be exactly one external definition for the identifier; [...]
   * </blockquote>
   *
   * Largely inspired by http://codereview.stackexchange.com/questions/48594/unique-type-id-no-rtti
   *
   * @ingroup openma_base
   */
    
  /**
   * @fn typeid_t::typeid_t();
   * Default constructor (nullptr id)
   */
    
  /**
   * @fn typeid_t::~typeid_t() _OPENMA_NOEXCEPT;
   * Destructor
   */
   
  /**
   * @fn typeid_t::typeid_t(const typeid_t& );
   * Copy constructor
   */
   
  /**
   * @fn typeid_t::typeid_t(typeid_t&& other) _OPENMA_NOEXCEPT;
   * Move constructor
   */
   
  /**
   * @fn typeid_t& typeid_t::operator=(const typeid_t& );
   * Assignment operator
   */
   
  /**
   * @fn typeid_t& typeid_t::operator=(typeid_t&& other) _OPENMA_NOEXCEPT;
   * Assignment operator
   */
   
  /**
   * @fn typeid_t::operator size_t() const _OPENMA_NOEXCEPT;
   * Explicit converter to the type @c size_t
   */
   
  /**
   * @fn bool typeid_t::operator==(typeid_t lhs, typeid_t rhs) _OPENMA_NOEXCEPT;
   * Equality operator. Compare if the internal ID are the same between both objects
   */
   
  /**
   * @fn bool typeid_t::operator!=(typeid_t lhs, typeid_t rhs) _OPENMA_NOEXCEPT;
   * Inequality operator. Compare if the internal ID are not the same between both objects
   */
   
  /**
   * @fn template<typename T> typeid_t static_typeid() _OPENMA_NOEXCEPT
   * Returns the identifier associated with the given template type
   */
}

/**
 * Convenient macro to export the static type ID associated with the new class @a classname with the export symbol @a exportname.
 * Internally this create an external reference to a symbol defined by the macro OPENMA_INSTANCE_STATIC_TYPEID()
 * This is required to verify the static typeid of an object created by another shared library.
 * @relates ma::Node
 * @ingroup openma_base
 */
#define OPENMA_EXPORT_STATIC_TYPEID(classname, exportname) \
  namespace ma { \
    extern template exportname typeid_t static_typeid<classname>() _OPENMA_NOEXCEPT; \
  };
/**
 * Convient macro to instantiate the static_typeid template for the given @a classname.
 * This is required to verify the static typeid of an object by from another shared library.
 * @relates ma::Node
 * @ingroup openma_base
 */
#define OPENMA_INSTANCE_STATIC_TYPEID(classname) \
  namespace ma { \
    template typeid_t static_typeid<classname>() _OPENMA_NOEXCEPT; \
  }; 
  
// Arithmetic types
OPENMA_EXPORT_STATIC_TYPEID(bool, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(char, OPENMA_BASE_EXPORT)
// OPENMA_EXPORT_STATIC_TYPEID(char16_t, OPENMA_BASE_EXPORT) // same as unsigned short int
// OPENMA_EXPORT_STATIC_TYPEID(char32_t, OPENMA_BASE_EXPORT) // same as unsigned int
OPENMA_EXPORT_STATIC_TYPEID(wchar_t, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(signed char, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(short int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(long int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(long long int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(unsigned char, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(unsigned short int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(unsigned int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(unsigned long int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(unsigned long long int, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(float, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(double, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(long double, OPENMA_BASE_EXPORT)
// String types
OPENMA_EXPORT_STATIC_TYPEID(std::string, OPENMA_BASE_EXPORT)
OPENMA_EXPORT_STATIC_TYPEID(const char*, OPENMA_BASE_EXPORT)

#endif // __openma_base_typeid_h