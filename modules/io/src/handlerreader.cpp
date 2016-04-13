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

#include "openma/io/handlerreader.h"
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
  
  class HandlerReaderPrivate
  {
  public:
    HandlerReaderPrivate(Device* device, const std::string& format);
    ~HandlerReaderPrivate() _OPENMA_NOEXCEPT;
    
    Device* Source;
    std::string Format;
    Handler* Reader;
    Error ErrorCode;
    std::string ErrorMessage;
  };
  
  HandlerReaderPrivate::HandlerReaderPrivate(Device* device, const std::string& format)
  : Source(device), Format(format), Reader(nullptr), ErrorCode(Error::None), ErrorMessage(std::string{})
  {};
  
  HandlerReaderPrivate::~HandlerReaderPrivate() _OPENMA_NOEXCEPT = default; // Cannot be inlined
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
   * @class HandlerReader openma/io/handlerreader.h
   * @brief Format independent interface to read movement analysis data from devices.
   *
   * The reading of movement analysis data is simplifed with this interface that manage different kind of formats.
   * In case the format is not know (i.e. set to an empty string), the method read() and canRead() will attempt to detect the good format associated with the content of the device. The detected format can be retrieved using the method format().
   * You could also decide to force the type of format using the method setFormat().
   *
   * A simple usage to read a file is for example
   * @code{.unparsed}
   * // Create a file device
   * ma::io::File file;
   * // Open a file in read mode
   * file.open(filename, ma::io::Mode::In);
   * // Pass it to the reader
   * ma::io::HandlerReader reader(&file);
   * // Create a root node
   * ma::Node root("root");
   * // Extract the content of the file and assign it as a child of root
   * reader.read(&root);
   * // Depending of your needs, you can directly access to some children data.
   * auto timeSequences = root.findChildren<ma::TimeSequence*>();
   * // Or retrieve a complete trial
   * ma::Trial* trial = root.findChild<ma::Trial*>();
   * @endcode
   *
   * You can reuse a reader by modifying the set device or format using the methods setDevice() and setFormat() respectively. The modification of the format will reset the internal format handler. If you set a new device and you are not sure about its format, you should set the later to an empty string. This will run a new auto-detection when the method read() will be used.
   *
   * @ingroup openma_io
   */
  
  /**
   * Constructor. 
   * The given @a device and @a format (optional) are stored to be later used to extract data using the method read().
   * In case, you have no idea on the format, leave it blank. The method canRead() will attempt to find it.
   */
  HandlerReader::HandlerReader(Device* device, const std::string& format)
  : mp_Pimpl(new HandlerReaderPrivate(device, format))
  {};
    
  /**
   * Destructor (default)
   */
  HandlerReader::~HandlerReader() = default;
  
  /**
   * Set a new device.
   * If no format was set, the methods canRead() or read() will attempt to find a handler that fit with the set device.
   */
  void HandlerReader::setDevice(Device* device)
  {
    auto optr = this->pimpl();
    optr->Source = device;
  };
  
  /**
   * When a new format is set, the current handler (if any) is reset. The use of the methods canRead() or read() will attempt to find a handler that fit with the given format.
   * If the given @a format is set to an empty string, the reader will try to auto-detect the format, based on the content of the set device.
   */
  void HandlerReader::setFormat(const std::string& format)
  {
    auto optr = this->pimpl();
    if (optr->Format.compare(format) == 0)
      return;
    optr->Format = format;
    delete optr->Reader;
    optr->Reader = nullptr;
  };
  
  /**
   * Return the current format used by this reader.
   * In case no (or empty) format was set to the reader, the returned value will be the one detected by the method canRead().
   * If a format was set, this one is returned.
   */
  const std::string& HandlerReader::format() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Format;
  };
  
  /**
   * Returns true if the signature used by the set format is found.
   * If no format was previously set (i.e. if the format is set to an empty string), this method will attempt to find the good format 
   * @note This method can return true but the reading of data with the method read() can fail if the content of the set device is corrupted.
   */
  bool HandlerReader::canRead()
  {
    auto optr = this->pimpl();
    this->setError(Error::None);
    if ((optr->Source == nullptr) || ((optr->Source->openMode() & Mode::In) != Mode::In) || !optr->Source->isOpen())
    {
      this->setError(Error::Device, "No valid device assigned.");
      return false;
    }
    if (optr->Reader != nullptr)
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
      std::vector<HandlerPlugin*> missingSignature, invalidSignature;
      // First pass: use handler signatures
      for (const auto& plugin: plugins)
      {
        auto signature = plugin->detectSignature(optr->Source, &(optr->Format));
        if (signature == Signature::Valid)
        {
          optr->Reader = plugin->create(optr->Source, optr->Format);
          break;
        }
        else if (signature == Signature::NotAvailable)
        {
          missingSignature.push_back(plugin);
        }
        else
        {
          invalidSignature.push_back(plugin);
        }
      }
      // No yet found? Let's go in the second pass for specific plugin
      // Maybe the name of the device will help to find the good format (extension)
      if (optr->Reader == nullptr)
      {
        const std::string name(optr->Source->name());
        for (const auto& plugin: missingSignature)
        {
          if (plugin->detectExtension(name, &(optr->Format)) && ((plugin->capabilities(optr->Format) & Capability::CanRead) == Capability::CanRead))
          {
            optr->Reader = plugin->create(optr->Source, optr->Format);
            break;
          }
        }
      }
      // Finally, it might be possible than one of the signed handler was not detected because of invalid data in the device.
      // Lets check if the extension was recognized.
      if (optr->Reader == nullptr)
      {
        const std::string name(optr->Source->name());
        std::string format;
        for (const auto& plugin: invalidSignature)
        {
          if (plugin->detectExtension(name, &format) && ((plugin->capabilities(format) & Capability::CanRead) == Capability::CanRead))
          {
            this->setError(Error::InvalidData, "The content of your device might be corrupted as no handler signature was found, but the extension is known");
            break;
          }
        }
      }
      // Final test
      if ((optr->Reader == nullptr) && (this->errorCode() == Error::None))
        this->setError(Error::UnsupportedFormat, "No handler found to support the given device and/or format");
    }
    // Pre-determined format?
    else 
    {
      for (const auto& plugin: plugins)
      {
        if ((plugin->capabilities(optr->Format) & Capability::CanRead) == Capability::CanRead)
        {
          optr->Reader = plugin->create(optr->Source, optr->Format);
          break;
        }
      }
    }
    // Final test
    return (optr->Reader != nullptr);
  };
  
  /**
   * Read the content of the set device using the set/detected format and add the result as a child (children) of the given object @a root.
   * This method returns @c true if no error was thrown during the reading of the device.
   * In case @c false is returned, you could find more information on the error using the methods errorCode() and errorMessage().
   */
  bool HandlerReader::read(Node* root)
  {
    auto optr = this->pimpl();
    if ((optr->Reader == nullptr) && !this->canRead())
      return false;
    auto result = optr->Reader->read(root);
    this->setError(optr->Reader->errorCode(), optr->Reader->errorMessage());
    return result;
  };
  
  /**
   * Returns the code associated with a posible error set during the reading of a device.
   * By default this method returns Error::None.
   * This method should be used if the method read() returns false.
   */
  Error HandlerReader::errorCode() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ErrorCode;
  };
  
  /**
   * Returns the message associated with a posible error set during the reading of a device.
   * By default this method returns an empty string.
   * This method could be used if the method read() returns false.
   */
  const std::string& HandlerReader::errorMessage() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ErrorMessage;
  };
  
  /**
   * Returns a vector with all formats that have the capability to read data from a device.
   */
  const std::vector<std::string>& HandlerReader::availableFormats() _OPENMA_NOEXCEPT
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
          if ((plugin->capabilities(format) & Capability::CanRead) == Capability::CanRead)
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
  void HandlerReader::setError(Error code, const std::string& msg) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->ErrorCode = code;
    optr->ErrorMessage = msg;
  };
};
};