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
#include "openma/body/externalwrenchassigner.h"
#include "openma/body/inversedynamicsprocessor.h"
#include "openma/body/inertialparametersestimator.h"
#include "openma/body/landmarkstranslator.h"
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
  : NodePrivate(pint,name), Region(region), Side(side), Gravity{{0.,0.,0.}}
  {};
  
  SkeletonHelperPrivate::~SkeletonHelperPrivate() = default;
  
  bool SkeletonHelperPrivate::hasNonNullGravity() const _OPENMA_NOEXCEPT
  {
    return (this->Gravity[0] != 0.) || (this->Gravity[1] != 0.) || (this->Gravity[2] != 0.);
  };
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::SkeletonHelper);

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

#ifdef DOXYGEN_SHOULD_TAKE_THIS
  /** * @brief Fake structure to create node's properties */
  struct SkeletonHelper::__Doxygen_Properties
  {
    /**
     * This property holds the gravity direction and magnitude used by some calculation like the inverse dynamics. By default, this property contains a vector of zeros.
     * The unit to use must be the metre by second squared (m/s^2). If the gravity is not set (or reset to 0.), the method reconstruct() will not compute the inverse dynamics. Only the geometry foir each model will be computed.
     * @sa gravity() setGravity()
     */
    std::array<double,3> Gravity;
  };
#endif
  
  /**
   * Constructor. Store the @a region and @a side for later use. You can use the enum Region and Side to set these values.
   */
  SkeletonHelper::SkeletonHelper(const std::string& name, int region, int side, Node* parent)
  : Node(*new SkeletonHelperPrivate(this,name,region,side), parent)
  {};
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  SkeletonHelper::SkeletonHelper(SkeletonHelperPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
  /**
   * Destructor
   */
  SkeletonHelper::~SkeletonHelper() _OPENMA_NOEXCEPT = default;
  
  /**
   * Sets the internal parameter Gravity.
   */
  void SkeletonHelper::setGravity(const std::array<double,3>& g)
  {
    auto optr = this->pimpl();
    if (optr->Gravity == g)
      return;
    optr->Gravity = g;
    this->modified();
  };
  
  /**
   * Returns the internal parameter Gravity.
   */
  const std::array<double,3>& SkeletonHelper::gravity() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Gravity;
  };
  
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
    auto optr = this->pimpl();
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
      auto model = new Model(std::string{});
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
      // Attach the model to the output
      model->addParent(output);
      // Copy the gravity
      // FIXME WE MUST FIND A WAY TO NOT FORCE THE SETTING OF THE GRAVITY TO THE UNIT MM/S^2
      const double g[3] = {optr->Gravity[0] * 1000., optr->Gravity[1] * 1000., optr->Gravity[2] * 1000.};
      model->setGravity(g);
      // Copy dynamic properties to the model. They can be used later.
      const auto& props = this->dynamicProperties();
      for (const auto& prop : props)
        model->setProperty(prop.first, prop.second);
      // Attach the landmarks translator to the model. It can be used later.
      auto translator = this->findChild<LandmarksTranslator*>({},{},false);
      if (translator != nullptr)
        translator->addParent(model);
      // Attach the trial to the model. It can be used later.
      trial->addParent(model);
      // Computation of the inverse dynamics?
      if (optr->hasNonNullGravity())
      {
        Node temp("_TRID");
        model->addParent(&temp);
        // Associate FP wrench to feet
        ExternalWrenchAssigner* ewa = this->findChild<ExternalWrenchAssigner*>({},{},false);
        if (ewa == nullptr)
          ewa = this->defaultExternalWrenchAssigner();
        if ((ewa != nullptr) && !ewa->run(&temp))
        {
          error("SkeletonHelper - Error during the setting of external wrenches. Inverse dynamics computation skipped for the trial #%i", inc);
          continue;
        }
        // Compute BSIPs
        InertialParametersEstimator* ipe = this->findChild<InertialParametersEstimator*>({},{},false);
        if (ipe == nullptr)
          ipe = this->defaultInertialParametersEstimator();
        if ((ipe != nullptr) && !ipe->generate(&temp))
        {
          error("SkeletonHelper - Error during the estimation of the segment inertial parameters. Inverse dynamics computation skipped for the trial #%i", inc);
          continue;
        }
        // Compute inverse dynamics in the global frame
        InverseDynamicProcessor* idp = this->findChild<InverseDynamicProcessor*>({},{},false);
        if (idp == nullptr)
          idp = this->defaultInverseDynamicProcessor();
        if ((idp != nullptr) && !idp->run(&temp))
        {
          error("SkeletonHelper - Error during the computation of the inverse dynamics. Inverse dynamics computation skipped for the trial #%i", inc);
          continue;
        }
      }
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
   * Thus, it will be created only one time in calibrate() and found as a child in reconstruct().
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
    optr->Gravity = optr_src->Gravity;
  };
};
};