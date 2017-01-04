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

#include "openma/analysis/taskeventdetection.h"
#include "openma/base/node.h"

namespace ma
{
namespace analysis
{
  /**
   * @class TaskEventDetection "openma/analysis/taskeventdetection.h"
   * @brief Interface to implement task event detection algorithm
   *
   * Each inheriting class must overload the method detect(). This method has to define the event detection algorithm.
   * Internally, the metod processSource() uses the given source, extract the children and call the detect() method on each of them.
   * The arguments passed to the method processSource() are passed to the method detect().
   * 
   * @ingroup openma_analysis
   */
  
  /**
   * Constructor
   */
  TaskEventDetection::TaskEventDetection()
  : Algorithm()
  {};
  
  /**
   * Destructor (default)
   */
  TaskEventDetection::~TaskEventDetection() _OPENMA_NOEXCEPT = default;
  
  /**
   * General method to detect task related events. See the detailed description of the class TaskEventDetection for more information on its behaviour.
   */
  bool TaskEventDetection::processSource(Node* output, Node* source, const std::unordered_map<std::string,Any>& arguments)
  {
    bool res = true;
    const auto& children = source->children();
    for (auto child : children)
    {
      auto events = new Node(child->name()+".Events", output);
      res &= this->detect(events, child, arguments);
    }
    return res;
  };
};
};