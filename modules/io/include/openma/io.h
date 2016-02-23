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

#ifndef __openma_io_h
#define __openma_io_h

#include "openma/io/binarystream.h"
#include "openma/io/buffer.h"
#include "openma/io/device.h"
#include "openma/io/enums.h"
#include "openma/io/file.h"
#include "openma/io/handler.h"
#include "openma/io/handlerplugin.h"
#include "openma/io/handlerreader.h"

#include "openma/base/node.h"
#include "openma/base/logger.h"

#include <string>

namespace ma
{
namespace io
{
  /**
   * Convenient function to read the content of a file and set it in @a root.
   * Internally, this function use the class HandlerReader.
   * @relates HandlerReader
   * @ingroup openma_io
   */
  inline bool read(Node* root, const std::string& filepath, const std::string& format = {})
  {
    File file;
    file.open(filepath.c_str(), Mode::In);
    HandlerReader reader(&file, format);
    bool result = reader.read(root);
    if (!result && (reader.errorCode() != Error::None))
      error(reader.errorMessage().c_str());
    return result;
  };
    
  /**
   * Convenient function to read the content of a file and return it in a Node object.
   * In case the result is null (@c nullptr), this certainly means that an error was thrown. Error message are sent to the logger and might help to determine the problem.
   * @note The returned Node was created by the new() operator. It is the responsability of the developer to delete it.
   * @relates HandlerReader
   * @ingroup openma_io
   */
  inline Node* read(const std::string& filepath, const std::string& format = {})
  {
    Node* root = new Node("root");
    bool result = read(root, filepath, format);
    if (!result)
    {
      delete root;
      root = nullptr;
    }
    return root;
  };
}  
}

#endif // __openma_io_h