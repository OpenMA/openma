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

#ifndef __openma_body_dynamicdescriptor_h
#define __openma_body_dynamicdescriptor_h

#include "openma/body/descriptor.h"

#include <array>

namespace ma
{
namespace body
{
  class DynamicDescriptorPrivate;
  
  class OPENMA_BODY_EXPORT DynamicDescriptor : public Descriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(DynamicDescriptor)
    OPENMA_DECLARE_NODEID(DynamicDescriptor, Descriptor)
        
  public:
    DynamicDescriptor(Node* parent = nullptr);
    DynamicDescriptor(const std::array<unsigned,9>& order, const std::array<double,9>& scale, Node* parent = nullptr);
    ~DynamicDescriptor() _OPENMA_NOEXCEPT;
    
    DynamicDescriptor(const DynamicDescriptor& ) = delete;
    DynamicDescriptor(DynamicDescriptor&& ) _OPENMA_NOEXCEPT = delete;
    DynamicDescriptor& operator=(const DynamicDescriptor& ) = delete;
    DynamicDescriptor& operator=(DynamicDescriptor&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual DynamicDescriptor* clone(Node* parent = nullptr) const override;
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
    
  protected:
    virtual bool prepare(const Node* input, const std::unordered_map<std::string, Any>& options) final;
    virtual bool process(const std::unordered_map<std::string, Any>& options) final;
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) final;
    
  private:
    DynamicDescriptor(const std::string& name);
    DynamicDescriptor(DescriptorPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT;
  };
};
};

OPENMA_EXPORT_STATIC_TYPEID(ma::body::DynamicDescriptor, OPENMA_BODY_EXPORT);

#endif // __openma_body_dynamicdescriptor_h
