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

#include "openma/body/landmarkstranslator.h"
#include "openma/body/landmarkstranslator_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  LandmarksTranslatorPrivate::LandmarksTranslatorPrivate(LandmarksTranslator* pint, const std::string& name, const std::unordered_map<std::string,std::string>& conversionTable)
  : NodePrivate(pint,name), ConversionTable(conversionTable)
  {};
  
  LandmarksTranslatorPrivate::~LandmarksTranslatorPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace body
{
  /**
   * @class LandmarksTranslator openma/body/landmarkstranslator
   * @brief Map labels to other labels
   * No acquisition system and protocol uses the same label for same (bony) landmarks. However, to build the same model in each case, this one needs to use an internal landmarks convention. The translation between external and internal label is realized by a LandmarksTranslator.
   * @ingroup openma_body
   */
  
  /**
   * Constructor. Create a node with the given @a name and store the @a conversionTable.
   */
  LandmarksTranslator::LandmarksTranslator(const std::string& name, const std::unordered_map<std::string,std::string>& conversionTable, Node* parent)
  : Node(*new LandmarksTranslatorPrivate(this,name,conversionTable), parent)
  {};
  
  /**
   * Destructor (default)
   */
  LandmarksTranslator::~LandmarksTranslator() _OPENMA_NOEXCEPT = default;
  
  /**
   * Return the mapped name found in the stored conversion table. If @a name is not found, then a copy of itself is returned.
   */
  std::string LandmarksTranslator::convert(const std::string& name) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    auto it = optr->ConversionTable.find(name);
    if (it != optr->ConversionTable.end())
      return it->second;
    return name;
  };
  
  /**
   * Return the mapped name found in the stored conversion table. If @a name is not found, then an empty string is returned.
   */
  std::string LandmarksTranslator::convertIfExists(const std::string& name) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    auto it = optr->ConversionTable.find(name);
    if (it != optr->ConversionTable.end())
      return it->second;
    return std::string{};
  };
};
};