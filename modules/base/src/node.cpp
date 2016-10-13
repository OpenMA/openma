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

#include "openma/base/node.h"
#include "openma/base/node_p.h"
#include "openma/base/logger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
  NodePrivate::NodePrivate(Node* pint, const std::string& name)
  : ObjectPrivate(),
    Name(name), Description(), DynamicProperties(), Parents(), Children(),
    mp_Pint(pint)
  {};
  
  NodePrivate::~NodePrivate() _OPENMA_NOEXCEPT = default;
  
  bool NodePrivate::retrievePath(std::vector<const Node*>& path, const Node* current, const Node* stop)
  {
    const auto& children = current->children();
    // Direct child search
    for (auto it = children.cbegin() ; it != children.cend() ; ++it)
    {
      if (*it == stop)
      {
        path.push_back(current);
        path.push_back(*it);
        return true;
      };
    }
    // Deep child search
    for (const auto& child : children)
    {
      std::vector<const Node*> temp;
      if (NodePrivate::retrievePath(temp,child,stop))
      {
        path.push_back(current);
        path.insert(path.end(),temp.begin(),temp.end());
        return true;
      }
    }
    return false;
  };
  
  /*
   * Checks if @a node is already a parent and add it if it is not the case.
   * In case @a node is already a parent a warning message is send to the message logger.
   * This method returns also false if the given @a node is null.
   */
  bool NodePrivate::attachParent(Node* node) _OPENMA_NOEXCEPT
  {
    if (node == nullptr)
      return false;
    for (const auto& parent : this->Parents)
    {
      if (parent == node)
      {
        warning("The parent '%s' was already attached to the node '%s'", node->name().c_str(), this->Name.c_str());
        return false;
      }
    }
    this->Parents.push_back(node);
    return true;
  };
  
  /*
   * Removes the given @a node if it is a parent of the current object.
   * In case @a node is not a parent or is null, this method returns false.
   */
  bool NodePrivate::detachParent(Node* node) _OPENMA_NOEXCEPT
  {
    if (node == nullptr)
      return false;
    for (auto it = this->Parents.begin() ; it != this->Parents.end() ; ++it)
    {
      if (*it == node)
      {
        this->Parents.erase(it);
        return true;
      }
    }
    return false;
  };
  
  /*
   * Checks if @a node is already a child and add it if it is not the case.
   * In case @a node is already a child, this method returns false, true otherwise.
   * This method returns also false if the given @a node is null.
   */
  bool NodePrivate::attachChild(Node* node) _OPENMA_NOEXCEPT
  {
    if (node == nullptr)
      return false;
    for (const auto& child :this->Children)
    {
      if (child == node)
        return false;
    }
    this->Children.push_back(node);
    return true;
  };
  
  /*
   * Removes the given @a node if it is a child of the current object.
   */
  bool NodePrivate::detachChild(Node* node) _OPENMA_NOEXCEPT
  {
    if (node == nullptr)
      return false;
    for (auto it = this->Children.begin() ; it != this->Children.end() ; ++it)
    {
      if (*it == node)
      {
        this->Children.erase(it);
        return true;
      }
    }
    return false;
  };
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::Node);

