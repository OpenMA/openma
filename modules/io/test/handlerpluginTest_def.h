#ifndef handlerpluginTest_def_h
#define handlerpluginTest_def_h

#include "openma/io/handlerplugin.h"
#include "openma/io/handler.h"
#include "openma/io/device.h"
#include "openma/io/enums.h"
#include "openma/base/node.h"

class DummyHandlerOne;
class DummyHandlerTwo;

#define _OPENMA_IO_TEST_DUMMYONE_FORMAT "openma.dummy1"
#define _OPENMA_IO_TEST_DUMMYTWO_FORMAT "openma.dummy2"

class DummyHandlerPlugin : public ma::io::HandlerPlugin
{
public:
  DummyHandlerPlugin() : ma::io::HandlerPlugin() {};
  
  virtual std::vector<std::string> supportedFormats() const _OPENMA_NOEXCEPT final;
  
  virtual ma::io::Capability capabilities(const std::string& format) const _OPENMA_NOEXCEPT final;
  virtual ma::io::Signature detectSignature(const ma::io::Device* const device, std::string* format = nullptr) const _OPENMA_NOEXCEPT final;
  
  virtual ma::io::Handler* create(ma::io::Device* device, const std::string& format) final;
};

// ------------------------------------------------------------------------- //

class DummyHandlerOne : public ma::io::Handler
{
public:
  static ma::io::Signature verifySignature(const ma::io::Device* const device) _OPENMA_NOEXCEPT;
  
protected:
  virtual ma::io::Signature verifySignature() const _OPENMA_NOEXCEPT final;
  virtual void readDevice(ma::Node* output) final;
  virtual void writeDevice(const ma::Node* const input) final;
};

// ------------------------------------------------------------------------- //

class DummyHandlerTwo : public ma::io::Handler
{
public:
  static ma::io::Signature verifySignature(const ma::io::Device* const device) _OPENMA_NOEXCEPT;
  
  virtual ma::io::Signature verifySignature() const _OPENMA_NOEXCEPT final;
  
protected:
  virtual void readDevice(ma::Node* output) final;
  // virtual void writeDevice(const Node* input) final; Not Write capability
};

class DummyDeviceTwo : public ma::io::Device
{
public:
  DummyDeviceTwo() : ma::io::Device()
  {
    this->setName("myfile.dummy2");
    this->setOpenMode(ma::io::Mode::In);
  };
  virtual bool isOpen() const _OPENMA_NOEXCEPT final {return true;};
  virtual void close() final {};
  virtual Size peek(char* , Size ) const final {return 0;};
  virtual void read(char* , Size ) final {};
  virtual void write(const char* , Size ) final {};
  
  // Random access IO device only
  virtual void seek(Offset , ma::io::Origin ) final {};
  virtual Position tell() const _OPENMA_NOEXCEPT final {return 0;};
  virtual const char* data() const _OPENMA_NOEXCEPT final {return nullptr;};
  virtual Size size() const _OPENMA_NOEXCEPT final {return 0;};
  
  // Sequential IO only
  virtual bool isSequential() const _OPENMA_NOEXCEPT final {return false;};
};

// ------------------------------------------------------------------------- //

std::vector<std::string> DummyHandlerPlugin::supportedFormats() const _OPENMA_NOEXCEPT
{
  return {_OPENMA_IO_TEST_DUMMYONE_FORMAT,_OPENMA_IO_TEST_DUMMYTWO_FORMAT};
};

ma::io::Capability DummyHandlerPlugin::capabilities(const std::string& format) const _OPENMA_NOEXCEPT
{
  ma::io::Capability capability = ma::io::Capability::None;
  if (format.compare(_OPENMA_IO_TEST_DUMMYONE_FORMAT) == 0)
  {
    capability = ma::io::Capability::CanReadAndWrite;
  }
  else if (format.compare(_OPENMA_IO_TEST_DUMMYTWO_FORMAT) == 0)
  {
    capability = ma::io::Capability::CanRead;
  }
  return capability;
};

ma::io::Signature DummyHandlerPlugin::detectSignature(const ma::io::Device* const device, std::string* format) const _OPENMA_NOEXCEPT
{
  ma::io::Signature detected = ma::io::Signature::Invalid;
  if ((detected = DummyHandlerOne::verifySignature(device)) == ma::io::Signature::Valid)
  {
    if (format != nullptr)
      *format = _OPENMA_IO_TEST_DUMMYONE_FORMAT;
  }
  else if ((detected = DummyHandlerTwo::verifySignature(device)) == ma::io::Signature::Valid)
  {
    if (format != nullptr)
      *format = _OPENMA_IO_TEST_DUMMYTWO_FORMAT;
  }
  return detected;
};

ma::io::Handler* DummyHandlerPlugin::create(ma::io::Device* device, const std::string& format)
{
  ma::io::Handler* handler = nullptr;
  if (format.compare(_OPENMA_IO_TEST_DUMMYONE_FORMAT) == 0)
  {
    handler = new DummyHandlerOne;
    handler->setDevice(device);
  }
  else if (format.compare(_OPENMA_IO_TEST_DUMMYTWO_FORMAT) == 0)
  {
    handler = new DummyHandlerTwo;
    handler->setDevice(device);
  }
  return handler;
};

// ------------------------------------------------------------------------- //

ma::io::Signature DummyHandlerOne::verifySignature(const ma::io::Device* const device) _OPENMA_NOEXCEPT
{
  char signature[2] = {0};
  device->peek(signature,sizeof(signature));
  if ((signature[0] <= 0) || (signature[1] != 0x50))
    return ma::io::Signature::Invalid;
  return ma::io::Signature::Valid;  
};
  
ma::io::Signature DummyHandlerOne::verifySignature() const _OPENMA_NOEXCEPT
{
  return DummyHandlerOne::verifySignature(this->device());
};
  
void DummyHandlerOne::readDevice(ma::Node* output)
{
  output->setProperty("Dummy:One","I was added by a dummy handler!");
};

void DummyHandlerOne::writeDevice(const ma::Node* const input)
{
  OPENMA_UNUSED(input)
  char signature[2] = {0x00, 0x50};
  this->device()->write(signature,2);
};

// ------------------------------------------------------------------------- //

ma::io::Signature DummyHandlerTwo::verifySignature(const ma::io::Device* const device) _OPENMA_NOEXCEPT
{
  OPENMA_UNUSED(device);
  // No signature for this handler
  return ma::io::Signature::NotAvailable;
};
  
ma::io::Signature DummyHandlerTwo::verifySignature() const _OPENMA_NOEXCEPT
{
  return DummyHandlerTwo::verifySignature(this->device());
};
  
void DummyHandlerTwo::readDevice(ma::Node* output)
{
  output->setProperty("Dummy:Two","I was added by the other dummy handler.");
};

#endif // handlerpluginTest_def_h