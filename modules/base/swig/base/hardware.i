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
  SWIG_TYPEMAP_NODE_OUT(ma, Hardware)
  SWIG_CREATE_TEMPLATE_HELPER_1(ma, Hardware, SWIGTYPE)
  
  %nodefaultctor;
  class Hardware : public Node
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(ma, Hardware, SWIGTYPE)
  
    Node* channels();
    TimeSequence* channel(const std::string& label) const;
    unsigned channelsNumberRequired() const;
    Node* outputs();
    
    %extend {
      TimeSequence* channel(unsigned idx) const;
      void setChannel(unsigned idx, TimeSequence* sig);
    }
    void setChannel(const std::string& label, TimeSequence* sig);
  };
  %clearnodefaultctor;
};

%{

ma::TimeSequence* ma_Hardware_channel(const ma::Hardware* self, unsigned idx)
{
#if defined(SWIGMATLAB)
  if ((idx > 0) && (idx <= self->channelsNumberRequired()))
    return self->channel(idx-1);
#else
  if (idx < self->channelsNumberRequired())
    return self->channel(idx);
#endif
  else
  {
    SWIG_SendError(SWIG_IndexError, "Index out of range");
    return nullptr;
  }
};

void ma_Hardware_setChannel(ma::Hardware* self, unsigned idx, ma::TimeSequence* sig)
{
#if defined(SWIGMATLAB)
  if ((idx > 0) && (idx <= self->channelsNumberRequired()))
    self->setChannel(idx-1, sig);
#else
  if (idx < self->channelsNumberRequired())
    self->setChannel(idx, sig);
#endif
  else
  {
    SWIG_SendError(SWIG_IndexError, "Index out of range");
  }
};

%}