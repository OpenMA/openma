#include <cxxtest/TestDrive.h>

#include "anyTest_def.h"

#include <openma/base/typeid.h>

#include <limits>
#include <vector>

CXXTEST_SUITE(AnyTest)
{
  CXXTEST_TEST(defaultConstructor)
  {
    ma::Any a;
    TS_ASSERT_EQUALS(a.isEmpty(),true);
    TS_ASSERT_EQUALS(a.isValid(),false);
    TS_ASSERT_EQUALS(a.isArithmetic(),false);
    TS_ASSERT_EQUALS(a.isString(),false);
    TS_ASSERT_EQUALS(a.dimensions(),std::vector<unsigned>());
    TS_ASSERT_EQUALS(a.size(),size_t(0));
    TS_ASSERT_EQUALS(a.cast<int>(),0);
    TS_ASSERT_EQUALS(a.cast<double>(),0.0);
    TS_ASSERT_EQUALS(a.cast<std::string>(),std::string());
  }
  
  CXXTEST_TEST(singleInt)
  {
    ma::Any a = 12;
    TS_ASSERT_EQUALS(a.isArithmetic(),true);
    TS_ASSERT_EQUALS(a.isString(),false);
    TS_ASSERT_EQUALS(a.type(),ma::static_typeid<int>());
    TS_ASSERT_EQUALS(a.cast<int>(),12);
    TS_ASSERT_EQUALS(static_cast<int>(a),12);
    TS_ASSERT_EQUALS(static_cast<float>(a),12.0f);
  };
  
  CXXTEST_TEST(singleFloat)
  {
    ma::Any b = 12.5f;
    TS_ASSERT_EQUALS(b.isArithmetic(),true);
    TS_ASSERT_EQUALS(b.isString(),false);
    TS_ASSERT_EQUALS(b.type(),ma::static_typeid<float>());
    TS_ASSERT_EQUALS(b.cast<int>(),12);
    TS_ASSERT_EQUALS(static_cast<int>(b),12);
    TS_ASSERT_EQUALS(b.cast<float>(),12.5f);
    TS_ASSERT_EQUALS(b.cast<double>(),12.5);
  };
  
  CXXTEST_TEST(singleChar)
  {
    ma::Any c = 'a';
    TS_ASSERT_EQUALS(c.isArithmetic(),true);
    TS_ASSERT_EQUALS(c.isString(),false);
    TS_ASSERT_EQUALS(c.type(),ma::static_typeid<char>());
    TS_ASSERT_EQUALS(c.cast<int>(),97);
    TS_ASSERT_EQUALS(static_cast<float>(c),97.0f);
    TS_ASSERT_EQUALS(c.cast<char>(),'a');
    TS_ASSERT_EQUALS(c.cast<std::string>(),"97"); // Because internally a (signed) char is casted into a (signed) short int before to be converted into a string. This was chosen to priorize int8_t to string conversion.
  };
  
  CXXTEST_TEST(singleComparison)
  {
    ma::Any a = 12;
    ma::Any b = 12.5f;
    ma::Any c = "singleComparison";
    ma::Any d; // invalid
    
    TS_ASSERT(a == a);
    TS_ASSERT(a == 12);
    TS_ASSERT(12 == a);
    TS_ASSERT(a != 0);
    TS_ASSERT(0 != a);
    
    TS_ASSERT(b == b);
    TS_ASSERT(b == 12.5f);
    TS_ASSERT(12.5f == b);
    TS_ASSERT(b != 0.0f);
    TS_ASSERT(0.0f != b);
    
    TS_ASSERT(c == c);
    TS_ASSERT(c == "singleComparison");
    TS_ASSERT("singleComparison" == c);
    TS_ASSERT(c != "");
    TS_ASSERT("" != c);
    
    TS_ASSERT(d != d);
    TS_ASSERT(d != "invalid");
    TS_ASSERT("invalid" != d);
    
    TS_ASSERT(a == b); // b is casted to int
    TS_ASSERT(a != c);
    TS_ASSERT(a != d);
    
    TS_ASSERT(b != a);
    TS_ASSERT(b != c);
    TS_ASSERT(b != d);
    
    TS_ASSERT(c != a);
    TS_ASSERT(c != b);
    TS_ASSERT(c != d);
    
    TS_ASSERT(d != a);
    TS_ASSERT(d != b);
    TS_ASSERT(d != c);
  };
  
  CXXTEST_TEST(singleComparison2)
  {
    ma::Any a = 12;
    ma::Any b = 12.5f;
    ma::Any n;
    TS_ASSERT_EQUALS(n.isValid(),false);
    n = 12.5f;
    TS_ASSERT_EQUALS(n.isValid(),true);
    TS_ASSERT(n == b);
    TS_ASSERT(b == n);
    TS_ASSERT(n.cast<int>() == a);
    TS_ASSERT(static_cast<int>(n) == a);
    TS_ASSERT(n != a);
    TS_ASSERT(a == n);
  };
  
  CXXTEST_TEST(singleInt8ToString)
  {
    ma::Any c = (int8_t)45;
    TS_ASSERT_EQUALS(c.cast<std::string>(),"45");
  };
  
  CXXTEST_TEST(singleBoolAndString)
  {
    ma::Any d = true;
    TS_ASSERT_EQUALS(d.cast<std::string>(),"1");
    TS_ASSERT_EQUALS(d.cast<int>(),1);
    d = false;
    TS_ASSERT_EQUALS(d.cast<std::string>(),"0");
    TS_ASSERT_EQUALS(d.cast<int>(),0);
    TS_ASSERT_EQUALS(d.cast<bool>(),false);
    d = "true";
    TS_ASSERT_EQUALS(d.cast<unsigned int>(),0u);
    TS_ASSERT_EQUALS(d.cast<bool>(),true);
    d = "false";
    TS_ASSERT_EQUALS(d.cast<unsigned int>(),0u);
    TS_ASSERT_EQUALS(d.cast<bool>(),false);
  };
  
  CXXTEST_TEST(singleString)
  {
    ma::Any e = std::string("1.45");
    TS_ASSERT_EQUALS(e.cast<float>(),1.45f);
    TS_ASSERT_EQUALS(e.cast<double>(),1.45);
    // TS_ASSERT_DELTA(e.cast<long double>(),static_cast<long double>(1.45),std::numeric_limits<long double>::epsilon());
    TS_ASSERT_EQUALS(e.cast<int>(),1);
    TS_ASSERT_EQUALS(static_cast<int>(e),1);
  };  
  
  CXXTEST_TEST(singleIntImplicitConversion)
  {
    ma::Any f = 45;
    TS_ASSERT_EQUALS(45,f);
    TS_ASSERT_EQUALS(f,45);
    TS_ASSERT_DIFFERS(f,false);
    TS_ASSERT_DIFFERS(12.4,f);
  };
  
  CXXTEST_TEST(singleCustomTypeRegistered)
  {
    ma::Any g = Date{1999,12,19};
    TS_ASSERT_EQUALS(g,Date({1999,12,19}));
    TS_ASSERT_EQUALS(g.cast<std::string>(),"");
    TS_ASSERT_EQUALS(g.cast<bool>(),false);
    ma::Any::Register<Date, ma::Any::Conversion<std::string>, ma::Any::Conversion<std::string>>();
    TS_ASSERT_EQUALS(g.cast<std::string>(),"1999-12-19");
    g = std::string("2009-05-02");
    TS_ASSERT_EQUALS(g.cast<Date>(),Date({2009,05,02}));
    TS_ASSERT_EQUALS(g.cast<bool>(),true);
    ma::Any::Unregister<Date>();
    TS_ASSERT_EQUALS(g.cast<Date>(),Date({0,0,0}));
    g = Date{1999,12,19};
    TS_ASSERT_EQUALS(g,Date({1999,12,19}));
  };
  
  CXXTEST_TEST(singleCustomTypeNotRegistered)
  {
    ma::Any g = Foo{1,1};
    TS_ASSERT_EQUALS(g,Foo({1,1}));
    TS_ASSERT_DIFFERS(g,Foo({2,2}));
  };

  CXXTEST_TEST(singleIntHexadecimal)
  {
    ma::Any h = int(0xFFFF);
    TS_ASSERT_EQUALS(h.cast<int8_t>(),int8_t(-1));
  };
  
  CXXTEST_TEST(singleInt8)
  {
    ma::Any i = int8_t(-1);
    TS_ASSERT_EQUALS(i.cast<int>(),-1);
    TS_ASSERT_EQUALS(i.cast<double>(),-1.0);
    TS_ASSERT_EQUALS(i.cast<std::string>(),"-1");
  };
  
  CXXTEST_TEST(singleFromString)
  {
    ma::Any m("foo");
    TS_ASSERT_EQUALS(m.cast<std::string>(),"foo");
    TS_ASSERT_EQUALS(m.cast<int>(),0);
    TS_ASSERT_EQUALS(static_cast<int>(m),0);
    TS_ASSERT_EQUALS(static_cast<float>(m),0.0f);
    TS_ASSERT_EQUALS(static_cast<double>(m),0.0);
    TS_ASSERT_EQUALS(static_cast<bool>(m),true);
  };
  
  CXXTEST_TEST(singleEnum)
  {
    enum {Foo= 1, Bar = 2, Toto = 4};
    ma::Any m(Toto);
    TS_ASSERT_EQUALS(m.cast<std::string>(),"4");
    TS_ASSERT_EQUALS(m.cast<int>(),4);
  };
  
  CXXTEST_TEST(singleIntToArray)
  {
    ma::Any a = 12;
    std::vector<int> ref{12};
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),ref);
  };
  
  CXXTEST_TEST(singleCustomToArray)
  {
    ma::Any g = Foo{1,1};
    std::vector<Foo> ref{{1,1}};
    TS_ASSERT_EQUALS(g.cast<std::vector<Foo>>(),ref);
  };
  
  CXXTEST_TEST(singleTypeid)
  {
    ma::Any b = ma::static_typeid<int>();
    TS_ASSERT_EQUALS(b.cast<ma::typeid_t>(),ma::static_typeid<int>());
    TS_ASSERT_DIFFERS(b.cast<ma::typeid_t>(),ma::static_typeid<float>());
  };
  
  CXXTEST_TEST(singleBool)
  {
    ma::Any a = true;
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    TS_ASSERT_EQUALS(a.cast<double>(),1.0);
    auto v = a.cast<std::vector<bool>>();
    TS_ASSERT_EQUALS(v.size(),size_t(1));
    bool b = v.at(0); // Compiilation error when trying to do TS_ASSERT_EQUALS(,true);
    TS_ASSERT_EQUALS(b, true);
  };
  
  CXXTEST_TEST(assignSingle)
  {
    ma::Any a;
    a.assign(true);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    a.assign(10);
    TS_ASSERT_EQUALS(a.cast<int>(),10);
    TS_ASSERT_EQUALS(a.cast<float>(),10.0f);
    TS_ASSERT_EQUALS(a.cast<bool>(),true);
    a.assign("dawn of the");
    TS_ASSERT_EQUALS(a.cast<int>(),0);
    TS_ASSERT_EQUALS(a.cast<bool>(),true);
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"dawn of the"),0);
  };
  
  CXXTEST_TEST(arrayIntVector)
  {
    std::vector<int> bar, foo{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    // Vector constructor
    ma::Any a = foo;
    TS_ASSERT_EQUALS(a.isArithmetic(),true);
    TS_ASSERT_EQUALS(a.isString(),false);
    TS_ASSERT_EQUALS(a.type(),ma::static_typeid<int>());
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],10ul);
    TS_ASSERT_EQUALS(a.size(),10ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayIntVector2)
  {
    std::vector<int> foo = {1,2,3,4};
    // Use a vector of dimensions already set
    std::vector<uint8_t> dims = {1,3};
    ma::Any a = ma::Any(foo,dims);
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],1u);
      TS_ASSERT_EQUALS(a.dimensions()[1],3u);
    }
    TS_ASSERT_EQUALS(a.size(),3ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    foo = {1,2,3};
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayIntVector3)
  {
    std::vector<int> foo = {1,2,3,4};
    // - Dimensions and values not matching (more values than dimensions)
    ma::Any a = ma::Any(foo,std::vector<unsigned>({2,2}));
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],2u);
      TS_ASSERT_EQUALS(a.dimensions()[1],2u);
    }
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    foo = {1,2,3,4};
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayIntInitializer)
  {
    // Initializer constructor
    // - Dimensions and values matching
    std::vector<int> foo = {1,2,3,4};
    ma::Any a = ma::Any({1,2,3,4},{2,2});
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],2ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],2ul);
    }
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayIntInitializer2)
  {
    // - Dimensions and values not matching (less values than dimensions)
    ma::Any a = ma::Any({1,2,3,4},{3,3});
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],3ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],3ul);
    }
    TS_ASSERT_EQUALS(a.size(),9ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    std::vector<int> foo = {1,2,3,4,0,0,0,0,0};
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayIntInitializer3)
  {
    // Implicit initializer constructor
    std::vector<int> foo = {1,2,3,4};
    ma::Any a = {1,2,3,4};
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],4ul);
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    for (size_t i = 0 ; i < foo.size() ; ++i)
      TS_ASSERT_EQUALS(a.cast<int>(i),foo[i]);
    std::vector<int> bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayIntComparison)
  {
    ma::Any a = {1,2,3,4};
    std::vector<int> bar = {1,2,3,4};
    TS_ASSERT_EQUALS(a,bar);
    ma::Any b = ma::Any({1,2,3,4},{2,2});
    TS_ASSERT_EQUALS(a,b);
    b = {1,2,3,4};
    TS_ASSERT_EQUALS(a,b);
    b = {1,2,3};
    TS_ASSERT_DIFFERS(a,b);
  };
  
  CXXTEST_TEST(arrayString)
  {
    // Another test with strings
    ma::Any a = std::vector<std::string>({"Coco","Vroum","Another"});
    TS_ASSERT_EQUALS(a.cast<std::string>(),"Coco");
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"Coco"),0);
    a = {"Coco","Vroum","Another"};
    TS_ASSERT_EQUALS(a.cast<std::string>(),"Coco");
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"Coco"),0);
    auto toto = a.cast<std::vector<const char*>>();
    TS_ASSERT_EQUALS(toto.size(),3ul);
    TS_ASSERT_EQUALS(strcmp(toto[0],"Coco"),0);
    TS_ASSERT_EQUALS(strcmp(toto[1],"Vroum"),0);
    TS_ASSERT_EQUALS(strcmp(toto[2],"Another"),0);
    a = ma::Any(std::vector<std::string>({"Coco","Vroum","Another"}),std::vector<unsigned>(1,4));
    auto bar = a.cast<std::vector<std::string>>();
    TS_ASSERT_EQUALS(bar.size(),4ul);
    TS_ASSERT_EQUALS(bar[0],std::string("Coco"));
    TS_ASSERT_EQUALS(bar[1],std::string("Vroum"));
    TS_ASSERT_EQUALS(bar[2],std::string("Another"));
    TS_ASSERT_EQUALS(bar[3],std::string(""));
    TS_ASSERT_EQUALS(a.cast<std::string>(0),std::string("Coco"));
    TS_ASSERT_EQUALS(a.cast<std::string>(1),std::string("Vroum"));
    TS_ASSERT_EQUALS(a.cast<std::string>(2),std::string("Another"));
    TS_ASSERT_EQUALS(a.cast<std::string>(3),std::string(""));
  };
  
  CXXTEST_TEST(arrayString_Empty)
  {
    std::vector<std::string> p;
    std::vector<unsigned> dims{0};
    ma::Any a{p,dims};
    TS_ASSERT_EQUALS(a.isArithmetic(),false);
    TS_ASSERT_EQUALS(a.isString(),true);
    TS_ASSERT_EQUALS(a.type(),ma::static_typeid<std::string>());
    TS_ASSERT_EQUALS(a.isValid(),true);
    TS_ASSERT_EQUALS(a.isEmpty(),true);
    p = {{"foo"}};
    a = p;
    TS_ASSERT_EQUALS(a.isValid(),true);
    TS_ASSERT_EQUALS(a.isEmpty(),false);
    a = std::vector<std::string>{};
    TS_ASSERT_EQUALS(a.isValid(),true);
    TS_ASSERT_EQUALS(a.isEmpty(),true);
  };
  
  CXXTEST_TEST(arrayCustomType)
  {
    ma::Any a = {Date{2009,05,01},Date{2005,12,12},Date{1945,07,23}};
    TS_ASSERT_EQUALS(a.cast<Date>(),Date({2009,05,01}));
    TS_ASSERT_EQUALS(a.cast<Date>(0),Date({2009,05,01}));
    TS_ASSERT_EQUALS(a.cast<Date>(1),Date({2005,12,12}));
    TS_ASSERT_EQUALS(a.cast<Date>(2),Date({1945,07,23}));
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"","",""}));  
    ma::Any::Register<Date, ma::Any::Conversion<std::string>, ma::Any::Conversion<std::string>>();
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"2009-05-01","2005-12-12","1945-07-23"}));  
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),std::vector<int>({0,0,0}));  
    ma::Any::Unregister<Date>();
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"","",""}));
  };
    
  CXXTEST_TEST(arrayFromSingle)
  {
    ma::Any a = 1.2f;
    TS_ASSERT_EQUALS(a.cast<std::vector<float>>(),std::vector<float>({1.2f}));
  };
  
  CXXTEST_TEST(arrayFromsingleCustomType)
  {
    ma::Any a = "2009-05-02";
    ma::Any::Register<Date, ma::Any::Conversion<std::string>, ma::Any::Conversion<std::string>>();
    TS_ASSERT_EQUALS(a.cast<std::vector<Date>>(),std::vector<Date>({Date({2009,05,02})}));
    a = Date{2014,03,01}; // The conversion to a string is correctly "2014-3-1".
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"2014-3-1"}));
    ma::Any::Unregister<Date>();
  };
  
  CXXTEST_TEST(arrayIntArray)
  {
    std::array<int,4> bar, foo{{1, 2, 3, 4}};
    // Vector constructor
    ma::Any a = foo;
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],4ul);
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::array<int,4>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(arrayEnum)
  {
    enum class myEnum {Foo= 1, Bar = 2, Toto = 4};
    ma::Any m(std::vector<myEnum>{myEnum::Foo,myEnum::Toto});
    TS_ASSERT_EQUALS(m.isArithmetic(),true);
    TS_ASSERT_EQUALS(m.isString(),false);
    TS_ASSERT_EQUALS(m.type(),ma::static_typeid<int>()); // Convert to the underlying type
    TS_ASSERT_EQUALS(m.cast<int>(0),1);
    TS_ASSERT_EQUALS(m.cast<int>(1),4);
    TS_ASSERT_EQUALS(m.cast<std::string>(0),"1");
    TS_ASSERT_EQUALS(m.cast<std::string>(1),"4");
    TS_ASSERT_EQUALS(m.cast<myEnum>(0),myEnum::Foo);
    TS_ASSERT_EQUALS(m.cast<myEnum>(1),myEnum::Toto);
    TS_ASSERT_EQUALS(m.cast<std::vector<int>>(),std::vector<int>({1,4}));
    TS_ASSERT_EQUALS(m.cast<std::vector<std::string>>(),std::vector<std::string>({"1","4"}));
    TS_ASSERT_EQUALS(m.cast<std::vector<myEnum>>(),std::vector<myEnum>({myEnum::Foo,myEnum::Toto}));
  };
  
  CXXTEST_TEST(arrayChar)
  {
    std::array<char,4> foo{{'a','b','c','d'}};
    ma::Any a = foo;
    TS_ASSERT_EQUALS(a.cast<char>(0),'a');
    TS_ASSERT_EQUALS(a.cast<char>(1),'b');
    TS_ASSERT_EQUALS(a.cast<char>(2),'c');
    TS_ASSERT_EQUALS(a.cast<char>(3),'d');
    TS_ASSERT_EQUALS(a.cast<std::vector<char>>(),std::vector<char>({'a','b','c','d'}));
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),std::vector<int>({97,98,99,100}));
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"97","98","99","100"}));
  };
  
  CXXTEST_TEST(arrayBool_One)
  {
    std::array<bool,4> foo{{true,false,false,true}};
    ma::Any a = foo;
    TS_ASSERT_EQUALS(a.cast<bool>(0),true);
    TS_ASSERT_EQUALS(a.cast<bool>(1),false);
    TS_ASSERT_EQUALS(a.cast<bool>(2),false);
    TS_ASSERT_EQUALS(a.cast<bool>(3),true);
    TS_ASSERT_EQUALS(a.cast<char>(0),0x01);
    TS_ASSERT_EQUALS(a.cast<char>(1),0x00);
    TS_ASSERT_EQUALS(a.cast<char>(2),0x00);
    TS_ASSERT_EQUALS(a.cast<char>(3),0x01);
    auto bar = a.cast<std::vector<bool>>();
    for (size_t i = 0 ; i < 4 ; ++i)
    {
      bool f = foo[i], b = bar[i];
      TS_ASSERT_EQUALS(f,b);
    }
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),std::vector<int>({1,0,0,1}));
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"1","0","0","1"}));
  };
  
  CXXTEST_TEST(arrayBool_Two)
  {
    std::vector<bool> foo{{true,false,false,true}};
    ma::Any a = foo;
    TS_ASSERT_EQUALS(a.isArithmetic(),true);
    TS_ASSERT_EQUALS(a.isString(),false);
    TS_ASSERT_EQUALS(a.type(),ma::static_typeid<bool>());
    TS_ASSERT_EQUALS(a.cast<bool>(0),true);
    TS_ASSERT_EQUALS(a.cast<bool>(1),false);
    TS_ASSERT_EQUALS(a.cast<bool>(2),false);
    TS_ASSERT_EQUALS(a.cast<bool>(3),true);
    TS_ASSERT_EQUALS(a.cast<char>(0),0x01);
    TS_ASSERT_EQUALS(a.cast<char>(1),0x00);
    TS_ASSERT_EQUALS(a.cast<char>(2),0x00);
    TS_ASSERT_EQUALS(a.cast<char>(3),0x01);
    auto bar = a.cast<std::vector<bool>>();
    for (size_t i = 0 ; i < 4 ; ++i)
    {
      bool f = foo[i], b = bar[i];
      TS_ASSERT_EQUALS(f,b);
    }
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),std::vector<int>({1,0,0,1}));
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"1","0","0","1"}));
  };
  
  CXXTEST_TEST(assignArray)
  {
    ma::Any a;
    a.assign(std::vector<bool>({true,false,false,true}), std::vector<unsigned>({2,2}));
    TS_ASSERT_EQUALS(a.cast<bool>(0),true);
    TS_ASSERT_EQUALS(a.cast<bool>(1),false);
    TS_ASSERT_EQUALS(a.cast<bool>(2),false);
    TS_ASSERT_EQUALS(a.cast<bool>(3),true);
    TS_ASSERT_EQUALS(a.dimensions(), std::vector<unsigned>({2,2}));
    a.assign(std::array<char,4>{{'a','b','c','d'}});
    TS_ASSERT_EQUALS(a.cast<char>(0),'a');
    TS_ASSERT_EQUALS(a.cast<char>(1),'b');
    TS_ASSERT_EQUALS(a.cast<char>(2),'c');
    TS_ASSERT_EQUALS(a.cast<char>(3),'d');
    TS_ASSERT_EQUALS(a.dimensions(), std::vector<unsigned>(1,4u));
    a.assign({1,2,3,4,5,6},{2,3});
    TS_ASSERT_EQUALS(a.dimensions(), std::vector<unsigned>({2,3}));
    TS_ASSERT_EQUALS(a.cast<std::vector<float>>(), std::vector<float>({1.0f,2.0f,3.0f,4.0f,5.0f,6.0f}));
  };
};

