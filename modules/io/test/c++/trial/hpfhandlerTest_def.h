#ifndef hpfhandlertest_def_h
#define hpfhandlertest_def_h

#include <openma/io.h>
#include <openma/base/node.h>
#include <openma/base/trial.h>
#include <openma/base/timesequence.h>

void hpfhandlertest_read_Run_number_34_VTT_Rep_1_6(ma::Node *root)
{
  REQUIRE( root->children().size() == 1u );
  auto tss = root->findChildren<ma::TimeSequence*>();
  REQUIRE( tss.size() == 40u );
  CHECK( tss[ 0]->name() == "R TRAPEZIUS MIDDLE FIBERS: EMG 1");
  CHECK( tss[ 1]->name() == "R TRAPEZIUS MIDDLE FIBERS: ACC X 1" );
  CHECK( tss[ 2]->name() == "R TRAPEZIUS MIDDLE FIBERS: ACC Y 1" );
  CHECK( tss[ 3]->name() == "R TRAPEZIUS MIDDLE FIBERS: ACC Z 1" );
  CHECK( tss[ 4]->name() == "L TRAPEZIUS MIDDLE FIBERS: EMG 2" );
  CHECK( tss[ 5]->name() == "L TRAPEZIUS MIDDLE FIBERS: ACC X 2" );
  CHECK( tss[ 6]->name() == "L TRAPEZIUS MIDDLE FIBERS: ACC Y 2" );
  CHECK( tss[ 7]->name() == "L TRAPEZIUS MIDDLE FIBERS: ACC Z 2" );
  CHECK( tss[ 8]->name() == "R EXTENSOR DIGITORUM: EMG 3" );
  CHECK( tss[ 9]->name() == "R EXTENSOR DIGITORUM: ACC X 3" );
  CHECK( tss[10]->name() == "R EXTENSOR DIGITORUM: ACC Y 3" );
  CHECK( tss[11]->name() == "R EXTENSOR DIGITORUM: ACC Z 3" );
  CHECK( tss[12]->name() == "L EXTENSOR DIGITORUM: EMG 4" );
  CHECK( tss[13]->name() == "L EXTENSOR DIGITORUM: ACC X 4" );
  CHECK( tss[14]->name() == "L EXTENSOR DIGITORUM: ACC Y 4" );
  CHECK( tss[15]->name() == "L EXTENSOR DIGITORUM: ACC Z 4" );
  CHECK( tss[16]->name() == "R FLEXOR CARPI RADIALIS: EMG 5" );
  CHECK( tss[17]->name() == "R FLEXOR CARPI RADIALIS: ACC X 5" );
  CHECK( tss[18]->name() == "R FLEXOR CARPI RADIALIS: ACC Y 5" );
  CHECK( tss[19]->name() == "R FLEXOR CARPI RADIALIS: ACC Z 5" );
  CHECK( tss[20]->name() == "L FLEXOR CARPI RADIALIS: EMG 6" );
  CHECK( tss[21]->name() == "L FLEXOR CARPI RADIALIS: ACC X 6" );
  CHECK( tss[22]->name() == "L FLEXOR CARPI RADIALIS: ACC Y 6" );
  CHECK( tss[23]->name() == "L FLEXOR CARPI RADIALIS: ACC Z 6" );
  CHECK( tss[24]->name() == "R BICEPS BRACHII: EMG 7" );
  CHECK( tss[25]->name() == "R BICEPS BRACHII: ACC X 7" );
  CHECK( tss[26]->name() == "R BICEPS BRACHII: ACC Y 7" );
  CHECK( tss[27]->name() == "R BICEPS BRACHII: ACC Z 7" );
  CHECK( tss[28]->name() == "L BICEPS BRACHII: EMG 8" );
  CHECK( tss[29]->name() == "L BICEPS BRACHII: ACC X 8" );
  CHECK( tss[30]->name() == "L BICEPS BRACHII: ACC Y 8" );
  CHECK( tss[31]->name() == "L BICEPS BRACHII: ACC Z 8" );
  CHECK( tss[32]->name() == "R TRICEPS BRACHII: EMG 9" );
  CHECK( tss[33]->name() == "R TRICEPS BRACHII: ACC X 9" );
  CHECK( tss[34]->name() == "R TRICEPS BRACHII: ACC Y 9" );
  CHECK( tss[35]->name() == "R TRICEPS BRACHII: ACC Z 9" );
  CHECK( tss[36]->name() == "L TRICEPS BRACHII: EMG 10" );
  CHECK( tss[37]->name() == "L TRICEPS BRACHII: ACC X 10" );
  CHECK( tss[38]->name() == "L TRICEPS BRACHII: ACC Y 10" );
  CHECK( tss[39]->name() == "L TRICEPS BRACHII: ACC Z 10" );

  CHECK( tss[ 0]->unit() == "V" );
  CHECK( tss[ 1]->unit() == "g" );
  CHECK( tss[ 2]->unit() == "g" );
  CHECK( tss[ 3]->unit() == "g" );
  CHECK( tss[ 4]->unit() == "V" );
  CHECK( tss[ 5]->unit() == "g" );
  CHECK( tss[ 6]->unit() == "g" );
  CHECK( tss[ 7]->unit() == "g" );
  CHECK( tss[ 8]->unit() == "V" );
  CHECK( tss[ 9]->unit() == "g" );
  CHECK( tss[10]->unit() == "g" );
  CHECK( tss[11]->unit() == "g" );
  CHECK( tss[12]->unit() == "V" );
  CHECK( tss[13]->unit() == "g" );
  CHECK( tss[14]->unit() == "g" );
  CHECK( tss[15]->unit() == "g" );
  CHECK( tss[16]->unit() == "V" );
  CHECK( tss[17]->unit() == "g" );
  CHECK( tss[18]->unit() == "g" );
  CHECK( tss[19]->unit() == "g" );
  CHECK( tss[20]->unit() == "V" );
  CHECK( tss[21]->unit() == "g" );
  CHECK( tss[22]->unit() == "g" );
  CHECK( tss[23]->unit() == "g" );
  CHECK( tss[24]->unit() == "V" );
  CHECK( tss[25]->unit() == "g" );
  CHECK( tss[26]->unit() == "g" );
  CHECK( tss[27]->unit() == "g" );
  CHECK( tss[28]->unit() == "V" );
  CHECK( tss[29]->unit() == "g" );
  CHECK( tss[30]->unit() == "g" );
  CHECK( tss[31]->unit() == "g" );
  CHECK( tss[32]->unit() == "V" );
  CHECK( tss[33]->unit() == "g" );
  CHECK( tss[34]->unit() == "g" );
  CHECK( tss[35]->unit() == "g" );
  CHECK( tss[36]->unit() == "V" );
  CHECK( tss[37]->unit() == "g" );
  CHECK( tss[38]->unit() == "g" );
  CHECK( tss[39]->unit() == "g" );

  for (auto ts : tss)
  {
    for (int i = 0 ; i < 10 ; ++i)
    {
      CHECK( ts->data()[i] == AROUND(0.0, 1e-5) );
    }
  }

  CHECK( tss[ 0]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 1]->data()[10] == AROUND(0.3238856, 1e-5) );
  CHECK( tss[ 2]->data()[10] == AROUND(1.326369, 1e-5) );
  CHECK( tss[ 3]->data()[10] == AROUND(-3.640687, 1e-5) );
  CHECK( tss[ 4]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 5]->data()[10] == AROUND(-0.3735166, 1e-5) );
  CHECK( tss[ 6]->data()[10] == AROUND(-1.078769, 1e-5) );
  CHECK( tss[ 7]->data()[10] == AROUND(-3.737764, 1e-5) );
  CHECK( tss[ 8]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 9]->data()[10] == AROUND(-2.818379, 1e-5) );
  CHECK( tss[10]->data()[10] == AROUND(1.680531, 1e-5) );
  CHECK( tss[11]->data()[10] == AROUND(-1.932598, 1e-5) );
  CHECK( tss[12]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[13]->data()[10] == AROUND(-3.235401, 1e-5) );
  CHECK( tss[14]->data()[10] == AROUND(-0.7581546, 1e-5) );
  CHECK( tss[15]->data()[10] == AROUND(-1.25684, 1e-5) );
  CHECK( tss[16]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[17]->data()[10] == AROUND(-2.622296, 1e-5) );
  CHECK( tss[18]->data()[10] == AROUND(2.348345, 1e-5) );
  CHECK( tss[19]->data()[10] == AROUND(0.5745529, 1e-5) );
  CHECK( tss[20]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[21]->data()[10] == AROUND(-2.841617, 1e-5) );
  CHECK( tss[22]->data()[10] == AROUND(-1.628069, 1e-5) );
  CHECK( tss[23]->data()[10] == AROUND(0.3194182, 1e-5) );
  CHECK( tss[24]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[25]->data()[10] == AROUND(-2.612428, 1e-5) );
  CHECK( tss[26]->data()[10] == AROUND(2.221158, 1e-5) );
  CHECK( tss[27]->data()[10] == AROUND(-1.152982, 1e-5) );
  CHECK( tss[28]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[29]->data()[10] == AROUND(-2.339089, 1e-5) );
  CHECK( tss[30]->data()[10] == AROUND(-2.129879, 1e-5) );
  CHECK( tss[31]->data()[10] == AROUND(-0.7680386, 1e-5) );
  CHECK( tss[32]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[33]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[34]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[35]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[36]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[37]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[38]->data()[10] == AROUND(0.0, 1e-5) );
  CHECK( tss[39]->data()[10] == AROUND(0.0, 1e-5) );

  CHECK( tss[ 0]->data()[5135] == AROUND(-4.01E-05, 1e-5) );
  CHECK( tss[ 1]->data()[5135] == AROUND(-0.06477713, 1e-5) );
  CHECK( tss[ 2]->data()[5135] == AROUND(0.5684439, 1e-5) );
  CHECK( tss[ 3]->data()[5135] == AROUND(-0.8942038, 1e-5) );
  CHECK( tss[ 4]->data()[5135] == AROUND(-1.51E-06, 1e-5) );
  CHECK( tss[ 5]->data()[5135] == AROUND(-0.06225277, 1e-5) );
  CHECK( tss[ 6]->data()[5135] == AROUND(-0.5076562, 1e-5) );
  CHECK( tss[ 7]->data()[5135] == AROUND(-0.9666631, 1e-5) );
  CHECK( tss[ 8]->data()[5135] == AROUND(1.09E-05, 1e-5) );
  CHECK( tss[ 9]->data()[5135] == AROUND(-0.7209805, 1e-5) );
  CHECK( tss[10]->data()[5135] == AROUND(0.1867257, 1e-5) );
  CHECK( tss[11]->data()[5135] == AROUND(-0.4664891, 1e-5) );
  CHECK( tss[12]->data()[5135] == AROUND(7.22E-06, 1e-5) );
  CHECK( tss[13]->data()[5135] == AROUND(-0.9059124, 1e-5) );
  CHECK( tss[14]->data()[5135] == AROUND(-0.1895387, 1e-5) );
  CHECK( tss[15]->data()[5135] == AROUND(-0.3142101, 1e-5) );
  CHECK( tss[16]->data()[5135] == AROUND(5.00E-05, 1e-5) );
  CHECK( tss[17]->data()[5135] == AROUND(-0.6395844, 1e-5) );
  CHECK( tss[18]->data()[5135] == AROUND(0.6981567, 1e-5) );
  CHECK( tss[19]->data()[5135] == AROUND(0.0, 1e-5) );
  CHECK( tss[20]->data()[5135] == AROUND(6.88E-06, 1e-5) );
  CHECK( tss[21]->data()[5135] == AROUND(-0.7749864, 1e-5) );
  CHECK( tss[22]->data()[5135] == AROUND(-0.6887985, 1e-5) );
  CHECK( tss[23]->data()[5135] == AROUND(0.1277673, 1e-5) );
  CHECK( tss[24]->data()[5135] == AROUND(2.23E-05, 1e-5) );
  CHECK( tss[25]->data()[5135] == AROUND(-0.6220067, 1e-5) );
  CHECK( tss[26]->data()[5135] == AROUND(0.6346164, 1e-5) );
  CHECK( tss[27]->data()[5135] == AROUND(-0.5124366, 1e-5) );
  CHECK( tss[28]->data()[5135] == AROUND(-7.55E-06, 1e-5) );
  CHECK( tss[29]->data()[5135] == AROUND(-0.7386595, 1e-5) );
  CHECK( tss[30]->data()[5135] == AROUND(-0.6454179, 1e-5) );
  CHECK( tss[31]->data()[5135] == AROUND(-0.2560129, 1e-5) );
  CHECK( tss[32]->data()[5135] == AROUND(1.51E-06, 1e-5) );
  CHECK( tss[33]->data()[5135] == AROUND(-0.6322685, 1e-5) );
  CHECK( tss[34]->data()[5135] == AROUND(-0.1950575, 1e-5) );
  CHECK( tss[35]->data()[5135] == AROUND(0.5104636, 1e-5) );
  CHECK( tss[36]->data()[5135] == AROUND(7.17E-05, 1e-5) );
  CHECK( tss[37]->data()[5135] == AROUND(-0.5763358, 1e-5) );
  CHECK( tss[38]->data()[5135] == AROUND(0.3774208, 1e-5) );
  CHECK( tss[39]->data()[5135] == AROUND(0.700477   , 1e-5) );

  CHECK( tss[ 0]->data()[10751] == AROUND(-2.11E-05, 1e-5) );
  CHECK( tss[ 1]->data()[10751] == AROUND(-0.06477713, 1e-5) );
  CHECK( tss[ 2]->data()[10751] == AROUND(0.6316043, 1e-5) );
  CHECK( tss[ 3]->data()[10751] == AROUND(-0.8942038, 1e-5) );
  CHECK( tss[ 4]->data()[10751] == AROUND(3.19E-06, 1e-5) );
  CHECK( tss[ 5]->data()[10751] == AROUND(-0.06225277, 1e-5) );
  CHECK( tss[ 6]->data()[10751] == AROUND(-0.4441991, 1e-5) );
  CHECK( tss[ 7]->data()[10751] == AROUND(-0.9666631, 1e-5) );
  CHECK( tss[ 8]->data()[10751] == AROUND(2.52E-06, 1e-5) );
  CHECK( tss[ 9]->data()[10751] == AROUND(-0.8520679, 1e-5) );
  CHECK( tss[10]->data()[10751] == AROUND(0.1244838, 1e-5) );
  CHECK( tss[11]->data()[10751] == AROUND(-0.4664891, 1e-5) );
  CHECK( tss[12]->data()[10751] == AROUND(9.06E-06, 1e-5) );
  CHECK( tss[13]->data()[10751] == AROUND(-0.8412044, 1e-5) );
  CHECK( tss[14]->data()[10751] == AROUND(-0.1895387, 1e-5) );
  CHECK( tss[15]->data()[10751] == AROUND(-0.188526, 1e-5) );
  CHECK( tss[16]->data()[10751] == AROUND(3.19E-05, 1e-5) );
  CHECK( tss[17]->data()[10751] == AROUND(-0.7035428, 1e-5) );
  CHECK( tss[18]->data()[10751] == AROUND(0.6981567, 1e-5) );
  CHECK( tss[19]->data()[10751] == AROUND(-0.06383921, 1e-5) );
  CHECK( tss[20]->data()[10751] == AROUND(6.88E-06, 1e-5) );
  CHECK( tss[21]->data()[10751] == AROUND(-0.7104042, 1e-5) );
  CHECK( tss[22]->data()[10751] == AROUND(-0.5635625, 1e-5) );
  CHECK( tss[23]->data()[10751] == AROUND(0.2555345, 1e-5) );
  CHECK( tss[24]->data()[10751] == AROUND(7.89E-06, 1e-5) );
  CHECK( tss[25]->data()[10751] == AROUND(-0.746408, 1e-5) );
  CHECK( tss[26]->data()[10751] == AROUND(0.5076932, 1e-5) );
  CHECK( tss[27]->data()[10751] == AROUND(-0.5124366, 1e-5) );
  CHECK( tss[28]->data()[10751] == AROUND(-7.22E-06, 1e-5) );
  CHECK( tss[29]->data()[10751] == AROUND(-0.6771045, 1e-5) );
  CHECK( tss[30]->data()[10751] == AROUND(-0.5808761, 1e-5) );
  CHECK( tss[31]->data()[10751] == AROUND(-0.1280064, 1e-5) );
  CHECK( tss[32]->data()[10751] == AROUND(-1.43E-05, 1e-5) );
  CHECK( tss[33]->data()[10751] == AROUND(-0.7587223, 1e-5) );
  CHECK( tss[34]->data()[10751] == AROUND(-0.1950575, 1e-5) );
  CHECK( tss[35]->data()[10751] == AROUND(0.5104636, 1e-5) );
  CHECK( tss[36]->data()[10751] == AROUND(1.51E-06, 1e-5) );
  CHECK( tss[37]->data()[10751] == AROUND(-0.5763358, 1e-5) );
  CHECK( tss[38]->data()[10751] == AROUND(0.5661312, 1e-5) );
  CHECK( tss[39]->data()[10751] == AROUND(0.0, 1e-5) );
};

