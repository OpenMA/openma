#ifndef c3dhandlerTest_def_h

#include "openma/io/handler.h"
#include "openma/base/node.h"
#include "openma/base/trial.h"
#include "openma/base/timesequence.h"
#include "openma/base/event.h"

inline void c3dhandlertest_sample01(const char* msgid, const char* filename, const char* filepath)
{
  ma::io::File file;
  file.open(filepath, ma::io::Mode::In);
  ma::io::HandlerReader reader(&file, "org.c3d");
  ma::Node root("root");
  
  TSM_ASSERT_EQUALS(msgid, reader.read(&root),true);
  TSM_ASSERT_EQUALS(msgid, reader.errorCode(), ma::io::Error::None);
  TSM_ASSERT_EQUALS(msgid, reader.errorMessage(), "");
  TSM_ASSERT_EQUALS(msgid, root.children().size(),1ul);
  
  ma::Trial* trial = root.findChild<ma::Trial*>();
  TS_ASSERT_DIFFERS(trial, nullptr);
  if (trial == nullptr)
    return;
  TSM_ASSERT_EQUALS(msgid, trial->name(), std::string(filename));
  
  ma::TimeSequence* ts = trial->timeSequences()->findChild<ma::TimeSequence*>("RFT1");
  TSM_ASSERT_DIFFERS(msgid, ts, nullptr);
  TSM_ASSERT_EQUALS(msgid, ts->description(), "DIST/LAT FOOT");
  TSM_ASSERT_EQUALS(msgid, ts->sampleRate(), 50.0);
  TSM_ASSERT_DELTA(msgid, ts->data(8,0), 250.4166, 1e-4);
  TSM_ASSERT_DELTA(msgid, ts->data(0,3), 1.3333, 1e-4);
  TSM_ASSERT_DELTA(msgid, ts->data()[3*ts->samples()], 1.3333, 1e-4);
  
  auto tss = trial->timeSequences()->findChildren<ma::TimeSequence*>();
  TSM_ASSERT_EQUALS(msgid, tss.size(), 26u+16u); // 26 videos and 16 analogs
  TSM_ASSERT_DELTA(msgid, tss[14]->data(20,3), 1.91667, 1e-4);
  TSM_ASSERT_DELTA(msgid, tss[17]->data(16,0), 285.0, 1e-4);
  TSM_ASSERT_DELTA(msgid, tss[17]->data(16,1), 222.4166, 1e-4);
  TSM_ASSERT_DELTA(msgid, tss[17]->data(16,2), 564.4166, 1e-4);
  TSM_ASSERT_DELTA(msgid, tss[26]->data(0), -26.6599, 1e-3);
  TSM_ASSERT_DELTA(msgid, tss[26]->data(1), -25.8, 1e-4);
  TSM_ASSERT_EQUALS(msgid, tss[28]->description(), "FORCE Z-COMP 1");
  TSM_ASSERT_EQUALS(msgid, tss[41]->name(), "CH16");

  auto evts = trial->events()->findChildren<ma::Event*>();
  TSM_ASSERT_EQUALS(msgid, evts.size(), 3u);
  TSM_ASSERT_EQUALS(msgid, evts[0]->name(), "RIC");
  TSM_ASSERT_DELTA(msgid, evts[0]->time(), 2.72, 1e-4);
  TSM_ASSERT_EQUALS(msgid, evts[1]->name(), "RHS");
  TSM_ASSERT_DELTA(msgid, evts[1]->time(), 5.40, 1e-4);
  TSM_ASSERT_EQUALS(msgid, evts[2]->name(), "RTO");
  TSM_ASSERT_DELTA(msgid, evts[2]->time(), 7.32, 1e-4);
};

#endif // c3dhandlerTest_def_h

