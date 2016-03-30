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

#ifndef __openma_io_handlerplugin_h
#define __openma_io_handlerplugin_h

#include "openma/io_export.h"
#include "openma/base/macros.h"

#include <vector>
#include <string>

namespace ma
{
namespace io
{
  class Device;
  class Handler;
  class HandlerPlugin;
  enum class Capability;
  enum class Signature;
  
  const std::vector<HandlerPlugin*>& load_handler_plugins();
  
  class OPENMA_IO_EXPORT HandlerPlugin
  {
  public:
    HandlerPlugin();
    virtual ~HandlerPlugin();
    
    virtual std::string name() const _OPENMA_NOEXCEPT = 0;
    
    virtual std::vector<std::string> supportedFormats() const _OPENMA_NOEXCEPT = 0;
    
    virtual Capability capabilities(const std::string& format) const _OPENMA_NOEXCEPT = 0;
    virtual Signature detectSignature(const Device* const device, std::string* format = nullptr) const _OPENMA_NOEXCEPT = 0;
    virtual bool detectExtension(const std::string& str, std::string* format = nullptr) const _OPENMA_NOEXCEPT;
    
    virtual Handler* create(Device* device, const std::string& format) = 0;
  };
};
};

#endif // __openma_io_handlerplugin_h