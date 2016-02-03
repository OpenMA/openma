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

#ifndef __openma_io_handleroption_h
#define __openma_io_handleroption_h

#include "openma/base/logger.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT

#include <vector>
#include <tuple>
#include <array>

#define OPENMA_DECLARE_IO_HANDLER_OPTIONS(...) \
  public: \
    virtual std::vector<const char*> availableOptions() const _OPENMA_NOEXCEPT override \
    { \
      using _TupleSize = std::tuple_size<_Tuple>; \
      std::vector<const char*> options(_TupleSize::value); \
      __details::_Handler_options_iterate<_Tuple,0,_TupleSize::value>::extract_name(&options); \
      return options; \
    }; \
    virtual std::vector<const char*> availableOptionChoices(const char* option) const _OPENMA_NOEXCEPT override \
    { \
      using _TupleSize = std::tuple_size<_Tuple>; \
      return __details::_Handler_options_iterate<_Tuple,0,_TupleSize::value>::extract_choices(option); \
    }; \
    virtual void option(const char* option, void* value) const _OPENMA_NOEXCEPT override \
    { \
      using _TupleSize = std::tuple_size<_Tuple>; \
      __details::_Handler_options_iterate<_Tuple,0,_TupleSize::value>::get_value(&(this->m_Options),option,value); \
    }; \
    virtual void setOption(const char* option, const void* value) _OPENMA_NOEXCEPT override \
    { \
      using _TupleSize = std::tuple_size<_Tuple>; \
      __details::_Handler_options_iterate<_Tuple,0,_TupleSize::value>::set_value(&(this->m_Options),option,value); \
    }; \
  private: \
    using _Tuple = decltype(std::make_tuple(__VA_ARGS__)); \
    struct _Options : _Tuple \
    { \
      _Options() : _Tuple(__VA_ARGS__) {}; \
    } m_Options;
    
#define OPENMA_STRINGIFY_OPTION_NAME(option,str) \
    template <> \
    struct stringify_option_name<typename option::ValueType> \
    { \
      static inline _OPENMA_CONSTEXPR_CONST char* c_str() _OPENMA_NOEXCEPT {return str;}; \
    };
    
#define OPENMA_STRINGIFY_OPTION_VALUE(option,value,str) \
    template <> \
    struct stringify_option_value<typename option::ValueType,value> \
    { \
      static inline _OPENMA_CONSTEXPR_CONST char* c_str() _OPENMA_NOEXCEPT {return str;}; \
    };
    
