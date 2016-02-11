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

#ifndef __openma_io_buffer_p_h
#define __openma_io_buffer_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/io/device_p.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

namespace ma
{
namespace io
{
  class ChunkBuffer;
  
  class BufferPrivate : public DevicePrivate
  {
  public:
    BufferPrivate();
    ~BufferPrivate() _OPENMA_NOEXCEPT;
    
    BufferPrivate(const BufferPrivate& ) = delete;
    BufferPrivate(BufferPrivate&& ) _OPENMA_NOEXCEPT = delete;
    BufferPrivate& operator=(const BufferPrivate& ) = delete;
    BufferPrivate& operator=(const BufferPrivate&& ) _OPENMA_NOEXCEPT = delete;
    
    ChunkBuffer* Buffer;
  };
  
  class ChunkBuffer
  {
  public:
    using Size = Device::Size;
    using Offset = Device::Offset;
    using Position = Device::Position;
    
    ChunkBuffer() _OPENMA_NOEXCEPT;
    ~ChunkBuffer() _OPENMA_NOEXCEPT {this->close();};
    
    ChunkBuffer* open(char* data, size_t dataSize, const std::vector<size_t>& chunkIds, size_t chunkSize, Mode mode, bool owned) _OPENMA_NOEXCEPT;
    bool isOpen() const _OPENMA_NOEXCEPT {return this->mp_Data != 0;};
    ChunkBuffer* close() _OPENMA_NOEXCEPT;

    bool hasWriteMode() const _OPENMA_NOEXCEPT {return this->m_Writing;};
    
    Size dataSize() const _OPENMA_NOEXCEPT {return this->m_DataSize;};
    const char* data() const _OPENMA_NOEXCEPT {return this->mp_Data;};
    
    Size peek(char* s, Size n) const _OPENMA_NOEXCEPT;
    Size read(char* s, Size n) _OPENMA_NOEXCEPT;
    Size write(const char* s, Size n) _OPENMA_NOEXCEPT;
    
    Position seek(Offset off, Origin whence) _OPENMA_NOEXCEPT;
    
    const std::vector<size_t>& chunkIDs() const;
    size_t chunkSize() const;
    void setChunks(const std::vector<size_t>& ids, size_t size);
    
    bool updateChunkOffset(Offset* dataOffset, Offset* chunkOffset) const;
    
  private:
    char* mp_Data;
    bool m_DataOwned;
    Size m_DataSize;
    std::vector<size_t> m_ChunkIds;
    Size m_ChunkSize;
    Offset m_ChunkOffset;
    Offset m_Offset;
    bool m_Writing;
  };
};
};

#endif // __openma_io_buffer_p_h
