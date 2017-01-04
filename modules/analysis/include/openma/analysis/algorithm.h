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

#ifndef __openma_analysis_algorithm_h
#define __openma_analysis_algorithm_h

#include "openma/analysis_export.h"
#include "openma/base/any.h"
#include "openma/base/opaque.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT
#include "openma/base/exception.h"
#include "openma/base/logger.h"

#include <string>
#include <memory> // std::unique_ptr

namespace ma
{
  class Node;
  
namespace analysis
{
  enum class Failure;
  
  class AlgorithmPrivate;
  
  class OPENMA_ANALYSIS_EXPORT ExecutionFailure : public Exception
  {
  public:
    explicit ExecutionFailure(const std::string& msg) : Exception(msg) {};
  };
  
  class OPENMA_ANALYSIS_EXPORT Algorithm
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Algorithm)
    
  public:
    Algorithm();
    virtual ~Algorithm() _OPENMA_NOEXCEPT;
    
    Algorithm(const Algorithm& ) = delete;
    Algorithm(Algorithm&& ) _OPENMA_NOEXCEPT = delete;
    Algorithm& operator=(const Algorithm& ) = delete;
    Algorithm& operator=(const Algorithm&& ) _OPENMA_NOEXCEPT = delete;
    
    bool execute(Node* output, const std::unordered_map<std::string,Any>& options);
 
    void setSource(Node* source) _OPENMA_NOEXCEPT;
    Node* source() const _OPENMA_NOEXCEPT;
    
    void setAbortOnFailure(bool value) _OPENMA_NOEXCEPT;
    bool abortOnFailure() const _OPENMA_NOEXCEPT;
    
    Failure failureCode() const _OPENMA_NOEXCEPT;
    const std::string& failureMessage() const _OPENMA_NOEXCEPT;
  
  protected:
    Algorithm(AlgorithmPrivate& pimpl) _OPENMA_NOEXCEPT;
    
    void setFailure(Failure code, const std::string& msg = std::string{});
    Any extractArgument(const std::unordered_map<std::string,Any>& arguments, const std::string& name) const _OPENMA_NOEXCEPT;
    virtual bool processSource(Node* output, Node* source, const std::unordered_map<std::string,Any>& options) = 0;
    
  private:
    void fillFailure(Failure code, const std::string& msg) _OPENMA_NOEXCEPT;
    std::unique_ptr<AlgorithmPrivate> mp_Pimpl;
  };
};
};

#endif // __openma_analysis_algorithm_h
