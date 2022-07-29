// use #include "" only for your local include and put
// those in the first line(s) before any #include <>
// otherwise you are asking for weird behavior
// (more info - check the difference in include path search when using "" versus <>)

#include "InttMon.h"

#include <onlmon/OnlMon.h>  // for OnlMon
#include <onlmon/OnlMonDB.h>
#include <onlmon/OnlMonServer.h>

#include <Event/Event.h>
#include <Event/msg_profile.h>

#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>

#include <cmath>
#include <cstdio>  // for printf
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>  // for allocator, string, char_traits

enum
{
	TRGMESSAGE = 1,
	FILLMESSAGE = 2
};

InttMon::InttMon(const std::string &name) : OnlMon(name)
{
	// leave ctor fairly empty, its hard to debug if code crashes already
	// during a new InttMon()
	
	return;
}

InttMon::~InttMon()
{
	// you can delete NULL pointers it results in a NOOP (No Operation)
	//Depending on how these are associated with an output file, they might not be deletable

	return;
}

int InttMon::InitHists()
{
	OnlMonServer* se = OnlMonServer::instance();

	const char *CHARM[] = {"S", "N"};
	const int base_packet_number[] = {25000, 25100};

	for(int i = 0; i < NARMS; i++)
        {
		for (int istation=0; istation < NSTATIONS; istation++)
		{
			int nchips = 26;
			if (istation == 0) nchips = 10;
			hFvtxDisk[i][istation] = new TH2F(
				Form("hFvtxDisk%d_%d", i, istation),
				Form("Hit activity for %s arm - station %d; wedge; chip", CHARM[i], istation),
				48, -0.5, 47.5,
				nchips, -0.5, nchips-0.5);
			se->registerHisto(this, hFvtxDisk[i][istation]);

			hFvtxAdc[i][istation] = new TH1F(
				Form("hFvtxAdc%d_%d", i, istation),
				Form("%s - station %d; ADC value; counts", CHARM[i], istation),
				8, -0.5, 7.5);
			se->registerHisto(this, hFvtxAdc[i][istation]);
		}
		
		hFvtxYields[i] = new TH1F(
			Form("hFvtxYields%d", i),
			Form("Yield per event vs. wedge for %s arm; wedge; N_{Hit}/N_{Event}", CHARM[i]),
			192, -0.5, 191.5);
		se->registerHisto(this, hFvtxYields[i]);

		hFvtxYieldsByPacket[i] = new TH1F(
			Form("hFvtxYieldsByPacket%d", i),
			Form("Yield per event vs. packet for %s arm; packet number - %d; N_{Hit}", CHARM[i], base_packet_number[i]),
			24, 0.5, 24.5);
		se->registerHisto(this, hFvtxYieldsByPacket[i]);

		hFvtxControlWord[i] = new TH2F(Form("hFvtxControlWord%d", i),
			Form("FEM Control Word for %s arm; packet number - %d; bit", CHARM[i], base_packet_number[i]),
			24, 0.5, 24.5,
			16, -0.5, 15.5);
		se->registerHisto(this, hFvtxControlWord[i]);

		hFvtxYieldsByPacketVsTime[i] = new TH2F(Form("hFvtxYieldsByPacketVsTime%d",i),
			Form("Yield per event of packets vs. time for %s arm; time(min) ; packet number - %d", CHARM[i], base_packet_number[i]),
			960, 0, 480,
			24, 0.5, 24.5);
		se->registerHisto(this, hFvtxYieldsByPacketVsTime[i]);

                hFvtxYieldsByPacketVsTimeShort[i] = new TH2F(Form("hFvtxYieldsByPacketVsTimeShort%d",i),
			Form("Yield per event of packets vs. time for %s arm; time(s) ; packet number - %d", CHARM[i], base_packet_number[i]),
			600, 0, 600,
			24, 0.5, 24.5);
		se->registerHisto(this, hFvtxYieldsByPacketVsTimeShort[i]);
	}

        fvtxH1NumEvent = new TH1F(
			Form("fvtxH1NumEvent"),
                        Form("Number of processed events"),
                        1, 0.5, 1.5);
        se->registerHisto(this,fvtxH1NumEvent);

        hFvtxEventNumberVsTime = new TH1F(
			Form("hFvtxEventNumberVsTime"),
                        Form("EventNumberVsTime"),
                        960, 0, 480);
        se->registerHisto(this,hFvtxEventNumberVsTime);

        hFvtxEventNumberVsTimeShort = new TH1F(
			Form("hFvtxEventNumberVsTimeShort"),
                        Form("EventNumberVsTimeShort"),
                        600, 0, 600);
        se->registerHisto(this,hFvtxEventNumberVsTimeShort);

	return 0;
}

int InttMon::Init()
{
	// read our calibrations from InttMonData.dat
	std::string fullfile = std::string(getenv("INTTCALIB")) + "/" + "InttMonData.dat";
	std::ifstream calib(fullfile);
	calib.close();

	std::cout << "Running Joseph B's version" << std::endl;

	// initialize the histograms
	InitHists();

	// remove this line later
	rnd = new TRandom();

	Reset();

	return 0;
}

int InttMon::BeginRun(const int /*runno*/)
{
	// if you need to read calibrations on a run by run basis
	// this is the place to do it
	return 0;
}

int InttMon::process_event(Event* evt)
{
        NumEvtIn++;
        if (verbosity>2)
	{
		std::cout << "FvtxMon::process_event("<<NumEvtIn<<")"<<std::endl;
	}

        time_t time = evt->getTime();
        if(start_time < 1)
        {
                start_time = time;
        }

        fvtxH1NumEvent->Fill(1); // one entry per event

        hFvtxEventNumberVsTime->Fill((time - start_time)/60.);
        if(time - start_time <= 600)hFvtxEventNumberVsTimeShort->Fill((time - start_time));

	// no unpacker lol
	// random values below
	float dum1 = 0;
	float dum2 = 0;

	for(int arm = 0; arm < NARMS; arm++)
	{
		for(int station = 0; station < NSTATIONS; station++)
		{
			dum1 = rnd->Exp(5.0);
			dum2 = rnd->Gaus(5.0, 1.0);
			hFvtxDisk[arm][station]->Fill(dum1, dum2);

			dum1 = rnd->Exp(3.0);
			hFvtxAdc[arm][station]->Fill(dum1);
		}

		dum1 = rnd->Exp(110.0);
		hFvtxYields[arm]->Fill(dum1);

		dum1 = rnd->Gaus(12.5, 3.5);
		hFvtxYieldsByPacket[arm]->Fill(dum1);

		dum1 = rnd->Gaus(12.5, 3.5);
		dum2 = rnd->Exp(7.5);
		hFvtxControlWord[arm]->Fill(dum1, dum2);
	}

	return 0;
}

int InttMon::Reset()
{
	NumEvtIn = 0;
	//NumEvtAna = 0;
	start_time = 0;

	return 0;
}

