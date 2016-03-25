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

#include "openma/io/handlerplugin.h"
#include "openma/io/device.h"
#include "openma/io/enums.h"
#include "openma/base/pluginmanager.h"

// #ifdef OPENMA_IO_STATIC_DEFINE
#include "../plugins/staticiopluginloader.h"
// #endif

namespace ma
{
namespace io
{
  /**
   * Convenient method to load handler plugins.
   * Internally this method uses a static object. It will be populated the first time this function is used.
   */
  const std::vector<HandlerPlugin*>& load_handler_plugins()
  {
    static PluginManager<HandlerPlugin> manager;
    if (manager.plugins().empty())
    {
// #ifndef OPENMA_IO_STATIC_DEFINE
      // Dynamic loading
      // {"trialformats","bodyformats"})
// #error Not yet implemented
// #else
      // Static include
      load_handler_plugins(&manager);
// #endif
    }
    return manager.plugins();
  };
  
  /**
   * @class HandlerPlugin openma/io/handlerplugin.h
   * @brief Interface to load handler(s) dynamically.
   *
   * A format is a string composed of two parts. Both separated by a dot. The left side of the dot is the name of company that developed the format (e.g.'codamotion'), while the right part is the (file) extension used for this format (e.g. mdf).
   *
   * @ingroup openma_io
   */
  
  /**
   * Default constructor
   */
  HandlerPlugin::HandlerPlugin()
  {};
  
  /**
   * Destructor (default)
   */
  HandlerPlugin::~HandlerPlugin() = default;
  
  /**
   * @fn virtual std::vector<std::string> supportedFormats() const _OPENMA_NOEXCEPT = 0;
   * Returns a list of supported format by this plugin.
   */
  
  /**
   * @fn virtual Capability HandlerPlugin::capabilities(const std::string& format) const _OPENMA_NOEXCEPT
   * Returns the capabilities availble for a handler managed by this plugin. In case multiple handlers are managed by this plugin, you can use @a format to returns the corresponding capabilities. For example:
   * @code{.unparsed}
   * Capability MyHandler::capabilities(const std::string& format) const _OPENMA_NOEXCEPT
   * {
   *    if (format.compare("openma.foo") == 0)
   *      eturn Capability::CanReadAndWrite;
   *    else if (format.compare("openma.bar") == 0)
   *      eturn Capability::CanRead;
   * };
   * @endcode
   */
    
  /**
   * @fn virtual Signature HandlerPlugin::detectSignature(const Device* const device, std::string* format) const _OPENMA_NOEXCEPT = 0;
   * Detect the signature used fo one of the supported format. If the string @a format is not null, the detected format will be set. Because, the @a device is a const pointer, it is needed to use the method Device::peek() to extract content without modifying the device.
   * In case no signature exists for the supported formats, this method shall return Signature::NotAvailable.
   */
  
  /**
   * Try to detect the extension used by one of the supported format in the string @a str.
   * If the string @a format is not null, the detected format will be set inside.
   * @note This detection method is not robust. If possible use the signature detection.
   */
  bool HandlerPlugin::detectExtension(const std::string& str, std::string* format) const _OPENMA_NOEXCEPT
  {
    auto formats = this->supportedFormats();
    auto idx = str.find_last_of('.');
    if (idx != std::string::npos)
    {  
      auto ext = str.substr(idx+1);
      auto len = ext.length();
      for (const auto& f: formats)
      {
        if (f.substr(f.length()-len,len).compare(ext) == 0)
        {
          if (format != nullptr)
            *format = f;
          return true;
        }
      }
    }
    return false;
  };
  
  /**
   * @fn virtual Handler* HandlerPlugin::create(Device* device, const std::string& format) = 0;
   * Create a new handler. The @a device is passed to the created handler. The usage of @a format can be twofold. First, it can be used to verify if the given format is supported by this plugin. Second, it will help to choose the good handler if multiple formats are managed by the plugin.
   */
};
};