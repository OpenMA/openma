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

#include "openma/math.h"

bool _ma_math_verify_timesequence(const ma::TimeSequence* ts, int type, unsigned components, unsigned offset)
{
  if (ts == nullptr)
    return false;
  if ((ts->type() & ma::TimeSequence::Reconstructed) != ma::TimeSequence::Reconstructed)
    return false;
  if ((type != -1) && ((ts->type() & type) != type))
    return false;
  if (components > ts->components())
    return false;
  if ((offset + components) > ts->components())
    return false;
  return true;
}

namespace ma
{
namespace math
{
  TimeSequence* to_timesequence(unsigned components, unsigned samples, const double* values, const double* residuals, const std::string& name, double rate, double start, int type, const std::string& unit, Node* parent)
  {
    assert(components > 0);
    assert(samples > 0);
    auto ts = parent->findChild<TimeSequence*>(name,{{"type",type}},false);
    if (ts != nullptr)
    {
      assert(ts->components() == components);
      ts->resize(samples); // Grow / shrink the data (if necessary)
      ts->setSampleRate(rate); // Assign possibly a new rate
      ts->setStartTime(start); // Same for the start time
      ts->setUnit(unit); // Same for the unit
    }
    else
    {
      ts = new TimeSequence(name, components, samples, rate, start, type, unit, parent);
    }
    if ((values != nullptr) && (residuals != nullptr))
    {  
      const unsigned valuesComponents = components - 1;
      const unsigned offset = valuesComponents * samples;
      std::copy_n(values, samples * valuesComponents, ts->data());
      std::copy_n(residuals, samples, ts->data() + offset);
    }
    return ts;
  };
};
};