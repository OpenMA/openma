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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#ifndef __openma_instrument_forceplate_p_h
#define __openma_instrument_forceplate_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/base/hardware_p.h"

#include <array>
#include <vector>

namespace ma
{
namespace instrument
{
  class ForcePlate;
  
  class ForcePlatePrivate : public HardwarePrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(ForcePlate)
      
    OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(ForcePlate, Hardware,
      Property<ForcePlate, int, &ForcePlate::type>{"type"}
    )
      
  public:
    ForcePlatePrivate(ForcePlate* pint, const std::string& name, int type, std::vector<std::string>&& labels, unsigned rows, unsigned cols);
    ForcePlatePrivate(ForcePlate* pint, const std::string& name, int type, std::vector<std::string>&& labels);
    ~ForcePlatePrivate() _OPENMA_NOEXCEPT;
    
    int Type;
    std::array<double,12> ReferenceFrame;
    std::array<double,12> SurfaceCorners;
    std::array<double,3> RelativeSurfaceOrigin;
    std::array<unsigned,2> CalibrationMatrixDimensions;
    std::vector<double> CalibrationMatrixData;
  };
};
};

#endif // __openma_instrument_forceplate_p_h