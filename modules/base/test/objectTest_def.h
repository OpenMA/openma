#ifndef objectTest_def_h
#define objectTest_def_h

#include <openma/base/object.h>

// ------------------------------------------------------------------------- //

// Test object using default private implementation of the ma::Object class

class ObjectDefaultPimpl : public ma::Object
{
public:
  ObjectDefaultPimpl() = default;
};

// ------------------------------------------------------------------------- //

// Test object using custom private implementation of the d-pointer used in the ma::Object class

class ObjectCustomPimplPrivate;

class ObjectCustomPimpl : public ma::Object
{
  OPENMA_DECLARE_PIMPL_ACCESSOR(ObjectCustomPimpl)
    
public:
  const std::string& name() const;
  void setName(const std::string& name);
  int version() const;
  void setVersion(int ver);
  
  ObjectCustomPimpl() = delete;
  ObjectCustomPimpl(const std::string& name);
  
  virtual ObjectCustomPimpl* clone() const;
  virtual void copy(const ObjectCustomPimpl* src) _OPENMA_NOEXCEPT;
  
  
protected:
  ObjectCustomPimpl(ObjectCustomPimplPrivate& pimpl) _OPENMA_NOEXCEPT;
  
  std::unique_ptr<ObjectCustomPimplPrivate> mp_Pimpl;
};

class ObjectCustomPimplPrivate
{
public:
  ObjectCustomPimplPrivate(const std::string& name="")
  : Name(name), Version(1) {};
  
  ObjectCustomPimplPrivate(const ObjectCustomPimplPrivate& ) = delete;
  ObjectCustomPimplPrivate(ObjectCustomPimplPrivate&& ) _OPENMA_NOEXCEPT = delete;
  ObjectCustomPimplPrivate& operator=(const ObjectCustomPimplPrivate& ) = delete;
  ObjectCustomPimplPrivate& operator=(const ObjectCustomPimplPrivate&& ) _OPENMA_NOEXCEPT = delete;
  
  std::string Name;
  int Version;
};

ObjectCustomPimpl::ObjectCustomPimpl(const std::string& name)
: ma::Object(), mp_Pimpl(new ObjectCustomPimplPrivate(name))
{};

ObjectCustomPimpl::ObjectCustomPimpl(ObjectCustomPimplPrivate& pimpl) _OPENMA_NOEXCEPT
: ma::Object(), mp_Pimpl(&pimpl)
{};

const std::string& ObjectCustomPimpl::name() const
{
  auto optr = this->pimpl();
  return optr->Name;
};

void ObjectCustomPimpl::setName(const std::string& val)
{
  auto optr = this->pimpl();
  if (optr->Name != val)
  {
    optr->Name = val;
    this->modified();
  }
};

int ObjectCustomPimpl::version() const
{
  auto optr = this->pimpl();
  return optr->Version;
};

void ObjectCustomPimpl::setVersion(int val)
{
  auto optr = this->pimpl();
  if (optr->Version != val)
  {
    optr->Version = val;
    this->modified();
  }
};

ObjectCustomPimpl* ObjectCustomPimpl::clone() const
{
  auto dest = new ObjectCustomPimpl(*new ObjectCustomPimplPrivate);
  dest->copy(this);
  return dest;
};

void ObjectCustomPimpl::copy(const ObjectCustomPimpl* src) _OPENMA_NOEXCEPT
{
  if (src == nullptr)
    return;
  this->setTimestamp(src->timestamp());
  auto optr = this->pimpl();
  auto optr_src = src->pimpl();
  optr->Name = optr_src->Name;
  optr->Version = optr_src->Version;
};

#endif //objectTest_def_h