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

#ifndef __openma_io_binarystream_p_h
#define __openma_io_binarystream_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <cstdint>
#include <string>

namespace ma
{
namespace io
{
  class Device;
  class ByteOrderConverter;
  enum class ByteOrder;
  
  class BinaryStreamPrivate
  {
  public:
    BinaryStreamPrivate(Device* source, ByteOrderConverter* converter);
    virtual ~BinaryStreamPrivate() _OPENMA_NOEXCEPT;
    
    BinaryStreamPrivate(const BinaryStreamPrivate& ) = delete;
    BinaryStreamPrivate(BinaryStreamPrivate&& ) _OPENMA_NOEXCEPT = delete;
    BinaryStreamPrivate& operator=(const BinaryStreamPrivate& ) = delete;
    BinaryStreamPrivate& operator=(const BinaryStreamPrivate&& ) _OPENMA_NOEXCEPT = delete;
    
    Device* Source;
    ByteOrderConverter* Converter;
  };
  
  // ----------------------------------------------------------------------- //
  
  class ByteOrderConverter
  {
  public:
    virtual ~ByteOrderConverter() _OPENMA_NOEXCEPT = default;
    
    ByteOrderConverter(const ByteOrderConverter& ) = delete;
    ByteOrderConverter(ByteOrderConverter&& ) _OPENMA_NOEXCEPT = delete;
    ByteOrderConverter& operator=(const ByteOrderConverter& ) = delete;
    ByteOrderConverter& operator=(ByteOrderConverter&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual ByteOrder byteOrder() const _OPENMA_NOEXCEPT = 0;
    
    char readChar(Device* src) const;
    int8_t readI8(Device* src) const;
    uint8_t readU8(Device* src) const;
    virtual int16_t readI16(Device* src) const = 0;
    virtual uint16_t readU16(Device* src) const = 0;
    virtual int32_t readI32(Device* src) const = 0;
    virtual uint32_t readU32(Device* src) const = 0;
    virtual int64_t readI64(Device* src) const = 0;
    virtual uint64_t readU64(Device* src) const = 0;
    virtual float readFloat(Device* src) const = 0;
    virtual double readDouble(Device* src) const = 0;
    std::string readString(size_t len, Device* src) const;
    
    void writeChar(char val, Device* dest) const;
    void writeI8(int8_t val, Device* dest) const;
    void writeU8(uint8_t val, Device* dest) const;
    virtual void writeI16(int16_t val, Device* dest) const = 0;
    virtual void writeU16(uint16_t val, Device* dest) const = 0;
    virtual void writeI32(int32_t val, Device* dest) const = 0;
    virtual void writeU32(uint32_t val, Device* dest) const = 0;
    virtual void writeFloat(float val, Device* dest) const = 0;
    void writeString(const std::string& val, Device* dest) const;
    /* TODO
    virtual void writeI64(int64_t val, Device* dest) const = 0;
    virtual void writeU64(uint64_t val, Device* dest) const = 0;
    virtual void writeDouble(double val, Device* dest) const = 0;
     */
    
  protected:
    ByteOrderConverter();
  };
  
  class VAXLittleEndianConverter : public ByteOrderConverter
  {
  public:
    VAXLittleEndianConverter();
    ~VAXLittleEndianConverter() = default;
    
    VAXLittleEndianConverter(const VAXLittleEndianConverter& ) = delete;
    VAXLittleEndianConverter(VAXLittleEndianConverter&& ) _OPENMA_NOEXCEPT = delete;
    VAXLittleEndianConverter& operator=(const VAXLittleEndianConverter& ) = delete;
    VAXLittleEndianConverter& operator=(VAXLittleEndianConverter&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual ByteOrder byteOrder() const _OPENMA_NOEXCEPT final;
    
    virtual int16_t readI16(Device* src) const final;
    virtual uint16_t readU16(Device* src) const final;
    virtual int32_t readI32(Device* src) const final;
    virtual uint32_t readU32(Device* src) const final;
    virtual int64_t readI64(Device* src) const final;
    virtual uint64_t readU64(Device* src) const final;
    virtual float readFloat(Device* src) const final;
    virtual double readDouble(Device* src) const final;
  
    virtual void writeI16(int16_t val, Device* dest) const final;
    virtual void writeU16(uint16_t val, Device* dest) const final;
    virtual void writeI32(int32_t val, Device* dest) const final;
    virtual void writeU32(uint32_t val, Device* dest) const final;
    virtual void writeFloat(float val, Device* dest) const final;
  };

  class IEEELittleEndianConverter : public ByteOrderConverter
  {
  public:
    IEEELittleEndianConverter();
    ~IEEELittleEndianConverter() = default;
    
    IEEELittleEndianConverter(const IEEELittleEndianConverter& ) = delete;
    IEEELittleEndianConverter(IEEELittleEndianConverter&& ) _OPENMA_NOEXCEPT = delete;
    IEEELittleEndianConverter& operator=(const IEEELittleEndianConverter& ) = delete;
    IEEELittleEndianConverter& operator=(IEEELittleEndianConverter&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual ByteOrder byteOrder() const _OPENMA_NOEXCEPT final;
    
    virtual int16_t readI16(Device* src) const final;
    virtual uint16_t readU16(Device* src) const final;
    virtual int32_t readI32(Device* src) const final;
    virtual uint32_t readU32(Device* src) const final;
    virtual int64_t readI64(Device* src) const final;
    virtual uint64_t readU64(Device* src) const final;
    virtual float readFloat(Device* src) const final;
    virtual double readDouble(Device* src) const final;
  
    virtual void writeI16(int16_t val, Device* dest) const final;
    virtual void writeU16(uint16_t val, Device* dest) const final;
    virtual void writeI32(int32_t val, Device* dest) const final;
    virtual void writeU32(uint32_t val, Device* dest) const final;
    virtual void writeFloat(float val, Device* dest) const final;
  };

  class IEEEBigEndianConverter : public ByteOrderConverter
  {
  public:
    IEEEBigEndianConverter();
    ~IEEEBigEndianConverter() = default;
    
    IEEEBigEndianConverter(const IEEEBigEndianConverter& ) = delete;
    IEEEBigEndianConverter(IEEEBigEndianConverter&& ) _OPENMA_NOEXCEPT = delete;
    IEEEBigEndianConverter& operator=(const IEEEBigEndianConverter& ) = delete;
    IEEEBigEndianConverter& operator=(IEEEBigEndianConverter&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual ByteOrder byteOrder() const _OPENMA_NOEXCEPT final;
    
    virtual int16_t readI16(Device* src) const final;
    virtual uint16_t readU16(Device* src) const final;
    virtual int32_t readI32(Device* src) const final;
    virtual uint32_t readU32(Device* src) const final;
    virtual int64_t readI64(Device* src) const final;
    virtual uint64_t readU64(Device* src) const final;
    virtual float readFloat(Device* src) const final;
    virtual double readDouble(Device* src) const final;
  
    virtual void writeI16(int16_t val, Device* dest) const final;
    virtual void writeU16(uint16_t val, Device* dest) const final;
    virtual void writeI32(int32_t val, Device* dest) const final;
    virtual void writeU32(uint32_t val, Device* dest) const final;
    virtual void writeFloat(float val, Device* dest) const final;
  };
};
};

#endif // __openma_io_binarystream_p_h
