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

#include "openma/analysis/taskeventdetector.h"
#include "openma/analysis/enums.h"
#include "openma/analysis/taskeventdetection.h"
#include "openma/analysis/taskeventdetectionplugin.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace analysis
{
  class TaskEventDetectorPrivate
  {
  public:
    TaskEventDetectorPrivate(Node* source, const std::string& format);
    ~TaskEventDetectorPrivate() _OPENMA_NOEXCEPT;
    
    Node* Source;
    std::string Algorithm;
    TaskEventDetection* Detection;
    Failure FailureCode;
    std::string FailureMessage;
  };
  
  TaskEventDetectorPrivate::TaskEventDetectorPrivate(Node* source, const std::string& algorithm)
  : Source(source), Algorithm(algorithm), Detection(nullptr), FailureCode(Failure::None), FailureMessage()
  {};
  
  TaskEventDetectorPrivate::~TaskEventDetectorPrivate() _OPENMA_NOEXCEPT = default; // Cannot be inlined
};
};

#endif

// ------------------------------------------------------------------------- //
//                                 PUBLIC API                                //
// ------------------------------------------------------------------------- //

namespace ma
{
namespace analysis
{
  /**
   * @class TaskEventDetector "openma/analysis/taskeventdetector.h"
   * @brief Independant task related event detector
   *
   * @ingroup openma_analysis
   */
  
  /**
   *
   */
  TaskEventDetector::TaskEventDetector(Node* source, const std::string& algorithm)
  : mp_Pimpl(new TaskEventDetectorPrivate(source, algorithm))
  {};
    
  /**
   * Destructor (default)
   */
  TaskEventDetector::~TaskEventDetector() = default;
  
  /**
   *
   */
  void TaskEventDetector::setSource(Node* source)
  {
    auto optr = this->pimpl();
    optr->Source = source;
  };
  
  /**
   *
   */
  void TaskEventDetector::setAlgorithm(const std::string& algorithm)
  {
    auto optr = this->pimpl();
    if (optr->Algorithm.compare(algorithm) == 0)
      return;
    optr->Algorithm = algorithm;
    delete optr->Detection;
    optr->Detection = nullptr;
  };
  
  /**
   *
   */
  const std::string& TaskEventDetector::algorithm() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Algorithm;
  };
  
  /**
   *
   */
  bool TaskEventDetector::run(Node* output, const std::unordered_map<std::string,Any>& arguments)
  {
    auto optr = this->pimpl();
    this->setFailure(Failure::None);
    if (optr->Source == nullptr)
    {
      this->setFailure(Failure::Source, "No valid source assigned.");
      return false;
    }
    // Do we need to find the good algorithm?
    if (optr->Detection == nullptr)
    {
      const auto& plugins = load_task_event_detection_plugins();
      for (const auto& plugin: plugins)
      {
        if (plugin->supports(optr->Algorithm))
        {
          optr->Detection = plugin->create(optr->Source, optr->Algorithm);
          break;
        }
      }
    }
    // Did we find an algorithm?
    if (optr->Detection == nullptr)
    {
      this->setFailure(Failure::UnsupportedAlgorithm, "The algorithm '" + optr->Algorithm + "' is not available");
      return false;
    }
    // Let's run it!
    return optr->Detection->execute(output, arguments);
  };
  
  /**
   *
   */
  Failure TaskEventDetector::failureCode() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->FailureCode;
  };
  
  /**
   *
   */
  const std::string& TaskEventDetector::failureMessage() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->FailureMessage;
  };
  
  /**
   *
   */
  const std::vector<std::string>& TaskEventDetector::availableAlgorithms() _OPENMA_NOEXCEPT
  {
    static std::vector<std::string> algorithms;
    if (algorithms.empty())
    {  
      const auto& plugins = load_task_event_detection_plugins();
      for (auto plugin: plugins)
      {
        const auto& supportedalgorithms = plugin->supportedAlgorithms();
        algorithms.insert(algorithms.end(), supportedalgorithms.begin(), supportedalgorithms.end());
      }
    }
    return algorithms;
  };
  
 /**
  *
  */
 void TaskEventDetector::setFailure(Failure code, const std::string& msg) _OPENMA_NOEXCEPT
 {
   auto optr = this->pimpl();
   optr->FailureCode = code;
   optr->FailureMessage = msg;
 }; 
}  
};