// ------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  template <typename V> struct stringify_option_name;
  template <typename V, V v> struct stringify_option_value;
  
  namespace __details
  {
    template <typename V, V... vs>
    class _Handler_option
    {
    public:
      using ValueType = V;
      
      static inline _OPENMA_CONSTEXPR_CONST char* name() _OPENMA_NOEXCEPT {return stringify_option_name<V>::c_str();};
      static inline std::vector<const char*> choices() _OPENMA_NOEXCEPT {return {stringify_option_value<V,vs>::c_str()...};};
    
      _Handler_option(V&& v)
      : Value(std::forward<V>(v))
      {};
      
      inline const V& value() const _OPENMA_NOEXCEPT {return this->Value;};
      inline void setValue(const V& value) _OPENMA_NOEXCEPT
      {
        _OPENMA_CONSTEXPR size_t num = sizeof...(vs);
        _OPENMA_CONSTEXPR std::array<V,num> choices{{vs...}};
        for (size_t i = 0 ; i < num ; ++i)
        {
          if (choices[i] == value)
          {
            this->Value = value;
            return;
          }
        }
        warning("Impossible to set an option with a value not in the predefined choices.");
      };
      
    private:
      V Value;
    };
    
    template<class T, size_t I, size_t N>
    struct _Handler_options_iterate
    {
      static inline void extract_name(std::vector<const char*>* output)
      {
        output->operator[](I) = std::tuple_element<I,T>::type::name();
        _Handler_options_iterate<T,I+1,N>::extract_name(output);
      };
      
      static inline std::vector<const char*> extract_choices(const char* option)
      {
        using _Elt = typename std::tuple_element<I,T>::type;
        if (strcmp(_Elt::name(),option) == 0)
          return _Elt::choices();
        else
          return _Handler_options_iterate<T,I+1,N>::extract_choices(option);
      };
      
      static inline void get_value(const T* tuple, const char* option, void* value)
      {
        using _Elt = typename std::tuple_element<I,T>::type;
        if (strcmp(_Elt::name(),option) == 0)
          *static_cast<typename _Elt::ValueType*>(value) = std::get<I>(*tuple).value();
        else
          _Handler_options_iterate<T,I+1,N>::get_value(tuple, option, value);
      };
      
      static inline void set_value(T* tuple, const char* option, const void* value)
      {
        using _Elt = typename std::tuple_element<I,T>::type;
        if (strcmp(_Elt::name(),option) == 0)
          std::get<I>(*tuple).setValue(*static_cast<const typename _Elt::ValueType*>(value));
        else
          _Handler_options_iterate<T,I+1,N>::set_value(tuple, option, value);
      };
    };
    
    template<class T, size_t N>
    struct _Handler_options_iterate<T,N,N>
    {
      static inline void extract_name(std::vector<const char*>* ) {};
      static inline std::vector<const char*> extract_choices(const char* ) {return std::vector<const char*>();};
      static inline void get_value(const T* , const char* , void* ) {};
      static inline void set_value(T* , const char* , const void* ) {};
    };
  };
    
  // --------------------------------------------------------------------- //
  
  /**
   * @headerfile openma/io/handleroption.h
   * @brief Handler option generator
   * @tparam O Option definition
   * Instead of manualy creating option with a complex syntax, this structure simplifies it. It is only necessary to give the option's type and single / multiple choices available. 
   * @relates Handler
   * @ingroup openma_io
   */
  template <typename O>
  struct make_option
  {
    /**
     * Alias for the type used by the option @a O.
     */
    using V = typename O::ValueType;
  
    /**
     * Create an option with the multiple choice.
     * @tparam vs Values available for the option
     */
    template <V... vs>
    struct multiple_choices
    {
      /**
       * Set the initial value used for the proposed multiple choices.
       */
      static inline __details::_Handler_option<V,vs...> initial_value(V&& value)
      {
        return __details::_Handler_option<V,vs...>(std::forward<V>(value));
      };
      multiple_choices() = delete;
      ~multiple_choices() _OPENMA_NOEXCEPT = delete;
      multiple_choices(const multiple_choices& ) = delete;
      multiple_choices(multiple_choices&& ) _OPENMA_NOEXCEPT = delete;
      multiple_choices& operator=(const multiple_choices& ) = delete;
      multiple_choices& operator=(multiple_choices&& ) _OPENMA_NOEXCEPT = delete;
    };
    /**
     * Create an option with a fixed choice. This is usefull for inehriting handler that can sometime have only one possibility (like internal ASCII/binary structure, etc.).
     * @tparam v Value fixed for the option 
     */
    template <V v>
    static inline __details::_Handler_option<V,v> single_choice()
    {
      return __details::_Handler_option<V,v>(std::forward<V>(v));
    };
  };

  template <typename V>
  class HandlerOption
  {
  public:
    using ValueType = V;
  
    static inline _OPENMA_CONSTEXPR_CONST char* name() _OPENMA_NOEXCEPT {return stringify_option_name<V>::c_str();};
  
    HandlerOption() = delete;
    ~HandlerOption() _OPENMA_NOEXCEPT = delete;
    HandlerOption(const HandlerOption& ) = delete;
    HandlerOption(HandlerOption&& ) _OPENMA_NOEXCEPT = delete;
    HandlerOption& operator=(const HandlerOption& ) = delete;
    HandlerOption& operator=(HandlerOption&& ) _OPENMA_NOEXCEPT = delete;
  };
  
  /**
   * @class HandlerOption openma/io/handleroption.h
   * @brief To define I/O handler options.
   *
   * A I/O handler (class Handler) has capacity to read and/or write data from/to a device.
   * By default a I/O handler has options to set encoding format, byte order format, etc.
   * These options can be set using scoped enumarotrs or associated strings. The former could be used 
   * in the implementation of the handler, while the second is more for dynamic code (e.g. reader/writer 
   * of several handler, language wrappers)
   * 
   * But a specific handler could have other dedicated option (for example, C3DHandler::AnalogStorageFormat).
   * It would be then interesting to have a simple way to create them giving the same behaviour than what presented before.
   * The class Option fill this role. More exaclty the addition of option in a handler is realised using the macro OPENMA_DECLARE_IO_HANDLER_OPTIONS
   * and the class make_option. All the declaration must be done in the private implementation of the handler.
   * 
   * For example, in the C3D handler (class C3DHandler), the option C3DHandler::AnalogStorageForma was added simply.
   *
   * First a scoped enumeration was created in the public interface as well as a type definition.
   *
   * @code
   * // Scoped enumartor
   * enum class AnalogStorage : int {Signed, Unsigned};
   * // Option type
   * //  - The template variable _ma_C3DHandler_AnalogStorage store a static array of characters related to the option's name.
   * //  - The template typename AnalogStorage indicates the type to use for this kind of option
   * using AnalogStorageFormat = Option<_ma_C3DHandler_AnalogStorage,AnalogStorage>;
   * @endcode
   *
   * Then, in the private implementation, all the options to define for the handler are finalized. Tipically this is used to set the possible values to store (indeed, not all enumartors could be allowed)
   * Inside the macro OPENMA_DECLARE_IO_HANDLER_OPTIONS, it is needed to set the optons. For example
   *
   * @code
   * OPENMA_DECLARE_IO_HANDLER_OPTIONS(
   *   make_option<C3DHandler::EncodingFormat>::single_choice<C3DHandler::Encoding::Binary>(),
   *   make_option<C3DHandler::ByteOrderFormat>::multiple_choices<C3DHandler::ByteOrder::VAXLittleEndian,C3DHandler::ByteOrder::IEEELittleEndian,C3DHandler::ByteOrder::IEEEBigEndian>::init(C3DHandler::ByteOrder::Native),
   *   make_option<C3DHandler::DataStorageFormat>::multiple_choices<C3DHandler::DataStorage::Integer,C3DHandler::DataStorage::Float>::init(C3DHandler::DataStorage::Float),
   *   make_option<C3DHandler::AnalogStorageFormat>::multiple_choices<C3DHandler::AnalogStorage::Signed,C3DHandler::AnalogStorage::Unsigned>::init(C3DHandler::AnalogStorage::Signed)
   * )
   * @endcode
   *
   * Internally this creates a member Options set as a tuple. The use of make_option::single_choice or make_option::multiple_choices::init depend of the possibilities of the implemented handler.
   * In the previous example, there are 4 options.
   *  - EncodingFormat can be set only to Binary (not Text or Mix)
   *  - ByteOrderFormat can be set only to VAXLittleEndian, IEEELittleEndian, and IEEEBigEndian (NotApplicable cannot be set)
   *  - DataStorageFormat can be set to Integer or Float (not NotApplicable)
   *  - AnalogStorageFormat can be set to signed or Unsigned
   *
   * Finally, one missing part is the associated string for the values associated with the option AnalogStorageFormat.
   * In the implementation (CPP) file, it is needed to specialized one function: stringify_option_value.
   * This is required to use the setting of options using strings. The specialization is done simple like that:
   *
   * @code
   * template <>
   * struct stringify_option_value<C3DHandler::AnalogStorage,C3DHandler::AnalogStorage::Signed>
   * {
   *   static constexpr const char* c_str = "Signed";
   * };
   * 
   * template <>
   * struct stringify_option_value<C3DHandler::AnalogStorage,C3DHandler::AnalogStorage::Unsigned>
   * {
   *   static constexpr const char* c_str = "Unsigned";
   * };
   * @endcode
   */
  /**
   * @var HandlerOption::ValueType
   * Alias for the template passed to the class.
   */
  
  /**
   * @fn template <typename V> static inline _OPENMA_CONSTEXPR_CONST char* HandlerOption<V>::name() _OPENMA_NOEXCEPT 
   * Returns the name of the option.
   */
};
};

#endif // __openma_io_handleroption_h