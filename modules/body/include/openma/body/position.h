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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#ifndef __openma_body_position_h
#define __openma_body_position_h

#include "openma/body_export.h"
#include "openma/base/node.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

namespace ma
{
namespace body
{
  class PositionPrivate;
  
  class OPENMA_NODE(OPENMA_BODY_EXPORT, Position) : public Node
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(Position)
    OPENMA_DECLARE_NODEID(Position, Node)
    
  public:
    Position(const std::string& name, const double coordinates[3] = nullptr, Node* parent = nullptr);
    ~Position() _OPENMA_NOEXCEPT;
    
    Position(const Position& ) = delete;
    Position(Position&& ) _OPENMA_NOEXCEPT = delete;
    Position& operator=(const Position& ) = delete;
    Position& operator=(Position&& ) _OPENMA_NOEXCEPT = delete;
    
    const double* data() const _OPENMA_NOEXCEPT;
    double* data() _OPENMA_NOEXCEPT;
    
    void setData(const double* values) _OPENMA_NOEXCEPT;
    
    Position* clone(Node* parent = nullptr) const;
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
    
  private:
    Position(PositionPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT;
  };
};
};

#endif // __openma_body_position_h