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

#ifndef __openma_base_node_p_h
#define __openma_base_node_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/base/object_p.h"
#include "openma/config.h" // USE_REFCOUNT_MECHANISM
#include "openma/base/typeid.h"
#include "openma/base/property.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <string>
#include <unordered_map>
#include <vector>

namespace ma
{
  class Node;
  
  class OPENMA_BASE_EXPORT NodePrivate : public ObjectPrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(Node)
    OPENMA_DECLARE_STATIC_PROPERTIES_BASE(Node,
      Property<Node, const std::string&, &Node::name, &Node::setName>{"name"},
      Property<Node, const std::string&, &Node::description, &Node::setDescription>{"description"}
    )

  public:
    static bool retrievePath(std::vector<const Node*>& path, const Node* current, const Node* stop);
    
    NodePrivate() = delete;
    NodePrivate(Node* pint, const std::string& name);
    ~NodePrivate() _OPENMA_NOEXCEPT;
    NodePrivate(const NodePrivate& ) = delete;
    NodePrivate(NodePrivate&& ) _OPENMA_NOEXCEPT = delete;
    NodePrivate& operator=(const NodePrivate& ) = delete;
    NodePrivate& operator=(const NodePrivate&& ) _OPENMA_NOEXCEPT = delete;
    
    bool attachParent(Node* node) _OPENMA_NOEXCEPT;
    bool detachParent(Node* node) _OPENMA_NOEXCEPT;
    
    bool attachChild(Node* node) _OPENMA_NOEXCEPT;
    bool detachChild(Node* node) _OPENMA_NOEXCEPT;
    
    std::string Name;
    std::string Description;
    std::unordered_map<std::string,Any> DynamicProperties;
    std::vector<Node*> Parents;
    std::vector<Node*> Children;
#if defined(USE_REFCOUNT_MECHANISM)
    int ReferenceCounter;
#endif
    
  protected:
    Node* mp_Pint;
  };
};

#endif // __openma_base_node_p_h