void hpfhandlertest_read_Set1_Rep2__v3(ma::Node* root)
{
  REQUIRE( root->children().size() == 1u );
  auto tss = root->findChildren<ma::TimeSequence*>();
  REQUIRE( tss.size() == 8u );

  int inc = 0;
  const auto range = std::array<double,2>{{-1.0, 1.0}};
  for (auto ts : tss)
  {
    REQUIRE( ts->samples() == 60000 );
    CHECK( ts->sampleRate() == 2000.0 );
    CHECK( ts->name() == "EMG Ch" + std::to_string(inc+1) + " (V)" );
    CHECK( ts->unit() == "V" );
    CHECK( ts->range()[0] == range[0] );
    CHECK( ts->range()[1] == range[1] );
    ++inc;
  }
  CHECK( tss[0]->data()[0] == AROUND(1.6327e-5, 1e-9) );
  CHECK( tss[2]->data()[29999] == AROUND(3.4180e-5, 1e-9) );
  CHECK( tss[5]->data()[12000] == AROUND(3.4180e-5, 1e-9) );
  CHECK( tss[7]->data()[59999] == AROUND(4.3945e-5, 1e-9) );
};

void hpfhandlertest_read_Run_number_53_Plot_and_Store_Rep_1_1(ma::Node* root)
{
  REQUIRE( root->children().size() == 1u );
  auto tss = root->findChildren<ma::TimeSequence*>();
  REQUIRE( tss.size() == 32u );

  const auto range = std::array<double,2>{{-1.65, 1.65}};
  for (auto ts : tss)
  {
    REQUIRE( ts->samples() == 21000 );
    CHECK( ts->sampleRate() == 2000.0 );
    CHECK( ts->range()[0] == range[0] );
    CHECK( ts->range()[1] == range[1] );
  }

  CHECK( tss[ 0]->name() == "R RECTUS FEMORIS: EMG 1" );
  CHECK( tss[ 1]->name() == "R RECTUS FEMORIS: ACC X 1" );
  CHECK( tss[ 2]->name() == "R RECTUS FEMORIS: ACC Y 1" );
  CHECK( tss[ 3]->name() == "R RECTUS FEMORIS: ACC Z 1" );
  CHECK( tss[ 4]->name() == "R TIBIALIS ANTERIOR: EMG 3" );
  CHECK( tss[ 5]->name() == "R TIBIALIS ANTERIOR: ACC X 3" );
  CHECK( tss[ 6]->name() == "R TIBIALIS ANTERIOR: ACC Y 3" );
  CHECK( tss[ 7]->name() == "R TIBIALIS ANTERIOR: ACC Z 3" );
  CHECK( tss[ 8]->name() == "L TIBIALIS ANTERIOR: EMG 4" );
  CHECK( tss[ 9]->name() == "L TIBIALIS ANTERIOR: ACC X 4" );
  CHECK( tss[10]->name() == "L TIBIALIS ANTERIOR: ACC Y 4" );
  CHECK( tss[11]->name() == "L TIBIALIS ANTERIOR: ACC Z 4" );
  CHECK( tss[12]->name() == "R BICEPS FEMORIS: EMG 5" );
  CHECK( tss[13]->name() == "R BICEPS FEMORIS: ACC X 5" );
  CHECK( tss[14]->name() == "R BICEPS FEMORIS: ACC Y 5" );
  CHECK( tss[15]->name() == "R BICEPS FEMORIS: ACC Z 5" );
  CHECK( tss[16]->name() == "L BICEPS FEMORIS: EMG 6" );
  CHECK( tss[17]->name() == "L BICEPS FEMORIS: ACC X 6" );
  CHECK( tss[18]->name() == "L BICEPS FEMORIS: ACC Y 6" );
  CHECK( tss[19]->name() == "L BICEPS FEMORIS: ACC Z 6" );
  CHECK( tss[20]->name() == "R GASTROCNEMIUS LATERAL HEAD: EMG 7" );
  CHECK( tss[21]->name() == "R GASTROCNEMIUS LATERAL HEAD: ACC X 7" );
  CHECK( tss[22]->name() == "R GASTROCNEMIUS LATERAL HEAD: ACC Y 7" );
  CHECK( tss[23]->name() == "R GASTROCNEMIUS LATERAL HEAD: ACC Z 7" );
  CHECK( tss[24]->name() == "L GASTROCNEMIUS LATERAL HEAD: EMG 8" );
  CHECK( tss[25]->name() == "L GASTROCNEMIUS LATERAL HEAD: ACC X 8" );
  CHECK( tss[26]->name() == "L GASTROCNEMIUS LATERAL HEAD: ACC Y 8" );
  CHECK( tss[27]->name() == "L GASTROCNEMIUS LATERAL HEAD: ACC Z 8" );
  CHECK( tss[28]->name() == "L RECTUS FEMORIS: EMG 10" );
  CHECK( tss[29]->name() == "L RECTUS FEMORIS: ACC X 10" );
  CHECK( tss[30]->name() == "L RECTUS FEMORIS: ACC Y 10" );
  CHECK( tss[31]->name() == "L RECTUS FEMORIS: ACC Z 10" );

  CHECK( tss[ 0]->unit() == "V" );
  CHECK( tss[ 1]->unit() == "g" );
  CHECK( tss[ 2]->unit() == "g" );
  CHECK( tss[ 3]->unit() == "g" );
  CHECK( tss[ 4]->unit() == "V" );
  CHECK( tss[ 5]->unit() == "g" );
  CHECK( tss[ 6]->unit() == "g" );
  CHECK( tss[ 7]->unit() == "g" );
  CHECK( tss[ 8]->unit() == "V" );
  CHECK( tss[ 9]->unit() == "g" );
  CHECK( tss[10]->unit() == "g" );
  CHECK( tss[11]->unit() == "g" );
  CHECK( tss[12]->unit() == "V" );
  CHECK( tss[13]->unit() == "g" );
  CHECK( tss[14]->unit() == "g" );
  CHECK( tss[15]->unit() == "g" );
  CHECK( tss[16]->unit() == "V" );
  CHECK( tss[17]->unit() == "g" );
  CHECK( tss[18]->unit() == "g" );
  CHECK( tss[19]->unit() == "g" );
  CHECK( tss[20]->unit() == "V" );
  CHECK( tss[21]->unit() == "g" );
  CHECK( tss[22]->unit() == "g" );
  CHECK( tss[23]->unit() == "g" );
  CHECK( tss[24]->unit() == "V" );
  CHECK( tss[25]->unit() == "g" );
  CHECK( tss[26]->unit() == "g" );
  CHECK( tss[27]->unit() == "g" );
  CHECK( tss[28]->unit() == "V" );
  CHECK( tss[29]->unit() == "g" );
  CHECK( tss[30]->unit() == "g" );
  CHECK( tss[31]->unit() == "g" );

  CHECK( tss[ 0]->data()[0] == AROUND(-1.02388E-05, 1e-5) );
  CHECK( tss[ 1]->data()[0] == AROUND(-0.989828765, 1e-5) );
  CHECK( tss[ 2]->data()[0] == AROUND(-0.031906806, 1e-5) );
  CHECK( tss[ 3]->data()[0] == AROUND(-0.031403169, 1e-5) );
  CHECK( tss[ 4]->data()[0] == AROUND(-1.14137E-05, 1e-5) );
  CHECK( tss[ 5]->data()[0] == AROUND(-1.000256777, 1e-5) );
  CHECK( tss[ 6]->data()[0] == AROUND(-0.142900556, 1e-5) );
  CHECK( tss[ 7]->data()[0] == AROUND(-0.23492077 , 1e-5) );
  CHECK( tss[ 8]->data()[0] == AROUND(-0.000354833, 1e-5) );
  CHECK( tss[ 9]->data()[0] == AROUND(-0.972704828, 1e-5) );
  CHECK( tss[10]->data()[0] == AROUND(-0.097201049, 1e-5) );
  CHECK( tss[11]->data()[0] == AROUND(-0.333123446, 1e-5) );
  CHECK( tss[12]->data()[0] == AROUND(-2.23239E-05, 1e-5) );
  CHECK( tss[13]->data()[0] == AROUND(-1.026670933, 1e-5) );
  CHECK( tss[14]->data()[0] == AROUND(0.          , 1e-5) );
  CHECK( tss[15]->data()[0] == AROUND(-0.266495347, 1e-5) );
  CHECK( tss[16]->data()[0] == AROUND(2.51774E-06 , 1e-5) );
  CHECK( tss[17]->data()[0] == AROUND(-0.989518702, 1e-5) );
  CHECK( tss[18]->data()[0] == AROUND(0.047027297 , 1e-5) );
  CHECK( tss[19]->data()[0] == AROUND(-0.195257366, 1e-5) );
  CHECK( tss[20]->data()[0] == AROUND(-2.56809E-05, 1e-5) );
  CHECK( tss[21]->data()[0] == AROUND(-0.962079287, 1e-5) );
  CHECK( tss[22]->data()[0] == AROUND(0.015628289 , 1e-5) );
  CHECK( tss[23]->data()[0] == AROUND(-0.35240519 , 1e-5) );
  CHECK( tss[24]->data()[0] == AROUND(0.000199908 , 1e-5) );
  CHECK( tss[25]->data()[0] == AROUND(-0.945137084, 1e-5) );
  CHECK( tss[26]->data()[0] == AROUND(-0.174561858, 1e-5) );
  CHECK( tss[27]->data()[0] == AROUND(-0.383455008, 1e-5) );
  CHECK( tss[28]->data()[0] == AROUND(-5.87472E-06, 1e-5) );
  CHECK( tss[29]->data()[0] == AROUND(-1.00484848 , 1e-5) );
  CHECK( tss[30]->data()[0] == AROUND(-0.109455965, 1e-5) );
  CHECK( tss[31]->data()[0] == AROUND(0.032571524 , 1e-5) );

  CHECK( tss[ 0]->data()[10809] == AROUND( -1.69528E-05, 1e-5) );
  CHECK( tss[ 1]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[ 2]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[ 3]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[ 4]->data()[10809] == AROUND(4.51514E-05  , 1e-5) );
  CHECK( tss[ 5]Q->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[ 6]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[ 7]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[ 8]->data()[10809] == AROUND(-0.000280644 , 1e-5) );
  CHECK( tss[ 9]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[10]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[11]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[12]->data()[10809] == AROUND(1.99741E-05  , 1e-5) );
  CHECK( tss[13]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[14]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[15]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[16]->data()[10809] == AROUND(-8.94636E-05 , 1e-5) );
  CHECK( tss[17]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[18]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[19]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[20]->data()[10809] == AROUND(0.000155261  , 1e-5) );
  CHECK( tss[21]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[22]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[23]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[24]->data()[10809] == AROUND(1.17494E-06  , 1e-5) );
  CHECK( tss[25]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[26]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[27]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[28]->data()[10809] == AROUND(-8.72816E-06 , 1e-5) );
  CHECK( tss[29]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[30]->data()[10809] == AROUND(0            , 1e-5) );
  CHECK( tss[31]->data()[10809] == AROUND(0            , 1e-5) );

  CHECK( tss[ 0]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 1]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 2]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 3]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 4]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 5]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 6]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 7]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 8]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[ 9]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[10]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[11]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[12]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[13]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[14]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[15]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[16]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[17]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[18]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[19]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[20]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[21]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[22]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[23]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[24]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[25]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[26]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[27]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[28]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[29]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[30]->data()[20999] == AROUND(0.0, 1e-5) );
  CHECK( tss[31]->data()[20999] == AROUND(0.0, 1e-5) );
}

#endif // hpfhandlertest_def_h
