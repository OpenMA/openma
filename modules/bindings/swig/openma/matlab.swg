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
  template<typename T>
  mxArray* to_double_matrix(const std::vector<T>* in)
  {
    mxArray* out = mxCreateDoubleMatrix(in->empty() ? 0 : 1, in->size(), mxREAL);
    if (out != nullptr)
    {
      double* data = mxGetPr(out);
      for (size_t i = 0, len = in->size() ; i < len ; ++i)
        data[i] = in->operator[](i);
    }
    return out;
  };
%};

%fragment("OpenMA_ToList", "header")
%{
  template <typename T>
  mxArray* to_list(const std::vector<T*>* in, swig_type_info* type)
  {
    mxArray* out = mxCreateCellMatrix(in->empty() ? 0 : 1, in->size());
    if (out != nullptr)
    {
      for (size_t i = 0, len = in->size() ; i < len ; ++i)
      {
        mxArray* elt = SWIG_NewPointerObj(SWIG_as_voidptr(in->operator[](i)), type, 1 |  0 );
        if (elt == nullptr) break;
        _ma_refcount_incr(in->operator[](i));
        mxSetCell(out, i, elt);
      }
    }
    return out;
  };
%}

// ------------------------------------------------------------------------- //
//                                 TYPEMAPS
// ------------------------------------------------------------------------- //

// Convert std::vector<Node*> to MATLAB cell array
%typemap(out, fragment="OpenMA", noblock=1) const std::vector<ma::Node*>&
{
  $result = to_list($1, $descriptor(ma::Node*));
};

// Convert MATLAB double matrix to std::vector<unsigned int>
%typemap(typecheck, precedence=SWIG_TYPECHECK_DOUBLE_ARRAY, noblock=1) const std::vector<unsigned int>&
{
  mxClassID id = mxGetClassID($input);
  $1 = (id == mxDOUBLE_CLASS)
    || (id == mxINT8_CLASS)
    || (id == mxUINT8_CLASS)
    || (id == mxINT16_CLASS)
    || (id == mxUINT16_CLASS)
    || (id == mxINT32_CLASS)
    || (id == mxUINT32_CLASS)
    || (id == mxINT64_CLASS)
    || (id == mxUINT64_CLASS)
    || (id == mxSINGLE_CLASS);
};
%typemap(in) const std::vector<unsigned int>& (std::vector<unsigned int> temp)
{
  $1 = &temp;  
  mwSize numelts = mxGetNumberOfElements($input);
  mxClassID id = mxGetClassID($input);
  void* data = mxGetData($input);
  temp.resize(numelts);
  for (mwSize i = 0 ; i < numelts ; ++i)
  {
    double iptr = 0.;
    double d = 0.;
    switch(id)
    {
    case mxDOUBLE_CLASS:
      d = static_cast<double*>(data)[i];
      break;
    case mxINT8_CLASS:
      d = static_cast<double>(static_cast<int8_t*>(data)[i]);
      break;
    case mxUINT8_CLASS:
      d = static_cast<double>(static_cast<uint8_t*>(data)[i]);
      break;
    case mxINT16_CLASS:
      d = static_cast<double>(static_cast<int16_t*>(data)[i]);
      break;
    case mxUINT16_CLASS:
      d = static_cast<double>(static_cast<uint16_t*>(data)[i]);
      break;
    case mxINT32_CLASS:
      d = static_cast<double>(static_cast<int32_t*>(data)[i]);
      break;
    case mxUINT32_CLASS:
      d = static_cast<double>(static_cast<uint32_t*>(data)[i]);
      break;
    case mxINT64_CLASS:
      d = static_cast<double>(static_cast<int64_t*>(data)[i]);
      break;
    case mxUINT64_CLASS:
      d = static_cast<double>(static_cast<uint64_t*>(data)[i]);
      break;
    case mxSINGLE_CLASS:
      d = static_cast<double>(static_cast<float*>(data)[i]);
      break;
    default:
      SWIG_exception_fail(SWIG_RuntimeError, "Unexpected class ID. Please, report this error");
    }
    
    if (modf(d,&iptr) >= std::numeric_limits<double>::epsilon())
    {
      SWIG_exception_fail(SWIG_ValueError, "size inputs must be integers in 'const std::vector<unsigned int>&' typemap (in)"); 
      break;
    }
    temp[i] = static_cast<unsigned>(d);
  }
};

// Convert std::vector<unsigned> to MATLAB double matrix
%typemap(out, fragment="OpenMA", noblock=1) std::vector<unsigned>
{
  $result = to_double_matrix(&$1);
};

// Convert const std::vector<unsigned>& to MATLAB double matrix
%typemap(out, fragment="OpenMA", noblock=1) const std::vector<unsigned>&
{
  $result = to_double_matrix($1);
};

// Convert std::array<double,2> to MATLAB double matrix
%typemap(out, noblock=1) std::array<double,2>&
{
  $result = mxCreateDoubleMatrix(1,2,mxREAL);
  if ($result != nullptr)
  {
    double* data = mxGetPr($result);
    data[0] = (*$1)[0];
    data[1] = (*$1)[1];
  };
};

// Convert MATLAB double matrix to std::array<double,2>
%typemap(typecheck, noblock=1) std::array<double,2>&
{
  $1 = (mxGetClassID($input) == mxDOUBLE_CLASS ? 1 : 0) && (mxGetNumberOfElements($input) == 2);
};
%typemap(in) std::array<double,2>& (std::array<double,2> temp)
{
  $1 = &temp;
  double* data = mxGetPr($input);
  temp[0] = data[0];
  temp[1] = data[1];
};

