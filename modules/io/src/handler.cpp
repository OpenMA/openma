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

#include "openma/io/handler.h"
#include "openma/io/handler_p.h"
#include "openma/io/device.h"
#include "openma/base/node.h"

#include <iostream>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace io
{
  HandlerPrivate::HandlerPrivate()
  : Device(nullptr), ErrorCode(Handler::Error::None), ErrorMessage()
  {};
  
  HandlerPrivate::~HandlerPrivate() _OPENMA_NOEXCEPT = default; // Cannot be inlined
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
   * @class Handler
   * @brief To read/write data from/to an I/O device in a specific format
   *
   * This interface is the core to implement data reader/writer (e.g. trials, models).
   * Several methods and mecanisms are already implemented to facilitate the inclusion
   * of new formats. For example, it is not necessary to manage exception as this is done internaly.
   * Some methods are available to set errors, retrieve them. The extraction of handler's options 
   * (see Option class) is implemented. In fact, only three methods must be implemented for inherting handler:
   *  - readDevice() method: extract data from a device and set it into a node
   *  - writeDevice() method: write data to a given device
   *  - validateSignature() method: test if the read device has a signature valid for this handler.
   *
   * @todo Give an example for the implementation of a handler.
   *
   * @ingroup openma_io
   */
  /**
   * @var Handler::mp_Pimpl
   * Storage for the private implementation. 
   */
  
  /**
   * @enum Handler::Signature
   * Constant values to notify if the content of the handle has a valid/invalid signature.
   */
  /**
   * @var Handler::Signature Handler::Valid
   * Detected signature is valid
   */
  /**
   * @var Handler::Signature Handler::Invalid
   * Detected signature is not valid
   */
  /**
   * @var Handler::Signature Handler::NotAvailable
   * Not signature exists for this handler.
   */
  
  /**
   * @enum Handler::Error
   * Predefined error code used to explain possible errors during the reading/writing of the data.
   */
  /**
   * @var Handler::Error Handler::None
   * No error
   */
  /**
   * @var Handler::Error Handler::Device
   * Error related to the device.
   */
  /**
   * @var Handler::Error Handler::UnsupportedFormat
   * The current handler does not support the format requested
   */
  /**
   * @var Handler::Error Handler::InvalidData
   * The given data cannot be used with this handler
   */
  /**
   * @var Handler::Error Handler::Unexpected
   * Unexpected error but at least there is a message with it
   */
  /**
   * @var Handler::Error Handler::Unknown
   * Unexpected error but at least it was catched.
   */
  
  /**
   * @enum Handler::Capability
   * Inform on the capability to read/write data from/to a device.
   */
  /**
   * @var Handler::Capability Handler::CanRead
   * Inform that the handker can read data from a device
   */
  /**
   * @var Hander::Capability Handler::CanWrite
   * Inform that the handker can write data to a device
   */
  
  /**
   * @enum Handler::Encoding
   * Inform on the encoding format used by the handler.
   */
  /**
   * @var Handler::Encoding Handler::Text
   * The handler is for text data.
   */
  /**
   * @var Hander::Encoding Handler::Binary
   * The handler is for binary data.
   */
  /**
   * @var Hander::Encoding Handler::Mixed
   * The handler is for text and binary data.
   */
  
  /**
   * @typedef Handler::EncodingFormat
   * Type related to the option used to get/set the encoding format of the handler
   */
  
  /**
   * @typedef Handler::ByteOrder
   * Alias for the byte order used inside the file.
   */
   
  /**
   * @typedef Handler::ByteOrderFormat
   * Type related to the option used to get/set the byte order format of the handler
   */
   
  /**
   * @enum Handler::DataStorage
   * Inform on the data storage used by the handler. This is mostly for handler using a binary encoding format
   */
  /**
   * @var Handler::DataStorage Handler::NotApplicable
   * The data storage information is not applicable for this handler.
   */
  /**
   * @var Hander::DataStorage Handler::Integer
   * Data are stored as integer values
   */
  /**
   * @var Hander::DataStorage Handler::Float
   * Data are stored as real values
   */
   
  /**
   * @typedef Handler::DataStorageFormat
   * Type related to the option used to get/set the data storage format of the handler
   */
  
  /**
   * Destructor (default)
   */
  Handler::~Handler() _OPENMA_NOEXCEPT = default;
  
  /**
   * By default, this is assumed that each handler can read only.
   */
  Handler::Capability Handler::capability() const _OPENMA_NOEXCEPT
  {
    return Capability::CanRead;
  };
  
  /**
   * Read the content of the current set device and fill the given @a output.
   * If an exception is thrown during the reading of the device, no content is added to the output and false is returned.
   * In case this method returns false, you can use the methods errorCode() and errorMessage() to retrieve the error.
   * Internally this methods call readDevice() to extract data. Each inheriting handler must overload the method readDevice().
   */
  bool Handler::read(Node* output)
  {
    auto optr = this->pimpl();
    if (output == nullptr)
    {
      this->setError(Error::Unexpected, "Impossible to load the content of a device into a null output");
      return false;
    }
    else if (optr->Device == nullptr)
    {
      this->setError(Error::Device, "No device assigned");
      return false;
    }
    else if (!optr->Device->isOpen())
    {
      this->setError(Error::Device, "Device not open");
      return false;
    }
    else if (this->validateSignature() == Signature::Invalid)
    {
      this->setError(Error::UnsupportedFormat, "Format not supported by this I/O handler");
      return false;
    }
    
    try
    {
      this->setError(); // reset
      Node temp("_TIOHR"); // _THIOR: Temporary I/O Handler Root
      this->readDevice(&temp);
      if (optr->ErrorCode == Error::None) // In case the handler does not use exception but only error code/message.
      {
        for (auto& child: temp.children())
        {
          if (child != nullptr)
            child->addParent(output);
        }
      }
    }
    catch (Device::Failure& )
    {
      std::string excmsg; 
      if (optr->Device->atEnd())
        excmsg = "Unexpected end of device";
      else if(optr->Device->hasError())
        excmsg = "Loss of integrity in the device";
      else if(optr->Device->hasFailure())
        excmsg = "Internal logic operation error associated with the device";
      else
        excmsg = "Unknown error associated with the device";
      this->setError(Error::Device, excmsg);
    }
    catch (Handler::FormatError& e)
    {
      this->setError(Error::InvalidData, e.what());
    }
    catch (std::exception& e)
    {
      this->setError(Error::Unexpected, "Unexpected exception during the reading of a device. Please report this to the support: " + std::string(e.what()));
    }
    catch(...)
    {
      this->setError(Error::Unknown, "Unknown exception during the reading of a device. Please report this to the support");
    }
    
    if (optr->ErrorCode == Error::None)
      return true;
    return false;
  };

  /**
   * Write the content of the @a input to the current set device.
   * If an exception is thrown during the writing of the device, false is returned.
   * In case this method returns false, you can use the methods errorCode() and errorMessage() to retrieve the error.
   * Internally this methods call writeDevice() to extract data. Each inheriting handler must overload the method writeDevice().
   */
  bool Handler::write(const Node* input)
  {
    auto optr = this->pimpl();
    if (input == nullptr)
    {
      this->setError(Error::Unexpected, "Impossible to write a null input to a device");
      return false;
    }
    else if (optr->Device == nullptr)
    {
      this->setError(Error::Device, "No device assigned");
      return false;
    }
    else if (!optr->Device->isOpen())
    {
      this->setError(Error::Device, "Device not open");
      return false;
    }
    
    try
    {
      this->setError(); // reset
      this->writeDevice(input);
    }
    catch (Handler::FormatError& e)
    {
      this->setError(Error::InvalidData, e.what());
    }
    catch (std::exception& e)
    {
      this->setError(Error::Unexpected, "Unexpected exception during the writing of a device. Please report this to the support: " + std::string(e.what()));
    }
    catch(...)
    {
      this->setError(Error::Unknown, "Unknown exception during the writing of a device. Please report this to the support");
    }
    
    if (optr->ErrorCode == Error::None)
      return true;
    return false;
  };
  
  /**
   * Returns the current device associated with this handler.
   */
  Device* Handler::device() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Device;
  };
  
  /**
   * Sets the current device associated with this handler.
   */
  void Handler::setDevice(Device* device) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Device = device;
  };
  
  /**
   * Return the available options furnished with this handler.
   */
  std::vector<const char*> Handler::availableOptions() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->availableOptions();
  };
  
  /**
   * Return the available choice for the requested @a option.
   */
  std::vector<const char*> Handler::availableOptionChoices(const char* option) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->availableOptionChoices(option);
  };
  
  /**
   * Returns the current error code.
   */
  Handler::Error Handler::errorCode() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ErrorCode;
  };
  
  /**
   * Returns the current error message.
   */
  const std::string& Handler::errorMessage() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ErrorMessage;
  };
  
  /**
   * Try to detect the signature of the handler in the given device.
   * A signature is generaly a keyword at the beginning of the data that help to determine the content. For example, if the device is a File, this way is safer than relying on the file extension.
   * Internaly this methods use validateSignature(). Each inheriting handler must overload the method validateSignature().
   */
  Handler::Signature Handler::detectSignature() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Device == nullptr)
      return Signature::Invalid;
    return this->validateSignature();
  };
  
  /**
   * @fn virtual Signature Handler::validateSignature() const _OPENMA_NOEXCEPT = 0;
   * Method to overload to determine if the associated device is compatible with the handler.
   */
  
  /**
   * Method to overload to read data from the current set device.
   */
  void Handler::readDevice(Node* output)
  {
    OPENMA_UNUSED(output);
    this->setError(Error::Unexpected, "You called the default Handler::readDevice method. The instanced I/O handler has certainly not the capability to read data.");
  };
  
  /**
   * Method to overload to write data to the current set device.
   */
  void Handler::writeDevice(const Node* input)
  {
    OPENMA_UNUSED(input);
    this->setError(Error::Unexpected, "You called the default Handler::writeDevice method. The instanced I/O handler has certainly not the capability to write data.");
  };
  
  /**
   * Constructor for inheriting class with extended private implementation
   */
  Handler::Handler(HandlerPrivate& pimpl) _OPENMA_NOEXCEPT
  : mp_Pimpl(&pimpl)
  {};
  
  /**
   * @fn template <typename O> typename O::ValueType Handler::option() const _OPENMA_NOEXCEPT;
   * Returns the options based on the static template parameter @a O.
   */
  
  /**
   * @fn template <typename O, typename V> inline void Handler::setOption(const V& value) _OPENMA_NOEXCEPT;
   * Sets the options based on the static template parameter @a O and the given @a value.
   */
  
  /**
   * Retrieves the given option's value base on its @a name. The result is set in @a value.
   */
  void Handler::option(const char* name, void* value) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->option(name,value);
  };

  /**
   * Sets @a value to the option @a name.
   */
  void Handler::setOption(const char* name, const void* value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->setOption(name,value);
  };
  
  /**
   * Set a code and a detailed message for an error which happened during the reading or writing of a device.
   */
  void Handler::setError(Error code, const std::string& msg) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->ErrorCode = code;
    optr->ErrorMessage = msg;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class Handler::FormatError openma/io/handler.h
   * @brief General exception to use when an error happens during the reading/writing of data.
   * The internal implementation of a Handler use exception to determine if something wrong happened.
   * All kind of exceptions are catched, but it is strongly advise to use FormatError for case managed by an handler.
   */
  
  /**
   * @fn explicit Handler::FormatError::FormatError(const std::string& msg)
   * Construct a FormatError exception
   */
};
};
