#ifndef INTT_INTTMON_H
#define INTT_INTTMON_H

#include <onlmon/OnlMon.h>
//testest
class Event;
class OnlMonDB;
class TH1;
class TH2;

class InttMon : public OnlMon
{
 public:
  InttMon(const std::string &name = "INTTMON");
  virtual ~InttMon();

  int process_event(Event *evt);
  int Init();
  int BeginRun(const int runno);
  int Reset();

 protected:
  int DBVarInit();
  int evtcnt = 0;
  int idummy = 0;
  OnlMonDB *dbvars = nullptr;
  TH1 *intthist1 = nullptr;
  TH2 *intthist2 = nullptr;
};

#endif /* INTT_INTTMON_H */
