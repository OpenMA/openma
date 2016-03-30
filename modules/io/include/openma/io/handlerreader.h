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

#ifndef __openma_io_handlerreader_h
#define __openma_io_handlerreader_h

#include "openma/io_export.h"
#include "openma/base/opaque.h"
#include "openma/base/macros.h"

#include <memory> // std::unique_ptr
#include <string>
#include <vector>

namespace ma
{
  class Node;
  
namespace io
{
  class Device;
  enum class Error;
  
  class HandlerReaderPrivate;
  
  class OPENMA_IO_EXPORT HandlerReader
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(HandlerReader)
    
  public:
    HandlerReader(Device* device = nullptr, const std::string& format = std::string{});
      
    ~HandlerReader();
    
    void setDevice(Device* device);
    void setFormat(const std::string& format);
    const std::string& format() const _OPENMA_NOEXCEPT;
    
    bool canRead();
    bool read(Node* root);
    
    Error errorCode() const _OPENMA_NOEXCEPT;
    const std::string& errorMessage() const _OPENMA_NOEXCEPT;
    
    static const std::vector<std::string>& availableFormats() _OPENMA_NOEXCEPT;
    
  private:
    void setError(Error code, const std::string& msg = std::string{}) _OPENMA_NOEXCEPT;
    
    std::unique_ptr<HandlerReaderPrivate> mp_Pimpl;
  };
};
};

#endif // __openma_io_handlerreader_h