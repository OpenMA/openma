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

// Specific code for Matlab (typemaps, etc.)

// Convert std::array<unsigned,2>& to Matlab matrix
%typemap(out, noblock=1) std::array<int,2> const &
{
  $result = mxCreateDoubleMatrix(1,2,mxREAL);
  if ($result != nullptr)
  {
    double* data = mxGetPr($result);
    data[0] = (*$1)[0];
    data[1] = (*$1)[1];
  };
}; 

// Convert Matlab matrix to std::array<double,3>
%typemap(in) std::array<int,2> const & (std::array<int,2> temp)
{
  mxClassID id = mxGetClassID($input);
  if ( (mxGetNumberOfElements($input) != 2)
    && (id != mxDOUBLE_CLASS)
    && (id != mxINT8_CLASS)
    && (id != mxUINT8_CLASS)
    && (id != mxINT16_CLASS)
    && (id != mxUINT16_CLASS)
    && (id != mxINT32_CLASS)
    && (id != mxUINT32_CLASS)
    && (id != mxINT64_CLASS)
    && (id != mxUINT64_CLASS)
    && (id != mxSINGLE_CLASS) )
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of 2 numbers in 'const std::array<int,2>&' typemap (in)");   
  void* data = mxGetData($input);
  for (mwSize i = 0 ; i < 3 ; ++i)
  {
    switch(id)
    {
    case mxDOUBLE_CLASS:
      temp[i] = static_cast<int>(static_cast<double*>(data)[i]);
      break;
    case mxINT8_CLASS:
      temp[i] = static_cast<int>(static_cast<int8_t*>(data)[i]);
      break;
    case mxUINT8_CLASS:
      temp[i] = static_cast<int>(static_cast<uint8_t*>(data)[i]);
      break;
    case mxINT16_CLASS:
      temp[i] = static_cast<int>(static_cast<int16_t*>(data)[i]);
      break;
    case mxUINT16_CLASS:
      temp[i] = static_cast<int>(static_cast<uint16_t*>(data)[i]);
      break;
    case mxINT32_CLASS:
      temp[i] = static_cast<int>(static_cast<int32_t*>(data)[i]);
      break;
    case mxUINT32_CLASS:
      temp[i] = static_cast<int>(static_cast<uint32_t*>(data)[i]);
      break;
    case mxINT64_CLASS:
      temp[i] = static_cast<int>(static_cast<int64_t*>(data)[i]);
      break;
    case mxUINT64_CLASS:
      temp[i] = static_cast<int>(static_cast<uint64_t*>(data)[i]);
      break;
    case mxSINGLE_CLASS:
      temp[i] = static_cast<int>(static_cast<float*>(data)[i]);
      break;
    default:
      SWIG_exception_fail(SWIG_RuntimeError, "Unexpected class ID. Please, report this error");
    }
  }
  $1 = &temp;
};

// Convert std::array<unsigned,2>& to Matlab matrix
%typemap(out, noblock=1) std::array<unsigned,2> const &
{
  $result = mxCreateDoubleMatrix(1,2,mxREAL);
  if ($result != nullptr)
  {
    double* data = mxGetPr($result);
    data[0] = (*$1)[0];
    data[1] = (*$1)[1];
  };
}; 

// Convert std::array<double,3> to Matlab matrix
%typemap(out, noblock=1) std::array<double,3> const &
{
  $result = mxCreateDoubleMatrix(1,3,mxREAL);
  if ($result != nullptr)
  {
    double* data = mxGetPr($result);
    data[0] = (*$1)[0];
    data[1] = (*$1)[1];
    data[2] = (*$1)[2];
  };
};

