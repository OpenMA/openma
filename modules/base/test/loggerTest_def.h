#ifndef anyTest_classdef_h
#define anyTest_classdef_h

#include <openma/base/logger.h>

struct Verifier : ma::Logger::Device
{
  Verifier() : ma::Logger::Device(), ref(nullptr) {};
  ~Verifier() = default;
  
  virtual void write(ma::Message , const char* msg) _OPENMA_NOEXCEPT
  {
    TSM_ASSERT_EQUALS(msg,strcmp(msg,this->ref),0);
  };
  
  const char* ref;
};

#endif // anyTest_classdef_h