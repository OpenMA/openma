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

#include "openma/base/logger.h"

#include <iostream>
#include <cstdio> // vsnprintf
#include <cstring> // strlen
#include <cstdarg> // va_start, va_end

namespace ma
{
  struct Logger::Private
  {
    Private() _OPENMA_NOEXCEPT : Output(nullptr), Quiet(false) {};
    ~Private() _OPENMA_NOEXCEPT = default;
    
    Private(const Private& ) = delete;
    Private(Private&& ) _OPENMA_NOEXCEPT = delete;
    Private& operator=(const Private& ) = delete;
    Private& operator=(Private&& ) _OPENMA_NOEXCEPT = delete;
    
    Device* Output;
    bool Quiet;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @fn template<typename... Args> void info(const char* msg, Args&&... args) _OPENMA_NOEXCEPT
   * Write a Message::Info log.
   */
  
  /**
   * @fn template<typename... Args> void warning(const char* msg, Args&&... args) _OPENMA_NOEXCEPT
   * Write a Message::Warning log.
   */
  
  /**
   * @fn template<typename... Args> void error(const char* msg, Args&&... args) _OPENMA_NOEXCEPT   
   * Write a Message::Error log.
   */
  
  // ----------------------------------------------------------------------- //
 
  /**
   * @enum Message
   * Enums used to specify message's type.
   * @ingroup openma_base
   */
  /**
   * @var Message Message::Info
   * Enum value used for information messages.
   */
  /**
   * @var Message Message::Warning
   * Enum value used for warning messages.
   */
  /**
   * @var Message Message::Error
   * Enum value used for error messages.
   */
  
  /**
   * @struct Logger::Device openma/base/logger.h
   * @brief Interface class used to write message sent by the logger.
   *
   * To be effective, the Logger class needs a device. This device is used to
   * write log messages.
   *
   * A default implementation used to print messages on the standard std::cout 
   * and std::cerr streams is proposed and correspond to the following lines.
   *
   * @code{.unparsed}
   * struct Console : Logger::Device
   * {
   *   virtual void write(Logger::Category category, const char* msg) _OPENMA_NOEXCEPT override
   *   {
   *     if (category == Logger::Info)
   *       std::cout << "INFO: " << msg << std::endl;
   *     else if (category == Logger::Warning)
   *       std::cout << "WARNING: " << msg << std::endl;
   *     else if (category == Logger::Error)
   *       std::cout << "ERROR: " << msg << std::endl;
   *   }
   * };
   * @endcode
   *
   * Only one device can be used at a time. To create your custom device you only need
   * to inherit from the class Logger::Device and override the method Logger::Device::write().
   * To use this custom device with the logger, you have to use the method Device::setDevice().
   */
  
  /**
   * Default (empty) constructor
   */
  Logger::Device::Device() _OPENMA_NOEXCEPT = default;
  
  /**
   * Default (empty) destructor
   */
  Logger::Device::~Device() _OPENMA_NOEXCEPT = default;
  
  /**
   * @fn virtual void Logger::Device::write(Message category, const char* msg) _OPENMA_NOEXCEPT = 0;
   * The logger seng message to this method. The @a category input specifies if the message is for
   * an information, a warning, or an error. The @a msg input is directly the string given to the functions
   * info(), warning(), or error().
   */
  
  // ----------------------------------------------------------------------- //

