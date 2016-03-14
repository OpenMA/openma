#include <cxxtest/TestDrive.h>

#include "loggerTest_def.h"

CXXTEST_SUITE(LoggerTest)
{
  CXXTEST_TEST(variadicArguments)
  {
    Verifier* verifier = new Verifier;
    ma::Logger::setDevice(verifier); // deleted by the logger
    
    verifier->ref = "foo";
    ma::info("foo");
    ma::warning("foo");
    ma::error("foo");

    verifier->ref = "I have an integer 12345 included.";
    ma::info("I have an integer %i included.",12345);
    ma::warning("I have an integer %i included.",12345);
    ma::error("I have an integer %i included.",12345);

    verifier->ref = "tiny at the beginning but huge after the variadic expansion. 1.54";
    ma::info("tiny %s. %0.2f","at the beginning but huge after the variadic expansion",1.54f);
    ma::warning("tiny %s. %0.2f","at the beginning but huge after the variadic expansion",1.54f);
    ma::error("tiny %s. %0.2f","at the beginning but huge after the variadic expansion",1.54f);
    
    ma::Logger::setDevice(nullptr); // Reset the device
  };
};

CXXTEST_SUITE_REGISTRATION(LoggerTest)
CXXTEST_TEST_REGISTRATION(LoggerTest, variadicArguments)