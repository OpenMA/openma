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

#ifndef __openma_base_trial_h
#define __openma_base_trial_h

#include "openma/base/node.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

namespace ma
{
  class TimeSequence;
  class Event;
  
  class TrialPrivate;
  
  class OPENMA_BASE_EXPORT Trial : public Node
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Trial)
    OPENMA_DECLARE_NODEID(Trial, Node)
    
  public:
    Trial(const std::string& name, Node* parent = nullptr);
    ~Trial() _OPENMA_NOEXCEPT;
    
    Trial(const Node& ) = delete;
    Trial(Node&& ) _OPENMA_NOEXCEPT = delete;
    Trial& operator=(const Trial& ) = delete;
    Trial& operator=(Trial&& ) _OPENMA_NOEXCEPT = delete;
    
    Node* timeSequences();
    TimeSequence* timeSequence(unsigned idx) _OPENMA_NOEXCEPT;
      
    Node* events();
    Event* event(unsigned idx) _OPENMA_NOEXCEPT;
    
    virtual Trial* clone(Node* parent = nullptr) const override;
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
  };
};

OPENMA_EXPORT_STATIC_TYPEID(ma::Trial, OPENMA_BASE_EXPORT);

#endif // __openma_base_trial_h
