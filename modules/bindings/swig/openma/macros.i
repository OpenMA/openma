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

%{
  SWIGTYPE SWIG_NewNullObj()
  {
#if defined(SWIGMATLAB)
    return mxCreateDoubleMatrix(0,0,mxREAL);
#elif defined(SWIGPYTHON)
    SWIGTYPE retval = Py_None;
    Py_INCREF(retval);
    return retval;
#endif
  }
%}

%define SWIG_SetType(swigtype)
  #define SWIGTYPE swigtype
  %begin %{ #define SWIGTYPE swigtype %}
%enddef
  
#if defined(SWIGMATLAB)
  SWIG_SetType(mxArray*)
  %{ #define SWIG_SendError(code, msg) mexErrMsgIdAndTxt(SWIG_ErrorType(code), msg) %}
#elif defined(SWIGPYTHON)
  SWIG_SetType(PyObject*)
  %{ #define SWIG_SendError(code, msg) SWIG_Error(code, msg) %}
#else
  #error Unsupported language
#endif

#define SWIG_CREATE_TEMPLATE_HELPER_1(ns, cn, st) \
  %{ \
    static const ma::bindings::TemplateHelper T_##ns##_##cn = {&SWIGTYPE_p_##ns##__##cn, &cast_helper<ns::cn>, &find_child_helper<ns::cn>, &find_children_helper<ns::cn>}; \
  %} \
  %constant ma::bindings::TemplateHelper T_##cn = T_##ns##_##cn;
  
#define SWIG_CREATE_TEMPLATE_HELPER_2(ns, nns, cn, st) \
  %{ \
    static const ma::bindings::TemplateHelper T_##ns##_##nns##_##cn = {&SWIGTYPE_p_##ns##__##nns##__##cn, &cast_helper<ns::nns::cn>, &find_child_helper<ns::nns::cn>, &find_children_helper<ns::nns::cn>}; \
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
  // The result is always owned because some language check this before decrease the reference counter.
  $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $1_descriptor, 1);
  _ma_refcount_incr($1);
};

%enddef
