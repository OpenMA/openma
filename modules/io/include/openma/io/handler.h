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
  enum class Error;
  enum class Signature;
  
  class HandlerPrivate;
  
  class OPENMA_IO_EXPORT Handler
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Handler)
    
  public:
    Handler();
    virtual ~Handler() _OPENMA_NOEXCEPT;
    
    Handler(const Handler& ) = delete;
    Handler(Handler&& ) _OPENMA_NOEXCEPT = delete;
    Handler& operator=(const Handler& ) = delete;
    Handler& operator=(const Handler&& ) _OPENMA_NOEXCEPT = delete;
    
    bool read(Node* output);
    bool write(const Node* input);
 
    Device* device() const _OPENMA_NOEXCEPT;
    void setDevice(Device* device) _OPENMA_NOEXCEPT;
    
    Error errorCode() const _OPENMA_NOEXCEPT;
    const std::string& errorMessage() const _OPENMA_NOEXCEPT;
  
  protected:
    class FormatError : public Exception
    {
    public:
      explicit FormatError(const std::string& msg)
      : Exception(msg)
      {};
    };
    
    Handler(HandlerPrivate& pimpl) _OPENMA_NOEXCEPT;
    
    void setError(Error code, const std::string& msg = {}) _OPENMA_NOEXCEPT;
    
    virtual Signature verifySignature() const _OPENMA_NOEXCEPT = 0;
    virtual void readDevice(Node* output);
    virtual void writeDevice(const Node* const input);
    
    std::unique_ptr<HandlerPrivate> mp_Pimpl;
  };
};
};

#endif // __openma_io_handler_h
