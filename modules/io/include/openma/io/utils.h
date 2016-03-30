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

#ifndef __openma_io_utils_h
#define __openma_io_utils_h

#include <cstring> // strrchr
#include <string>

namespace ma
{
namespace io
{
  /**
   * This function keeps only the string part after the last file separator.
   * @ingroup openma_io
   */
  inline const char* strip_path(const char* str)
  {
#if defined(_WIN32)
    const char* substr = strrchr(str, '\\');
    if (substr == nullptr)
      substr = strrchr(str, '/');
#else
    const char* substr = strrchr(str, '/');
#endif
    return (substr == nullptr) ? str : substr+1;
  };

  /**
   * This function removes the character @a c at the beginning and the end of the given string
   * By default the character removed is the white space.
   * @warning The input is directly modified and returned.
   * @ingroup openma_io
   */
  inline void trim_string(std::string* str, const char c = ' ')
  {
    *str = str->erase(str->find_last_not_of(c) + 1);
    *str = str->erase(0, str->find_first_not_of(c));
  };

  /**
   * This function removes the character @a c at the beginning and the end of the given string
   * By default the character removed is the white space.
   * @ingroup openma_io
   */
  inline std::string trim_string(const std::string& str, const char c = ' ')
  {
    std::string str_ = str;
    trim_string(&str_, c);
    return str_;
  };
};
};

#endif // __openma_io_utils_h
