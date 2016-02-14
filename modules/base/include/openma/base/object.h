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

#ifndef __openma_base_object_h
#define __openma_base_object_h

#include "openma/base_export.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT
#include "openma/base/opaque.h" // OPENMA_DECLARE_PIMPL_ACCESSOR

#include <memory> // std::unique_ptr

namespace ma
{
  class ObjectPrivate;
  
  class OPENMA_BASE_EXPORT Object
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Object)
    
  public:
    virtual ~Object() _OPENMA_NOEXCEPT;
    
    Object(const Object& ) = delete;
    Object(Object&& ) _OPENMA_NOEXCEPT = delete;
    Object& operator=(const Object& ) = delete;
    Object& operator=(Object&& ) _OPENMA_NOEXCEPT = delete;
    
    unsigned long timestamp() const _OPENMA_NOEXCEPT;
    
    virtual void modified() _OPENMA_NOEXCEPT;
    
  protected:
    Object();
    Object(ObjectPrivate& pimpl) _OPENMA_NOEXCEPT;
    
    void setTimestamp(unsigned long ts);
   
    std::unique_ptr<ObjectPrivate> mp_Pimpl;
  };
};

#endif // __openma_base_object_h
