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

#include "openma/body/descriptor.h"
#include "openma/body/descriptor_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  // Note: The member Proximal and Distal are used to simplify the mangement of the segment in case one of them is set to null (to represent the global frame).
  
  DescriptorPrivate::DescriptorPrivate(Descriptor* pint, const std::string& name)
  : NodePrivate(pint,name)
  {};
  
  DescriptorPrivate::~DescriptorPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::Descriptor);

namespace ma
{
namespace body
{
  /**
   * @class Descriptor openma/body/descriptor.h
   * @brief Abstract class to describe a node or set of node
   * To generalize the way to describe/measure Node's data, this class proposes an evaluate() method. Internally, it calls prepare(), process() and finalize() methods. These three last methods have to be speciallized by each inheriting object.
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  Descriptor::Descriptor(const std::string& name, Node* parent)
  : Node(*new DescriptorPrivate(this,name), parent)
  {};
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) into the private implementation.
   */
  Descriptor::Descriptor(DescriptorPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Node(pimpl, parent)
  {};
  
  /**
   * Destructor
   */
  Descriptor::~Descriptor() _OPENMA_NOEXCEPT = default;

  /**
   * Call in this order:
   *  1. prepare()
   *  2. process()
   *  3. finalize()
   * These three methods must be overloaded in the intheriting classes. For example, prepare() could extract data from the @a input and store them in an internal buffer. The method process() shall process this internal buffer and store the final result(s). Finally, the method finalize is here to export the result into the @a output. For each method, the @a options are passed. You can imagine to have different behaviour depending of the option set for the evaluation (unit scaling, etc.).
   */
  bool Descriptor::evaluate(Node* output, const Node* input, const std::unordered_map<std::string, Any>& options)
  {
    return (!this->prepare(input, options) || !this->process(options) || !this->finalize(output, options));
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void Descriptor::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const Descriptor*>(source);
    if (src == nullptr)
      return;
    this->Node::copy(src);
  };
  
  /**
   * @fn virtual bool Descriptor::prepare(const Node* input, const std::unordered_map<std::string, Any>& options) = 0;
   * Method to extract/adapt/store the @a input with possibly different behaviours depending of the @a options.
   * @note The way to store the (adapted) @a input is up to the developer. 
   */
  
  /**
   * @fn virtual bool Descriptor::process(const std::unordered_map<std::string, Any>& options) = 0;
   * Transform the stored input into the requested description of the input used by prepare().
   */
  
  /**
   * @fn virtual bool Descriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options) = 0;
   * Export the processed data to the @a ouput.
   */
 };
};