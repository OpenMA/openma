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

// Specific code for Python (typemaps, etc.)

%{
#include <numpy/arrayobject.h>
%}
%include "openmapython.swg"
%fragment("OpenMA_TemplateHelper");

// Convert std::vector<Node*> to Python list
%typemap(out, fragment="OpenMA", noblock=1) const std::vector<ma::Node*>&
{
  $result = to_list($1, $descriptor(ma::Node*));
};

// Convert std::vector<unsigned> to Python list
%typemap(out, noblock=1) std::vector<unsigned>
{
  $result = PyList_New($1.size());
  for (int i = 0, len = $1.size(); i < len; ++i)
    PyList_SetItem($result, i, PyInt_FromLong($1[i]));
};

// Convert const std::vector<unsigned>& to Python list
%typemap(out, noblock=1) const std::vector<unsigned>&
{
  $result = PyList_New($1->size());
  for (int i = 0, len = $1->size(); i < len; ++i)
    PyList_SetItem($result, i, PyInt_FromLong($1->operator[](i)));
};

// Convert std::array<double,2> to Python list
%typemap(out, noblock=1) std::array<double,2>&
{
  $result = PyList_New(2);
  PyList_SetItem($result, 0, PyFloat_FromDouble($1->operator[](0)));
  PyList_SetItem($result, 1, PyFloat_FromDouble($1->operator[](1)));
};

// Convert Python list to std::array<double,2>
%typemap(typecheck, noblock=1) std::array<double,2>&
{
  $1 = PyList_Check($input) && (PyList_Size($input) == 2) && PyFloat_Check(PyList_GetItem($input,0)) && PyFloat_Check(PyList_GetItem($input,1));
};
%typemap(in) std::array<double,2>& (std::array<double,2> temp)
{
  $1 = &temp;
  temp[0] = PyFloat_AsDouble(PyList_GetItem($input,0));
  temp[1] = PyFloat_AsDouble(PyList_GetItem($input,1));
};

// Convert Python list to std::unordered_map<std::string, ma::Any>
%typemap(typecheck, noblock=1) std::unordered_map<std::string, ma::Any>&&
{
  $1 = PyList_Check($input);
  if ($1)
  {
    Py_ssize_t numelts = PyList_Size($input);
    for (Py_ssize_t i = 0 ; i < numelts ; ++i)
    {
      auto list = PyList_GetItem($input,i);
      if ((!PyList_Check(list))
       || (PyList_Size(list) != Py_ssize_t(2))
       || (PyString_Check(PyList_GetItem(list,0)) == 0)
/*       || (???(PyList_GetItem(list,1)) == 0)*/
      )
      {
        $1 = 0;
        break;
      }
    }
  }
}
%typemap(in) std::unordered_map<std::string, ma::Any>&& (std::unordered_map<std::string, ma::Any> temp)
{
  $1 = &temp;
  std::string tkey;
  ma::Any tvalue;
  Py_ssize_t numelts = PyList_Size($input);
  for (Py_ssize_t i = 0 ; i < numelts ; ++i)
  {
    auto pair = PyList_GetItem($input,i);
    // Key
    std::string* key = nullptr;
    int sres1 = SWIG_AsPtr_std_string(PyList_GetItem(pair,0), &key);
    if (!SWIG_IsOK(sres1)) {
      SWIG_exception_fail(SWIG_ArgError(sres1), "conversion failed for 'std::string' in 'std::unordered_map<std::string, ma::Any> const &' typemap (in)"); 
    }
    if (!key) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'std::string' in 'std::unordered_map<std::string, ma::Any> const &' typemap (in)"); 
    }
    tkey = *key;
    if (SWIG_IsNewObj(sres1)) delete key;
    // Value (ma::Any)
    void* value = nullptr;
    if (SWIG_IsOK(SWIG_ConvertPtr(PyList_GetItem(pair,1), &value, SWIGTYPE_p_ma__Any, 0)))
    {
      if (!value) {
        SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'ma::Any' in 'std::unordered_map<std::string, ma::Any> const &' typemap (in)"); 
      }
      $1->emplace(tkey, (ma::Any const &)*reinterpret_cast<ma::Any*>(value));
    }
    // Value (other)
    else
    {
      ma_Any_assign(&tvalue, PyList_GetItem(pair,1));
      $1->emplace(tkey, tvalue);
    }
  }
}

