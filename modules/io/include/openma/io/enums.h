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
  enum class Mode : int {Unknown = 0x00, In = 0x01, Out = 0x02, Append = 0x04, Truncate = 0x08, End = 0x10};
  
  /**
   * @enum Mode
   * Details on the way to use the device.
   */
  /**
   * @var Mode Mode::Unknown
   * No known mode. This is usually a default mode to initialize a variable.
   */
  /**
   * @var Mode Mode::In
   * Use the device to read data from it.
   */
  /**
   * @var Mode Mode::Out
   * Use the device to write data from it.
   */
  /**
   * @var Mode Mode::Append
   * Set the internal pointer used by the device at the end.
   */
  /**
   * @var Mode Mode::Truncate
   * Erase the content of the device.
   */
  /**
   * @var Mode Mode::End
   * Set the internal pointer used by the device at the begining. 
   */
  
  /**
   * Bitwise and operator for Mode enumeration
   * @relates Device
   */
  inline _OPENMA_CONSTEXPR Mode operator& (Mode lhs, Mode rhs)
  {
    return static_cast<Mode>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };

  /**
   * Bitwise or operator for Mode enumeration
   * @relates Device
   */
  inline _OPENMA_CONSTEXPR Mode operator| (Mode lhs, Mode rhs)
  {
    return static_cast<Mode>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };
  
  /**
   * Bitwise not operator for Mode enumeration
   * @relates Device
   */
  inline _OPENMA_CONSTEXPR Mode operator~ (Mode x)
  {
    return Mode(~static_cast<int>(x));
  };
 
  enum class Origin : int {Begin, Current, End};
  
  /**
   * @enum Origin
   * Used as anchor for the seek() method.
   */
  /**
   * @var Origin Origin::Begin
   * Start at the beginning of the internal buffer.
   */
  /**
   * @var Origin Origin::Current
   * Start at the current position in the internal buffer.
   */
  /**
   * @var Origin Origin::End
   * Start at the end of the internal buffer.
   */
  
  enum class State : int {End = 0x01, Fail = 0x02, Error = 0x04, Good = 0x00};
  
  /**
   * @enum State
   * Internal state of the device. This 
   */
  /**
   * @var State  State::End
   * The device is at the end of its internal buffer.
   */
  /**
   * @var State  State::Fail
   * A failure happened within the device.
   */
  /**
   * @var State State::Error
   * An unexpected error happened within the device.
   */
  /**
   * @var State State::Good
   * Everything is fine!
   */
  
  /**
   * Bitwise and operator for State enumeration
   * @relates Device
   */
  inline _OPENMA_CONSTEXPR State operator& (State lhs, State rhs)
  {
    return static_cast<State>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
  
  /**
   * Bitwise or operator for State enumeration
   * @relates Device
   */
  inline _OPENMA_CONSTEXPR State operator| (State lhs, State rhs)
  {
    return static_cast<State>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };
  
  // ----------------------------------------------------------------------- //
  
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
  
  enum class Capability : int {None = 0x00, CanRead = 0x01, CanWrite = 0x02, CanReadAndWrite = CanRead | CanWrite};
  
  /**
   * @enum Signature::Capability
   * Inform on the capability to read/write data from/to a device.
   */
  /**
   * @var Signature::Capability Signature::None
   * Inform that a handker cannot do anything.
   */
  /**
   * @var Signature::Capability Signature::CanRead
   * Inform that a handker can read data from a device.
   */
  /**
   * @var Hander::Capability Signature::CanWrite
   * Inform that a handker can write data to a device.
   */
  /**
   * @var Signature::Capability Signature::CanReadAndWrite
   * Inform that a handker can read/write data from/to a device.
   */
  
  /**
   * Bitwise or operator for Capability enumeration
   * @relates Handler
   */
  inline _OPENMA_CONSTEXPR Capability operator| (Capability lhs, Capability rhs)
  {
    return static_cast<Capability>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };

  /**
   * Bitwise and operator for Capability enumeration
   * @relates Handler
   */
  inline _OPENMA_CONSTEXPR Capability operator& (Capability lhs, Capability rhs)
  {
    return static_cast<Capability>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
};
};

#endif // __openma_io_enums