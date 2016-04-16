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

#include "openma/io/handlerwriter.h"
#include "openma/io/handlerplugin.h"
#include "openma/io/device.h"
#include "openma/io/handler.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace io
{
  enum class Error;
  
  class HandlerWriterPrivate
  {
  public:
    HandlerWriterPrivate(Device* device, const std::string& format);
    ~HandlerWriterPrivate() _OPENMA_NOEXCEPT;
    
    Device* Source;
    std::string Format;
    Handler* Writer;
    Error ErrorCode;
    std::string ErrorMessage;
  };
  
  HandlerWriterPrivate::HandlerWriterPrivate(Device* device, const std::string& format)
  : Source(device), Format(format), Writer(nullptr), ErrorCode(Error::None), ErrorMessage(std::string{})
  {};
  
  HandlerWriterPrivate::~HandlerWriterPrivate() _OPENMA_NOEXCEPT = default; // Cannot be inlined
};
};

#endif

// ------------------------------------------------------------------------- //
//                                 PUBLIC API                                //
// ------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  /**
   * @class HandlerWriter openma/io/handlerwriter.h
   * @brief Format independent interface to write movement analysis data to devices.
   *
   * The writing of movement analysis data is simplifed with this interface that manage different kind of formats.
   * In case the format is not know (i.e. set to an empty string), the method write() and canWrite() will attempt to detect the good format associated with the content of the device. The detected format can be retrieved using the method format().
   * You could also decide to force the type of format using the method setFormat().
   *
   * A simple usage to write a file is for example
   * @code{.unparsed}
   * // The variable root contain a Trial child object.
   * // This one can be, for example, created from scratch or loaded from a file
   * // Create a file device
   * ma::io::File file;
   * // Open a file in write mode
   * file.open(filename, ma::io::Mode::Out);
   * // Pass it to the writer
   * ma::io::HandlerWriter writer(&file);
   * // Export the content to the file
   * writer.write(&root);
   * @endcode
   *
   * You can reuse a writer by modifying the set device or format using the methods setDevice() and setFormat() respectively. The modification of the format will reset the internal format handler. If you set a new device and you are not sure about its format, you should set the later to an empty string. This will run a new auto-detection when the method write() will be used.
   *
   * @ingroup openma_io
   */
  
  /**
   * Constructor. 
   * The given @a device and @a format (optional) are stored to be later used to extract data using the method write().
   * In case, you have no idea on the format, leave it blank. The method canWrite() will attempt to find it.
   */
  HandlerWriter::HandlerWriter(Device* device, const std::string& format)
  : mp_Pimpl(new HandlerWriterPrivate(device, format))
  {};
    
  /**
   * Destructor (default)
   */
  HandlerWriter::~HandlerWriter() = default;
  
  /**
   * Set a new device.
   * If no format was set, the methods canWrite() or write() will attempt to find a handler that fit with the set device.
   */
  void HandlerWriter::setDevice(Device* device)
  {
    auto optr = this->pimpl();
    optr->Source = device;
  };
  
  /**
   * When a new format is set, the current handler (if any) is reset. The use of the methods canWrite() or write() will attempt to find a handler that fit with the given format.
   * If the given @a format is set to an empty string, the writer will try to auto-detect the format, based on the content of the set device.
   */
  void HandlerWriter::setFormat(const std::string& format)
  {
    auto optr = this->pimpl();
    if (optr->Format.compare(format) == 0)
      return;
    optr->Format = format;
    delete optr->Writer;
    optr->Writer = nullptr;
  };
  
  /**
   * Return the current format used by this writer.
   * In case no (or empty) format was set to the writer, the returned value will be the one detected by the method canWrite().
   * If a format was set, this one is returned.
   */
  const std::string& HandlerWriter::format() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Format;
  };
  
  /**
   * Returns true if the extension used by the set format is found.
   * If no format was previously set (i.e. if the format is set to an empty string), this method will attempt to find the good format 
   * @note This method can return true but the writing of data with the method write() can fail if the content is not compatible with the format.
   */
  bool HandlerWriter::canWrite()
  {
    auto optr = this->pimpl();
    this->setError(Error::None);
    if ((optr->Source == nullptr) || ((optr->Source->openMode() & Mode::Out) != Mode::Out) || !optr->Source->isOpen())
    {
      this->setError(Error::Device, "No valid device assigned.");
      return false;
    }
    if (optr->Writer != nullptr)
    {
      // If a handler is already cached, it must be because the given device (and format) helped to detect/extract it. For this, let's return true.
      return true;
    }
    // Let's find the good handler
    const auto& plugins = load_handler_plugins();
    // Format auto detection?
    bool formatAutoDetection = optr->Format.empty();
    // In case no format is specified, it is necessary to pass through all the loaded handlers. If none of them detects a managed signature, then, a second pass is made for handler with non specific signature. These handlers rely on device name and handler format.
    if (formatAutoDetection)
    {
      const std::string name(optr->Source->name());
      for (const auto& plugin: plugins)
      {
        if (plugin->detectExtension(name, &(optr->Format)) && ((plugin->capabilities(optr->Format) & Capability::CanWrite) == Capability::CanWrite))
        {
          optr->Writer = plugin->create(optr->Source, optr->Format);
          break;
        }
      }
      if ((optr->Writer == nullptr) && (this->errorCode() == Error::None))
        this->setError(Error::UnsupportedFormat, "No handler found to support the given device and/or format");
    }
    // Pre-determined format?
    else 
    {
      for (const auto& plugin: plugins)
      {
        if ((plugin->capabilities(optr->Format) & Capability::CanWrite) == Capability::CanWrite)
        {
          optr->Writer = plugin->create(optr->Source, optr->Format);
          break;
        }
      }
    }
    // Final test
    return (optr->Writer != nullptr);
  };
  
  /**
   * Write the content of the @a root object using the set/detected format.
   * This method returns @c true if no error was thrown during the writing of the data.
   * In case @c false is returned, you could find more information on the error using the methods errorCode() and errorMessage().
   */
  bool HandlerWriter::write(Node* root)
  {
    if (!this->canWrite())
      return false;
    auto optr = this->pimpl();
    auto result = optr->Writer->write(root);
    this->setError(optr->Writer->errorCode(), optr->Writer->errorMessage());
    return result;
  };
  
  /**
   * Returns the code associated with a posible error set during the writing of data.
   * By default this method returns Error::None.
   * This method should be used if the method write() returns false.
   */
  Error HandlerWriter::errorCode() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ErrorCode;
  };
  
  /**
   * Returns the message associated with a posible error set during the writing of data.
   * By default this method returns an empty string.
   * This method could be used if the method write() returns false.
   */
  const std::string& HandlerWriter::errorMessage() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ErrorMessage;
  };
  
  /**
   * Returns a vector with all formats that have the capability to write data to a device.
   */
  const std::vector<std::string>& HandlerWriter::availableFormats() _OPENMA_NOEXCEPT
  {
    static std::vector<std::string> formats;
    if (formats.empty())
    {  
      const auto& plugins = load_handler_plugins();
      for (auto plugin: plugins)
      {
        const auto& supportedformats = plugin->supportedFormats();
        for (const auto& format: supportedformats)
        {
          if ((plugin->capabilities(format) & Capability::CanWrite) == Capability::CanWrite)
          {
            formats.push_back(format);
          }
        }
      }
    }
    return formats;
  };
  
  /**
   * Helper method to set error information (code and message).
   */
  void HandlerWriter::setError(Error code, const std::string& msg) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->ErrorCode = code;
    optr->ErrorMessage = msg;
  };
};
};