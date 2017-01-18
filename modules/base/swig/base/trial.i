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

namespace ma
{
  SWIG_TYPEMAP_NODE_OUT(ma, Trial)
  SWIG_CREATE_TEMPLATE_HELPER_1(ma, Trial, SWIGTYPE)
  
  %nodefaultctor;
  class Trial : public Node
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(ma, Trial, SWIGTYPE)

    Trial(const std::string& name, Node* parent = nullptr);
    ~Trial();
    Node* timeSequences();
    Node* events();
    Node* hardwares();
    
    %extend
    {
      TimeSequence* timeSequence(unsigned idx);
      Event* event(unsigned idx);
      Hardware* hardware(unsigned idx);
    };
  };
  %clearnodefaultctor;
};
  
%{

ma::Event* ma_Trial_event(ma::Trial* self, unsigned index)
{    
#if defined(SWIGMATLAB)
  if ((index > 0) && (index <= self->events()->children().size()))
    return self->event(index-1);
#else
  if (index < self->events()->children().size())
    return self->event(index);
#endif
  else
  {
    SWIG_SendError(SWIG_IndexError, "Index out of range");
    return nullptr;
  }
};

ma::Hardware* ma_Trial_hardware(ma::Trial* self, unsigned index)
{
#if defined(SWIGMATLAB)
  if ((index > 0) && (index <= self->hardwares()->children().size()))
    return self->hardware(index-1);
#else
  if (index < self->hardwares()->children().size())
    return self->hardware(index);
#endif
  else
  {
    SWIG_SendError(SWIG_IndexError, "Index out of range");
    return nullptr;
  }
};

ma::TimeSequence* ma_Trial_timeSequence(ma::Trial* self, unsigned index)
{
#if defined(SWIGMATLAB)
  if ((index > 0) && (index <= self->timeSequences()->children().size()))
    return self->timeSequence(index-1);
#else
  if (index < self->timeSequences()->children().size())
    return self->timeSequence(index);
#endif
  else
  {
    SWIG_SendError(SWIG_IndexError, "Index out of range");
    return nullptr;
  }
};

%};
