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

#ifndef __openma_io_file_p_h
#define __openma_io_file_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/io/device_p.h"
#include "openma/config.h" // HAVE_SYS_MMAP
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#if !defined(HAVE_SYS_MMAP) && !defined(_MSC_VER)
  #error Missing header to build the File class (memory mapping not found).
#endif

#if defined(_MSC_VER) // Windows
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  // Defining NOMINMAX to prevent compiler error with std::min/std::max when including windows.h
  #ifndef NOMINMAX
    #define NOMINMAX
  # endif
  #include <windows.h>
  #define _OPENMA_MMFILEBUF_NO_FILE INVALID_HANDLE_VALUE
#else // POSIX
  #define _OPENMA_MMFILEBUF_NO_FILE -1
#endif

namespace ma
{
namespace io
{
  class MemoryMappedBuffer;
  
  class FilePrivate : public DevicePrivate
  {
  public:
    FilePrivate();
    ~FilePrivate() _OPENMA_NOEXCEPT;
    
    FilePrivate(const FilePrivate& ) = delete;
    FilePrivate(FilePrivate&& ) _OPENMA_NOEXCEPT = delete;
    FilePrivate& operator=(const FilePrivate& ) = delete;
    FilePrivate& operator=(const FilePrivate&& ) _OPENMA_NOEXCEPT = delete;
    
    MemoryMappedBuffer* Buffer;
  };
  
  class MemoryMappedBuffer
  {
  public:
    using Size = Device::Size;
    using Offset = Device::Offset;
    using Position = Device::Position;
    
    MemoryMappedBuffer() _OPENMA_NOEXCEPT;
    ~MemoryMappedBuffer() _OPENMA_NOEXCEPT {this->close();};
    
    MemoryMappedBuffer* open(const char* s, Mode mode) _OPENMA_NOEXCEPT;
    bool isOpen() const _OPENMA_NOEXCEPT {return !(this->m_File == _OPENMA_MMFILEBUF_NO_FILE);};
    MemoryMappedBuffer* close() _OPENMA_NOEXCEPT;

    bool hasWriteMode() const _OPENMA_NOEXCEPT {return this->m_Writing;};
    
    Size dataSize() const _OPENMA_NOEXCEPT {return this->m_DataSize;};
    const char* data() const _OPENMA_NOEXCEPT {return this->mp_Data;};
    
    Size peek(char* s, Size n) const _OPENMA_NOEXCEPT;
    Size read(char* s, Size n) _OPENMA_NOEXCEPT;
    Size write(const char* s, Size n) _OPENMA_NOEXCEPT;
    
    Position seek(Offset off, Origin whence) _OPENMA_NOEXCEPT;
    
    MemoryMappedBuffer* map() _OPENMA_NOEXCEPT;
    MemoryMappedBuffer* resizeMap() _OPENMA_NOEXCEPT;
    
    static int granularity() _OPENMA_NOEXCEPT;
    
  private:
    char* mp_Data;
    Size m_DataSize;
    Size m_LogicalSize;
#if defined(HAVE_SYS_MMAP)
    int m_File;
#else
    HANDLE m_File;
    HANDLE m_Map;
#endif
    Offset m_Offset;
    bool m_Writing;
  };
};
};

#endif // __openma_io_file_p_h
