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

%include "openmamatlab.swg"
%fragment("OpenMA_TemplateHelper");

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


//-------------------------------------------------------------------------- //
//                                     Any
//-------------------------------------------------------------------------- //

%{

template <typename T>
void _ma_Any_cast(mxArray** out, const ma::Any* in)
{
  if (*out == nullptr) 
    mexErrMsgIdAndTxt("SWIG:Any:cast","Cannot allocate pointer");
  size_t numelts = mxGetNumberOfElements(*out);
  T* data = (T*)mxGetData(*out);
  for (size_t i = 0 ; i < numelts ; ++i)
    data[i] = in->cast<T>(i);
};

template <>
void _ma_Any_cast<std::string>(mxArray** out, const ma::Any* in)
{
  if (in->size() <= 1)
  {
    *out = mxCreateString(in->cast<std::string>().c_str());
    if (*out == nullptr) 
      mexErrMsgIdAndTxt("SWIG:Any:cast","Cannot allocate pointer");
  }
  else
  {
    const auto& dimensions = in->dimensions(); // NOTE: dimensions() returns a temporary vector
    mwSize ndim = static_cast<mwSize>(dimensions.size());
    std::vector<mwSize> dims(dimensions.cbegin(), dimensions.cend());
    *out = mxCreateCellArray(ndim, dims.data());
    if (*out == nullptr) 
      mexErrMsgIdAndTxt("SWIG:Any:cast","Cannot allocate cell pointer");
    for (size_t i = 0, len = in->size() ; i < len ; ++i)
    {
      mxArray* elt = mxCreateString(in->cast<std::string>(i).c_str());
      if (elt == nullptr) 
        mexErrMsgIdAndTxt("SWIG:Any:cast","Cannot allocate string pointer");
      mxSetCell(*out, i, elt);
    }
  }
};

mxArray* ma_Any_cast(const ma::Any* self, const char* type)
{
  mxArray* ptr = nullptr;
  const auto& dimensions = self->dimensions(); // NOTE: dimensions() returns a temporary vector
  mwSize ndim = static_cast<mwSize>(dimensions.size());
  std::vector<mwSize> dims(dimensions.cbegin(), dimensions.cend());
  // The minimum number of dimensions in Matlab is 2
  if (ndim < 2)
  {
    ndim = 2;
    while (dims.size() < 2)
      dims.insert(dims.begin(), 1);
  }
  if (strcmp(type,"char") == 0)
  {
    _ma_Any_cast<std::string>(&ptr, self);
  }
  else if (strcmp(type,"logical") == 0)
  {
    ptr = mxCreateLogicalArray(ndim, dims.data());
    _ma_Any_cast<bool>(&ptr, self);
  }
  else if (strcmp(type,"int8") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxINT8_CLASS, mxREAL);
    _ma_Any_cast<int8_t>(&ptr, self);
  }
  else if (strcmp(type,"uint8") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxUINT8_CLASS, mxREAL);
    _ma_Any_cast<uint8_t>(&ptr, self);
  }
  else if (strcmp(type,"int16") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxINT16_CLASS, mxREAL);
    _ma_Any_cast<int16_t>(&ptr, self);
  }
  else if (strcmp(type,"uint16") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxUINT16_CLASS, mxREAL);
    _ma_Any_cast<uint16_t>(&ptr, self);
  }
  else if (strcmp(type,"int32") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxINT32_CLASS, mxREAL);
    _ma_Any_cast<int32_t>(&ptr, self);
  }
  else if (strcmp(type,"uint32") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxUINT32_CLASS, mxREAL);
    _ma_Any_cast<uint32_t>(&ptr, self);
  }
  else if (strcmp(type,"int64") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxINT64_CLASS, mxREAL);
    _ma_Any_cast<int64_t>(&ptr, self);
  }
  else if (strcmp(type,"uint64") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxUINT64_CLASS, mxREAL);
    _ma_Any_cast<uint64_t>(&ptr, self);
  }
  else if (strcmp(type,"single") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxSINGLE_CLASS, mxREAL);
    _ma_Any_cast<float>(&ptr, self);
  }
  else if (strcmp(type,"double") == 0)
  {
    ptr = mxCreateNumericArray(ndim, dims.data(), mxDOUBLE_CLASS, mxREAL);
    _ma_Any_cast<double>(&ptr, self);
  }
  else
  {
    mexErrMsgIdAndTxt("SWIG:Any:cast","Unsupported data type for conversion: '%s'.", type);
  }
  return ptr;
}

mxArray* ma_Any_cast(const ma::Any* self)
{
  return ma_Any_cast(self, "double");
};

// ------------------------------------------------------------------------- //

template <typename T>
void _ma_Any_assign(ma::Any* out, const mxArray* in)
{
  size_t numelts = mxGetNumberOfElements(in);
  auto dataout = std::vector<T>(numelts);
  T* datain = (T*)mxGetData(in);
  memcpy(dataout.data(), datain, numelts*sizeof(T));
  const mwSize numdims = mxGetNumberOfDimensions(in);
  const mwSize* dims = mxGetDimensions(in);
  auto dimsout = std::vector<size_t>(dims, dims+numdims);
  out->assign(dataout, dimsout);
};

template <>
void _ma_Any_assign<bool>(ma::Any* out, const mxArray* in)
{
  size_t numelts = mxGetNumberOfElements(in);
  auto dataout = std::vector<bool>(numelts);
  mxLogical* datain = mxGetLogicals(in);
  for (size_t i = 0 ; i < numelts ; ++i)
    dataout[i] = datain[i];
  const mwSize numdims = mxGetNumberOfDimensions(in);
  const mwSize* dims = mxGetDimensions(in);
  auto dimsout = std::vector<size_t>(dims, dims+numdims);
  out->assign(dataout, dimsout);
};

