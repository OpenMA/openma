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

#ifndef __openma_instrument_forceplate_h
#define __openma_instrument_forceplate_h

#include "openma/instrument_export.h"
#include "openma/instrument/enums.h"
#include "openma/base/hardware.h"

#include <vector>

namespace ma
{
namespace instrument
{
  class ForcePlatePrivate;
  
  class OPENMA_INSTRUMENT_EXPORT ForcePlate : public Hardware
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(ForcePlate)
    OPENMA_DECLARE_NODEID(ForcePlate, Hardware)
    
  public:
    typedef enum : int {
      Unknown = 0x00,
      // Type1 = 0x01,
      // Raw9x6 = Type1,
      Type2 = 0x02,
      Raw6x6 = Type2,
      // Type3 = 0x03,
      // Raw6x8 = Type3,
      Type4 = 0x04,
      Cal6x6 = Type4,
      Type5 = 0x05,
      Cal6x8 = Type5,
      // Type6 = 0x06,
      // Cal12x12 = Type6,
      // Type7 = 0x07,
      // Poly8x8 = Type7,
      // Type11 = 0x0B,
      // KistlerSplitBeltTreadmill = Type11,
      // Type12 = 0x0C,
      // KistlerGaitwayTreadmill = Type12,
      // Type21 = 0x15,
      // AmtiStairway = Type21
    } Type;
    
    ~ForcePlate() _OPENMA_NOEXCEPT;
    
    int type() const _OPENMA_NOEXCEPT;
    
    void setGeometry(const double rso[3], const double sc1[3], const double sc2[3], const double sc3[3], const double sc4[3]);
    
    const double* referenceFrame() const _OPENMA_NOEXCEPT;
    // void setReferenceFrame(double u[3], double v[3], double w[3], double o[3]);
    
    const double* surfaceCorners() const _OPENMA_NOEXCEPT;
    // void setSurfaceCorners(double c1[3], double c2[3], double c3[3], double c4[3]);
    
    const double* relativeSurfaceOrigin() const _OPENMA_NOEXCEPT;
    // void setRelativeSurfaceOrigin(double off[3]);
   
    const unsigned* calibrationMatrixDimensions() const _OPENMA_NOEXCEPT;
    double* calibrationMatrixData() const _OPENMA_NOEXCEPT;
    void setCalibrationMatrixData(const double* data);
    
    TimeSequence* wrench(Location loc, bool global = true, double threshold = 10.0, double rate = -1.0);
    
  protected:
    ForcePlate(ForcePlatePrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT;
    
    std::string stringifyLocation(Location loc) const _OPENMA_NOEXCEPT;
    std::vector<TimeSequence*> retrieveChannels() const _OPENMA_NOEXCEPT;
    virtual bool computeWrenchAtOrigin(TimeSequence* w) = 0;
    bool resampleWrench(TimeSequence* w, double factor);
    bool computePosition(TimeSequence* w, Location loc, double threshold);
    bool transformToGlobal(TimeSequence* w);
    
    virtual void copyContents(const Node* source) _OPENMA_NOEXCEPT override;
  };
}
};

OPENMA_EXPORT_STATIC_TYPEID(ma::instrument::ForcePlate, OPENMA_INSTRUMENT_EXPORT);

#endif // __openma_instrument_forceplate_h