// Convert Matlab matrix to std::array<double,3>
%typemap(in) std::array<double,3> const & (std::array<double,3> temp)
{
  mxClassID id = mxGetClassID($input);
  if ( (mxGetNumberOfElements($input) != 3)
    && (id != mxDOUBLE_CLASS)
    && (id != mxINT8_CLASS)
    && (id != mxUINT8_CLASS)
    && (id != mxINT16_CLASS)
    && (id != mxUINT16_CLASS)
    && (id != mxINT32_CLASS)
    && (id != mxUINT32_CLASS)
    && (id != mxINT64_CLASS)
    && (id != mxUINT64_CLASS)
    && (id != mxSINGLE_CLASS) )
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of 3 numbers in 'const std::array<double,3>&' typemap (in)");   
  void* data = mxGetData($input);
  for (mwSize i = 0 ; i < 3 ; ++i)
  {
    switch(id)
    {
    case mxDOUBLE_CLASS:
      temp[i] = static_cast<double*>(data)[i];
      break;
    case mxINT8_CLASS:
      temp[i] = static_cast<double>(static_cast<int8_t*>(data)[i]);
      break;
    case mxUINT8_CLASS:
      temp[i] = static_cast<double>(static_cast<uint8_t*>(data)[i]);
      break;
    case mxINT16_CLASS:
      temp[i] = static_cast<double>(static_cast<int16_t*>(data)[i]);
      break;
    case mxUINT16_CLASS:
      temp[i] = static_cast<double>(static_cast<uint16_t*>(data)[i]);
      break;
    case mxINT32_CLASS:
      temp[i] = static_cast<double>(static_cast<int32_t*>(data)[i]);
      break;
    case mxUINT32_CLASS:
      temp[i] = static_cast<double>(static_cast<uint32_t*>(data)[i]);
      break;
    case mxINT64_CLASS:
      temp[i] = static_cast<double>(static_cast<int64_t*>(data)[i]);
      break;
    case mxUINT64_CLASS:
      temp[i] = static_cast<double>(static_cast<uint64_t*>(data)[i]);
      break;
    case mxSINGLE_CLASS:
      temp[i] = static_cast<double>(static_cast<float*>(data)[i]);
      break;
    default:
      SWIG_exception_fail(SWIG_RuntimeError, "Unexpected class ID. Please, report this error");
    }
  }
  $1 = &temp;
};

// Convert std::array<double,12> to Matlab matrix
%typemap(out, noblock=1) std::array<double,12> const &
{
  $result = mxCreateDoubleMatrix(1,12,mxREAL);
  if ($result != nullptr)
  {
    double* data = mxGetPr($result);
    data[ 0] = (*$1)[ 0];
    data[ 1] = (*$1)[ 1];
    data[ 2] = (*$1)[ 2];
    data[ 3] = (*$1)[ 3];
    data[ 4] = (*$1)[ 4];
    data[ 5] = (*$1)[ 5];
    data[ 6] = (*$1)[ 6];
    data[ 7] = (*$1)[ 7];
    data[ 8] = (*$1)[ 8];
    data[ 9] = (*$1)[ 9];
    data[10] = (*$1)[10];
    data[11] = (*$1)[11];
  };
};

