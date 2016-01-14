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

#include "openma/base/event.h"
#include "openma/base/event_p.h"

#include <limits>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
  EventPrivate::EventPrivate(Event* pint, const std::string& name, double time, const std::string& context, const std::string& subject)
  : NodePrivate(pint,name), Time(time), Context(context), Subject(subject)
  {};
  
  EventPrivate::~EventPrivate() _OPENMA_NOEXCEPT = default;
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
  /**
   * @class Event openma/base/event.h
   * @brief Label a specific time.
   *
   * An event is defined by:
   *  - a @a name;
   *  - a @a time (in seconds);
   *  - a @a context (e.g. "Right" side, "Left" side, or "General" context);
   *  - a @a subject associated with this event
   *  - a @a description which could give more details about the event
   *
   * All these information can be accessed/modified by using dedicated accessor/mutator but also using the property() and setProperty() methods.
   *
   * A time can be positive, null, or negative. The null time indicates the start of the recording, while a negative and postive time corresponds to an event before and after the beginning of the recording, respectively.
   *
   * There is no standard for the definition of a context. However, it is common to use the following context for task analysis:
   * - @a Right: Events associated with the right side of the (human) body
   * - @a Left: Events associated with the left side of the (human) body
   * - @a General: Other events that cannot be assigned to the left or the right side of the body
   *
   * The role of the context may be important or not depending of your later data processing/analysis.
   * For example, inside a clinical gait analysis you can have several "Foot Strike" and "Foot Off" events assigned to the "Right" and "Left" sides.
   * You could want to retrieve the events by name or by context or both. Thus, choosing a common orthography could help you as well as other researchers.
   * The previous example could be implemented like in the following code. The important thing to remind is the case used for any string assigned (name, context, subject, ...)
   *
   * @code{.unparsed}
   * // Several events are defined in the node "Events".
   * // A algorithm could have generated the following events
   * ma::Node* events = new ma::Node("Events");
   * new ma::Event("Foot Strike",1.3,"Right","JDoe",&events);
   * new ma::Event("Foot Off",2.4,"Right","JDoe",&events);
   * new ma::Event("Foot Strike",1.8,"Left","JDoe",&events);
   * new ma::Event("Foot Off",3.0,"Left","JDoe",&events);
   * // Later in the code, you could retrive only:
   * // Events with the context "Right"
   * std::list<ma::Event*> rightEvents = events.findChild<ma::Event*>({},{{"context","Right"}});
   * // Events with the name "Foot Strike"
   * std::list<ma::Event*> footStrikeEvents = events.findChild<ma::Event*>("Foot Strike");
   * @endcode
   *
   * The subject's identification is used to distinguish events between subject which are in the same trial (if any).
   *
   * @ingroup openma_base
   */
  
#ifdef DOXYGEN_SHOULD_TAKE_THIS
  /** * @brief Fake structure to create node's properties */
  struct Event::__Doxygen_Properties
  {
  /**
   * This property holds the context (or side) associated with the Event. By default, this property contains an empty string.
   * @sa context() setContext()
   */
  std::string context;
  /**
   * This property holds the subject identification associated with the Event. By default, this property contains an empty string.
   * @sa subject() setSubject()
   */
  std::string subject;
  /**
   * This property holds the time (in second) associated with the Event. By default, this property is set to 0.0.
   * @sa time() setTime()
   */
  double time;
  };
#endif
  
  /**
   * Constructor.
   */
  Event::Event(const std::string& name, double time, const std::string& context, const std::string& subject, Node* parent)
  : Node(*new EventPrivate(this,name,time,context,subject),parent)
  {};
  
  /**
   * Destructor (default)
   */
  Event::~Event() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the time associated with this event.
   * The time can be positive, null, or negative.
   * The null time indicates the start of the recording, while a negative and postive time corresponds to an event before and after the beginning of the recording, respectively.
   */
  double Event::time() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Time;
  };

  /**
   * Sets the time associated with an event.
   * The time can be positive, null, or negative.
   * The null time indicates the start of the recording, while a negative and postive time corresponds to an event before and after the beginning of the recording, respectively.
   */
  void Event::setTime(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (std::fabs(value - optr->Time) < std::numeric_limits<double>::epsilon())
      return;
    optr->Time = value;
    this->modified();
  };
  
  /**
   * Returns the context associated with this event.
   * There is no standard for the definition of a context. However, it is common to use the following context for task analysis:
   * - @a Right: Events associated with the right side of the (human) body
   * - @a Left: Events associated with the left side of the (human) body
   * - @a General: Other events which cannot be assigned to the left or the right side of the body
   */
  const std::string& Event::context() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Context;
  };
  
  /**
   * Sets the context associated with this event.
   * There is no standard for the definition of a context. However, it is common to use the following context for task analysis:
   * - @a Right: Events associated with the right side of the (human) body
   * - @a Left: Events associated with the left side of the (human) body
   * - @a General: Other events which cannot be assigned to the left or the right side of the body
   */
  void Event::setContext(const std::string& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Context)
      return;
    optr->Context = value;
    this->modified();
  };
  
  /**
   * Return the subject's identification.
   */
  const std::string& Event::subject() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Subject;
  };
  
 /**
  * Sets the subject's identification.
  */
  void Event::setSubject(const std::string& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Subject)
      return;
    optr->Subject = value;
    this->modified();
  };
};