namespace ma
{
  /**
   * @class Node openma/base/node.h
   * @brief Base class for the data structure
   *
   * The data struture in OpenMA is mostly a tree-like structure (more exactly it is a graph as a node can have several parents).
   * The general idea is to store data in a dynamic structure without the need to modify the internal storage each time a new category of data is added (e.g. pressure, GPS, etc.).
   * Thus, it would be simpler to integrate new kind of file formats as well as new models.
   *
   * Children nodes are owned by their parents. Thus only the first parent(s) (e.g. the root' tree) has to be deleted. 
   * This one can be stored in a smart pointer (shared, unique pointer) to not manage its deletion.
   * For example:
   *
   * @code{.unparsed}
   * ma::Node* root = new ma::Node("root");
   * ma::Node* leafA = new ma::Node("leafA",&root); // Owned by the root
   * ma::Node* leafB = new ma::Node("leafB",&root); // Owned by the root
   * // ...
   * delete leafB; // Early deletion. LeafB is removed from the root's children.
   * // ...
   * delete root; // End of the program/function, the root is deleted and leafA at the same time.
   * @endcode
   *
   * It is strongly adviced to create nodes on the heap (using the new operator). This is important for the child management when some of them might be replaced internally (for example using the method replaceChild()), otherwise your program might crash due to an undefined behaviour.
   * 
   * In case you want to use Node objects only as a tree data structure without later modification (e.g. child replacement, child deletion), these ones can be created on the stack (using regular constructor), they must follow a specific order: the parent must be created before the children. 
   * For example:
   *
   * @code{.unparsed}
   * ma::Node root("root");
   * ma::Node leafA("leafA",&root); // Owned by the root
   * ma::Node leafB("leafB",&root); // Owned by the root
   * // ...
   * // end of the program/function. Nodes leafB, leafA, and root are destroyed in thid order.
   * @endcode
   *
   * In case this order is not respected, a runtime error (or crash) should occur like in the next example:
   *
   * @code{.unparsed}
   * ma::Node leafA("leafA");
   * ma::Node root("root");
   * ma::Node leafB("leafB",&root); // Owned by the root
   * leafA.addParent(&root); // Owned by the root
   * // ...
   * // End of the program/function, leafB is destroyed, then root which destroys also its children. What about leafA?
   * @endcode
   *
   * In the previous example, the remaining child of root (pointer to leafA) is a local variable and calling its destructor is incorrect. Thus, when the variable leafA goes out of scope, its destructor is called again. The same memory is freed two times that should crash the program.
   *
   * Finally, to declare a custom node type (i.e. a new inheriting class), several macros must be used:
   * - OPENMA_EXPORT_STATIC_TYPEID() and OPENMA_INSTANCE_STATIC_TYPEID()
   * - OPENMA_DECLARE_NODEID()
   * - OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED()
   *
   * For example if you want to add a class inside OpenMA (base mdoule), it is advised to do as following 
   * @code{.unparsed}
   * // HEADER FILE (e.g MyNode.h)
   *
   * // Public API
   *
   * namespace ma
   * {
   *
   * class OPENMA_BASE_EXPORT MyNode : public ma::Node
   * {
   *   OPENMA_DECLARE_PIMPL_ACCESSOR(MyNode)
   *   OPENMA_DECLARE_NODEID(MyNode, Node)
   *
   * public:
   *   // ...
   * }
   *
   * };
   *
   * OPENMA_EXPORT_STATIC_TYPEID(ma::MyNode, OPENMA_BASE_EXPORT);
   *
   * // PRIVATE API (declared for example in a private header file)
   * class MyNode;
   *
   * class MyNodePrivate : public NodePrivate
   * {
   *   OPENMA_DECLARE_PINT_ACCESSOR(MyNode)
   * 
   *   OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(MyNode, Node,
   *     Property<MyNode,double,&MyNode::time,&MyNode::setTime>{"time"}
   *   )
   *
   * public:
   *   // ...
   * }; 
   *
   * };
   *
   * // SOURCE FILE (e.g. MyNode.cpp)
   *
   * OPENMA_INSTANCE_STATIC_TYPEID(ma::MyNode);
   *
   * namespace ma
   * {
   * // Definition of the class MyNode...
   *
   * @endcode
   *
   * For more details about properties you can read the documentation of the class Property.
   *
   * @ingroup openma_base
   */
  
#ifdef DOXYGEN_SHOULD_TAKE_THIS
  /** * @brief Fake structure to create node's properties */
  struct Node::__Doxygen_Properties
  {
  /**
   * This property holds the description of a Node. By default, this property contains an empty string.
   * @sa description() setDescription()
   */
  std::string description;
  /**
   * This property holds the name of a Node. By default, this property contains an empty string.
   * @sa name() setName()
   */
  std::string name;
  };
#endif
  
