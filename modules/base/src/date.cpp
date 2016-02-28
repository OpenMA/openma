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

#include "openma/base/date.h"
#include "openma/base/logger.h"

namespace ma
{
  /**
   * @class
   * @brief Integer representation of a year, month, and day.
   */
  
  /**
   *
   */
  Date::Date(unsigned year, unsigned month, unsigned day)
  : m_Value()
  {
    this->setYear(year);
    this->setMonth(month);
    this->setDay(day);
  };

  /**
   *
   */    
  void Date::setYear(unsigned value) _OPENMA_NOEXCEPT
  {
    this->m_Value = (this->m_Value & 0x0000ffff) | (value << 16);
  };
  
  /**
   *
   */
  unsigned Date::year() const _OPENMA_NOEXCEPT
  {
    return (this->m_Value & 0xffff0000) >> 16;
  };
  
  /**
   *
   */
  void Date::setMonth(unsigned value) _OPENMA_NOEXCEPT
  {
    if (value < 1)
    {
      warning("Value out of the acceptable range [1-12]. The value is set to 1.");
      value = 1;
    }
    else if (value > 12)
    {
      warning("Value out of the acceptable range [1-12]. The value is set to 12.");
      value = 12;
    }
    this->m_Value = (this->m_Value & 0xffff00ff) | (value << 8);
  };
  
  /**
   *
   */
  unsigned Date::month() const _OPENMA_NOEXCEPT
  {
    return (this->m_Value & 0x0000ff00) >> 8;
  };
  
  /**
   *
   */
  void Date::setDay(unsigned value) _OPENMA_NOEXCEPT
  {
    if (value < 1)
    {
      warning("Value out of the acceptable range [1-31]. The value is set to 1.");
      value = 1;
    }
    else if (value > 31)
    {
      warning("Value out of the acceptable range [1-31]. The value is set to 31.");
      value = 31;
    }
    this->m_Value = (this->m_Value & 0xffffff00) | value;
  };
  
  /**
   *
   */
  unsigned Date::day() const _OPENMA_NOEXCEPT
  {
    return (this->m_Value & 0x000000ff);
  };
  
  bool operator==(const Date& lhs, const Date& rhs)
  {
    return (lhs.m_Value == rhs.m_Value);
  };
  
  bool operator!=(const Date& lhs, const Date& rhs)
  {
    return !(lhs == rhs);
  };
};