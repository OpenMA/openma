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

#ifndef __openma_io_device_h
#define __openma_io_device_h

#include "openma/io_export.h"
#include "openma/io/enums.h"
#include "openma/base/opaque.h"
#include "openma/base/exception.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT

#include <memory> // std::unique_ptr
#include <ios> // std::streamsize, std::streamoff, std::streampos

namespace ma
{
namespace io
{
  class DevicePrivate;
  
  class OPENMA_IO_EXPORT Device
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Device)
    
  public:
    using Offset = std::streamoff;
    using Position = std::streampos;
    using Size = std::streamsize;
    
    class Failure : public Exception
    {
    public:
     explicit Failure(const std::string& msg) _OPENMA_NOEXCEPT : Exception(msg) {};
     virtual ~Failure() _OPENMA_NOEXCEPT = default;
    };
    
    Device(const Device& ) = delete;
    Device(Device&& ) _OPENMA_NOEXCEPT = delete;
    Device& operator=(const Device& ) = delete;
    Device& operator=(Device&& ) _OPENMA_NOEXCEPT = delete;

    virtual ~Device() _OPENMA_NOEXCEPT;
    
    // State
    bool isGood() const _OPENMA_NOEXCEPT;
    bool atEnd() const _OPENMA_NOEXCEPT;
    bool hasFailure() const _OPENMA_NOEXCEPT;
    bool hasError() const _OPENMA_NOEXCEPT;
    State state() const _OPENMA_NOEXCEPT;
    void setState(State state);
    void clear(State state = State::Good);
    State exceptions() _OPENMA_NOEXCEPT;
    void setExceptions(State mask);
    
    // General
    const char* name() const _OPENMA_NOEXCEPT;
    virtual bool isOpen() const _OPENMA_NOEXCEPT = 0;
    virtual void close() = 0;
    virtual Size peek(char* s, Size n) const = 0;
    virtual void read(char* s, Size n) = 0;
    virtual void write(const char* s, Size n) = 0;
    
    // Random access IO device only
    virtual void seek(Offset offset, Origin whence) = 0;
    virtual Position tell() const _OPENMA_NOEXCEPT = 0;
    virtual const char* data() const _OPENMA_NOEXCEPT = 0;
    virtual Size size() const _OPENMA_NOEXCEPT = 0;
    
    // Sequential IO only
    virtual bool isSequential() const _OPENMA_NOEXCEPT = 0;
    
  protected:
    void setName(const char* name = nullptr);
    bool verifyMode(Mode mode);
    
    Device() _OPENMA_NOEXCEPT;
    Device(DevicePrivate& pimpl) _OPENMA_NOEXCEPT;
   
    std::unique_ptr<DevicePrivate> mp_Pimpl;
  };
};
};

#endif // __openma_io_device_h