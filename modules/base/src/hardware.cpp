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

#include "openma/base/hardware.h"
#include "openma/base/hardware_p.h"
#include "openma/base/timesequence.h"
#include "openma/base/logger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
  HardwarePrivate::HardwarePrivate(Hardware* pint, const std::string& name, std::vector<std::string>&& labels)
  : NodePrivate(pint,name), MappedChannels()
  {
    for (const auto& label : labels)
      this->MappedChannels.emplace(label, nullptr);
  };
  
  HardwarePrivate::~HardwarePrivate() _OPENMA_NOEXCEPT = default;
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::Hardware);

namespace ma
{
  /**
   * @class Hardware openma/base/hardware.h
   * @brief
   *
   * @ingroup openma_base
   */
  
  /**
   *
   */
  Node* Hardware::channels()
  {
    auto pt = this->findChild("Channels",{},false);
    if (pt == nullptr)
      pt = new Node("Channels",this);
    return pt;
  };
  
  /**
   *
   */
  TimeSequence* Hardware::channel(const std::string& label) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    const auto it = optr->MappedChannels.find(label);
    if (it == optr->MappedChannels.cend())
      return nullptr;
    return it->second;
  };
  
  /**
   * In case no mapped channel use the given @a label, and error is sent to the logger. Thus, the channel @a sig will be not assigned.
   */
  void Hardware::setChannel(const std::string& label, TimeSequence* sig)
  {
    auto optr = this->pimpl();
    auto it = optr->MappedChannels.find(label);
    if (it != optr->MappedChannels.end())
    {
      if (it->second == sig)
        return;
      this->replaceChild(it->second, sig);
      it->second = sig;
      // this->modified(); // replaceChild call modified
    }
    else
      error("No mapped channel with the label: %s", label.c_str());
  };
  
  /**
   *
   */
  Node* Hardware::outputs()
  {
    auto pt = this->findChild("Outputs",{},false);
    if (pt == nullptr)
      pt = new Node("Outputs",this);
    return pt;
  };
  
  /**
   *
   */
  void Hardware::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Hardware*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    for(const auto channel: optr_src->MappedChannels)
      optr->MappedChannels[channel.first] = this->findChild<TimeSequence*>(channel.second->name());
  };
  
  /**
   *
   */
  Hardware::Hardware(HardwarePrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
};
