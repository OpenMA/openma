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
  
  TestNode* clone(Node* parent = nullptr) const;
  void copy(const Node* source) _OPENMA_NOEXCEPT;
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

TestNode* TestNode::clone(Node* parent) const
{
  auto dest = new TestNode(this->name());
  dest->copy(this);
  dest->addParent(parent);
  return dest;
};

void TestNode::copy(const Node* source) _OPENMA_NOEXCEPT
{
  auto src = ma::node_cast<const TestNode*>(source);
  if (src == nullptr)
    return;
  this->Node::copy(src);
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
  
  TestNode2* clone(Node* parent = nullptr) const;
  void copy(const Node* source) _OPENMA_NOEXCEPT;
  
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

TestNode2* TestNode2::clone(Node* parent) const
{
  auto dest = new TestNode2(this->name());
  dest->copy(this);
  dest->addParent(parent);
  return dest;
};

void TestNode2::copy(const Node* source) _OPENMA_NOEXCEPT
{
  auto src = ma::node_cast<const TestNode2*>(source);
  if (src == nullptr)
    return;
  this->Node::copy(src);
  auto optr = this->pimpl();
  auto optr_src = src->pimpl();
  optr->Version = optr_src->Version;
};

#endif // nodeTest_def_h