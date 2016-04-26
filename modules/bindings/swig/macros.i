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

%define SWIG_SetType(swigtype)
  #define SWIGTYPE swigtype
  %begin %{ #define SWIGTYPE swigtype %}
%enddef
  
#if defined(SWIGMATLAB)
  %{ #define SWIG_SendError(code, msg) mexErrMsgIdAndTxt(SWIG_ErrorType(code), msg) %}
#elif defined(SWIGPYTHON)
  %{ #define SWIG_SendError(code, msg) SWIG_Error(code, msg) %}
#else
  #error Unsupported language
#endif

#define SWIG_CREATE_TEMPLATE_HELPER_1(ns, cn, st) \
  %{ \
    static const ma::bindings::TemplateHelper T_##ns##_##cn = {&SWIGTYPE_p_##ns##__##cn, &find_child_helper<ns::cn>, &find_children_helper<ns::cn>}; \
  %} \
  %constant ma::bindings::TemplateHelper T_##cn = T_##ns##_##cn;
  
#define SWIG_CREATE_TEMPLATE_HELPER_2(ns, nns, cn, st) \
  %{ \
    static const ma::bindings::TemplateHelper T_##ns##_##nns##_##cn = {&SWIGTYPE_p_##ns##__##nns##__##cn, &find_child_helper<ns::nns::cn>, &find_children_helper<ns::nns::cn>}; \
  %} \
  %constant ma::bindings::TemplateHelper T_##cn = T_##ns##_##nns##_##cn;

%define SWIG_TYPEMAP_NODE_OUT(nspace, cname)
// Need to verify if the generated object is not null before being added in the workspace.
%typemap(out, noblock=1) nspace:: ## cname* nspace:: ## cname:: ## cname
{
  if (!$1) {
    SWIG_exception_fail(SWIG_RuntimeError, "Impossible to create or cast an object of type 'nspace::cname' with given input(s)");
  }
  $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $1_descriptor, $owner);
};
%typemap(out, noblock=1) nspace:: ## cname*
{
  if ($1 == nullptr) SWIG_fail;
  $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $1_descriptor, $owner);
};

%enddef

%define SWIG_EXTEND_CAST_CONSTRUCTOR(nspace, cname, swigtype)
%extend {
cname(swigtype* other)
{
  void* argp1 = nullptr;
  int res1 = SWIG_ConvertPtr(other, &argp1, SWIGTYPE_p_ma__Node, 0 |  0 );
  if (!SWIG_IsOK(res1))
  {
    SWIG_Error(SWIG_ArgError(res1), "in method 'new_" "cname" "', argument 1 of type 'ma::Node *'");
    return nullptr;
  }
  return ma::node_cast<nspace::## cname*>(reinterpret_cast<ma::Node*>(argp1));
};
};
%enddef 

%define SWIG_EXTEND_DEEPCOPY(nspace, cname)
%extend {
void copy(ma::Node* source)
{
  _ma_clear_node($self);
  int count = _ma_refcount_get(source);
  $self->copy(source);
  _ma_refcount_reset($self, count, false);
  auto& children = $self->children();
  for (auto child : children)
    _ma_refcount_reset(child, 0);
};
%newobject clone;
nspace::## cname* clone(Node* parent = nullptr) const
{
  nspace::## cname* ptr = $self->clone(parent);
  _ma_refcount_reset(ptr, -1, false); // -1: because the SWIG generated code will take the ownership
  if (parent != nullptr) _ma_refcount_incr(ptr);
  auto& children = ptr->children();
  for (auto child : children)
    _ma_refcount_reset(child, 0);
  return ptr;
};
};
%enddef 