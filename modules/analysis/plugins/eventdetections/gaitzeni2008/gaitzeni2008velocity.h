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

#ifndef __openma_analysis_gaitzeni2008velocity_h
#define __openma_analysis_gaitzeni2008velocity_h

#include "openma/analysis/taskeventdetection.h"

namespace ma
{
namespace analysis
{
  class GaitZeni2008Velocity : public TaskEventDetection
  {
  public:
    GaitZeni2008Velocity();
    virtual ~GaitZeni2008Velocity() _OPENMA_NOEXCEPT;
    
    GaitZeni2008Velocity(const GaitZeni2008Velocity& ) = delete;
    GaitZeni2008Velocity(GaitZeni2008Velocity&& ) _OPENMA_NOEXCEPT = delete;
    GaitZeni2008Velocity& operator=(const GaitZeni2008Velocity& ) = delete;
    GaitZeni2008Velocity& operator=(const GaitZeni2008Velocity&& ) _OPENMA_NOEXCEPT = delete;
  
  protected:
    virtual bool detect(Node* output, Node* input, const std::unordered_map<std::string,Any>& arguments) final;
  };
};
};

#endif // __openma_analysis_gaitzeni2008velocity_h