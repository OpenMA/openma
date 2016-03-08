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

#include "openma/body/skeletonhelper.h"
#include "openma/body/skeletonhelper_p.h"
#include "openma/body/model.h"
#include "openma/base/trial.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  SkeletonHelperPrivate::SkeletonHelperPrivate(SkeletonHelper* pint, const std::string& name, int region, int side)
  : NodePrivate(pint,name), Region(region), Side(side)
  {};
  
  SkeletonHelperPrivate::~SkeletonHelperPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace body
{
  /**
   * @class SkeletonHelper openma/body/skeletonhelper.h
   * @brief Helper class to create a multi-segment model and recontruct its motion.
   * This class is usefull for the implementation of a model in known protocols (e.g. with a specific marker set like PluginGait, HelenHayes, or ISB).
   * @todo Write a very detailed description for this class.
   * @ingroup openma_body
   */
  
  /**
   * Constructor. Store the @a region and @a side for later use. You can use the enum Region and Side to set these values.
   */
  SkeletonHelper::SkeletonHelper(const std::string& name, int region, int side, Node* parent)
  : Node(*new SkeletonHelperPrivate(this,name,region,side), parent)
  {};
  
  /**
   * * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  SkeletonHelper::SkeletonHelper(SkeletonHelperPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
  /**
   * Destructor
   */
  SkeletonHelper::~SkeletonHelper() _OPENMA_NOEXCEPT = default;
  
  /**
   * @fn virtual bool SkeletonHelper::calibrate(Node* trials, Subject* subject) _OPENMA_NOEXCEPT = 0;
   * This methods must be overloaded by inheriting classes to calibrate the helper. For this, the content of @a trials and @a subject can be used.
   * For example, in the PluginGait helper, the method calibrate() is used to set several joint center coordinates and angle offsets. For that, the first Trial object found is used as well as the content of the @a subject.
   */
  
  /**
   * For each direct children corresponding to a Trial object in @a trials, create a model and reconstruct the associated movement. Each Model is added to the @a output.
   */
  bool SkeletonHelper::reconstruct(Node* output, Node* trials)
  {
    if (output == nullptr)
    {
      error("SkeletonHelper - Null output passed. Movement reconstruction aborted.");
      return false;
    }
    if (trials == nullptr)
    {
      error("SkeletonHelper - Null trials passed. Movement reconstruction aborted.");
      return false;
    }
    int inc = -1;
    auto _trials = trials->findChildren<Trial*>({},{},false);
    for (auto trial : _trials)
    {
      ++inc;
      if (trial == nullptr)
      {
        error("SkeletonHelper - Trial #%i is null. Movement reconstruction skipped for this trial.", inc);
        continue;
      }
      auto model = new Model({});
      if (!this->setupModel(model))
      {
        error("SkeletonHelper - Error in the model set up. Movement reconstruction skipped for the trial #%i", inc);
        delete model;
        continue;
      }
      if (!this->reconstructModel(model, trial))
      {
        error("SkeletonHelper - Error in the model reconstruction. Movement reconstruction skipped for the trial #%i", inc);
        delete model;
        continue;
      }
      model->addParent(output);
    }
    return true;
  };
  
  /**
   * @fn virtual bool SkeletonHelper::setupModel(Model* model) const = 0;
   * Create segments and joints related to the configuration given to the constructor (i.e. region and side).
   * This method should set the name of the model.
   */
  
  /**
   * @fn virtual bool SkeletonHelper::reconstructModel(Model* model, Trial* trial) const = 0;
   * Reconstruct @a model movement for the given @a trial. This part is complety dependant of the helper and the associated marker set.
   * This method should rename the model to try to have a unique name (or identifier).
   */
  
  /**
   * @fn virtual LandmarksTranslator* SkeletonHelper::defaultLandmarksTranslator() = 0;
   * Create a LandmarksTranslator adapted to the marker set used by the helper.
   * Because a landmarks translator is used in the calibrate() and reconstruct() methods, this method could parent the created translator to the helper.
   * Thus, it will be created only one time in calibrate () and found as a child in reconstruct().
   */
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void SkeletonHelper::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const SkeletonHelper*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    optr->Region = optr_src->Region;
    optr->Side = optr_src->Side;
  };
};
};