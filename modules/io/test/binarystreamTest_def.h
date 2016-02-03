#ifndef binarystreamTest_def_h
#define binarystreamTest_def_h

#include <openma/io/device.h>
#include <openma/io/device_p.h>
#include <openma/base/macros.h> // _OPENMA_NOEXCEPT
#include <cstring> // memcpy

class DummyBufferPrivate : public ma::io::DevicePrivate
{
public:
  DummyBufferPrivate(char* d) : ma::io::DevicePrivate(), Data(d), Pos(0) {};
  
  char* Data;
  ma::io::Device::Offset Pos;
};

class DummyBuffer : public ma::io::Device
{
  OPENMA_DECLARE_PIMPL_ACCESSOR(DummyBuffer)
  
public:
  DummyBuffer(char* d) : ma::io::Device(*new DummyBufferPrivate(d))
  {};
  virtual bool isOpen() const _OPENMA_NOEXCEPT  override {return true;};
  virtual void close()  override {};
  virtual Size peek(char* s, Size n) const override
  {
    auto optr = this->pimpl();
    memcpy(s,optr->Data + optr->Pos, n);
    return n;
  };
  virtual void read(char* s, Size n) override
  {
    auto optr = this->pimpl();
    optr->Pos += this->peek(s,n);
  };
  virtual void write(const char* s, Size n) override
  {
    auto optr = this->pimpl();
    memcpy(optr->Data + optr->Pos, s, n);
    optr->Pos += n;
  };
  virtual void seek(Offset , Origin ) override {}; // Not implemented
  virtual Position tell() const _OPENMA_NOEXCEPT  override {return Position(Offset(-1));}; // Not implemented
  virtual bool isSequential() const _OPENMA_NOEXCEPT  override {return true;};
  virtual const char* data() const _OPENMA_NOEXCEPT override {return nullptr;};
  virtual Size size() const _OPENMA_NOEXCEPT override {return 0;};
  
  void setPos(Offset pos)
  {
    auto optr = this->pimpl();
    optr->Pos = pos;
  }
};

#endif // binarystreamTest_def_h