%typemap(in) std::array<double,12> const & (std::array<double,12> temp)
{
  mxClassID id = mxGetClassID($input);
  if ( (mxGetNumberOfElements($input) != 12)
    && (id != mxDOUBLE_CLASS)
    && (id != mxINT8_CLASS)
    && (id != mxUINT8_CLASS)
    && (id != mxINT16_CLASS)
    && (id != mxUINT16_CLASS)
    && (id != mxINT32_CLASS)
    && (id != mxUINT32_CLASS)
    && (id != mxINT64_CLASS)
    && (id != mxUINT64_CLASS)
    && (id != mxSINGLE_CLASS) )
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of 12 numbers in 'const std::array<double,12>&' typemap (in)");   
  void* data = mxGetData($input);
  for (mwSize i = 0 ; i < 12 ; ++i)
  {
    switch(id)
    {
    case mxDOUBLE_CLASS:
      temp[i] = static_cast<double*>(data)[i];
      break;
    case mxINT8_CLASS:
      temp[i] = static_cast<double>(static_cast<int8_t*>(data)[i]);
      break;
    case mxUINT8_CLASS:
      temp[i] = static_cast<double>(static_cast<uint8_t*>(data)[i]);
      break;
    case mxINT16_CLASS:
      temp[i] = static_cast<double>(static_cast<int16_t*>(data)[i]);
      break;
    case mxUINT16_CLASS:
      temp[i] = static_cast<double>(static_cast<uint16_t*>(data)[i]);
      break;
    case mxINT32_CLASS:
      temp[i] = static_cast<double>(static_cast<int32_t*>(data)[i]);
      break;
    case mxUINT32_CLASS:
      temp[i] = static_cast<double>(static_cast<uint32_t*>(data)[i]);
      break;
    case mxINT64_CLASS:
      temp[i] = static_cast<double>(static_cast<int64_t*>(data)[i]);
      break;
    case mxUINT64_CLASS:
      temp[i] = static_cast<double>(static_cast<uint64_t*>(data)[i]);
      break;
    case mxSINGLE_CLASS:
      temp[i] = static_cast<double>(static_cast<float*>(data)[i]);
      break;
    default:
      SWIG_exception_fail(SWIG_RuntimeError, "Unexpected class ID. Please, report this error");
    }
  }
  $1 = &temp;
};

// Convert const std::vector<double>& to Matlab matrix
%typemap(out, fragment="OpenMA", noblock=1) std::vector<double> const &
{
  $result = to_double_matrix($1);
};

// Convert Matlab matrix to std::vector<double>
%typemap(in) std::vector<double> const & (std::vector<double> temp)
{
  mxClassID id = mxGetClassID($input);
  if ( (id != mxDOUBLE_CLASS)
    && (id != mxINT8_CLASS)
    && (id != mxUINT8_CLASS)
    && (id != mxINT16_CLASS)
    && (id != mxUINT16_CLASS)
    && (id != mxINT32_CLASS)
    && (id != mxUINT32_CLASS)
    && (id != mxINT64_CLASS)
    && (id != mxUINT64_CLASS)
    && (id != mxSINGLE_CLASS) )
    SWIG_exception_fail(SWIG_ValueError, "input must be a list of numbers in 'const std::vector<double>&' typemap (in)");   
  void* data = mxGetData($input);
  mwSize numelts = mxGetNumberOfElements($input);
  temp.resize(numelts);
  for (mwSize i = 0 ; i < numelts ; ++i)
  {
    switch(id)
    {
    case mxDOUBLE_CLASS:
      temp[i] = static_cast<double*>(data)[i];
      break;
    case mxINT8_CLASS:
      temp[i] = static_cast<double>(static_cast<int8_t*>(data)[i]);
      break;
    case mxUINT8_CLASS:
      temp[i] = static_cast<double>(static_cast<uint8_t*>(data)[i]);
      break;
    case mxINT16_CLASS:
      temp[i] = static_cast<double>(static_cast<int16_t*>(data)[i]);
      break;
    case mxUINT16_CLASS:
      temp[i] = static_cast<double>(static_cast<uint16_t*>(data)[i]);
      break;
    case mxINT32_CLASS:
      temp[i] = static_cast<double>(static_cast<int32_t*>(data)[i]);
      break;
    case mxUINT32_CLASS:
      temp[i] = static_cast<double>(static_cast<uint32_t*>(data)[i]);
      break;
    case mxINT64_CLASS:
      temp[i] = static_cast<double>(static_cast<int64_t*>(data)[i]);
      break;
    case mxUINT64_CLASS:
      temp[i] = static_cast<double>(static_cast<uint64_t*>(data)[i]);
      break;
    case mxSINGLE_CLASS:
      temp[i] = static_cast<double>(static_cast<float*>(data)[i]);
      break;
    default:
      SWIG_exception_fail(SWIG_RuntimeError, "Unexpected class ID. Please, report this error");
    }
  }
  $1 = &temp;
};