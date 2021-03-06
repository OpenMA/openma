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

#ifndef __openma_base_event_p_h
#define __openma_base_event_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/base/node_p.h"
#include "openma/base/property.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

namespace ma
{
  class Event;

  class EventPrivate : public NodePrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(Event)
  
    OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(Event, Node,
      Property<Event,double,&Event::time,&Event::setTime>{"time"},
      Property<Event,const std::string&,&Event::context,&Event::setContext>{"context"},
      Property<Event,const std::string&,&Event::subject,&Event::setSubject>{"subject"}
    )
  
  public:
    EventPrivate(Event* pint, const std::string& name, double time, const std::string& context, const std::string& subject);
    ~EventPrivate() _OPENMA_NOEXCEPT;
    
    double Time;
    std::string Context;
    std::string Subject;
  };

};

#endif // __openma_base_event_p_h