  /**
   * Constructor. The @a parent node takes ownership of this object.
   * @note It is adviced to have a unique @a name. This would simplify the research of specific nodes with the methods findChild() and findChildren().
   */
  Node::Node(const std::string& name, Node* parent)
  : Node(*new NodePrivate(this,name),parent)
  {};
  
  /**
   * Destructor.
   * Detach this object of these children. In case a child has no more parent, it is deleted.
   * Detach also this object of these parents
   */
  Node::~Node() _OPENMA_NOEXCEPT
  {
    this->clear();
  };

  /**
   * Returns the name of the node.
   * You can also access to this information using the property 'name'.
   */
  const std::string& Node::name() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Name;
  };
  
 /**
  * Sets the name of the node.
  * You can also modify this information using the property 'name'.
  * In case the value is different, the state of the node is set to modified.
  */
  void Node::setName(const std::string& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Name)
      return;
    optr->Name = value;
    this->modified();
  };
  
  /**
   * Returns the description of the node. By default the description is empty.
   * You can also access to this information using the property 'description'.
   */
  const std::string& Node::description() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Description;
  };
  
  /**
   * Sets the description of the node. By default the description is empty.
   * You can also modify this information using the property 'description'.
   * @note The state of the node is not modified if the description is different.
   */
  void Node::setDescription(const std::string& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Description = value;
  };
  
  /**
   * Returns the value's property associated to the given @a key.
   * The value is a Any object and can be converted to several types implicity.
   * For example, the following code shows two ways to extract property's value.
   *
   * @code{.unparsed}
   * ma::Node node("node");
   * node.setProperty("count","1");
   * // First way: use a Any object
   * ma::Any value = node.property("count");
   * int count1 = value.cast<int>();
   *  // Second way: implicit type conversion operator
   * int count2 = node.property("count");
   * @endcode
   */ 
  Any Node::property(const std::string& key) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    Any value;
    bool caught = optr->staticProperty(key.c_str(),&value);
    if (!caught)
    {
      std::unordered_map<std::string,Any>::const_iterator it = optr->DynamicProperties.find(key);
      if (it != optr->DynamicProperties.end())
        value = it->second;
    }
    return value;
  };
  
  /**
   * Sets the property @a key with the given @a value.
   * The @a value can be an Any object or a type supported by it (e.g. int, double, std::string,).
   * In case the property does not exist, or its value is different, or it is removed, the state of the node is set to modified.
   */
  void Node::setProperty(const std::string& key, const Any& value)
  {
    auto optr = this->pimpl();
    bool caught = optr->setStaticProperty(key.c_str(),&value);
    if (!caught)
    {
      auto it = optr->DynamicProperties.find(key);
      // Existing property
      if (it != optr->DynamicProperties.end())
      {
        // Modify its value
        if (value.isValid())
        {
          if (value != it->second)
          {
            it->second = value;
            this->modified();
          }
        }
        // Or remove it
        else
        {
          optr->DynamicProperties.erase(it);
          this->modified();
        }
      }
      // In case it does not exist add it
      else if (value.isValid())
      {
        optr->DynamicProperties[key] = value;
        this->modified();
      }
    }
  };
  
  /**
   * Returns the dynamic properties
   */
  const std::unordered_map<std::string, Any>& Node::dynamicProperties() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->DynamicProperties;
  };
  
  /**
   * @fn template <typename U = Node*> U Node::child(unsigned index) const _OPENMA_NOEXCEPT
   * Returns the node associated with the given @a index or null if out of range.
   * By default the returned type is Node but can be set to any inheriting class. If the set type does not correspond to the internal type for the given @a index, a null value is returned.
   * @code{.unparsed}
   * auto root = ma::Node("root");
   * auto ev1 = ma::Event("LHE");
   * auto out_of_range = root.child(1); // nullptr returned as the index is out of range
   * auto wrong_cast = root.child<ma::TimeSequence*>(0); // nullptr as the node at index #0 is a ma::Event object
   * auto default_cast = root.child(0); // Return a pointer to a ma::Node object
   * auto event_cast = root.child(0); // Return a pointer to a ma::Event object
   * @endcode
   */
  
  /**
   * Returns the vector of children attached with this node.
   */ 
  const std::vector<Node*>& Node::children() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Children;
  };
  
  /**
   * Returns true if the node has at least one child, false otherwise
   */
  bool Node::hasChildren() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return !optr->Children.empty();
  };
  
  /**
   * Returns the vector of parents attached with this node.
   */ 
  const std::vector<Node*>& Node::parents() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Parents;
  };
  
  /**
   * Returns true if the node has at least one parent, false otherwise.
   */
  bool Node::hasParents() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return !optr->Parents.empty();
  };
  
  /**
   * Appends a node if this one is not already a parent.
   * In case this node is added, @a node is attached as parent and its state is set to modified.
   */
  void Node::addParent(Node* node) _OPENMA_NOEXCEPT
  {
    if (this->pimpl()->attachParent(node))
    {
      node->pimpl()->attachChild(this);
      node->modified();
    }
  };
  
  /**
   * Remove @a node from associated parents.
   * @note It is the responsability to the developer to delete this node if this one has no more parent.
   */
  void Node::removeParent(Node* node) _OPENMA_NOEXCEPT
  {
    if (this->pimpl()->detachParent(node))
    {
      node->pimpl()->detachChild(this);
      node->modified();
    }
  };
  
  /**
   * Overload method which modify this object as well as all these parents.
   */
  void Node::modified() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    this->Object::modified();
    for (auto& parent : optr->Parents)
      parent->modified();
  };
  
  /**
   * Removes all parents, children and properties
   * If a child has no more parent, this one is deleted.
   */
  void Node::clear() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Parents.empty() && optr->Children.empty() && optr->DynamicProperties.empty())
      return;
    optr->DynamicProperties.clear();
    for (auto it = optr->Children.begin() ; it != optr->Children.end() ; ++it)
    {
      (*it)->pimpl()->detachParent(this);
      if (!(*it)->hasParents())
        delete *it;
    }
    optr->Children.clear();
    for (auto it = optr->Parents.begin() ; it != optr->Parents.end() ; ++it)
      (*it)->pimpl()->detachChild(this);
    optr->Parents.clear();
    this->modified();
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   * @note Each subclass must override this method to correctly do the deep copy.
   */
  Node* Node::clone(Node* parent) const
  {
    auto map = std::unordered_map<const Node*,Node*>();
    return this->cloneContents(parent, map);
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   * @note This method does not copy the parent. If you need to copy the parent, you must use the method addParent() afterwards.
   * @code{.unparsed}
   * // Let's copy a node, and add a parent.
   * Node node("IAmNew");
   * node.copy(source); // 'source' is a pointer to a Node object
   * std::cout << node.hasParents() << std::endl; // The answer is 0
   * node.addParent(parent); // 'parent' is a pointer to a Node object
   * @endcode
   */
  void Node::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    if (source == nullptr)
      return;
    this->clear();
    this->copyContents(source);
    auto map = std::unordered_map<const Node*,Node*>{{source, this}};
    source->cloneChildren(this, map);
  };
  
  /**
   * Constructor to be used by inherited object which want to add informations (static properties, members, etc) to the private implementation.
   */
  Node::Node(NodePrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Object(pimpl)
  {
    this->addParent(parent);
  };
  
  /**
   * Replace the child @a current, by a @a substitute.
   * The node @a current will be deleted if it has no more parent.
   */
  void Node::replaceChild(Node* current, Node* substitute)
  {
    if (current == substitute)
      return;
    if (current != nullptr)
    {
      current->removeParent(this);
      if (!current->hasParents())
        delete current;
    }
    substitute->addParent(this);
    // removeParent() and addParent() internally call modified(). No need to call it explicitely
  };
  
  /**
   * @fn template <typename U = Node*> U Node::findChild(const std::string& name = std::string{}, std::unordered_map<std::string,Any>&& properties = std::unordered_map<std::string,Any>{}, bool recursiveSearch = true) const _OPENMA_NOEXCEPT;
   * Returns the child with the given @a name and which can be casted to the type T. You can refine the search by adding @a properties to match. The search can be done recursively (by default) or only in direct children. The latter is available by setting @a recursiveSearch to false.
   * There are three ways to use this methods.
   *
   * You can explicitely define the type @a T and the @a name.
   * @code{.unparsed}
   * ma::Event* = root.findChild<ma::Event*>("RHS2");
   * @endcode
   *
   * You can also only give the type @a T. Then the first node found with this type will be returned. In this case, no name is given as it is set to a null string (not empty)
   * @code{.unparsed}
   * ma::ForcePlatform* = root.findChild<ma::ForcePlatform*>();
   * @endcode
   *
   * By default, the default type is set to ma::Node*, so you can even search for a "node" without giving the type..
   * @code{.unparsed}
   * ma::Node* foo = root.findChild(); // First child
   * ma::Node* bar = root.findChild("bar"); // A node with the name "bar"
   * @endcode
   *
   * In addition, you can add properties to refine the research. Sometimes this could be usefull to distinguish events with the same name but different properties.
   * As presented in the following examples, it is adviced to give matching properties in an initializer vector using curly brackets due to the use of the move semantics in the method. Inside, each matching property is given as a pair {key, value} given also by an initializer vector. So, for a single matching property, two pairs of curly brackets are used but this is correct.
   * @code{.unparsed}
   * ma::Node events("Events");
   * ma::Event evtA("Foo",0.0,"Right","JDoe",&events);
   * ma::Event evtB("Bar",0.0,"Left","JDoe",&events);
   * ma::Event evtC("Toto",1.1,"Left","JDoe",&events);
   * ma::Event evtD("Toto",1.5,"Right","Babar",&events);
   * events.findChild<ma::Event*>("Toto",{{"time",1.5}}); // pointer to evtD object
   * events.findChild<ma::Event*>({},{{"time",0.0},{"context","Left"}}); // pointer to evtB object
   * @endcode
   *
   * In any case you can set the third argument to false for a search in the direct children only. If no properties are added, the second argument to give is an empty pair or curly brackets (i.e. {}).
   * @code{.unparsed}
   * ma::Event* rhs2 = root.findChild<ma::Event*>("RHS2",{},false);
   * ma::ForcePlatform* fp = root.findChild<ma::ForcePlatform*>({},{},false); // {} and std::string() and "" are the same for the name.
   * ma::Node* foo = root.findChild({},{},false);
   * ma::Node* bar = root.findChild("bar",{},false);
   * @endcode
   */
  
  /**
   * @fn template <typename U = Node*> std::vector<U> Node::findChildren(const std::string& name = std::string{}, std::unordered_map<std::string,Any>&& properties = std::unordered_map<std::string,Any>{}, bool recursiveSearch = true) const _OPENMA_NOEXCEPT
   * Returns the children with the given @a name and which can be casted to the type T. You can refine the search by adding @a properties to match. The search can be done recursively (by default) or only in direct children. The latter is available by setting @a recursiveSearch to false.
   * As with the method findChild(), you can explicitely or implicitely give the type and/or the name of the children. For example:
   * @code{.unparsed}
   * std::vector<ma::Event*> events = root.findChildren<ma::Event*>(); // Find all events
   * std::vector<ma::Event*> footstrikes = root.findChildren<ma::Event*>("Foot Strike"); // Find all foot strike events
   * std::vector<ma::Node*> nodes = root.findChildren("Foot Strike"); // Find all node with the name "Foot Strike"
   * @endcode
   *
   * It is adviced to give matching properties by using an initializer vector due to the use of the move semantics in the method.
   * @code{.unparsed}
   * ma::Node events("Events");
   * ma::Event evtA("Foo",0.0,"Right","JDoe",&events);
   * ma::Event evtB("Bar",0.0,"Left","JDoe",&events);
   * ma::Event evtC("Toto",1.1,"Left","JDoe",&events);
   * ma::Event evtD("Toto",1.5,"Right","Babar",&events);
   * events.findChildren<ma::Event*>({},{{"time",0.0}}); // evtA and evtB
   * events.findChildren<ma::Event*>({},{{"context","Right"}}); // evtA and evtD
   * events.findChildren<ma::Event*>({},{{"context","Left"}}); // evtB and evtC
   * events.findChildren<ma::Event*>({},{{"subject","JDoe"}}); // evtA, evtB, and evtC
   * @endcode
   */
  
  /**
   * @fn template <typename U = Node*, typename V, typename > std::vector<U> Node::findChildren(const V& regexp, std::unordered_map<std::string,Any>&& properties = std::unordered_map<std::string,Any>{}, bool recursiveSearch = true) const _OPENMA_NOEXCEPT
   * Convenient method to find children using a regular expression.
   */
  
  /**
   * template <typename U> U Node::findChild(Node* node) const _OPENMA_NOEXCEPT
   * Convenient method to find a child based on its node ID and its pointer address.
   * This method is internally used for safeguard when an inheriting class store directly child adresses to quickly access to them (aka shortcuts). As a reminder, when a child is removed or replaced, the shorcut is not cleaned in consequence (there is no cleaning mechanisn propagation). Thus, the shortuct is invalid. The use of a safeguard is necessary to not return an invalid address.
   */
  
  /**
   * Retrieves the first path existing between the current node and the given @a node.
   * If no path exists between both, then an empty vector is returned, 
   * The first node in the retrieved path is the current one, while the last is the node to search.
   */
  std::vector<const Node*> Node::retrievePath(const Node* node) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    std::vector<const Node*> path;
    optr->retrievePath(path,this,node);
    return path;
  };
  
  /**
   * Retuns a new object allocated on the heap. The creation of the object can be done using a default constructor (if any).
   * This method is used by cloneContents() method for the default cloned object.
   * @note Each subclass must override this method to correctly create the correct object .
   */
  Node* Node::allocateNew() const
  {
    return new Node(this->name());
  };
  
  
  /**
   * Internal method to copy private implementation member WITHOUT the children.
   * @note Each subclass must override this method to correctly do the deep copy. Because the @a src is a Node object, each inheriting class must use the node_cast() function inside their copy() method to ensure the good type of the source.
   */
  void Node::copyContents(const Node* source) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    auto optr_src = source->pimpl();
    optr->Timestamp = optr_src->Timestamp;
    optr->Name = optr_src->Name;
    optr->Description = optr_src->Description;
    optr->DynamicProperties = optr_src->DynamicProperties;
  };
  
  /**
   * Internal method used to clone an object.
   * This method clone an object using the following steps:
   *  1. Allocate a new object on the heap using allocateNew()
   *  2. Copy the member of the @c this object using copyContents()
   *  3. Clone the children using cloneChildren()
   *  4. Add @a parent to the cloned object as one parent.
   * The use of @a map is necessary to determine children nodes shared inside the tree. 
   */
  Node* Node::cloneContents(Node* parent, std::unordered_map<const Node*,Node*>& map) const
  {
    auto dest = this->allocateNew();
    map.emplace(this,dest);
    this->cloneChildren(dest, map);
    dest->copyContents(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Clone children by managing shared children in the tree
   */
  void Node::cloneChildren(Node* parent, std::unordered_map<const Node*,Node*>& map) const
  {
    auto optr = this->pimpl();
    for (const auto& child : optr->Children)
    {
      auto it = map.find(child);
      if (it == map.cend())
        child->cloneContents(parent, map);
      else
        it->second->addParent(parent);
    }
  };
  
  /**
   * Implementation of the findChild method.
   */
  Node* Node::findNode(typeid_t id, const std::string& name, std::unordered_map<std::string,Any>&& properties, bool recursiveSearch) const _OPENMA_NOEXCEPT
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (const auto& child : optr->Children)
    {
      if (child->isCastable(id) && (name.empty() || (child->name() == name)))
      {
        bool found = true;
        for (const auto& prop : properties)
        {
          if (child->property(prop.first) != prop.second)
          {
            found = false;
            break;
          }
        }
        if (found)
          return child;
      }
    }
    // In case no corresponding child was found and the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (const auto& child : optr->Children)
      {
        Node* node = child->findNode(id,name,std::move(properties),recursiveSearch);
        if (node != nullptr)
          return node;
      }
    }
    return nullptr;
  };
  
  /*
   * Find a children based on its pointer address.
   */
  Node* Node::findNode(typeid_t id, Node* node) const _OPENMA_NOEXCEPT
  {
    if (node != nullptr)
    {
      // Search in the direct children
      auto optr = this->pimpl();
      for (const auto& child : optr->Children)
      {
        if (child->isCastable(id) && (child == node))
          return child;
      }
      // In case no corresponding child was found and the recursive search is actived, let's go deeper
      for (const auto& child : optr->Children)
      {
        if (child->findNode(id,node) != nullptr)
          return node;
      }
    }
    return nullptr;
  };
  
  /**
   * Implementation of the findChildren method.
   */
  void Node::findNodes(std::vector<void*>* vector, typeid_t id, const std::string& name, std::unordered_map<std::string,Any>&& properties, bool recursiveSearch) const _OPENMA_NOEXCEPT
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (const auto& child : optr->Children)
    {
      if (child->isCastable(id) && (name.empty() || (child->name() == name)))
      {
        bool found = true;
        for (const auto& prop : properties)
        {
          if (child->property(prop.first) != prop.second)
          {
            found = false;
            break;
          }
        }
        if (found) vector->emplace_back(child);
      }
        
    }
    // In case the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (const auto& child : optr->Children)
        child->findNodes(vector,id,name,std::move(properties),recursiveSearch);
    }
  };
  
  /**
   * Implementation of the findChildren method.
   */
  void Node::findNodes(std::vector<void*>* vector, typeid_t id, const std::regex& regexp, std::unordered_map<std::string,Any>&& properties, bool recursiveSearch) const _OPENMA_NOEXCEPT
  {
    // Search in the direct children
    auto optr = this->pimpl();
    for (const auto& child : optr->Children)
    {
      if (child->isCastable(id) && std::regex_match(child->name(),regexp))
      {
        bool found = true;
        for (const auto& prop : properties)
        {
          if (child->property(prop.first) != prop.second)
          {
            found = false;
            break;
          }
        }
        if (found) vector->emplace_back(child);
      }
    }
    // In case the recursive search is actived, let's go deeper
    if (recursiveSearch)
    {
      for (const auto& child : optr->Children)
        child->findNodes(vector,id,regexp,std::move(properties),recursiveSearch);
    }
  };
  
  /**
   * Returns true if the current object is isCastable to another with the given @a typeid_t value, false otherwise.
   */
  bool Node::isCastable(typeid_t id) const _OPENMA_NOEXCEPT
  {
    return (static_typeid<Node>() == id);
  };
  
  /**
   * @fn template <typename U> U node_cast(Node* node) _OPENMA_NOEXCEPT
   * Conveniant function to cast a pointer to a Node object to another type U. If it is not possible, this method returns nullptr.
   *
   * @code{.unparsed}
   * ma::Node root("root");
   * ma::Node* event = new ma::Event("RHS1",&root);
   * ma::Event* evt = node_cast<ma::Event*>(event); // Ok
   * ma::TimeSequence* tsq = node_cast<ma::TimeSequence*>(event); // nullptr
   * @endcode
   *
   * @note The type U must be a pointer type which inherit of the Node class. If this is not the case a compilation error will be thrown.
   */
};