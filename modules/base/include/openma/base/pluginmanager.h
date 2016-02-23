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

#ifndef __openma_base_pluginmanaager_h
#define __openma_base_pluginmanaager_h

#include "openma/base/macros.h" // _OPENMA_NOEXCEPT
#include "openma/base/logger.h"
#include <vector>

namespace ma
{
  template <typename T>
  class PluginManager
  {
  public:
    PluginManager() _OPENMA_NOEXCEPT;
    ~PluginManager() _OPENMA_NOEXCEPT;
    
    PluginManager(const PluginManager& ) = delete;
    PluginManager(PluginManager&& ) _OPENMA_NOEXCEPT = delete;
    PluginManager& operator=(const PluginManager& ) = delete;
    PluginManager& operator=(PluginManager&& ) _OPENMA_NOEXCEPT = delete;
    
    const std::vector<T*>& plugins() const _OPENMA_NOEXCEPT;
    
    bool add(T* plugin) _OPENMA_NOEXCEPT;
  
  private:
    std::vector<T*> m_Plugins;
  };
};

// ------------------------------------------------------------------------- //

namespace ma
{
  /**
   * @class PluginManager openma/base/pluginmanager.h
   * @brief Manage some plugins of the same type.
   * @tparam T Kind of plugin to manage
   *
   * This class is proposed in the public API to give to developers
   * the possibility to create new kind of plugin and managed them easily.
   *
   * @ingroup openma_base
   */
  
  
  /**
   * Create a plugin manager for the given type.
   * To populate the manager, you must passed by the add() method.
   */
  template <typename T>
  PluginManager<T>::PluginManager() _OPENMA_NOEXCEPT
  : m_Plugins()
  {};

  /**
   * Delete all the plugin added.
   */  
  template <typename T>
  PluginManager<T>::~PluginManager() _OPENMA_NOEXCEPT
  {
    for (auto p: this->m_Plugins)
      delete p;
  };
  
  /**
   * Returns a vector of the plugins managed
   */
  template <typename T>
  const std::vector<T*>& PluginManager<T>::plugins() const _OPENMA_NOEXCEPT
  {
    return this->m_Plugins;
  };
  
  /**
   * Add the @a plugin to this manager. The plugin will be then owned by the manager (and deleted when the object will be destroyed).
   * This method does some verification before adding the plugin. For example, it checks if the same @a plugin was not already added, or if another plugin with the same name is not already loaded.
   * @todo Add the plugin if an old version is already integrated (and unload the previous)?
   */
  template <typename T>
  bool PluginManager<T>::add(T* plugin) _OPENMA_NOEXCEPT
  {
    // Check if the plugin is already in the list
    for (auto p: this->m_Plugins)
    {
      // Same object?
      if (p == plugin)
      {
        error("Plugin %s is already loaded.", p->name().c_str());
        return false;
      }
      // Or another instance?
      else if (p->name().compare(plugin->name()) == 0)
      {
        error("Plugin %s has already another loaded instance.", p->name().c_str());
        return false;
      }
    }
    // If not, then add it
    this->m_Plugins.push_back(plugin);
    return true;
  };
};

#endif // __openma_base_pluginmanaager_h