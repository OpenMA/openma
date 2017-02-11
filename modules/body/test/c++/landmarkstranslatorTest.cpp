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
  
  CXXTEST_TEST(convertReverse)
  {
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"LFHD", "L.HF"},
      {"LBHD", "L.HB"},
      {"RFHD", "R.HF"},
      {"RBHD", "R.HB"},
      {"C7"  , "C7"  },  // same
    });
    TS_ASSERT_EQUALS(translator.convertReverse("L.HF"), std::string{"LFHD"});
    TS_ASSERT_EQUALS(translator.convertReverse("L.HB"), std::string{"LBHD"});
    TS_ASSERT_EQUALS(translator.convertReverse("R.HF"), std::string{"RFHD"});
    TS_ASSERT_EQUALS(translator.convertReverse("R.HB"), std::string{"RBHD"});
    TS_ASSERT_EQUALS(translator.convertReverse("C7"), std::string{"C7"});
    TS_ASSERT_EQUALS(translator.convertReverse("RBHD_"), std::string{"RBHD_"});
    TS_ASSERT_EQUALS(translator.convertReverse("T10"), std::string{"T10"});
  };

  CXXTEST_TEST(convertReverseIfExists)
  {
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"LFHD", "L.HF"},
      {"LBHD", "L.HB"},
      {"RFHD", "R.HF"},
      {"RBHD", "R.HB"},
      {"C7"  , "C7"  },  // same
    });
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("L.HF"), std::string{"LFHD"});
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("L.HB"), std::string{"LBHD"});
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("R.HF"), std::string{"RFHD"});
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("R.HB"), std::string{"RBHD"});
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("C7"), std::string{"C7"});
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("RBHD_"), std::string{});
    TS_ASSERT_EQUALS(translator.convertReverseIfExists("T10"), std::string{});
  };
  
  CXXTEST_TEST(clone)
  {
    ma::Node root("root");
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"LFHD", "L.HF"},
      {"LBHD", "L.HB"},
      {"RFHD", "R.HF"},
      {"RBHD", "R.HB"},
      {"C7"  , "C7"  },  // same
    });
    auto another = static_cast<ma::body::LandmarksTranslator*>(translator.clone(&root));
    TS_ASSERT_EQUALS(another->name(), std::string{"LandmarksTranslator"});
    TS_ASSERT_EQUALS(another->convert("LFHD"), std::string{"L.HF"});
    TS_ASSERT_EQUALS(another->convert("LBHD"), std::string{"L.HB"});
    TS_ASSERT_EQUALS(another->convert("RFHD"), std::string{"R.HF"});
    TS_ASSERT_EQUALS(another->convert("RBHD"), std::string{"R.HB"});
    TS_ASSERT_EQUALS(another->convert("C7"), std::string{"C7"});
    TS_ASSERT_EQUALS(another->hasParents(), true);
    TS_ASSERT_EQUALS(root.hasChildren(), true);
  };
  
  CXXTEST_TEST(copy)
  {
    ma::body::LandmarksTranslator translator("LandmarksTranslator",{
      {"LFHD", "L.HF"},
      {"LBHD", "L.HB"},
      {"RFHD", "R.HF"},
      {"RBHD", "R.HB"},
      {"C7"  , "C7"  },  // same
    });
    ma::body::LandmarksTranslator another("foo",{});
    another.copy(&translator);
    TS_ASSERT_EQUALS(another.name(), std::string{"LandmarksTranslator"});
    TS_ASSERT_EQUALS(another.convertIfExists("LFHD"), std::string{"L.HF"});
    TS_ASSERT_EQUALS(another.convertIfExists("LBHD"), std::string{"L.HB"});
    TS_ASSERT_EQUALS(another.convertIfExists("RFHD"), std::string{"R.HF"});
    TS_ASSERT_EQUALS(another.convertIfExists("RBHD"), std::string{"R.HB"});
    TS_ASSERT_EQUALS(another.convertIfExists("C7"), std::string{"C7"});
  };
};

CXXTEST_SUITE_REGISTRATION(LandmarksTranslatorTest)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, convert)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, convertIfExists)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, convertReverse)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, convertReverseIfExists)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, clone)
CXXTEST_TEST_REGISTRATION(LandmarksTranslatorTest, copy)