template <>
void _ma_Any_assign<std::string>(ma::Any* out, const mxArray* in)
{
  if (mxGetM(in) > 1)
    mexErrMsgIdAndTxt("SWIG:Any:assign","Only single string is supported. Use a cell of strings to store multiple strings");
  size_t num = mxGetN(in)+1;
  char* buffer = new char[num];
  if (mxGetString(in, buffer, num) != 0)
  {
    delete[] buffer;
    mexErrMsgIdAndTxt("SWIG:Any:assign","Internal error during string extraction");
  }
  std::string dataout(buffer, num);
  delete[] buffer;
  out->assign(dataout);
};

void ma_Any_assign(ma::Any* self, const mxArray* value)
{
  switch (mxGetClassID(value))
  {
  case mxLOGICAL_CLASS:
    _ma_Any_assign<bool>(self, value);
    break;
  case mxCHAR_CLASS:
    _ma_Any_assign<std::string>(self, value);
    break;
  case mxDOUBLE_CLASS:
    _ma_Any_assign<double>(self, value);
    break;
  case mxSINGLE_CLASS:
    _ma_Any_assign<float>(self, value);
    break;
  case mxINT8_CLASS:
    _ma_Any_assign<int8_t>(self, value);
    break;
  case mxUINT8_CLASS:
    _ma_Any_assign<uint8_t>(self, value);
    break;
  case mxINT16_CLASS:
    _ma_Any_assign<int16_t>(self, value);
    break;
  case mxUINT16_CLASS:
    _ma_Any_assign<uint16_t>(self, value);
    break;
  case mxINT32_CLASS:
  _ma_Any_assign<int32_t>(self, value);
    break;
  case mxUINT32_CLASS:
    _ma_Any_assign<uint32_t>(self, value);
    break;
  case mxINT64_CLASS:
    _ma_Any_assign<int64_t>(self, value);
    break;
  case mxUINT64_CLASS:
    _ma_Any_assign<uint64_t>(self, value);
    break;
  case mxCELL_CLASS:
    {
    // Check if is a cell of strings only
    mwSize numelts = mxGetNumberOfElements(value);
    for (size_t i = 0 ; i < numelts ; ++i)
    {
      auto cell = mxGetCell(value, i);
      if ((mxGetClassID(cell) != mxCHAR_CLASS) || mxGetM(cell) != 1)
        mexErrMsgIdAndTxt("SWIG:Any:assign","The use of cells is supported to store single string only");
    }
    auto dataout = std::vector<std::string>(numelts);
    for (size_t i = 0 ; i < numelts ; ++i)
    {
      auto cell = mxGetCell(value, i);
      size_t len = mxGetN(cell)+1;
      char* buffer = new char[len];
      if (mxGetString(cell, buffer, len) != 0)
      {
        delete[] buffer;
        mexErrMsgIdAndTxt("SWIG:Any:assign","Internal error during cell string extraction");
      }
      dataout[i].assign(buffer, len);
      delete[] buffer;
    }
    const mwSize numdims = mxGetNumberOfDimensions(value);
    const mwSize* dims = mxGetDimensions(value);
    auto dimsout = std::vector<size_t>(dims, dims+numdims);
    self->assign(dataout,dimsout);
    }
    break;
  default:
    mexErrMsgIdAndTxt("SWIG:Any:assign","Only logical, numerical, and string types are supported");
  }
};

%};

//-------------------------------------------------------------------------- //
//                                  TimeSequence
//-------------------------------------------------------------------------- //

%{

mxArray* ma_TimeSequence_data(const ma::TimeSequence* self)
{
  std::vector<mwSize> dims;
  dims.reserve(self->dimensions().size());
  dims.insert(dims.begin(), self->samples());
  dims.insert(dims.end(), self->dimensions().cbegin(), self->dimensions().cend());
  mxArray* out = mxCreateNumericArray(static_cast<mwSize>(dims.size()), dims.data(), mxDOUBLE_CLASS, mxREAL);
  double* dataout = mxGetPr(out);
  memcpy(dataout, self->data(), self->elements()*sizeof(double));
  return out;
};
  
void ma_TimeSequence_setData(ma::TimeSequence* self, const mxArray* data)
{
  const size_t numeltsin = static_cast<size_t>(mxGetNumberOfElements(data));
  if (numeltsin != self->elements())
    mexErrMsgIdAndTxt("SWIG:TimeSequence:setData","Incompatible number of elements");
  const mwSize numdimsin = mxGetNumberOfDimensions(data)-1;
  const auto& dimsout = self->dimensions();
  if (static_cast<size_t>(numdimsin) != dimsout.size())
    mexErrMsgIdAndTxt("SWIG:TimeSequence:setData","Incompatible number of dimensions");
  const mwSize* dimsin = mxGetDimensions(data);
  if (self->samples() != dimsin[0])
    mexErrMsgIdAndTxt("SWIG:TimeSequence:setData","Incompatible number of samples");
  for (size_t i = 0 ; i < static_cast<size_t>(numdimsin) ; ++i)
  {
    if (dimsin[i+1] != dimsout[i])
      mexErrMsgIdAndTxt("SWIG:TimeSequence:setData","Incompatible dimension value");
  }
  memcpy(self->data(), (double*)mxGetData(data), numeltsin*sizeof(double));
  self->modified();
};
  
%}