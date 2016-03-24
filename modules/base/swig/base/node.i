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

// MUST BE DEFINED BEFORE THE NODE INTERFACE
%{
void ma_Node_clear(ma::Node* self)
{
  int rc = 1;
  // Unref parents
  auto& p1 = self->parents();
  for (auto parent : p1)
  {
    rc = _SWIG_ma_Node_unref(self);
    self->removeParent(parent);
  }
  assert(rc >= 1);
  // Unref children
  auto& c1 = self->children();
  for (auto child : c1)
    _SWIG_ma_Node_unref(child);
  // Detach remaining children (still in the workspace)
  auto& c2 = self->children();
  for (auto child : c2)
    child->removeParent(self);
  // Node clear
  self->clear();
  // Reset the reference counter
  _SWIG_ma_Node_reset(self, rc);
};
%};

namespace ma
{
  SWIG_TYPEMAP_OUT_CONSTRUCTOR(Node)
  
  %newobject Node::child; // Used for the reference counting
  %nodefaultctor;
  class Node : public Object
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(Node, SWIGTYPE)
    SWIG_EXTEND_DEEPCOPY(Node)
  
    Node(const std::string& name, Node* parent = nullptr);
    ~Node();
    const std::string& name() const;
    void setName(const std::string& value);
    const std::string& description() const;
    void setDescription(const std::string& value);
    ma::Any property(const std::string& key) const;
    void setProperty(const std::string& key, const ma::Any& value);
    // TODO Extend setProperty to use directly the SWIGTYPE.
    /*
    %extend {
      void setProperty(const std::string& key, const SWIGTYPE* value);
    };
    */
    const std::unordered_map<std::string, Any>& dynamicProperties() const;
  /*
    template <typename U = Node*> U child(unsigned index) const;
  */
  
    %extend {
      Node* child(unsigned index) const;
      void clear();
    }
  
    const std::vector<Node*>& children() const;
    bool hasChildren() const;
    const std::vector<Node*>& parents() const;
    bool hasParents() const;
  /*
    void addParent(Node* node);
    void removeParent(Node* node);
    template <typename U = Node*> U findChild(const std::string& name = std::string{}, std::vector<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const;
    template <typename U = Node*> std::vector<U> findChildren(const std::string& name = std::string{}, std::vector<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const;
    template <typename U = Node*, typename V, typename = typename std::enable_if<std::is_same<std::regex, V>::value>::type> std::vector<U> findChildren(const V& regexp, std::vector<std::pair<std::string,Any>>&& properties = {}, bool recursiveSearch = true) const;
    std::vector<const Node*> retrievePath(const Node* node) const;
  */
    virtual void modified();
  };
  %clearnodefaultctor;
};