// Convert std::unordered_map<std::string, ma::Any> to MATLAB cell
%typemap(out, noblock=1) const std::unordered_map<std::string, ma::Any>&
{
  $result = PyList_New($1->size());
  if ($result != nullptr)
  {
    auto it = $1->cbegin();
    for (size_t i = 0, len = $1->size() ; i < len ; ++i)
    {
      // Pair
      PyObject* pair = PyList_New(2);
      if (pair == nullptr) break;
      PyList_SetItem($result, i, pair);
      // Associated key
      PyObject* key = SWIG_From_std_string(it->first);
      if (key == nullptr) break;
      PyList_SetItem(pair, 0, key);
      // Associated value
      PyObject* value = SWIG_NewPointerObj((new ma::Any(it->second)), SWIGTYPE_p_ma__Any, SWIG_POINTER_OWN | 0 );
      if (value == nullptr) break;
      PyList_SetItem(pair, 1, value);
      // Increment the map iterator
      ++it;
    }
  };
};


//-------------------------------------------------------------------------- //
//                                     Any
//-------------------------------------------------------------------------- //

%{
  
PyObject* _ma_Any_cast_generate_list(std::vector<PyObject*>* data, const std::vector<unsigned>& dims, unsigned idx)
{
  PyObject* ptr = PyList_New(dims[idx]);
  if (idx < (dims.size()-1))
  {
    for (Py_ssize_t i = 0 ; i < static_cast<Py_ssize_t>(dims[idx]) ; ++i)
      PyList_SetItem(ptr, i, _ma_Any_cast_generate_list(data, dims, idx+1));
  }
  else
  {
    data->push_back(ptr);
  }
  return ptr;
}

template <typename T>
PyObject* _ma_Any_cast(const ma::Any* self, PyObject*(*func)(T))
{
  PyObject* ptr = nullptr;
  size_t numelts = self->size();
  if (numelts <= 1)
  {
    ptr = func(self->cast<T>());
  }
  else
  {
    const auto& dims = self->dimensions();
    size_t numlists = numelts / dims.back();
    std::vector<PyObject*> data(numlists, nullptr);
    ptr = _ma_Any_cast_generate_list(&data, dims, 0);
    for (size_t i = 0 ; i < numlists ; ++i)
      for (size_t j = 0, len = dims.back() ; j < len ; ++j)
        PyList_SetItem(data[i], j, func(self->cast<T>(((i-1) * len) + j)));
  }
  return ptr;
};

inline PyObject* PyString_FromStdString(std::string v)
{
  return PyString_FromStringAndSize(v.c_str(), static_cast<Py_ssize_t>(v.size()));
};

PyObject* ma_Any_cast(const ma::Any* self, const char* type)
{
  PyObject* ptr = NULL;
  if (strcmp(type,"bool") == 0)
  {
    ptr = _ma_Any_cast<long>(self, &PyBool_FromLong);
  }
  else if (strcmp(type,"int") == 0)
  {
    ptr = _ma_Any_cast<long>(self, &PyInt_FromLong);
  }
  else if (strcmp(type,"long") == 0)
  {
    ptr = _ma_Any_cast<long>(self, &PyLong_FromLong);
  }
  else if (strcmp(type,"float") == 0)
  {
    ptr = _ma_Any_cast<double>(self, &PyFloat_FromDouble);
  }
  else if (strcmp(type,"string") == 0)
  {
    ptr = _ma_Any_cast<std::string>(self, &PyString_FromStdString);
  }
  else
    PyErr_Format(PyExc_ValueError,"Unsupported data type for conversion: '%s'.", type);
    
  return ptr;
}

PyObject* ma_Any_cast(const ma::Any* self)
{
  return ma_Any_cast(self, "double");
};

// ------------------------------------------------------------------------- //

void ma_Any_assign(ma::Any* self, const PyObject* value)
{
  if (PyList_Check(value))
  {
    PyErr_SetString(PyExc_NotImplementedError, "The support of list is not implemented.");
  }
  else
  {
    PyObject* _value = const_cast<PyObject*>(value);
    if (PyBool_Check(_value))
      self->assign(PyObject_IsTrue(_value) == 1);
    else if (PyInt_Check(_value))
      self->assign(PyInt_AsLong(_value));
    else if (PyLong_Check(_value))
      self->assign(PyLong_AsLong(_value));
    else if (PyFloat_Check(_value))
      self->assign(PyFloat_AsDouble(_value));
    else if (PyString_Check(_value))
    {
      char* buffer = nullptr;
      Py_ssize_t length = 0; 
      PyString_AsStringAndSize(_value, &buffer, &length);
      self->assign(std::string(buffer,static_cast<size_t>(length)));
    }
    else
      PyErr_SetString(PyExc_TypeError, "Unsupported type");
  }
};

%};

