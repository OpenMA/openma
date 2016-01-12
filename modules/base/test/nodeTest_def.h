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
  void copy(const TestNode* src) _OPENMA_NOEXCEPT;
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

void TestNode::copy(const TestNode* src) _OPENMA_NOEXCEPT
{
  if (src == nullptr)
    return;
  this->Node::copy(src);
  auto optr = this->pimpl();
  auto optr_src = src->pimpl();
  optr->Name = optr_src->Name;
  optr->Version = optr_src->Version;
};

#endif // nodeTest_def_h