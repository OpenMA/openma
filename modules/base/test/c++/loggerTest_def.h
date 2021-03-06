#ifndef loggerTest_def_h
#define loggerTest_def_h

#include <openma/base/logger.h>

#include <cstring> // strcmp

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

#endif // loggerTest_def_h