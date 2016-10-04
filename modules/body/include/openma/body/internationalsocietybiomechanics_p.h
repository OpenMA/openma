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

#ifndef __openma_body_internationalsocietybiomechanics_p_h
#define __openma_body_internationalsocietybiomechanics_p_h

#include "openma/body/skeletonhelper_p.h"
#include "openma/base/enums.h"

namespace ma
{
namespace body
{
  class Model;
  class InternationalSocietyBiomechanics;
  
  class InternationalSocietyBiomechanicsPrivate : public SkeletonHelperPrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(InternationalSocietyBiomechanics)
    
    OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(InternationalSocietyBiomechanics, SkeletonHelper,
      Property<InternationalSocietyBiomechanics, Sex, &InternationalSocietyBiomechanics::sex, &InternationalSocietyBiomechanics::setSex>{"sex"}
    )
    
  public:
    InternationalSocietyBiomechanicsPrivate(InternationalSocietyBiomechanics* pint, const std::string& name, int region, int side);
    ~InternationalSocietyBiomechanicsPrivate() _OPENMA_NOEXCEPT;
    
    bool calibrateUpperLimb(int side, const math::Pose* torsoç, TaggedPositions* landmarks);
    bool calibrateLowerLimb(int side, const math::Position* HJC, TaggedPositions* landmarks);
    
    bool reconstructUpperLimb(Model* model, Trial* trial, int side, TaggedMappedPositions* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT;
    bool reconstructLowerLimb(Model* model, Trial* trial, int side, TaggedMappedPositions* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT;
    
    Sex Sex_;
  };
};
};

#endif // __openma_body_internationalsocietybiomechanics_p_h