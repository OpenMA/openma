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

#ifndef __openma_io_handler_h
#define __openma_io_handler_h

#include "openma/io_export.h"
#include "openma/io/endianformat.h"
#include "openma/io/handleroption.h"
#include "openma/base/opaque.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT
#include "openma/base/exception.h"

#include <list>
#include <string>
#include <memory> // std::unique_ptr
#include <vector>
#include <type_traits> // std::is_same

namespace ma
{
  class Node;
  
namespace io
{
  class Device;
  
  class HandlerPrivate;
  
  class OPENMA_IO_EXPORT Handler
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Handler)
    
  public:
    enum class Signature : int {Valid, Invalid, NotAvailable};
    enum class Error : int {None, Device, UnsupportedFormat, InvalidData, Unexpected, Unknown};
    enum class Capability : int {CanRead = 0x01, CanWrite = 0x02};
    
    enum class Encoding : int {Text = 0x01, Binary, Mixed = Text|Binary};
    using EncodingFormat = HandlerOption<Encoding>;
    
    using ByteOrder = ByteOrder;
    using ByteOrderFormat = HandlerOption<ByteOrder>;
    
    enum class DataStorage : int {NotApplicable, Integer, Float};
    using DataStorageFormat = HandlerOption<DataStorage>;
    
    virtual ~Handler() _OPENMA_NOEXCEPT;
    
    Handler() = delete;
    Handler(const Handler& ) = delete;
    Handler(Handler&& ) _OPENMA_NOEXCEPT = delete;
    Handler& operator=(const Handler& ) = delete;
    Handler& operator=(const Handler&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual Capability capability() const _OPENMA_NOEXCEPT;

    Signature detectSignature() const _OPENMA_NOEXCEPT;
    bool read(Node* output);
    bool write(const Node* input);
 
    Device* device() const _OPENMA_NOEXCEPT;
    void setDevice(Device* device) _OPENMA_NOEXCEPT;
    
    std::vector<const char*> availableOptions() const _OPENMA_NOEXCEPT;
    std::vector<const char*> availableOptionChoices(const char* option) const _OPENMA_NOEXCEPT;
    
    template <typename O> typename O::ValueType option() const _OPENMA_NOEXCEPT;
    template <typename O, typename V> void setOption(const V& value) _OPENMA_NOEXCEPT;
    
    Error errorCode() const _OPENMA_NOEXCEPT;
    const std::string& errorMessage() const _OPENMA_NOEXCEPT;
  
  protected:
    template <typename V, V v> struct stringify_option_value;
    
    class FormatError : public Exception
    {
    public:
      explicit FormatError(const std::string& msg)
      : Exception(msg)
      {};
    };
    
    Handler(HandlerPrivate& pimpl) _OPENMA_NOEXCEPT;
    
    void option(const char* name, void* value) const _OPENMA_NOEXCEPT;
    void setOption(const char* name, const void* value) _OPENMA_NOEXCEPT;
    
    void setError(Error code = Error::None, const std::string& msg = "") _OPENMA_NOEXCEPT;
    
    virtual Signature validateSignature() const _OPENMA_NOEXCEPT = 0;
    virtual void readDevice(Node* output);
    virtual void writeDevice(const Node* input);
    
    std::unique_ptr<HandlerPrivate> mp_Pimpl;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename O>
  typename O::ValueType Handler::option() const _OPENMA_NOEXCEPT
  {
    typename O::ValueType value;
    this->option(O::name(),static_cast<void*>(&value));
    return value;
  };
  
  template <typename O, typename V>
  inline void Handler::setOption(const V& value) _OPENMA_NOEXCEPT
  {
    static_assert(std::is_same<typename O::ValueType,V>::value, "The type of the given value does not correspond to the type of the option's value.");
    this->setOption(O::name(),static_cast<const void*>(&value));
  };
  
  // ----------------------------------------------------------------------- //
  
  inline _OPENMA_CONSTEXPR Handler::Capability operator| (Handler::Capability lhs, Handler::Capability rhs)
  {
    return static_cast<Handler::Capability>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };

  inline _OPENMA_CONSTEXPR Handler::Capability operator& (Handler::Capability lhs, Handler::Capability rhs)
  {
    return static_cast<Handler::Capability>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
  
  // ----------------------------------------------------------------------- //
  
  OPENMA_STRINGIFY_OPTION_NAME(Handler::EncodingFormat, "Encoding");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::EncodingFormat, Handler::Encoding::Binary, "Binary");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::EncodingFormat, Handler::Encoding::Text, "Text");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::EncodingFormat, Handler::Encoding::Mixed, "Mixed");
  
  OPENMA_STRINGIFY_OPTION_NAME(Handler::ByteOrderFormat, "ByteOrder");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::ByteOrderFormat, Handler::ByteOrder::VAXLittleEndian, "VAXLittleEndian");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::ByteOrderFormat, Handler::ByteOrder::IEEELittleEndian, "IEEELittleEndian");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::ByteOrderFormat, Handler::ByteOrder::IEEEBigEndian, "IEEEBigEndian");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::ByteOrderFormat, Handler::ByteOrder::NotApplicable, "NotApplicable");
  
  OPENMA_STRINGIFY_OPTION_NAME(Handler::DataStorageFormat, "DataStorage");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::DataStorageFormat, Handler::DataStorage::NotApplicable, "NotApplicable");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::DataStorageFormat, Handler::DataStorage::Integer, "Integer");
  OPENMA_STRINGIFY_OPTION_VALUE(Handler::DataStorageFormat, Handler::DataStorage::Float, "Float");
};
};

#endif // __openma_io_handler_h
