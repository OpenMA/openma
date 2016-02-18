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

#include "openma/io/device.h"
#include "openma/io/device_p.h"

#include <cstring>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace io
{
  DevicePrivate::DevicePrivate()
  : Name(nullptr), OpenMode(Mode::Unknown), Status(State::Good), Exception(State::Good)
  {};
  
  DevicePrivate::~DevicePrivate() _OPENMA_NOEXCEPT
  {
    delete this->Name;
  };
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  /**
   * @class Device openma/io/device.h
   * @brief Interface to read and write on any type of device (e.g. File, Buffer, etc.).
   *
   * There is two types of devices. The first one is called "random access" (e.g. File, Buffer), while the second is called "sequential" (e.g. SerialPort).
   * The use of a Device class should be used with a BinaryStream or TextStream which interact at a higher level to read/write data in a device.
   *
   * This interface proposes also an exception mechanism in case the device fails to do some internal operation, or if there are error during I/O operations.
   * You can use the method setExceptions() to set the states which can trigger an exception. The exception thrown by this class corresponds to a Device::Failure exception.
   *
   * To implement a new device, several methods must me implemented. To facilitate this implementation some protected methods are proposed like verifyMode().
   */
  
  /**
   * @var Device::mp_Pimpl
   * Storage for the private implementation. 
   */
  
  /**
   * @typedef Device::Offset
   * Numerical type used to shift the position of the internal buffer pointer.
   */
  
  /**
   * @typedef Device::Position
   * Numerical type used to inform on the position of internal buffer pointer.
   */
   
  /**
   * @typedef Device::Size
   * Numerical type used for the size of the internal buffer.
   */
  
  /**
   * Destructor. 
   * This methods does nothing. It is the responsability of the inherited class to decide if their destructor does something specific regarding the state of the device (like closing it).
   */
  Device::~Device() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns true if the device's state is set to State::Good.
   */
  bool Device::isGood() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Status == State::Good;
  };
  
  /**
   * Returns true if the device's state contain the flag State::End.
   */
  bool Device::atEnd() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return (optr->Status & State::End) == State::End;
  };
 
  /**
   * Returns true if the device's state contain the flag State::Fail and/or State::Error.
   */
  bool Device::hasFailure() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return (optr->Status & (State::Fail | State::Error)) != State::Good;
  };
 
  /**
   * Returns true if the device's state contain the flag State::Fail
   */
  bool Device::hasError() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return (optr->Status & State::Error) == State::Error;
  };
 
  /**
   * Returns true current state of the device.
   */
  State Device::state() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Status;
  };
 
  /**
   * Sets the state of the device.
   * The given @a state is added to the current state.
   * Use the method Clear() if you want to reset the state of the device.
   */
  void Device::setState(State state)
  {
    this->clear(this->state() | state);
  };
 
  /**
   * Sets the state of the device.
   * If the given state @a flags meets some part of the exception's mask, then a Failure exception is thrown.
   * @note Setting the state to State::Good will reset the possible current failure/errors.
   */
  void Device::clear(State state)
  {
    auto optr = this->pimpl();
    optr->Status = state;
    if ((this->exceptions() & this->state()) != State::Good)
      throw Failure("ma::io::Device::clear");
  };
   
  /**
   * Returns the mask used to possibly throws an exception.
   */
  State Device::exceptions() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Exception;
  };
 
  /**
   * Sets the mask which will be used to throw an exceptions.
   * Setting the mask to State::Good will cancel the use of exception with the device. 
   */
  void Device::setExceptions(State mask)
  {
    auto optr = this->pimpl();
    optr->Exception = mask;
    this->clear(optr->Status);
  };
  
  /**
   * @fn virtual const char* Device::name() const _OPENMA_NOEXCEPT;
   * Returns the name associated with this device. 
   * The name of a device can be anything. For example for a file, it could be the full path of the filename read/write. For a serial port, it could be its identifiant. For a databse, it could the adress of the server.
   */
  const char* Device::name() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Name;
  };
  
  /**
   * @fn virtual bool Device::isOpen() const _OPENMA_NOEXCEPT = 0
   * Returns true if the device is opened otherwise returns false.
   */
  
  /**
   * Returns the mode used to open this device.
   */
  Mode Device::openMode() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->OpenMode;
  }
  
  /**
   * @fn virtual void Device::close() = 0
   * Close the device.
   * @note The inherited class should set the State::Fail state flag if any failure happens during the closing of the device.
   */
  
  /**
   * @fn virtual Size Device::peek(char* s, Size n) const = 0;
   * Extract data without modifying the position of the internal pointer
   */
  
  /**
   * @fn virtual void Device::read(char* s, Size n) = 0
   * Gets from the device a sequence of characters of size @a n and store it in @a s. The State::Error state flag is set if any issue happens during the reading operation. 
   */
    
  /**
   * @fn virtual void Device::write(const char* s, Size n) = 0
   * Puts the sequence of characters @a s of size @a n to the device. The State::Error state flag is set if any issue happens during the writing operation. 
   */
    
  /**
   * @fn virtual void Device::seek(Offset offset, SeekDir dir) = 0
   * Moves the pointer associated with a random access device of the given @a offset in the given direction @a dir.
   * @note The use of this method with a sequential device would set the flag State::Fail to true.
   */
  
  /**
   * @fn virtual Position Device::tell() const _OPENMA_NOEXCEPT = 0
   * Returns the position of the pointer associated with a random access device. 
   * @note The use of this method with a sequential device would set the flag State::Fail to true.
   */
  
  /**
   * @fn virtual const char* Device::data() const _OPENMA_NOEXCEPT = 0;
   * Returns a pointer to the first byte stored in the device (or null if it is not possible to access to the data).
   * @note The use of this method with a sequential device would set the flag State::Fail to true. At least the output would be null.
   */
  
 /**
  * @fn virtual Size Device::size() const _OPENMA_NOEXCEPT = 0;
  * Returns the size of the stored data (or -1 in case it is not known)
  * @note The use of this method with a sequential device would set the flag State::Fail to true. At least the output would be equal to -1.
  */
  
  /**
   * @fn virtual bool Device::isSequential() const = 0
   * Returns true if the device is sequential otherwise false.
   */
  
  /**
    * Sets the name of the device.
    * Internally, this method copy the given array of characters.
    */
  void Device::setName(const char* name)
  {
    auto optr = this->pimpl();
    delete optr->Name;
    optr->Name = nullptr;
    if (name != nullptr)
    {
      size_t len = strlen(name);
      optr->Name = new char[len+1];
      strncpy(optr->Name,name,len);
      optr->Name[len] = '\0';
    }
  };
  
  /**
   * Verify the coherency of the given open mode.
   * This method should be used by every inherited class in their Open() method before trying to open the implemented device.
   * Internally, this will set the flag State::Fail to true if one of the verification is not valid.
   * The following checks are realized by this method:
   *  - The device is not already opened.
   *  - The modes Append and Truncate cannot be set at the same time.
   *  - The mode Truncate is set but not the mode Out.
   */
  bool Device::verifyMode(Mode mode)
  {
    bool valid = true;
    if (this->isOpen()
      || (((mode & Mode::Append) == Mode::Append) && ((mode & Mode::Truncate) == Mode::Truncate))
      || (((mode & Mode::Truncate) == Mode::Truncate) && ((mode & Mode::Out) != Mode::Out)))
    {
      valid = false;
      this->setState(State::Fail);
    }
    return valid;
  };
  
  /**
   * Stores the open mode. This method shall be used when a device is opened.
   * @note This method does not influence the way the device is opened. This is only to provide information for other methods like isOpen(Mode mode).
   */
  void Device::setOpenMode(Mode mode) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->OpenMode = mode;
  };
  
  /**
   * Constructor.
   * Set the device's state to State::Good without exception enabled.
   */
  Device::Device() _OPENMA_NOEXCEPT
  : mp_Pimpl(new DevicePrivate)
  {};
  
  /**
   * Constructor to be used by inherited device that need extra informations (static properties, members, etc) inside the private implementation.
   */
  Device::Device(DevicePrivate& pimpl) _OPENMA_NOEXCEPT
  : mp_Pimpl(&pimpl)
  {};
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class Device::Failure openma/io/device.h
   * @brief Exception thrown by the inherited classes of the interface Device.
   */
  
  /**
   * @fn Device::Failure::Failure(const std::string& msg);
   * Constructor.
   */
  
  /**
   * @fn virtual Device::Failure::~Failure()
   * Destructor (empty).
   */
};
};