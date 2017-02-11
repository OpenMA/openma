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

#ifndef __openma_body_eulerdescriptor_h
#define __openma_body_eulerdescriptor_h

#include "openma/body/descriptor.h"

#include <array>

namespace ma
{
namespace body
{
  class EulerDescriptorPrivate;
  
  class OPENMA_BODY_EXPORT EulerDescriptor : public Descriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(EulerDescriptor, Descriptor)
        
  public:
#if defined(_MSC_VER) && (_MSC_VER < 1900)
    static _OPENMA_CONSTEXPR std::array<int,3> XYZ;
    static _OPENMA_CONSTEXPR std::array<int,3> YZX;
    static _OPENMA_CONSTEXPR std::array<int,3> ZXY;
    static _OPENMA_CONSTEXPR std::array<int,3> XZY;
    static _OPENMA_CONSTEXPR std::array<int,3> ZYX;
    static _OPENMA_CONSTEXPR std::array<int,3> YXZ;
    static _OPENMA_CONSTEXPR std::array<int,3> ZXZ;
    static _OPENMA_CONSTEXPR std::array<int,3> XYX;
    static _OPENMA_CONSTEXPR std::array<int,3> YZY;
    static _OPENMA_CONSTEXPR std::array<int,3> ZYZ;
    static _OPENMA_CONSTEXPR std::array<int,3> XZX;
    static _OPENMA_CONSTEXPR std::array<int,3> YXY;
#else
    // Tait-Bryan (Cardan) angles
    static _OPENMA_CONSTEXPR std::array<int,3> XYZ{{0,1,2}};
    static _OPENMA_CONSTEXPR std::array<int,3> YZX{{1,2,0}};
    static _OPENMA_CONSTEXPR std::array<int,3> ZXY{{2,0,1}};
    static _OPENMA_CONSTEXPR std::array<int,3> XZY{{0,2,1}};
    static _OPENMA_CONSTEXPR std::array<int,3> ZYX{{2,1,0}};
    static _OPENMA_CONSTEXPR std::array<int,3> YXZ{{1,0,2}};
    // Proper Euler angles
    static _OPENMA_CONSTEXPR std::array<int,3> ZXZ{{2,0,2}};
    static _OPENMA_CONSTEXPR std::array<int,3> XYX{{0,1,0}};
    static _OPENMA_CONSTEXPR std::array<int,3> YZY{{1,2,1}};
    static _OPENMA_CONSTEXPR std::array<int,3> ZYZ{{2,1,2}};
    static _OPENMA_CONSTEXPR std::array<int,3> XZX{{0,2,0}};
    static _OPENMA_CONSTEXPR std::array<int,3> YXY{{1,0,1}};
#endif
    
    EulerDescriptor(const std::string& name, const std::array<int,3>& sequence, Node* parent = nullptr);
    EulerDescriptor(const std::string& name, const std::array<int,3>& sequence, const std::array<double,3>& scale, Node* parent = nullptr);
    EulerDescriptor(const std::string& name, const std::array<int,3>& sequence, const std::array<double,3>& scale, const std::array<double,3>& offset, Node* parent = nullptr);
    ~EulerDescriptor() _OPENMA_NOEXCEPT;
    
    EulerDescriptor(const EulerDescriptor& ) = delete;
    EulerDescriptor(EulerDescriptor&& ) _OPENMA_NOEXCEPT = delete;
    EulerDescriptor& operator=(const EulerDescriptor& ) = delete;
    EulerDescriptor& operator=(EulerDescriptor&& ) _OPENMA_NOEXCEPT = delete;
    
    void configure(const std::array<int,3>& sequence, const std::array<double,3>& scale = {{1.,1.,1.}}, const std::array<double,3>& offset = {{0.,0.,0.}}) _OPENMA_NOEXCEPT;
    
    virtual EulerDescriptor* clone(Node* parent = nullptr) const override;
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
    
  protected:
    virtual bool prepare(const Node* input, const std::unordered_map<std::string, Any>& options) override;
    virtual bool process(const std::unordered_map<std::string, Any>& options) override;
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
    
  private:
    EulerDescriptor(const std::string& name);
    EulerDescriptor(DescriptorPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT;
  };
};
};

OPENMA_EXPORT_STATIC_TYPEID(ma::body::EulerDescriptor, OPENMA_BODY_EXPORT);

#endif // __openma_body_eulerdescriptor_h