//-------------------------------------------------------------------------- //
//                                     Node
//-------------------------------------------------------------------------- //

%{
PyObject* ma_Node_findChild(const ma::Node* self, const ma::bindings::TemplateHelper* id, const std::string& name = std::string(), std::unordered_map<std::string,ma::Any>&& properties = std::unordered_map<std::string,ma::Any>(), bool recursiveSearch = true)
{
  PyObject* out = nullptr;
  id->findChild(&out, *(id->SwigType), self, name, std::move(properties), recursiveSearch);
  if (out == nullptr)
    PyErr_SetString(PyExc_ValueError,"No child found");
  return out;
};

PyObject* ma_Node_findChildren(const ma::Node* self, const ma::bindings::TemplateHelper* id, const std::string& regexp = ".*", std::unordered_map<std::string,ma::Any>&& properties = std::unordered_map<std::string,ma::Any>(), bool recursiveSearch = true)
{
  PyObject* out = nullptr;
  id->findChildren(&out, *(id->SwigType), self, regexp, std::move(properties), recursiveSearch);
  if (out == nullptr)
    PyErr_SetString(PyExc_SystemError,"Internal error during list allocation");
  return out;
};

%};

//-------------------------------------------------------------------------- //
//                                  TimeSequence
//-------------------------------------------------------------------------- //

%{

PyObject* ma_TimeSequence_data(const ma::TimeSequence* self)
{
  std::vector<npy_intp> dims;
  dims.reserve(self->dimensions().size());
  dims.insert(dims.begin(), self->samples());
  dims.insert(dims.end(), self->dimensions().cbegin(), self->dimensions().cend());
  PyObject* out = PyArray_SimpleNew(static_cast<int>(dims.size()), dims.data(), NPY_DOUBLE);
  double* dataout = (double*)PyArray_DATA(out);
  memcpy(dataout, self->data(), self->elements()*sizeof(double));
  return out;
};
  
void ma_TimeSequence_setData(ma::TimeSequence* self, const PyObject* data)
{
  if (!PyArray_Check(data) || (PyArray_TYPE(data) != NPY_DOUBLE))
    PyErr_SetString(PyExc_TypeError, "The given input is not an NumPy array composed of floating point elements");
  const int numdimsin = PyArray_NDIM(data)-1;
  const auto& dimsout = self->dimensions();
  if (static_cast<size_t>(numdimsin) != dimsout.size())
    PyErr_SetString(PyExc_ValueError, "Incompatible number of dimensions");
  const npy_intp* dimsin = PyArray_DIMS(data);
  if (self->samples() != dimsin[0])
    PyErr_SetString(PyExc_ValueError, "Incompatible number of samples");
  for (size_t i = 0 ; i < static_cast<size_t>(numdimsin) ; ++i)
  {
    if (dimsin[i+1] != dimsout[i])
      PyErr_SetString(PyExc_ValueError, "Incompatible dimension value");
  }
  memcpy(self->data(), (double*)PyArray_DATA(data), self->elements()*sizeof(double));
  self->modified();
};
  
%}