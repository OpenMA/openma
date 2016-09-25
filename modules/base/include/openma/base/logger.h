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

#ifndef __openma_base_logger_h
#define __openma_base_logger_h

#include "openma/base_export.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <utility> // std::forward

namespace ma
{
  template<typename... Args> static inline void info(const char* msg, Args&&... args) _OPENMA_NOEXCEPT;
  template<typename... Args> static inline void warning(const char* msg, Args&&... args) _OPENMA_NOEXCEPT;
  template<typename... Args> static inline void error(const char* msg, Args&&... args) _OPENMA_NOEXCEPT;
  
  enum class Message : char {Info = 0x01, Warning, Error};
  
  class OPENMA_BASE_EXPORT Logger
  {
  public:
    struct OPENMA_BASE_EXPORT Device
    {
      Device() _OPENMA_NOEXCEPT;
      virtual ~Device() _OPENMA_NOEXCEPT;
      
      virtual void write(Message category, const char* msg) _OPENMA_NOEXCEPT = 0;
      
      Device(const Device& ) = delete;
      Device(Device&& ) _OPENMA_NOEXCEPT = delete;
      Device& operator=(const Device& ) = delete;
      Device& operator=(Device&& ) _OPENMA_NOEXCEPT = delete;
    };
    
    static void mute(bool active) _OPENMA_NOEXCEPT;
    static Device* device() _OPENMA_NOEXCEPT;
    static void setDevice(Device* output) _OPENMA_NOEXCEPT;
    
    static Logger& instance();
    void prepareAndSendMessage(Message category, const char* msg, ...);
    void sendMessage(Message category, const char* msg);
    
    ~Logger() _OPENMA_NOEXCEPT;
    
    Logger(const Logger& ) = delete;
    Logger(Logger&& ) _OPENMA_NOEXCEPT = delete;
    Logger& operator=(const Logger& ) = delete;
    Logger& operator=(Logger&& ) _OPENMA_NOEXCEPT = delete;
    
  private:
    Logger();
    
    struct Private;
    Private* mp_Pimpl;
  };
  
  // ----------------------------------------------------------------------- //
  
  namespace __details
  {
    template<typename... Args>
    static inline void _send_message(Message category, const char* msg, Args&&... args)
    {
      Logger::instance().prepareAndSendMessage(category, msg, std::forward<Args>(args)...);
    };
  
    template<>
    inline void _send_message<>(Message category, const char* msg)
    {
      Logger::instance().sendMessage(category, msg);
    };
  };
  
  // ----------------------------------------------------------------------- //
  
  template<typename... Args> static inline void info(const char* msg, Args&&... args) _OPENMA_NOEXCEPT
  {
    __details::_send_message(Message::Info,msg,std::forward<Args>(args)...);
  };
  
  template<typename... Args> static inline void warning(const char* msg, Args&&... args) _OPENMA_NOEXCEPT
  {
    __details::_send_message(Message::Warning,msg,std::forward<Args>(args)...);
  };
  
  template<typename... Args> static inline void error(const char* msg, Args&&... args) _OPENMA_NOEXCEPT
  {
    __details::_send_message(Message::Error,msg,std::forward<Args>(args)...);
  };
};

#endif // __openma_base_logger_h