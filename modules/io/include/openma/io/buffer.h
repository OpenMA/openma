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

#ifndef __openma_io_buffer_h
#define __openma_io_buffer_h

#include "openma/io/device.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <vector>

namespace ma
{
namespace io
{
  class BufferPrivate;
  
  class OPENMA_IO_EXPORT Buffer : public Device
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Buffer)
    
  public:
    Buffer();
    ~Buffer() _OPENMA_NOEXCEPT;
    
    Buffer(const Buffer& ) = delete;
    Buffer(Buffer&& ) _OPENMA_NOEXCEPT = delete;
    Buffer& operator=(const Buffer& ) = delete;
    Buffer& operator=(Buffer&& ) _OPENMA_NOEXCEPT = delete;
    
    void open(const char* data, size_t dataSize, bool owned = false);
    void open(char* data, size_t dataSize, Mode mode = Mode::In, bool owned = false);
    void open(char* data, size_t dataSize, const std::vector<size_t>& chunkIds, size_t chunkSize, Mode mode = Mode::In, bool owned = false);
    virtual bool isOpen() const _OPENMA_NOEXCEPT override;
    virtual void close() override;
    virtual Size peek(char* s, Size n) const override;
    virtual void read(char* s, Size n) override;
    virtual void write(const char* s, Size n) override;
    virtual void seek(Offset offset, Origin whence) override;
    virtual Position tell() const _OPENMA_NOEXCEPT override;
    virtual bool isSequential() const _OPENMA_NOEXCEPT override;
    virtual const char* data() const _OPENMA_NOEXCEPT override;
    virtual Size size() const _OPENMA_NOEXCEPT override;
    
    using Device::setName;
    
    const std::vector<size_t>& chunkIDs() const _OPENMA_NOEXCEPT;
    size_t chunkSize() const _OPENMA_NOEXCEPT;
    void setChunks(const std::vector<size_t>& ids, size_t size);
  };
};
};

#endif // __openma_io_buffer_h