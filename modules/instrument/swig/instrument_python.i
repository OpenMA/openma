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