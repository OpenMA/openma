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

%typemap(in) const ma::Any& (ma::Any temp)
{
  $1 = &temp;
  // Value (ma::Any)
  void* value = nullptr;
  if (SWIG_IsOK(SWIG_ConvertPtr($input, &value, SWIGTYPE_p_ma__Any, 0)))
  {
    if (!value) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference for 'ma::Any' in 'ma::Any const &' typemap (in)"); 
    }
    *$1 = (ma::Any const &)*reinterpret_cast<ma::Any*>(value);
  }
  // Value (other)
  else
  {
    ma_Any_assign($1, $input);
  }  
}

namespace ma
{
  SWIG_TYPEMAP_NODE_OUT(ma, Node)
  SWIG_CREATE_TEMPLATE_HELPER_1(ma, Node, SWIGTYPE)
  
  %nodefaultctor;
  class Node : public Object
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(ma, Node, SWIGTYPE)
  
    Node(const std::string& name, Node* parent = nullptr);
    ~Node();
    const std::string& name() const;
    void setName(const std::string& value);
    const std::string& description() const;
    void setDescription(const std::string& value);
    ma::Any property(const std::string& key) const;
    void setProperty(const std::string& key, const ma::Any& value);
    std::unordered_map<std::string, Any> dynamicProperties() const;
    /*
    template <typename U = Node*> U child(unsigned index) const;
    */
    %extend {
      Node* child(unsigned index) const;
      SWIGTYPE* findChild(const ma::bindings::TemplateHelper* id, const std::string& name = std::string(), std::unordered_map<std::string,ma::Any>&& properties = std::unordered_map<std::string,ma::Any>(), bool recursiveSearch = true);
      SWIGTYPE* findChildren(const ma::bindings::TemplateHelper* id, const std::string& regexp = ".*", std::unordered_map<std::string,ma::Any>&& properties = std::unordered_map<std::string,ma::Any>(), bool recursiveSearch = true);
    }
    const std::vector<Node*>& children() const;
    bool hasChildren() const;
    const std::vector<Node*>& parents() const;
    bool hasParents() const;
    void addParent(Node* node);
    void removeParent(Node* node);
    void copy(Node* source);
    ma::Node* clone(Node* parent = nullptr) const;
    void clear();
    /*
    template <typename U = Node*> U findChild(const std::string& name = std::string{}, std::unordered_map<std::string,Any>&& properties = std::unordered_map<std::string,Any>{}, bool recursiveSearch = true) const;
    template <typename U = Node*> std::vector<U> findChildren(const std::string& name = std::string{}, std::unordered_map<std::string,Any>&& properties = std::unordered_map<std::string,Any>{}, bool recursiveSearch = true) const;
    template <typename U = Node*, typename V, typename = typename std::enable_if<std::is_same<std::regex, V>::value>::type> std::vector<U> findChildren(const V& regexp, std::unordered_map<std::string,Any>&& properties = std::unordered_map<std::string,Any>{}, bool recursiveSearch = true) const;
    std::vector<const Node*> retrievePath(const Node* node) const;
    */
    virtual void modified();
    int refcount() const;
  };
  %clearnodefaultctor;
};

%{

ma::Node* ma_Node_child(const ma::Node* self, unsigned index)
{
#if defined(SWIGMATLAB)
  if ((index > 0) && (index <= self->children().size()))
    return self->child(index-1);
#else
  if (index < self->children().size())
    return self->child(index);
#endif
  else
  {
    SWIG_SendError(SWIG_IndexError, "Index out of range");
    return nullptr;
  }
};

SWIGTYPE* ma_Node_findChild(const ma::Node* self, const ma::bindings::TemplateHelper* id, const std::string& name = std::string(), std::unordered_map<std::string,ma::Any>&& properties = std::unordered_map<std::string,ma::Any>(), bool recursiveSearch = true)
{
  SWIGTYPE* out = nullptr;
  id->findChild(&out, *(id->SwigType), self, name, std::move(properties), recursiveSearch);
  if (out == nullptr)
    return SWIG_NewNullObj();
  return out;
};

SWIGTYPE* ma_Node_findChildren(const ma::Node* self, const ma::bindings::TemplateHelper* id, const std::string& regexp = ".*", std::unordered_map<std::string,ma::Any>&& properties = std::unordered_map<std::string,ma::Any>(), bool recursiveSearch = true)
{
  SWIGTYPE* out = nullptr;
  id->findChildren(&out, *(id->SwigType), self, regexp, std::move(properties), recursiveSearch);
  if (out == nullptr)
    SWIG_SendError(SWIG_SystemError,"Internal error during container allocation");
  return out;
};

%}
