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

#ifndef __openma_io_enums
#define __openma_io_enums

#include "openma/config.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR

namespace ma
{
namespace io
{
  enum class ByteOrder : int
  {
    NotApplicable,
    VAXLittleEndian,
    IEEELittleEndian,
    IEEEBigEndian,
#if _OPENMA_ARCH == _OPENMA_IEEE_LE
    Native = IEEELittleEndian
#elif _OPENMA_ARCH == _OPENMA_VAX_LE
    Native = VAXLittleEndian
#elif _OPENMA_ARCH == _OPENMA_IEEE_BE
    Native = IEEEBigEndian
#else
    #error The Native value cannot be unknown at compile time. Processor not supported
    // Native = NotApplicable
#endif
  };

  /**
   * @enum ByteOrder
   * Enums used to specify the endian format for the converter integrated in a BinaryStream.
   * @ingroup openma_io
   */
  /**
   * @var ByteOrder ByteOrder::NotApplicable
   * Enum value used to mention that no endian format is applicable. This value would be used only to inform and not to set.
   */
  /**
   * @var ByteOrder ByteOrder::VAXLittleEndian
   * Enum value representing the VAX architecture with little endian byte interpretation (DEC processor).
   */
  /**
   * @var ByteOrder ByteOrder::IEEELittleEndian
   * Enum value representing the IEEE architecture with little endian byte interpretation (Intel processor).
   */
  /**
   * @var ByteOrder ByteOrder::IEEEBigEndian
   * Enum value representing the IEEE architecture with big endian byte interpretation (MIPS processor).
   */
  /**
   * @var ByteOrder ByteOrder::Native
   * Convenient enum value which let the compiler chooses the default architecture (i.e. IEEELittleEndian, VAXLittleEndian, IEEEBigEndian)..
   */
  
  enum class Signature : int {Valid, Invalid, NotAvailable};
  
  /**
   * @enum Signature
   * Constant values to notify if the content of the handle has a valid/invalid signature.
   */
  /**
   * @var Signature Signature::Valid
   * Detected signature is valid
   */
  /**
   * @var Signature Signature::Invalid
   * Detected signature is not valid
   */
  /**
   * @var Signature Signature::NotAvailable
   * Not signature exists for this handler.
   */
  
  enum class Error : int {None, Device, UnsupportedFormat, InvalidData, Unexpected, Unknown};
  
  /**
   * @enum Error
   * Predefined error code used to explain possible errors during the reading/writing of the data.
   */
  /**
   * @var Error Error::None
   * No error
   */
  /**
   * @var Error Error::Device
   * Error related to the device.
   */
  /**
   * @var Error Error::UnsupportedFormat
   * The current handler does not support the format requested
   */
  /**
   * @var Error Error::InvalidData
   * The given data cannot be used with this handler
   */
  /**
   * @var Error Error::Unexpected
   * Unexpected error but at least there is a message with it
   */
  /**
   * @var Error Error::Unknown
   * Unexpected error but at least it was catched.
   */
  
  enum class Capability : int {CanRead = 0x01, CanWrite = 0x02, CanReadAndWrite = CanRead | CanWrite};
  
  /**
   * @enum Signature::Capability
   * Inform on the capability to read/write data from/to a device.
   */
  /**
   * @var Signature::Capability Signature::CanRead
   * Inform that the handker can read data from a device
   */
  /**
   * @var Hander::Capability Signature::CanWrite
   * Inform that the handker can write data to a device
   */
  
  /**
   * Bitwise or operator for Capability
   * @relates Handler
   */
  inline _OPENMA_CONSTEXPR Capability operator| (Capability lhs, Capability rhs)
  {
    return static_cast<Capability>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };

  /**
   * Bitwise and operator for Capability
   * @relates Handler
   */
  inline _OPENMA_CONSTEXPR Capability operator& (Capability lhs, Capability rhs)
  {
    return static_cast<Capability>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
};
};

#endif // __openma_io_enums