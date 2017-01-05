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

%rename(registerMarkerCluster) register_marker_cluster;
%rename(extractJointKinematics) extract_joint_kinematics;
%rename(extractJointKinetics) extract_joint_kinetics;

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