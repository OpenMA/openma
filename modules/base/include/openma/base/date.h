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

#ifndef __openma_base_date_h
#define __openma_base_date_h

#include "openma/base_export.h" // OPENMA_BASE_EXPORT
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT
#include "openma/base/typeid.h" // OPENMA_EXPORT_STATIC_TYPEID

namespace ma
{
  class OPENMA_BASE_EXPORT Date
  {
  public:
    Date(unsigned year = 1900, unsigned month = 01, unsigned day = 01);
    ~Date() _OPENMA_NOEXCEPT = default;
    Date(const Date& ) = default;
    Date(Date&& ) _OPENMA_NOEXCEPT;
    Date& operator=(const Date& ) = default;
    Date& operator=(Date&& ) _OPENMA_NOEXCEPT;
    
    void setYear(unsigned value) _OPENMA_NOEXCEPT;
    unsigned year() const _OPENMA_NOEXCEPT;
    
    void setMonth(unsigned value) _OPENMA_NOEXCEPT;
    unsigned month() const _OPENMA_NOEXCEPT;
    
    void setDay(unsigned value) _OPENMA_NOEXCEPT;
    unsigned day() const _OPENMA_NOEXCEPT;
    
    OPENMA_BASE_EXPORT friend bool operator==(const Date& lhs, const Date& rhs);
    OPENMA_BASE_EXPORT friend bool operator!=(const Date& lhs, const Date& rhs);
    
  private:
    unsigned m_Value;
  };
};

OPENMA_EXPORT_STATIC_TYPEID(ma::Date, OPENMA_BASE_EXPORT);

#endif // __openma_base_date_h