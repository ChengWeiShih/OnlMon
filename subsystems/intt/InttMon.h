#ifndef INTT_INTTMON_H
#define INTT_INTTMON_H

#include <onlmon/OnlMon.h>

class Event;

class OnlMonDB;

// remove this later
class TRandom;

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

	int InitHists();

protected:
	int NumEvtIn;
	int NumEvtAna;

	// remove this later
	TRandom* rnd;

	static const int NARMS = 2;
	static const int NROCS = 2;
	static const int NSTATIONS = 2;

	time_t start_time;

	TH2* hFvtxDisk[NARMS][NSTATIONS];
	TH1* hFvtxAdc[NARMS][NSTATIONS];
	TH1* fvtxH1NumEvent;
	TH1* hFvtxYields[NARMS];
	TH1* hFvtxYieldsByPacket[NARMS];
	TH2* hFvtxChipChannel[NROCS];
	TH2* hFvtxControlWord[NARMS];
	TH2* hFvtxYieldsByPacketVsTime[NARMS];
	TH1* hFvtxEventNumberVsTime;
	TH2* hFvtxYieldsByPacketVsTimeShort[NARMS];
	TH1* hFvtxEventNumberVsTimeShort;
};

#endif /* INTT_INTTMON_H */
