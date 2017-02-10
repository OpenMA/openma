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

%include "openma/common.swg"

%fragment("OpenMA", "header", fragment="OpenMA_ToList")
%{
%};

%fragment("OpenMA_ToList", "header")
%{
  template <typename T>
  PyObject* to_list(const std::vector<T*>* in, swig_type_info* type)
  {
    PyObject* out = PyList_New(in->size());
    if (out != nullptr)
    {
      for (size_t i = 0, len = in->size() ; i < len ; ++i)
      {
        PyObject* elt = SWIG_NewPointerObj(SWIG_as_voidptr(in->operator[](i)), type, 1 |  0 );
        if (elt == nullptr) break;
        _ma_refcount_incr(in->operator[](i));
        PyList_SetItem(out, i, elt);
      }
    }
    return out;
  };
%}

// ------------------------------------------------------------------------- //
//                                 TYPEMAPS
// ------------------------------------------------------------------------- //

// Convert std::vector<Node*> to Python list
%typemap(out, fragment="OpenMA", noblock=1) const std::vector<ma::Node*>&
{
  $result = to_list($1, $descriptor(ma::Node*));
};

// Convert Python list to std::vector<unsigned int>
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) const std::vector<unsigned int>&
{
  $1 = PyList_Check($input);
  if ($1)
  {
    Py_ssize_t numelts = PyList_Size($input);
    for (auto i = 0 ; i < numelts ; ++i)
    {
      auto value = PyList_GetItem($input,i);
      if ((PyInt_Check(value) == 0) && (PyLong_Check(value) == 0) && (PyFloat_Check(value) == 0))
      {
        $1 = 0;
        break;
      }
    }
  }
};
%typemap(in) const std::vector<unsigned int>& (std::vector<unsigned int> temp)
{
  $1 = &temp;
  Py_ssize_t numelts = PyList_Size($input);
  temp.resize(numelts);
  for (Py_ssize_t i = 0 ; i < numelts ; ++i)
  {
    double iptr = 0.;
    double d = PyFloat_AsDouble(PyList_GetItem($input,i));
    if (modf(d,&iptr) >= std::numeric_limits<double>::epsilon())
    {
      SWIG_exception_fail(SWIG_ValueError, "input must be integers in 'const std::vector<unsigned int>&' typemap (in)"); 
      break;
    }
    temp[i] = static_cast<unsigned>(d);
  }
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
      SWIG_exception_fail(SWIG_ArgError(sres1), "conversion failed for 'std::string' in 'std::unordered_map<std::string, ma::Any> &&' typemap (in)"); 
    }
    if (!key) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'std::string' in 'std::unordered_map<std::string, ma::Any> &&' typemap (in)"); 
    }
    tkey = *key;
    if (SWIG_IsNewObj(sres1)) delete key;
    // Value (ma::Any)
    void* value = nullptr;
    if (SWIG_IsOK(SWIG_ConvertPtr(PyList_GetItem(pair,1), &value, SWIGTYPE_p_ma__Any, 0)))
    {
      if (!value) {
        SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'ma::Any' in 'std::unordered_map<std::string, ma::Any> &&' typemap (in)"); 
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

// Convert std::unordered_map<std::string, ma::Any> to Python cell
%typemap(out, noblock=1) std::unordered_map<std::string, ma::Any>
{
  $result = PyList_New($1.size());
  if ($result != nullptr)
  {
    auto it = $1.cbegin();
    for (size_t i = 0, len = $1.size() ; i < len ; ++i)
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

// Convert Python list to std::unordered_map<std::string, std::string>
%typemap(typecheck, noblock=1) std::unordered_map<std::string, std::string> const &
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
       || (PyString_Check(PyList_GetItem(list,1)) == 0)
      )
      {
        $1 = 0;
        break;
      }
    }
  }
}
%typemap(in) std::unordered_map<std::string, std::string> const & (std::unordered_map<std::string, std::string> temp)
{
  $1 = &temp;
  std::string tkey, tvalue;
  Py_ssize_t numelts = PyList_Size($input);
  for (Py_ssize_t i = 0 ; i < numelts ; ++i)
  {
    auto pair = PyList_GetItem($input,i);
    // Key
    std::string* key = nullptr;
    int sres1 = SWIG_AsPtr_std_string(PyList_GetItem(pair,0), &key);
    if (!SWIG_IsOK(sres1)) {
      SWIG_exception_fail(SWIG_ArgError(sres1), "conversion failed for 'std::string' in 'std::unordered_map<std::string, std::string> const &' typemap (in)"); 
    }
    if (!key) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'std::string' in 'std::unordered_map<std::string, std::string> const &' typemap (in)"); 
    }
    tkey = *key;
    if (SWIG_IsNewObj(sres1)) delete key;
    // Value 
    std::string* value = nullptr;
    int sres2 = SWIG_AsPtr_std_string(PyList_GetItem(pair,1), &value);
    if (!SWIG_IsOK(sres2)) {
      SWIG_exception_fail(SWIG_ArgError(sres2), "conversion failed for 'std::string' in 'std::unordered_map<std::string, std::string> const &' typemap (in)"); 
    }
    if (!value) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'std::string' in 'std::unordered_map<std::string, std::string> const &' typemap (in)"); 
    }
    tvalue = *value;
    if (SWIG_IsNewObj(sres2)) delete value;
    // Assign
    $1->emplace(tkey, tvalue);
  }
}

