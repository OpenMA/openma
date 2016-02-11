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
#include "openma/io/enums.h"
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
  : Source(nullptr), ErrorCode(Error::None), ErrorMessage()
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
   * Some methods are available to set errors and retrieve them. The extraction of handler's class 
   * is implemented. In fact, only four (or five) methods must be implemented for inherting handler:
   *  - readDevice() method: extract data from a device and set it into a node
   *  - writeDevice() method: write data to a given device
   *  - validateSignature() method: test if the read device has a signature valid for this handler.
   *  - canRead() method: to inform on the capability of the handler to read data from a device
   *  - canWrite() method: to inform on the capability of the handler to write data to a device
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
   * Destructor (default)
   */
  Handler::~Handler() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the capability of the handler to read data from a device. By default this method returns false.
   */
  bool Handler::canRead() const _OPENMA_NOEXCEPT
  {
    return false;
  };
  
  /**
   * Returns the capability of the handler to write data to a device. By default this method returns false.
   */
  bool Handler::canWrite() const _OPENMA_NOEXCEPT
  {
    return false;
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
    else if (optr->Source == nullptr)
    {
      this->setError(Error::Device, "No device assigned");
      return false;
    }
    else if (!optr->Source->isOpen())
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
      this->setError(Error::None); // reset
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
      if (optr->Source->atEnd())
        excmsg = "Unexpected end of device";
      else if(optr->Source->hasError())
        excmsg = "Loss of integrity in the device";
      else if(optr->Source->hasFailure())
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
    else if (optr->Source == nullptr)
    {
      this->setError(Error::Device, "No device assigned");
      return false;
    }
    else if (!optr->Source->isOpen())
    {
      this->setError(Error::Device, "Device not open");
      return false;
    }
    
    try
    {
      this->setError(Error::None); // reset
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
    return optr->Source;
  };
  
  /**
   * Sets the current device associated with this handler.
   */
  void Handler::setDevice(Device* device) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Source = device;
  };
  
  /**
   * Returns the current error code.
   */
  Error Handler::errorCode() const _OPENMA_NOEXCEPT
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
  Signature Handler::detectSignature() const _OPENMA_NOEXCEPT
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
