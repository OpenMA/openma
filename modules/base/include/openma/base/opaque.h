/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
 * All rights reserved.
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

#ifndef __openma_base_opaque_h
#define __openma_base_opaque_h
  
/**
 * Creates pimpl() methods in the public interface to access to the private part (opaque pointer)
 * @relates ma::Object
 * @ingroup openma_base
 */
#define OPENMA_DECLARE_PIMPL_ACCESSOR(classname) \
  friend class classname##Private; \
  inline classname##Private* pimpl() {return reinterpret_cast<classname##Private*>(this->mp_Pimpl.get());}; \
  inline const classname##Private* pimpl() const {return reinterpret_cast<const classname##Private*>(this->mp_Pimpl.get());};
  
 /**
  * Creates pint() methods in the private implementation to access to the public part
  * @relates ma::Object
  * @ingroup openma_base
  */
#define OPENMA_DECLARE_PINT_ACCESSOR(classname) \
  inline classname* pint() {return reinterpret_cast<classname*>(this->mp_Pint);}; \
  inline const classname* pint() const {return reinterpret_cast<const classname*>(this->mp_Pint);};
    
#endif // __openma_base_opaque_h
