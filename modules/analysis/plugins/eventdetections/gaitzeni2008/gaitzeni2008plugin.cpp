/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solutanalysisn Inc., all rights reserved.
 * 
 * Redistributanalysisn and use in source and binary forms, with or without
 * modificatanalysisn, are permitted provided that the following conditanalysisns
 * are met:
 * 
 *     * Redistributanalysisns of source code must retain the above
 *       copyright notice, this list of conditanalysisns and the following
 *       disclaimer.
 *     * Redistributanalysisns in binary form must reproduce the above
 *       copyright notice, this list of conditanalysisns and the following
 *       disclaimer in the documentatanalysisn and/or other materials
 *       provided with the distributanalysisn.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific pranalysisr written
 *       permissanalysisn.
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

#include "gaitzeni2008plugin.h"
#include "gaitzeni2008coordinate.h"
#include "gaitzeni2008velocity.h"

#define _OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008(variant) \
  "gait.zeni2008" variant
#define _OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_COORDINATE _OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008(".coordinate-based")
#define _OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_VELOCITY _OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008(".velocity-based")

namespace ma
{
namespace analysis
{
  GaitZeni2008Plugin::GaitZeni2008Plugin() = default;
  
  GaitZeni2008Plugin::~GaitZeni2008Plugin() = default;
  
  std::string GaitZeni2008Plugin::name() const _OPENMA_NOEXCEPT
  {
    return "GaitZeni2008Plugin";
  };
  
  std::vector<std::string> GaitZeni2008Plugin::supportedAlgorithms() const _OPENMA_NOEXCEPT
  {
    return {_OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_COORDINATE, _OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_VELOCITY};
  };
  
  bool GaitZeni2008Plugin::supports(const std::string& algorithm) const _OPENMA_NOEXCEPT
  {
    return ((algorithm.compare(_OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_COORDINATE) == 0) || (algorithm.compare(_OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_VELOCITY) == 0));
  };
  
  TaskEventDetection* GaitZeni2008Plugin::create(Node* source, const std::string& algorithm)
  {
    TaskEventDetection* detection = nullptr;
    if (algorithm.compare(_OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_COORDINATE) == 0)
      detection = new GaitZeni2008Coordinate;
    else if (algorithm.compare(_OPENMA_ANALYSIS_ALGORITHM_GAIT_ZENI2008_VELOCITY) == 0)
      detection = new GaitZeni2008Velocity;
    if (detection != nullptr)
      detection->setSource(source);
    return detection;
  };
};
};