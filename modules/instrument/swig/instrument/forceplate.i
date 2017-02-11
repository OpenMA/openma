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

namespace ma
{
namespace instrument
{
  SWIG_TYPEMAP_NODE_OUT(ma::instrument, ForcePlate)
  SWIG_CREATE_TEMPLATE_HELPER_2(ma, instrument, ForcePlate, SWIGTYPE)
  
  %nodefaultctor;
  class ForcePlate : public Hardware
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(ma::instrument, ForcePlate, SWIGTYPE)
    
    %extend {
    enum class Type : int {
      Unknown = ForcePlate::Unknown,
      // Type1 = ForcePlate::Type1,
      // Raw9x6 = ForcePlate::Raw9x6,
      Type2 = ForcePlate::Type2,
      Raw6x6 = ForcePlate::Raw6x6,
      Type3 = ForcePlate::Type3,
      Raw6x8 = ForcePlate::Raw6x8,
      Type4 = ForcePlate::Type4,
      Cal6x6 = ForcePlate::Cal6x6,
      Type5 = ForcePlate::Type5,
      Cal6x8 = ForcePlate::Cal6x8,
      // Type6 = ForcePlate::Type6,
      // Cal12x12 = ForcePlate::Cal12x12,
      // Type7 = ForcePlate::Type7,
      // Poly8x8 = ForcePlate::Poly8x8,
      // Type11 = ForcePlate::Type11,
      // KistlerSplitBeltTreadmill = ForcePlate::KistlerSplitBeltTreadmill,
      // Type12 = ForcePlate::Type12,
      // KistlerGaitwayTreadmill = ForcePlate::KistlerGaitwayTreadmill,
      // Type21 = ForcePlate::Type21,
      // AmtiStairway = ForcePlate::AmtiStairway
    };
    };
    
    ~ForcePlate();
    
    int type() const;
    void setGeometry(const std::array<double,3>& rso, const std::array<double,3>& sc1, const std::array<double,3>& sc2, const std::array<double,3>& sc3, const std::array<double,3>& sc4);
    const std::array<double,12>& referenceFrame() const;
    const std::array<double,12>& surfaceCorners() const;
    const std::array<double,3>& relativeSurfaceOrigin() const;
    const std::array<unsigned,2>& calibrationMatrixDimensions() const;
    const std::vector<double>& calibrationMatrixData() const;
    void setCalibrationMatrixData(const std::vector<double>& value);
    void setSoftResetEnabled(bool value);
    bool isSoftResetEnabled() const;
    void setSoftResetSamples(const std::array<int,2>& value);
    const std::array<int,2>& softResetSamples() const;
    
    TimeSequence* wrench(ma::instrument::Location loc, bool global = true, double threshold = 10.0, double rate = -1.0);

  };
  %clearnodefaultctor;
};
};