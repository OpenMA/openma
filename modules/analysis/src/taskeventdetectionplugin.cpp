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

#include "openma/analysis/taskeventdetectionplugin.h"
#include "openma/analysis/taskeventdetection.h"

#include "openma/base/pluginmanager.h"

// #ifdef OPENMA_ANALYSIS_STATIC_DEFINE
#include "../plugins/staticanalysispluginloader.h"
// #endif

#include <algorithm> // std::transform

namespace ma
{
namespace analysis
{
  /**
   * Convenient method to load task event detection plugins.
   * Internally this method uses a static object. It will be populated the first time this function is used.
   */
  const std::vector<TaskEventDetectionPlugin*>& load_task_event_detection_plugins()
  {
    static PluginManager<TaskEventDetectionPlugin> manager;
    if (manager.plugins().empty())
    {
// #ifndef OPENMA_ANALYSIS_STATIC_DEFINE
      // Dynamic loading
      // {"eventdetections"})
// #error Not yet implemented
// #else
      // Static include
      load_task_event_detection_plugins(&manager);
// #endif
    }
    return manager.plugins();
  };
  
  /**
   * Constructor (default)
   */
  TaskEventDetectionPlugin::TaskEventDetectionPlugin() = default;
  
  /**
   * Destruction (default)
   */
  TaskEventDetectionPlugin::~TaskEventDetectionPlugin() = default;
  
  /**
   * @fn virtual std::string TaskEventDetectionPlugin::name() const _OPENMA_NOEXCEPT = 0;
   * Returns the name of the plugin
   */
  
  /**
   * @fn virtual std::vector<std::string> TaskEventDetectionPlugin::supportedAlgorithms() const _OPENMA_NOEXCEPT = 0;
   * Returns a list of supported algorithms by this plugin.
   */
  
  /**
   * @fn virtual TaskEventDetection* TaskEventDetectionPlugin::create(Node* source, const std::string& algorithm) = 0;
   * Create a new algorithm. The @a source is passed to the created algorithm. The usage of @a algorithm can be twofold. First, it can be used to verify if the given format is supported by this plugin. Second, it will help to choose the good algorithm if multiple algorithms are managed by the plugin.
   */
};
};