// Convert std::array<unsigned,2>& to Python list
%typemap(out, noblock=1) std::array<int,2> const &
{
  $result = PyList_New(2);
  PyList_SetItem($result, 0, PyInt_FromLong($1->operator[](0)));
  PyList_SetItem($result, 1, PyInt_FromLong($1->operator[](1)));
}; 

// Convert Python list to std::array<int,2>
%typemap(in) std::array<int,2> const & (std::array<int,2> temp)
{
  bool ok = PyList_Check($input) && (PyList_Size($input) == 2);
  if (ok)  
  {
    for (Py_ssize_t i = 0 ; i < 2 ; ++i)
    {
      auto value = PyList_GetItem($input,i);
      if ((PyInt_Check(value) == 0) && (PyLong_Check(value) == 0) && (PyFloat_Check(value) == 0))
      {
        ok = false;
        break;
      }
    }
  }
  if (!ok)
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of 2 numbers in 'const std::array<int,2>&' typemap (in)");
  temp[0] = static_cast<int>(PyInt_AsLong(PyList_GetItem($input,0)));
  temp[1] = static_cast<int>(PyInt_AsLong(PyList_GetItem($input,1)));
  $1 = &temp;
};

// Convert std::array<unsigned,2>& to Python list
%typemap(out, noblock=1) std::array<unsigned,2> const &
{
  $result = PyList_New(2);
  PyList_SetItem($result, 0, PyInt_FromLong($1->operator[](0)));
  PyList_SetItem($result, 1, PyInt_FromLong($1->operator[](1)));
}; 

// Convert std::array<double,3> to Python list
%typemap(out, noblock=1) std::array<double,3> const &
{
  $result = PyList_New(3);
  PyList_SetItem($result, 0, PyFloat_FromDouble($1->operator[](0)));
  PyList_SetItem($result, 1, PyFloat_FromDouble($1->operator[](1)));
  PyList_SetItem($result, 2, PyFloat_FromDouble($1->operator[](2)));
};

// Convert Python list to std::array<double,3>
%typemap(in) std::array<double,3> const & (std::array<double,3> temp)
{
  bool ok = PyList_Check($input) && (PyList_Size($input) == 3);
  if (ok)  
  {
    for (Py_ssize_t i = 0 ; i < 3 ; ++i)
    {
      auto value = PyList_GetItem($input,i);
      if ((PyInt_Check(value) == 0) && (PyLong_Check(value) == 0) && (PyFloat_Check(value) == 0))
      {
        ok = false;
        break;
      }
    }
  }
  if (!ok)
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of 3 numbers in 'const std::array<double,3>&' typemap (in)");
  temp[0] = PyFloat_AsDouble(PyList_GetItem($input,0));
  temp[1] = PyFloat_AsDouble(PyList_GetItem($input,1));
  temp[2] = PyFloat_AsDouble(PyList_GetItem($input,2));
  $1 = &temp;
};

