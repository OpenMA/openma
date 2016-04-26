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

%fragment("OpenMA_TemplateHelper", "header", fragment="OpenMA_ToList")
%{
  template <typename T>
  void find_child_helper(void* out, swig_type_info* type, const ma::Node* in, const std::string& name, std::unordered_map<std::string,ma::Any>&& properties, bool recursiveSearch)
  {
    SWIGTYPE** _out = reinterpret_cast<SWIGTYPE**>(out);
    T* node = in->findChild<T*>(name, std::move(properties), recursiveSearch);
    if (node == nullptr) return;
    *_out = SWIG_NewPointerObj(SWIG_as_voidptr(node), type, 1 |  0 );
    _ma_refcount_incr(node);
  };
  
  template <typename T>
  void find_children_helper(void* out, swig_type_info* type, const ma::Node* in, const std::string& regexp, std::unordered_map<std::string,ma::Any>&& properties, bool recursiveSearch)
  {
    SWIGTYPE** _out = reinterpret_cast<SWIGTYPE**>(out);
    const std::vector<T*>& nodes = in->findChildren<T*>(std::regex(regexp), std::move(properties), recursiveSearch);
    *_out = to_list(&nodes, type);
  };
%}