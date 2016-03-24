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

%module openma_base

%{
#include "openma/base.h"
%}

%include <std_string.i>

// ========================================================================= //
//                               UTIL FUNCTIONS
// ========================================================================= //

%{
#define _SWIG_ref_count "_SWIG_ref_count"

inline int _SWIG_ma_Node_count(ma::Node* node)
{
  return node->property(_SWIG_ref_count).cast<int>();
};

inline void _SWIG_ma_Node_reset(ma::Node* node, const ma::Any& value, bool recursive = true)
{
  if (node == nullptr) return;
  node->setProperty(_SWIG_ref_count, value);
  if (recursive)
  {
    auto& children = node->children();
    for (auto child : children)
      _SWIG_ma_Node_reset(child, value, recursive);
  }
};

inline void _SWIG_ma_Node_ref(ma::Node* node)
{
  if (node == nullptr) return;
  node->setProperty(_SWIG_ref_count, _SWIG_ma_Node_count(node) + 1);
/*  mexPrintf("_SWIG_ma_Node_ref - %s - new count: %i\n",node->name().c_str(), _SWIG_ma_Node_count(node));*/
};

inline int _SWIG_ma_Node_unref(ma::Node* node)
{
  if (node == nullptr) return 0;
  int rc = _SWIG_ma_Node_count(node);
  rc -= 1;
/*  mexPrintf("_SWIG_ma_Node_unref - %s - remaining: %i\n", node->name().c_str(), rc);*/
  if (rc <= 0)
  {
/*    mexPrintf("_SWIG_ma_Node_unref - %s - ~ma::Node\n", node->name().c_str());*/
    auto& children = node->children();
    for (auto child : children)
    {
      child->removeParent(node);
      _SWIG_ma_Node_unref(child);
    }
    delete node;
  }
  else
    node->setProperty(_SWIG_ref_count, rc);
  return rc;
};
%}

%define SWIG_TYPEMAP_OUT_CONSTRUCTOR(typename)
// If parent is passed to the constructor, then the C++ must take the ownership too.
%typemap(out, noblock=1) ma:: ## typename* ma:: ## typename:: ## typename
{
  if (!result) {
    SWIG_exception_fail(SWIG_RuntimeError, "Impossible to create or cast an object of type 'ma::typename' with given input(s)");
  }
  _out = SWIG_NewPointerObj(SWIG_as_voidptr(result), $descriptor(ma:: ## typename*), 1 | 0);
  if (result->hasParents() && (argc > 1)) _SWIG_ma_Node_ref(result);
};
/*// Same thing for the clone() method
%typemap(out, noblock=1) ma:: ## typename* ma:: ## typename::clone
{
  _out = SWIG_NewPointerObj(SWIG_as_voidptr(result), $descriptor(ma:: ## typename*), 1 | 0);
  if (result->hasParents()) _SWIG_ma_Node_ref(result);
};*/
%enddef

%define SWIG_EXTEND_CAST_CONSTRUCTOR(typename, swigtype)
%extend {
typename(swigtype* other)
{
  void* argp1 = nullptr;
  int res1 = SWIG_ConvertPtr(other, &argp1, SWIGTYPE_p_ma__Node, 0 |  0 );
  if (!SWIG_IsOK(res1))
  {
    SWIG_Error(SWIG_ArgError(res1), "in method 'new_" "typename" "', argument 1 of type 'ma::Node *'");
    return nullptr;
  }
  return ma::node_cast<ma::## typename*>(reinterpret_cast<ma::Node*>(argp1));
};
};
%enddef 

%define SWIG_EXTEND_DEEPCOPY(typename)
%extend {
void copy(ma::Node* source)
{
  ma_Node_clear($self);
  auto rc = $self->property(_SWIG_ref_count);
  $self->copy(source);
  _SWIG_ma_Node_reset($self, rc, false);
  auto& children = $self->children();
  for (auto child : children)
    _SWIG_ma_Node_reset(child,1);
};
%newobject clone;
typename* clone(Node* parent = nullptr) const
{
  ma::typename* ptr = $self->clone(parent);
  _SWIG_ma_Node_reset(ptr, 0,false); // 0: because the SWIG generated code will take the ownership
  if (parent != nullptr) _SWIG_ma_Node_ref(ptr);
  auto& children = ptr->children();
  for (auto child : children)
    _SWIG_ma_Node_reset(child,1);
  return ptr;
};
};
%enddef 

// ========================================================================= //
//                                INTERFACE
// ========================================================================= //


// Use a single threaded reference counting mechanism to release the data
%feature("ref", noblock=1) ma::Node {_SWIG_ma_Node_ref($this);};
%feature("unref", noblock=1) ma::Node {_SWIG_ma_Node_unref($this);};

#if defined(SWIGMATLAB)
%include "base_matlab.i"
#endif

%include "../include/openma/base/enums.h"

%include "base/any.i"
%include "base/object.i"
%include "base/node.i"
%include "base/event.i"
%include "base/subject.i"
%include "base/timesequence.i"
%include "base/trial.i"