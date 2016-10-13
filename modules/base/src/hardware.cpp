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
  : NodePrivate(pint,name), MappedChannels(labels.size())
  {
    for (size_t i = 0, len = labels.size() ; i < len ; ++i)
      this->MappedChannels[i] = std::make_pair(labels[i], nullptr);
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
   * @brief Base class for acquisition systems
   *
   * An acquisition system is defined as node with a configuration and associated time sequences.
   *
   * @ingroup openma_base
   */
  
  /**
   * Returns the node 'Channels'.
   * If the subnode does not exist, this one is created. 
   */
  Node* Hardware::channels()
  {
    auto pt = this->findChild("Channels",{},false);
    if (pt == nullptr)
      pt = new Node("Channels",this);
    return pt;
  };
  
  /**
   * Returns the @a idx mapped TimeSequence object.
   * If @a idx is out of range, the method returns nullptr.
   */
  TimeSequence* Hardware::channel(unsigned idx) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (idx >= optr->MappedChannels.size())
      return nullptr;
    return this->findChild<TimeSequence*>(optr->MappedChannels[idx].second);
  };
  
  /**
   * Associate the time sequence @a sig as the @a idx channel of this hardware.
   * @note the Hardware object comes the parent of the set time sequence @a sig. In case @a sig replace an orphan node (i.e. the last parent was this hardware), this one is deleted.
   */
  void Hardware::setChannel(unsigned idx, TimeSequence* sig)
  {
    auto optr = this->pimpl();
    if (idx >= optr->MappedChannels.size())
    {
      error("Index out of range. Impossible to assign the signal '%s' in the hardware '%s'.", (sig != nullptr ? sig->name().c_str() : "NULL"), this->name().c_str());
      return;
    }
    auto old = optr->MappedChannels[idx].second;
    if (old == sig)
      return;
    auto parent = this->channels();
    if (this->findChild(old) != nullptr)
    {
      old->removeParent(parent);
      if (!old->hasParents())
        delete old;
    }
    if (sig != nullptr)
      sig->addParent(parent);
    optr->MappedChannels[idx].second = sig;
    // this->modified(); // removeParent and addParent calls modified
  };
  
  /**
   * Convenient method to look for a channel with the name @a label.
   */
  TimeSequence* Hardware::channel(const std::string& label) const _OPENMA_NOEXCEPT
  {
    using vvt = std::pair<std::string, TimeSequence*>;
    auto optr = this->pimpl();
    const auto it = std::find_if(optr->MappedChannels.cbegin(), optr->MappedChannels.cend(), [&](const vvt& v){return v.first == label;});
    if (it == optr->MappedChannels.cend())
      return nullptr;
    return this->findChild<TimeSequence*>(it->second);
  };
  
  /**
   * Set an exist channel with the name @a label with the time sequence @a sig.
   * In case no mapped channel use the given @a label, an error is sent to the logger. Thus, the channel @a sig will be not assigned.
   */
  void Hardware::setChannel(const std::string& label, TimeSequence* sig)
  {
    using vvt = std::pair<std::string, TimeSequence*>;
    auto optr = this->pimpl();
    auto it = std::find_if(optr->MappedChannels.begin(), optr->MappedChannels.end(), [&](const vvt& v){return v.first == label;});
    if (it != optr->MappedChannels.end())
    {
      auto old = it->second;
      if (old == sig)
        return;
      auto parent = this->channels();
      if (this->findChild(old) != nullptr)
      {
        old->removeParent(parent);
        if (!old->hasParents())
          delete old;
      }
      if (sig != nullptr)
        sig->addParent(parent);
      it->second = sig;
      // this->modified(); // removeParent and addParent calls modified
    }
    else
      error("No mapped channel with the label: %s", label.c_str());
  };
  
  /**
   * Returns the number of channels required by this hardware.
   * This number corresponds to the number of mapped channels configured by an inheriting class.
   */
  unsigned Hardware::channelsNumberRequired() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return static_cast<unsigned>(optr->MappedChannels.size());
  };
  
  /**
   * Returns the node 'Channels'.
   * If the subnode does not exist, this one is created. 
   */
  Node* Hardware::outputs()
  {
    auto pt = this->findChild("Outputs",{},false);
    if (pt == nullptr)
      pt = new Node("Outputs",this);
    return pt;
  };
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  Hardware::Hardware(HardwarePrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
  /**
   * Copy the content of the @a source
   */
  void Hardware::copyContents(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Hardware*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copyContents(src);
    optr->MappedChannels.resize(optr_src->MappedChannels.size());
    for(size_t i = 0 ; i < optr_src->MappedChannels.size() ; ++i)
    {  
      const auto& ch = optr_src->MappedChannels[i];
      optr->MappedChannels[i] = std::make_pair(ch.first, this->findChild<TimeSequence*>(ch.second->name()));
    }
  };
  
};
