#include <cxxtest/TestDrive.h>

#include <openma/body/landmarkstranslator.h>

CXXTEST_SUITE(LandmarksTranslatorTest)
{
  CXXTEST_TEST(convert)
  {
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"LFHD", "L.HF"},
      {"LBHD", "L.HB"},
      {"RFHD", "R.HF"},
      {"RBHD", "R.HB"},
      {"C7"  , "C7"  },  // same
    });
    TS_ASSERT_EQUALS(translator.convert("LFHD"), std::string{"L.HF"});
    TS_ASSERT_EQUALS(translator.convert("LBHD"), std::string{"L.HB"});
    TS_ASSERT_EQUALS(translator.convert("RFHD"), std::string{"R.HF"});
    TS_ASSERT_EQUALS(translator.convert("RBHD"), std::string{"R.HB"});
    TS_ASSERT_EQUALS(translator.convert("C7"), std::string{"C7"});
    TS_ASSERT_EQUALS(translator.convert("RBHD_"), std::string{"RBHD_"});
    TS_ASSERT_EQUALS(translator.convert("T10"), std::string{"T10"});
  };
  
  CXXTEST_TEST(convertIfExists)
  {
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"LFHD", "L.HF"},
      {"LBHD", "L.HB"},
      {"RFHD", "R.HF"},
      {"RBHD", "R.HB"},
      {"C7"  , "C7"  },  // same
    });
    TS_ASSERT_EQUALS(translator.convertIfExists("LFHD"), std::string{"L.HF"});
    TS_ASSERT_EQUALS(translator.convertIfExists("LBHD"), std::string{"L.HB"});
    TS_ASSERT_EQUALS(translator.convertIfExists("RFHD"), std::string{"R.HF"});
    TS_ASSERT_EQUALS(translator.convertIfExists("RBHD"), std::string{"R.HB"});
    TS_ASSERT_EQUALS(translator.convertIfExists("C7"), std::string{"C7"});
    TS_ASSERT_EQUALS(translator.convertIfExists("RBHD_"), std::string{});
    TS_ASSERT_EQUALS(translator.convertIfExists("T10"), std::string{});
  };
};

CXXTEST_SUITE_REGISTRATION(LandmarksTranslatorTest)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, convert)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, convertIfExists)