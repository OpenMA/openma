#ifndef nodeTest_def_h
#define nodeTest_def_h

#include <openma/base/node.h>
#include <openma/base/node_p.h>
#include <openma/base/property.h>

class TestNodePrivate;

class TestNode : public ma::Node
{
  OPENMA_DECLARE_PIMPL_ACCESSOR(TestNode)
  OPENMA_DECLARE_NODEID(TestNode, ma::Node)
  
public:
  TestNode(const std::string& name, Node* parent = nullptr);
  
  int version() const;
  void setVersion(int value);
  
  int count() const;

protected:
  virtual ma::Node* allocateNew() const override;
  virtual void copyContents(const ma::Node* source) _OPENMA_NOEXCEPT override;
};

class TestNodePrivate : public ma::NodePrivate
{
  OPENMA_DECLARE_PINT_ACCESSOR(TestNode)
  
  OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(TestNode, ma::Node,
    ma::Property<TestNode,int,&TestNode::version,&TestNode::setVersion>{"version"}
  )
  
public:
  TestNodePrivate(TestNode* pint, const std::string& name) :  ma::NodePrivate(pint,name), Version(1) {};
  
  int Version;
};

TestNode::TestNode(const std::string& name, Node* parent)
: ma::Node(*new TestNodePrivate(this,name), parent)
{};

int TestNode::version() const
{
  auto optr = this->pimpl();
  return optr->Version;
};

void TestNode::setVersion(int value)
{
  auto optr = this->pimpl();
  if (value == optr->Version)
    return;
  optr->Version = value;
};

int TestNode::count() const
{
  auto optr = this->pimpl();
  int num = 1;
  for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
  {
    TestNode* node = ma::node_cast<TestNode*>(*it);
    if (node != nullptr)
      num += node->count();
  }
  return num;
};

ma::Node* TestNode::allocateNew() const
{
  return new TestNode(this->name());
};

void TestNode::copyContents(const ma::Node* source) _OPENMA_NOEXCEPT
{
  auto src = ma::node_cast<const TestNode*>(source);
  if (src == nullptr)
    return;
  this->Node::copyContents(src);
  auto optr = this->pimpl();
  auto optr_src = src->pimpl();
  optr->Version = optr_src->Version;
};

// ------------------------------------------------------------------------- //

class TestNode2Private;

class TestNode2 : public ma::Node
{
  OPENMA_DECLARE_PIMPL_ACCESSOR(TestNode2)
  OPENMA_DECLARE_NODEID(TestNode2, ma::Node)
  
public:
  TestNode2(const std::string& name, Node* parent = nullptr);
  
  int version() const;
  void setVersion(int value);
  
  int count() const;
  
protected:
  virtual ma::Node* allocateNew() const override;
  virtual void copyContents(const ma::Node* source) _OPENMA_NOEXCEPT override;
  
private:
  std::unique_ptr<TestNode2Private> mp_Pimpl;
};

class TestNode2Private
{
  OPENMA_DECLARE_PINT_ACCESSOR(TestNode2)
  
  OPENMA_DECLARE_STATIC_PROPERTIES_BASE(TestNode2,
    ma::Property<TestNode2,int,&TestNode2::version,&TestNode2::setVersion>{"version"}
  )
    
  TestNode2* mp_Pint;
  
public:
  TestNode2Private(TestNode2* pint) : mp_Pint(pint), Version(2) {};
  virtual ~TestNode2Private() = default;
  int Version;
};

TestNode2::TestNode2(const std::string& name, Node* parent)
: ma::Node(name, parent), mp_Pimpl(new TestNode2Private(this))
{};

int TestNode2::version() const
{
  auto optr = this->pimpl();
  return optr->Version;
};

void TestNode2::setVersion(int value)
{
  auto optr = this->pimpl();
  if (value == optr->Version)
    return;
  optr->Version = value;
};

int TestNode2::count() const
{
  int num = 1;
  for (auto it = this->children().cbegin() ; it != this->children().cend() ; ++it)
  {
    TestNode2* node = ma::node_cast<TestNode2*>(*it);
    if (node != nullptr)
      num += node->count();
  }
  return num;
};

ma::Node* TestNode2::allocateNew() const
{
  return new TestNode2(this->name());
};

void TestNode2::copyContents(const ma::Node* source) _OPENMA_NOEXCEPT
{
  auto src = ma::node_cast<const TestNode2*>(source);
  if (src == nullptr)
    return;
  this->Node::copyContents(src);
  auto optr = this->pimpl();
  auto optr_src = src->pimpl();
  optr->Version = optr_src->Version;
};

// ------------------------------------------------------------------------- //

class TestNode3Private;

class TestNode3 : public ma::Node
{
  OPENMA_DECLARE_PIMPL_ACCESSOR(TestNode3)
  OPENMA_DECLARE_NODEID(TestNode3, ma::Node)
  
public:
  TestNode3(const std::string& name, Node* parent = nullptr);
  
  Node* shortcut() const _OPENMA_NOEXCEPT;
  void setShortcut(Node* value);

protected:
  virtual ma::Node* allocateNew() const override;
  virtual void copyContents(const ma::Node* source) _OPENMA_NOEXCEPT override;
};

class TestNode3Private : public ma::NodePrivate
{
  OPENMA_DECLARE_PINT_ACCESSOR(TestNode3)
  
public:
  TestNode3Private(TestNode3* pint, const std::string& name) :  ma::NodePrivate(pint,name), Shortcut(nullptr) {};
  
  ma::Node* Shortcut;
};

TestNode3::TestNode3(const std::string& name, Node* parent)
: ma::Node(*new TestNode3Private(this,name), parent)
{};

ma::Node* TestNode3::shortcut() const _OPENMA_NOEXCEPT
{
  auto optr = this->pimpl();
  return this->findChild(optr->Shortcut);
};

void TestNode3::setShortcut(Node* value)
{
  auto optr = this->pimpl();
  if (value == optr->Shortcut)
    return;
  if (this->findChild(optr->Shortcut) != nullptr)
  {
    optr->Shortcut->removeParent(this);
    if (!optr->Shortcut->hasParents())
      delete optr->Shortcut;
  }
  if (value != nullptr)
    value->addParent(this);
  optr->Shortcut = value;
};

ma::Node* TestNode3::allocateNew() const
{
  return new TestNode3(this->name());
};

void TestNode3::copyContents(const ma::Node* source) _OPENMA_NOEXCEPT
{
  auto src = ma::node_cast<const TestNode3*>(source);
  if (src == nullptr)
    return;
  this->Node::copyContents(src);
  auto optr = this->pimpl();
  auto optr_src = src->pimpl();
  if (src->findChild(optr_src->Shortcut) != nullptr)
    optr->Shortcut = this->findChild(optr_src->Shortcut->name());
};

#endif // nodeTest_def_h