// Convert unsigned to Matlab double scalar
%typemap(out, noblock=1) unsigned
{
  $result = mxCreateDoubleScalar($1);
};

// Convert size_t to Matlab double scalar
%typemap(out, noblock=1) size_t
{
  $result = mxCreateDoubleScalar($1);
};

// Convert MATLAB cell to std::unordered_map<std::string, ma::Any>
%typemap(typecheck, noblock=1) std::unordered_map<std::string, ma::Any>&&
{
  $1 = (mxGetClassID($input) == mxCELL_CLASS ? 1 : 0);
  if ($1)
  {
    mwSize numelts = mxGetNumberOfElements($input);
    for (mwSize i = 0 ; i < numelts ; ++i)
    {
      auto cell =  mxGetCell($input,i);
      if ((mxGetClassID(cell) != mxCELL_CLASS)
       || (mxGetNumberOfElements(cell) != mwSize(2))
       || (mxGetClassID(mxGetCell(cell,0)) != mxCHAR_CLASS)
       || (mxGetClassID(mxGetCell(cell,1)) == mxUNKNOWN_CLASS))
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
  mwSize numelts = mxGetNumberOfElements($input);
  for (mwSize i = 0 ; i < numelts ; ++i)
  {
    auto pair = mxGetCell($input,i);
    // Key
    std::string* key = nullptr;
    int sres1 = SWIG_AsPtr_std_string(mxGetCell(pair,0), &key);
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
    if (SWIG_IsOK(SWIG_ConvertPtr(mxGetCell(pair,1), &value, SWIGTYPE_p_ma__Any, 0)))
    {
      if (!value) {
        SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'ma::Any' in 'std::unordered_map<std::string, ma::Any> const &' typemap (in)"); 
      }
      $1->emplace(tkey, (ma::Any const &)*reinterpret_cast<ma::Any*>(value));
    }
    // Value (other)
    else
    {
      ma_Any_assign(&tvalue, mxGetCell(pair,1));
      $1->emplace(tkey, tvalue);
    }
  }
}

// Convert std::unordered_map<std::string, ma::Any> to MATLAB cell
%typemap(out, noblock=1) std::unordered_map<std::string, ma::Any>
{
  $result = mxCreateCellMatrix(1, $1.size());
  if ($result != nullptr)
  {
    auto it = $1.cbegin();
    for (size_t i = 0, len = $1.size() ; i < len ; ++i)
    {
      // Pair
      mxArray* pair = mxCreateCellMatrix(1, 2);
      if (pair == nullptr) break;
      mxSetCell($result, i, pair);
      // Associated key
      mxArray* key = SWIG_From_std_string(it->first);
      if (key == nullptr) break;
      mxSetCell(pair, 0, key);
      // Associated value
      mxArray* value = SWIG_NewPointerObj((new ma::Any(it->second)), SWIGTYPE_p_ma__Any, SWIG_POINTER_OWN | 0 );
      if (value == nullptr) break;
      mxSetCell(pair, 1, value);
      // Increment the map iterator
      ++it;
    }
  };
};

// Convert MATLAB cell to std::unordered_map<std::string, std::string>
%typemap(typecheck, noblock=1) std::unordered_map<std::string, std::string> const &
{
  $1 = (mxGetClassID($input) == mxCELL_CLASS ? 1 : 0);
  if ($1)
  {
    mwSize numelts = mxGetNumberOfElements($input);
    for (mwSize i = 0 ; i < numelts ; ++i)
    {
      auto cell =  mxGetCell($input,i);
      if ((mxGetClassID(cell) != mxCELL_CLASS)
       || (mxGetNumberOfElements(cell) != mwSize(2))
       || (mxGetClassID(mxGetCell(cell,0)) != mxCHAR_CLASS)
       || (mxGetClassID(mxGetCell(cell,1)) != mxCHAR_CLASS))
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
  mwSize numelts = mxGetNumberOfElements($input);
  for (mwSize i = 0 ; i < numelts ; ++i)
  {
    auto pair = mxGetCell($input,i);
    // Key
    std::string* key = nullptr;
    int sres1 = SWIG_AsPtr_std_string(mxGetCell(pair,0), &key);
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
    int sres2 = SWIG_AsPtr_std_string(mxGetCell(pair,1), &value);
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

// Convert Python list to std::vector<ma::TimeSequence*>
%typemap(in) const std::vector<ma::TimeSequence*>& (std::vector<ma::TimeSequence*> temp)
{
  if (mxGetClassID($input) != mxCELL_CLASS) {
    SWIG_exception_fail(SWIG_TypeError, "Only Matlab cell argument is accepted in 'std::<ma::TimeSequence*> & const' typemap (in)"); 
  }
  $1 = &temp;
  mwSize numelts = mxGetNumberOfElements($input);
  temp.resize(numelts);
  for (mwSize i = 0 ; i < numelts ; ++i)
  {
    void* value = nullptr;
    auto ts = mxGetCell($input,i);
    if (!SWIG_IsOK(SWIG_ConvertPtr(ts, &value, SWIGTYPE_p_ma__TimeSequence, 0))) {
      SWIG_exception_fail(SWIG_ValueError, "conversion failed for 'ma::TimeSequence' in 'std::<ma::TimeSequence*> & const' typemap (in)"); 
    }
    if (!value) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'ma::TimeSequence' in 'std::<ma::TimeSequence*> & const' typemap (in)"); 
    }
    temp[i] = reinterpret_cast<ma::TimeSequence*>(value);
  }
}