// Convert std::array<double,12> to Python list
%typemap(out, noblock=1) std::array<double,12> const &
{
  $result = PyList_New(12);
  PyList_SetItem($result,  0, PyFloat_FromDouble($1->operator[]( 0)));
  PyList_SetItem($result,  1, PyFloat_FromDouble($1->operator[]( 1)));
  PyList_SetItem($result,  2, PyFloat_FromDouble($1->operator[]( 2)));
  PyList_SetItem($result,  3, PyFloat_FromDouble($1->operator[]( 3)));
  PyList_SetItem($result,  4, PyFloat_FromDouble($1->operator[]( 4)));
  PyList_SetItem($result,  5, PyFloat_FromDouble($1->operator[]( 5)));
  PyList_SetItem($result,  6, PyFloat_FromDouble($1->operator[]( 6)));
  PyList_SetItem($result,  7, PyFloat_FromDouble($1->operator[]( 7)));
  PyList_SetItem($result,  8, PyFloat_FromDouble($1->operator[]( 8)));
  PyList_SetItem($result,  9, PyFloat_FromDouble($1->operator[]( 9)));
  PyList_SetItem($result, 10, PyFloat_FromDouble($1->operator[](10)));
  PyList_SetItem($result, 11, PyFloat_FromDouble($1->operator[](11)));
};

%typemap(in) std::array<double,12> const & (std::array<double,12> temp)
{
  bool ok = PyList_Check($input) && (PyList_Size($input) == 12);
  if (ok)  
  {
    for (Py_ssize_t i = 0 ; i < 12 ; ++i)
    {
      auto value = PyList_GetItem($input,i);
      if ((PyInt_Check(value) == 0) && (PyLong_Check(value) == 0) && (PyFloat_Check(value) == 0))
      {
        ok = false;
        break;
      }
    }
  }
  if (!ok)
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of 12 numbers in 'const std::array<double,12>&' typemap (in)"); 
  temp[ 0] = PyFloat_AsDouble(PyList_GetItem($input, 0));
  temp[ 1] = PyFloat_AsDouble(PyList_GetItem($input, 1));
  temp[ 2] = PyFloat_AsDouble(PyList_GetItem($input, 2));
  temp[ 3] = PyFloat_AsDouble(PyList_GetItem($input, 3));
  temp[ 4] = PyFloat_AsDouble(PyList_GetItem($input, 4));
  temp[ 5] = PyFloat_AsDouble(PyList_GetItem($input, 5));
  temp[ 6] = PyFloat_AsDouble(PyList_GetItem($input, 6));
  temp[ 7] = PyFloat_AsDouble(PyList_GetItem($input, 7));
  temp[ 8] = PyFloat_AsDouble(PyList_GetItem($input, 8));
  temp[ 9] = PyFloat_AsDouble(PyList_GetItem($input, 9));
  temp[10] = PyFloat_AsDouble(PyList_GetItem($input,10));
  temp[11] = PyFloat_AsDouble(PyList_GetItem($input,11));
  $1 = &temp;
};

// Convert const std::vector<double>& to Python list
%typemap(out, noblock=1) std::vector<double> const &
{
  $result = PyList_New($1->size());
  for (unsigned i = 0, len = $1->size(); i < len; ++i)
    PyList_SetItem($result, i, PyFloat_FromDouble($1->operator[](i)));
};

// Convert Python list to std::vector<double>
%typemap(in) std::vector<double> const & (std::vector<double> temp)
{
  bool ok = PyList_Check($input);
  if (ok)
  {
    for (Py_ssize_t i = 0, numelts = PyList_Size($input) ; i < numelts ; ++i)
    {
      auto value = PyList_GetItem($input,i);
      if ((PyInt_Check(value) == 0) && (PyLong_Check(value) == 0) && (PyFloat_Check(value) == 0))
      {
        ok = false;
        break;
      }
    }
  }
  if (!ok)
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of numbers in 'const std::double<double>&' typemap (in)"); 
  Py_ssize_t numelts = PyList_Size($input);
  temp.resize(numelts);
  for (Py_ssize_t i = 0 ; i < numelts ; ++i)
    temp[i] = PyFloat_AsDouble(PyList_GetItem($input, i));
  $1 = &temp;
};

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

