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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#ifndef __openma_body_landmarkstranslator_h
#define __openma_body_landmarkstranslator_h

#include "openma/body_export.h"
#include "openma/base/node.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <vector>
#include <string>

OPENMA_EXPORT_NODE_CAST_2(ma, body, LandmarksTranslator, OPENMA_BODY_EXPORT);

namespace ma
{
namespace body
{
  class LandmarksTranslatorPrivate;
  
  class OPENMA_BODY_EXPORT LandmarksTranslator : public Node
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(LandmarksTranslator)
    OPENMA_DECLARE_NODEID(LandmarksTranslator, Node)
        
  public:
    LandmarksTranslator(const std::string& name, const std::unordered_map<std::string,std::string>& converstionTable, Node* parent = nullptr);
    ~LandmarksTranslator() _OPENMA_NOEXCEPT;
    
    LandmarksTranslator(const LandmarksTranslator& ) = delete;
    LandmarksTranslator(LandmarksTranslator&& ) _OPENMA_NOEXCEPT = delete;
    LandmarksTranslator& operator=(const LandmarksTranslator& ) = delete;
    LandmarksTranslator& operator=(LandmarksTranslator&& ) _OPENMA_NOEXCEPT = delete;
    
    std::string convert(const std::string& name) const _OPENMA_NOEXCEPT;
    std::string convertIfExists(const std::string& name) const _OPENMA_NOEXCEPT;
    
  private:
    LandmarksTranslator(LandmarksTranslatorPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT;
  };
};
};

#endif // __openma_body_landmarkstranslator_h