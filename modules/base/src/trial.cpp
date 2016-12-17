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

#include "openma/base/trial.h"
#include "openma/base/trial_p.h"
#include "openma/base/event.h"
#include "openma/base/hardware.h"
#include "openma/base/timesequence.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
  TrialPrivate::TrialPrivate(Trial* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  TrialPrivate::~TrialPrivate() _OPENMA_NOEXCEPT = default;
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::Trial);

namespace ma
{
  /**
   * @class Trial openma/base/trial.h
   * @brief Store the acquired data during an acquisition
   *
   * A Trial class is a helper to manage and store data using different category.
   * For example, time sequences could be stored in the subnode "TimeSequences" using the method timeSequences(). All events could be stored in the subnode "Events" using the method events(). 
   * @code{.unparsed}
   * // Create a trial
   * auto trial = Trial("myTrial");
   * // Try to find the child "TimeSequences"
   * auto nonode = trial->findChild("TimeSequences");
   * // Next line will display false.
   * std::cout <<  std::boolalpha << nonode != nullptr << std::endl;
   * // Creates and returns the subnode "TimeSequences".
   * auto tsnode = trial->timeSequences(); // Create the subnode "TimeSequences".
   * // Manual finding of the subnode.
   * auto tsnode2 = trial->findChild("TimeSequences");
   * // Next line will display true.
   * std::cout <<  std::boolalpha << (tsnode == tsnode2) << std::endl;
   * @endcode
   *
   * To easily add an object in a category, it is only necessary to give the adapted node as parent to the objet.
   * @code{.unparsed}
   * // Create a trial
   * auto trial = ma::Trial("myTrial");
   * // Add a time sequence in the trial by setting its parent to 'trial->timeSequences()'.
   * auto ts = ma::TimeSequence("RHEL", 4, 100, 100.0, 0.0, ma::TimeSequence::Position, "mm", trial->timeSequences());
   * // Next line will display true
   * std::cout <<  std::boolalpha << trial->findChild<ma::TimeSequence*>("RHEL") != nullptr << std::endl;
   * // This is the same for the next line. The differences is in the research time which might be shorter below.
   * * std::cout <<  std::boolalpha << trial->timeSequences()->findChild<ma::TimeSequence*>("RHEL") != nullptr << std::endl;
   * @endcode
   *
   * @ingroup openma_base
   */
  
  /**
   * Constructor
   */
  Trial::Trial(const std::string& name, Node* parent)
  : Node(*new TrialPrivate(this,name),parent)
  {};
  
  /**
   * Destructor (default)
   */
  Trial::~Trial() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the subnode "TimeSequences".
   * If the subnode does not exist, this one is created.
   */
  Node* Trial::timeSequences()
  {
    auto pt = this->findChild("TimeSequences",{},false);
    if (pt == nullptr)
      pt = new Node("TimeSequences",this);
    return pt;
  };
  
  /**
   * Returns the @a idx child of the subnode "TimeSequences" and cast it as a TimeSequence object.
   * If @a idx is out of range or if the extracted node is not a TimeSequence object, the method returns nullptr.
   */
  TimeSequence* Trial::timeSequence(unsigned idx) _OPENMA_NOEXCEPT
  {
    return this->timeSequences()->child<TimeSequence*>(idx);
  };
  
  /**
   * Returns the subnode "Events".
   * If the subnode does not exist, this one is created. 
   */
  Node* Trial::events()
  {
    auto pt = this->findChild("Events",{},false);
    if (pt == nullptr)
      pt = new Node("Events",this);
    return pt;
  };
  
  /**
   * Returns the @a idx child of the subnode "Events" and cast it as an Event object.
   * If @a idx is out of range or if the extracted node is not an Event object, the method returns nullptr.
   */
  Event* Trial::event(unsigned idx) _OPENMA_NOEXCEPT
  {
    return this->events()->child<Event*>(idx);
  };
  
  /**
   * Returns the subnode "Hardwares".
   * If the subnode does not exist, this one is created. 
   */
  Node* Trial::hardwares()
  {
    auto pt = this->findChild("Hardwares",{},false);
    if (pt == nullptr)
      pt = new Node("Hardwares",this);
    return pt;
  };
  
  /**
   * Returns the @a idx child of the subnode "Hardwares" and cast it as an Hardware object.
   * If @a idx is out of range or if the extracted node is not an Hardware object, the method returns nullptr.
   */
  Hardware* Trial::hardware(unsigned idx) _OPENMA_NOEXCEPT
  {
    return this->hardwares()->child<Hardware*>(idx);
  };
  
  /**
   * Create a new Event object on the heap
   */
  Node* Trial::allocateNew() const
  {
    return new Trial(this->name());
  };
};