  namespace __details
  {
    struct Console : Logger::Device
    {
      virtual void write(Message category, const char* msg) _OPENMA_NOEXCEPT override
      {
        if (category == Message::Info)
          std::cout << "INFO: " << msg << std::endl;
        else if (category == Message::Warning)
          std::cerr << "WARNING: " << msg << std::endl;
        else if (category == Message::Error)
          std::cerr << "ERROR: " << msg << std::endl;
      }
    };
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class Logger openma/base/logger.h
   * @brief Class to centralize log messages and write them to a device
   *
   * Three categories are available and could be used as proposed below:
   * - Message::Info: For users' information or debugging
   * - Message::Warning: For incorrect inputs in algorithms but still adatable.
   * - Message::Error: For undesired behaviour which can break algorithms' logic.
   *
   * Each logging category has a dedicated static function:
   * - info()
   * - warning()
   * - error()
   *
   * These functions send the given message to a Logger::Device set. This device 
   * has the role to write the message (in a console, a file, etc.). By default,
   * the message are sent into the standard cout/cerr streams. 
   * To set a device, you have to use the method Logger::setDevice()
   *
   * To disable the logger, you can use the method logger::mute().
   *
   * @ingroup openma_base
   */

  /**
   * Active/unactive the logger. If the logger is set to mute,
   * all the messages are eaten and destroyed.
   */
  void Logger::mute(bool active) _OPENMA_NOEXCEPT
  {
    Logger::instance().mp_Pimpl->Quiet = active;
  };
  
  /**
   * Returns the device which will write the log message
   */
  Logger::Device* Logger::device() _OPENMA_NOEXCEPT
  {
    return Logger::instance().mp_Pimpl->Output;
  };
  
  /**
   * Set the device which will write the log messages. If a previous device was set,
   * it will be deleted. The logger takes the ownership of the device.
   */
  void Logger::setDevice(Device* output) _OPENMA_NOEXCEPT
  {
    delete Logger::instance().mp_Pimpl->Output;
    Logger::instance().mp_Pimpl->Output = output;
  };
  
  /**
   * Destructor
   * Delete the set device
   */
  Logger::~Logger() _OPENMA_NOEXCEPT
  {
    delete this->mp_Pimpl->Output;
    delete this->mp_Pimpl;
  };
  
  /**
   * Singleton
   */
  Logger& Logger::instance()
  {
    static Logger singleton;
    return singleton;
  };
  
  /**
   * Constructor
   */
  Logger::Logger()
  : mp_Pimpl(new Logger::Private)
  {};
  
  /**
   * Send a message to the set device. If no device is set, a default one is created
   * and send info messages to the std::cout stream and warning and error messages to 
   * the std::cerr stream. You can set a device using the method Logger::setDevice().
   *
   * Compared to the method Logger::sendMessage(), this method creates also a string 
   * based on the given variadic arguments.
   */
  void Logger::prepareAndSendMessage(Message category, const char* msg, ...)
  {
    if (this->mp_Pimpl->Quiet)
      return;
    if (this->mp_Pimpl->Output == nullptr)
      this->mp_Pimpl->Output = new __details::Console;
    int n = strlen(msg)*2;
    char* str = new char[n];
    while (1)
    {
      va_list args;
      va_start(args, msg);
      int len = vsnprintf(str, n, msg, args);
      va_end(args);
#if !defined(_MSC_VER)
      // If something is wrong (negative length), the string is reset and sent like this
      if (len < 0)
      {
        str[0] = '\0';
        len = 0;
      }
      // If the string is complete (or an encoding error occurs)
      if (len < n)
        break;
      n += len - n + 1; // Should be done only one time. +1: null character
      delete[] str;
      str = new char[n];
#else
      // if the length is negative, then the buffer was truncated and need to be larger.
      if (len < 0)
      {
        n *= 2;
        delete[] str;
        str = new char[n];
      }
      else
        break;
#endif
    }
    this->mp_Pimpl->Output->write(category,str);
    delete[] str;
  };

  /**
   * Send a message to the set device. If no device is set, a default one is created
   * and send info messages to the std::cout stream and warning and error messages to 
   * the std::cerr stream. You can set a device using the method Logger::setDevice().
   */
  void Logger::sendMessage(Message category, const char* msg)
  {
    if (this->mp_Pimpl->Quiet)
      return;
    if (this->mp_Pimpl->Output == nullptr)
      this->mp_Pimpl->Output = new __details::Console;
    this->mp_Pimpl->Output->write(category,msg);
  };
};