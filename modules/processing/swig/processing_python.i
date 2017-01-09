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

// Convert Python list to std::vector<ma::TimeSequence*>
%typemap(in) const std::vector<ma::TimeSequence*>& (std::vector<ma::TimeSequence*> temp)
{
  if (!PyList_Check($input)) {
    SWIG_exception_fail(SWIG_TypeError, "Only Python list argument is accepted in 'std::<ma::TimeSequence*> & const' typemap (in)"); 
  }
  $1 = &temp;
  Py_ssize_t numelts = PyList_Size($input);
  temp.resize(numelts);
  for (Py_ssize_t i = 0 ; i < numelts ; ++i)
  {
    void* value = nullptr;
    auto ts = PyList_GetItem($input,i);
    if (!SWIG_IsOK(SWIG_ConvertPtr(ts, &value, SWIGTYPE_p_ma__TimeSequence, 0))) {
      SWIG_exception_fail(SWIG_ValueError, "conversion failed for 'ma::TimeSequence' in 'std::<ma::TimeSequence*> & const' typemap (in)"); 
    }
    if (!value) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'ma::TimeSequence' in 'std::<ma::TimeSequence*> & const' typemap (in)"); 
    }
    temp[i] = reinterpret_cast<ma::TimeSequence*>(value);
  }
}