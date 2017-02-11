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

#include "openma/body/landmarksregistrar.h"
#include "openma/body/landmarksregistrar_p.h"

#include "openma/body/landmarkstranslator.h"
#include "openma/base/timesequence.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  LandmarksRegistrarPrivate::LandmarksRegistrarPrivate(LandmarksRegistrar* pint, const std::string& name, const std::vector<std::string>& labels)
  : NodePrivate(pint,name), Labels(labels)
  {};
  
  LandmarksRegistrarPrivate::~LandmarksRegistrarPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::LandmarksRegistrar);

namespace ma
{
namespace body
{
  /**
   * @class LandmarksRegistrar openma/body/landmarksregistrar.h
   * @brief Store landmarks' label for registration procedure
   * This class stores landmarks' label. Its main aim is to be used with methods that requires to known automatically what markers to extract for specific operations (e.g. least square pose estimation).
   * @note The given labels must correspond to the internal name used by OpenMA.
   * @ingroup openma_body
   */
  
  /**
   * Constructor. Create a node with the given @a name and store the @a conversionTable.
   */
  LandmarksRegistrar::LandmarksRegistrar(const std::string& name, const std::vector<std::string>& labels, Node* parent)
  : Node(*new LandmarksRegistrarPrivate(this,name,labels), parent)
  {};
  
  /**
   * Destructor (default)
   */
  LandmarksRegistrar::~LandmarksRegistrar() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the labels stored by this registrar. 
   */
  const std::vector<std::string>& LandmarksRegistrar::labels() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Labels;
  };
  
  /**
   * Store the labels given by @a value.
   */
  void LandmarksRegistrar::setLabels(const std::vector<std::string>& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Labels == value)
      return;
    optr->Labels = value;
    this->modified();
  };
  
  /**
   *
   */
  std::vector<TimeSequence*> LandmarksRegistrar::retrieveLandmarks(LandmarksTranslator* lt, Node* node) const
  {
    auto optr = this->pimpl();
    std::vector<TimeSequence*> landmarks;
    landmarks.reserve(optr->Labels.size());
    if (lt == nullptr)
    {
      for (auto it = optr->Labels.cbegin() ; it != optr->Labels.cend() ; ++it)
      {
        auto lmk = node->findChild<TimeSequence*>(*it,{},false);
        if (lmk != nullptr)
          landmarks.push_back(lmk);
      }
    }
    else
    {
      for (auto it = optr->Labels.cbegin() ; it != optr->Labels.cend() ; ++it)
      {
        TimeSequence* lmk = nullptr;
        std::string name = lt->convertReverseIfExists(*it);
        if (!name.empty() && ((lmk = node->findChild<TimeSequence*>(name,{},false))) != nullptr)
          landmarks.push_back(lmk);
      }
    }
    return landmarks;
  };
  
  /**
   * Create a new LandmarksRegistrar object on the heap
   */
  Node* LandmarksRegistrar::allocateNew() const
  {
    return new LandmarksRegistrar(this->name(), {});
  };
  
  /**
   * Copy the content of the @a source
   */
  void LandmarksRegistrar::copyContents(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const LandmarksRegistrar*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copyContents(src);
    optr->Labels = optr_src->Labels;
  };
};
};