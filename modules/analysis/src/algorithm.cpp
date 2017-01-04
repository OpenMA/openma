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

#include "openma/analysis/algorithm.h"
#include "openma/analysis/algorithm_p.h"
#include "openma/analysis/enums.h"
#include "openma/base/node.h"
#include "openma/base/logger.h"
#include "openma/base/utils.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace analysis
{
  AlgorithmPrivate::AlgorithmPrivate()
  : Source(nullptr), AbortOnFailure(false), FailureCode(Failure::None), FailureMessage()
  {};
  
  AlgorithmPrivate::~AlgorithmPrivate() _OPENMA_NOEXCEPT = default; // Cannot be inlined
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace analysis
{
  /**
   * @class Algorithm 
   * @brief To perform a set of operations on data 
   *
   * This class is an interface to design algorithm. It is only necessary to overload the method processSource(). The method to call is execute().
   * 
   * You can use the methods abortOnFailure() and setAbortToFailure() to modify the behaviour of the algorithm when a failure happens (i.e when the method setFailure() is used). 
   * If this is set to true, the algorithm aborts at the first failure and the output stays blank. Otherwise, the message of the failure is simply sent to the logger as a warning and continue its execution.
   * In both cases, the throwing of an exception will stop the execution of the algorithm and the output will stay blank.
   * 
   * @ingroup openma_analysis
   */
  /**
   * @var Algorithm::mp_Pimpl
   * Storage for the private implementation. 
   */
  
  /**
   * Default constructor
   */
  Algorithm::Algorithm()
  : mp_Pimpl(new AlgorithmPrivate)
  {};
  
  /**
   * Destructor (default)
   */
  Algorithm::~Algorithm() _OPENMA_NOEXCEPT = default;
  
  /**
   * Process the content of the current set source and fill the given @a output.
   * Internally the method processSource() is called. The @a output and the source are tested to verify they are not null.
   * Moreover, this method catches exception. 
   * In case this method returns false, you can use the methods failureCode() and failureMessage() to retrieve the failure.
   */
  bool Algorithm::execute(Node* output, const std::unordered_map<std::string,Any>& arguments)
  {
    auto optr = this->pimpl();
    if (output == nullptr)
    {
      this->fillFailure(Failure::Unexpected, "Impossible to store events in a null output. Task event detection aborted");
      return false;
    }
    if (optr->Source == nullptr)
    {
      this->fillFailure(Failure::Unexpected, "Impossible to do computation from a null source. Task event detection aborted");
      return false;
    }
    try
    {
      // Reset possible previous error.
      this->fillFailure(Failure::None, std::string{});
      // Process the data
      Node temp("_TAR"); // _TAR: Temporary Algorithm Root
      this->processSource(&temp, optr->Source, arguments);
      // In case the processing does not use exception but only error code/message.
      if (optr->FailureCode == Failure::None)
      {
        for (const auto& p: temp.dynamicProperties())
          output->setProperty(p.first, p.second);
        for (auto& child: temp.children())
        {
          if (child != nullptr)
            child->addParent(output);
        }
      }
    }
    catch (ExecutionFailure& e)
    {
      this->fillFailure(Failure::AbortedExecution, e.what());
    }
    catch (std::exception& e)
    {
      this->fillFailure(Failure::Unexpected, "Unexpected exception during the reading of a device. Please report this to the support: " + std::string(e.what()));
    }
    catch(...)
    {
      this->fillFailure(Failure::Unknown, "Unknown exception during a task event detection. Please report this to the support");
    }
    
    if (this->failureCode() == Failure::None)
      return true;
    return false;
  };
  
  /**
   * Returns the current source associated with this algorithm.
   */
  Node* Algorithm::source() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Source;
  };
  
  /**
   * Sets the current source associated with this algorithm.
   */
  void Algorithm::setSource(Node* source) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Source = source;
  };
  
  /**
   * Set the behaviour of the algorithm when a failure happens.
   */
  void Algorithm::setAbortOnFailure(bool value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->AbortOnFailure = value;
  };
  
  /**
   * Returns the behaviour of the algorithm when a failure happens.
   */
  bool Algorithm::abortOnFailure() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->AbortOnFailure;
  };
  
  /**
   * Returns the current failure code.
   */
  Failure Algorithm::failureCode() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->FailureCode;
  };
  
  /**
   * Returns the current failure message.
   */
  const std::string& Algorithm::failureMessage() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->FailureMessage;
  };
  
  /**
   * Constructor for inheriting class with extended private implementation
   */
  Algorithm::Algorithm(AlgorithmPrivate& pimpl) _OPENMA_NOEXCEPT
  : mp_Pimpl(&pimpl)
  {};
  
  /**
   * Extract the value associated with the given @a name. If no argument was found, an invalid Any object is returned.
   * The comparison is done with an insensitive case method.
   */
  Any Algorithm::extractArgument(const std::unordered_map<std::string,Any>& arguments, const std::string& name) const _OPENMA_NOEXCEPT
  {
    Any val;
    for (const auto& arg : arguments)
    {
      if (strcmpi(arg.first, name))
      {
        val = arg.second;
        break;
      }
    }
    return val;
  };
  
  /**
   * Set a code and a detailed message for a  which happened during the reading or writing of a device.
   */
  void Algorithm::setFailure(Failure code, const std::string& msg)
  {
    OPENMA_UNUSED(code);
    auto optr = this->pimpl();
    if (optr->AbortOnFailure)
      throw(ExecutionFailure(msg));
    else
      warning(msg.c_str());
  };
  
  void Algorithm::fillFailure(Failure code, const std::string& msg = std::string{}) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->FailureCode = code;
    optr->FailureMessage = msg;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class ExecutionFailure openma/analysis/algorithm.h
   * @brief General exception to use when a failure happens during the execution of an algorithm.
   * The internal implementation of an Algorithm uses exception to determine if something wrong happened.
   * All kind of exceptions are catched, but it is strongly advise to use ExecutionFailure for case managed by an algorithm.
   */
  
  /**
   * @fn explicit ExecutionFailure::ExecutionFailure(const std::string& msg)
   * Construct a ExecutionFailure exception
   */
};
};