CXXTEST_SUITE_REGISTRATION(AnyTest)
CXXTEST_TEST_REGISTRATION(AnyTest, defaultConstructor)
// Single element
CXXTEST_TEST_REGISTRATION(AnyTest, singleInt)
CXXTEST_TEST_REGISTRATION(AnyTest, singleFloat)
CXXTEST_TEST_REGISTRATION(AnyTest, singleChar)
CXXTEST_TEST_REGISTRATION(AnyTest, singleComparison)
CXXTEST_TEST_REGISTRATION(AnyTest, singleComparison2)
CXXTEST_TEST_REGISTRATION(AnyTest, singleInt8ToString)
CXXTEST_TEST_REGISTRATION(AnyTest, singleBoolAndString)
CXXTEST_TEST_REGISTRATION(AnyTest, singleString)
CXXTEST_TEST_REGISTRATION(AnyTest, singleIntImplicitConversion)
CXXTEST_TEST_REGISTRATION(AnyTest, singleCustomTypeRegistered)
CXXTEST_TEST_REGISTRATION(AnyTest, singleCustomTypeNotRegistered)
CXXTEST_TEST_REGISTRATION(AnyTest, singleIntHexadecimal)
CXXTEST_TEST_REGISTRATION(AnyTest, singleInt8)
CXXTEST_TEST_REGISTRATION(AnyTest, singleFromString)
CXXTEST_TEST_REGISTRATION(AnyTest, singleEnum)
CXXTEST_TEST_REGISTRATION(AnyTest, singleIntToArray)
CXXTEST_TEST_REGISTRATION(AnyTest, singleCustomToArray)
CXXTEST_TEST_REGISTRATION(AnyTest, singleTypeid)
CXXTEST_TEST_REGISTRATION(AnyTest, singleBool)
CXXTEST_TEST_REGISTRATION(AnyTest, assignSingle)
// Array container
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntVector)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntVector2)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntVector3)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntInitializer)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntInitializer2)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntInitializer3)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntComparison)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayString)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayString_Empty)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayFromSingle)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayFromsingleCustomType)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayIntArray)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayEnum)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayChar)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayBool_One)
CXXTEST_TEST_REGISTRATION(AnyTest, arrayBool_Two)
CXXTEST_TEST_REGISTRATION(AnyTest, assignArray)