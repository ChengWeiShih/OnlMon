#include "InttMonDraw.h"

#include <onlmon/OnlMonClient.h>
#include <onlmon/OnlMonDB.h>
#include <onlmon/RunDBodbc.h>

#include <phool/phool.h>

#include <TAxis.h>  // for TAxis
#include <TCanvas.h>
#include <TDatime.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TPie.h>
#include <TPieSlice.h>
#include <TLine.h>
#include <TPad.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TText.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TArrow.h>

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>  // for memset
#include <ctime>
#include <iomanip>
#include <fstream>
#include <iostream>  // for operator<<, basic_ostream, basic_os...
#include <sstream>
#include <vector>  // for vector
#include <string>

using namespace std;

const char *CARM[] = { "South", "North" };
ofstream outfile("fvtxclientlog.txt");

// Definitions for array of canvases
const int SUMMARYS = 0;
const int SUMMARYN = 1;
const int YIELDSS = 2;
const int YIELDSN = 3;
const int YIELDSPACKETS = 4;
const int YIELDSPACKETN = 5;
const int ADCS = 6;
const int ADCN = 7;
const int CHIPCHANNELSE = 8;
const int CHIPCHANNELNE = 9;
const int CHIPCHANNELSW = 10;
const int CHIPCHANNELNW = 11;
const int CONTROLWORDS = 12;
const int CONTROLWORDN = 13;
const int YIELDSPACKETSVSTIME = 14;
const int YIELDSPACKETNVSTIME = 15;
const int YIELDSPACKETSVSTIMESHORT = 16;
const int YIELDSPACKETNVSTIMESHORT = 17;

//title : to be checked
const int NumberOfStations_temp = 12; //originally from -> FVTXGEOM::NumberOfStations

const char *ROCMap12[] = {  //set labels for Yield per Event plots here
  "SW0",  //      AB?
  "SW1",  //                       A -> 5B              B?
  "SW2",  // Large leak current in Run-12    A -> 0A   B -> 0B
  "SW3",  //                       A -> 1A   B -> 2A     B -> 2A
  "SW4",  //                       A -> 2B   B -> 3A     B -> 3A
  "SW5",  //  A -> 3B   B -> 4A     B -> 4A

  "SE0",  // Fiber Out in Run-12            B -> 0B    A?
  "SE1",  //                       A -> 1A  B -> 1B 
  "SE2",  //                       A -> 2A  B -> 2B 
  "SE3",  //                       A -> 3A?             B?
  "SE4",  //                           A -> 4A  B -> 4B
  "SE5"  //                           A -> 5A  B -> 5B
  //  "---",
  //  "---",
  //  "---",
  //  "---"
}; 

const char *ROCMap34[] = {

  "NW0",  // Large leak current in Run-12  A -> 0A   B -> 0B
  "NW1",  //                     A -> 1A   B -> 1B
  "NW2",  //  B1 fixed itself at 4BOFF in Run-12           AB?        
  "NW3",  //                     A -> 3A   B -> 3B      
  "NW4",  //                     A -> 4A   B -> 4B
  "NW5",  //                     B -> 5B   A?

  "NE0",  //            A -> 2A    B -> 2B
  "NE1",  //                             A -> 2A    B -> 2B
  "NE2",  //                             A -> 2A    B -> 2B
  "NE3",  //                             A -> 3A    B -> 3B
  "NE4",  //                             A -> 4A    B -> 4B
  "NE5"   //                             A -> 5A    B -> 5B
  //  "---",
  //  "---",
  //  "---"
};


const char *ROCMap12_cw[] = {  //set labels for Control Word plots here
  "SW0",  //  
  "SW1",  //                
  "SW2",  //
  "SW3",  //                
  "SW4",  //                 
  "SW5",  // 

  "SE0",  //
  "SE1",  //             
  "SE2",  //              
  "SE3",  //                  
  "SE4",  //                   
  "SE5"  //                  

}; 

const char *ROCMap34_cw[] = {

  "NW0",
  "NW1",  //                 
  "NW2",  //   
  "NW3",  //                     
  "NW4",  //                   
  "NW5",  //                 

  "NE0",  //          
  "NE1",  //                          
  "NE2",  //                          
  "NE3",  //                          
  "NE4",  //                        
  "NE5"   //                        

};

const char *ControlWordBit[] = {

  "Buck0 ",
  "Buck1 ",  //                 
  "Buck2 ",  //   
  "Buck3 ",  //                     
  "FPHX0 ",  //                   
  "FPHX1 ",  //                 
  "FPHX2",  //          
  "FPHX3 ",  //                          
  "ROC0 ",  //                          
  "ROC1 ",  //                          
  "ROC2 ",  //                        
  "ROC3 ",   //                        
  "FEM "
};

















InttMonDraw::InttMonDraw(const std::string &name)
  : OnlMonDraw(name)
{
  
  m_run_number = -1;
  m_run_type = "";
  rundb_odbc = new RunDBodbc();  
  // rundb_odbc = new RunDBodbc();


  canv_name[SUMMARYS]="fvtxSummarySouth";
  canv_name[SUMMARYN]="fvtxSummaryNorth";
  canv_name[YIELDSS]="fvtxYieldsSouth";
  canv_name[YIELDSN]="fvtxYieldsNorth";
  canv_name[YIELDSPACKETS]="fvtxYieldsByPacketSouth";
  canv_name[YIELDSPACKETN]="fvtxYieldsByPacketNorth";
  canv_name[ADCS]="fvtxAdcSouth";
  canv_name[ADCN]="fvtxAdcNorth";
  canv_name[CHIPCHANNELSE]="fvtxChipVsChannelSouthE";
  canv_name[CHIPCHANNELNE]="fvtxChipVsChannelNorthE";
  canv_name[CHIPCHANNELSW]="fvtxChipVsChannelSouthW";
  canv_name[CHIPCHANNELNW]="fvtxChipVsChannelNorthW";
  canv_name[CONTROLWORDS]="fvtxControlWordS";
  canv_name[CONTROLWORDN]="fvtxControlWordN";
  canv_name[YIELDSPACKETSVSTIME]="fvtxYieldsByPacketSouthVsTime";
  canv_name[YIELDSPACKETNVSTIME]="fvtxYieldsByPacketNorthVsTime";
  canv_name[YIELDSPACKETSVSTIMESHORT]="fvtxYieldsByPacketSouthVsTimeShort";
  canv_name[YIELDSPACKETNVSTIMESHORT]="fvtxYieldsByPacketNorthVsTimeShort";


  canv_desc[SUMMARYS]="fvtx_Summary_By_Wedge_South";
  canv_desc[SUMMARYN]="fvtx_Summary_By_Wedge_North";
  canv_desc[YIELDSS]="fvtx_Yields_Per_Event_By_Wedge_South";
  canv_desc[YIELDSN]="fvtx_Yields_Per_Event_By_Wedge_North";
  canv_desc[YIELDSPACKETS]="fvtx_Yields_Per_Event_By_Packet_South";
  canv_desc[YIELDSPACKETN]="fvtx_Yields_Per_Event_By_Packet_North";
  canv_desc[ADCS]="fvtx_Adc_By_Station_South";
  canv_desc[ADCN]="fvtx_Adc_By_Station_North";
  canv_desc[CHIPCHANNELSE]="fvtx_Chip_Vs_Channel_By_Roc_SouthE";
  canv_desc[CHIPCHANNELNE]="fvtx_Chip_Vs_Channel_By_Roc_NorthE";
  canv_desc[CHIPCHANNELSW]="fvtx_Chip_Vs_Channel_By_Roc_SouthW";
  canv_desc[CHIPCHANNELNW]="fvtx_Chip_Vs_Channel_By_Roc_NorthW";
  canv_desc[CONTROLWORDS]="fvtx_FEM_Control_Word_South";
  canv_desc[CONTROLWORDN]="fvtx_FEM_Control_Word_North";
  canv_desc[YIELDSPACKETSVSTIME]="fvtx_Yields_Per_Event_By_Packet_Vs_Time_South";
  canv_desc[YIELDSPACKETNVSTIME]="fvtx_Yields_Per_Event_By_Packet_Vs_Time_North";
  canv_desc[YIELDSPACKETSVSTIMESHORT]="fvtx_Yields_Per_Event_By_Packet_Vs_Time_South_Short";
  canv_desc[YIELDSPACKETNVSTIMESHORT]="fvtx_Yields_Per_Event_By_Packet_Vs_Time_North_Short";

  npad[SUMMARYS]=4;
  npad[SUMMARYN]=4;
  npad[YIELDSS]=1;
  npad[YIELDSN]=1;
  npad[YIELDSPACKETS]=1;
  npad[YIELDSPACKETN]=1;
  npad[ADCS]=4;
  npad[ADCN]=4;
  npad[CHIPCHANNELSE]=6;
  npad[CHIPCHANNELNE]=6;
  npad[CHIPCHANNELSW]=6;
  npad[CHIPCHANNELNW]=6;
  npad[CONTROLWORDS]=1;
  npad[CONTROLWORDN]=1;
  npad[YIELDSPACKETSVSTIME]=1;
  npad[YIELDSPACKETNVSTIME]=1;
  npad[YIELDSPACKETSVSTIMESHORT]=1;
  npad[YIELDSPACKETNVSTIMESHORT]=1;

  
  for(int icanvas=0; icanvas<ncanvas; icanvas++)
    {
      TC[icanvas]=0;
      transparent[icanvas]=0;
      for(int ipad=0; ipad<nmaxpad; ipad++)
        {
          TP[icanvas][ipad]=0;
        }
    }

  double vals[48];
  for (int ival=0; ival<48; ival++) vals[ival] = 1.;
  pieFvtxDisk = new TPie("pieFvtxDisk","", 48, vals);
  pieFvtxDisk->SetCircle(0.5,0.5,0.45);

  double vals2[12];
  for (int ival=0; ival<12; ival++)
    {
      vals2[ival] = 1.;
    }

  double pie_r = pieFvtxDisk->GetRadius();
  pieROCname = new TPie("pieROCname","", 12, vals2);
  pieROCname->SetRadius(0.65*pie_r);
  pieROCname->SetLabelsOffset(-0.10);

  double vals3[2];
  for (int ival=0; ival<1; ival++) vals3[ival] = 1.;
  pieCenter = new TPie("pieCenter","", 2, vals3);
  pieCenter->SetRadius(0.35*pie_r);
  pieCenter->SetLabelsOffset(-0.25);


  // CAA Set everything for text, lines, etc.
  tsta0 = new TPaveText(0.2,0.8,0.2,0.90,"NDC");
  tsta0->SetTextSize(0.035);
  tsta0->SetFillColor(kWhite);
  tsta0->SetBorderSize(0);
  tsta0->AddText("Station 0");
  tsta0->AddText("West      East");

  tsta1 = new TPaveText(0.4,0.8,0.4,0.88,"NDC");
  tsta1->SetTextSize(0.035);
  tsta1->SetFillColor(kWhite);
  tsta1->SetBorderSize(0);
  tsta1->AddText("Station 1");
  tsta1->AddText("West      East");

  tsta2 = new TPaveText(0.6,0.8,0.6,0.88,"NDC");
  tsta2->SetTextSize(0.035);
  tsta2->SetFillColor(kWhite);
  tsta2->SetBorderSize(0);
  tsta2->AddText("Station 2");
  tsta2->AddText("West      East");

  tsta3 = new TPaveText(0.8,0.8,0.8,0.88,"NDC");
  tsta3->SetTextSize(0.035);
  tsta3->SetFillColor(kWhite);
  tsta3->SetBorderSize(0);
  tsta3->AddText("Station 3");
  tsta3->AddText("West      East");

  line1 = new TLine(48-0.5,0,48-0.5,50);
  line1->SetLineStyle(2);
  line2 = new TLine(96-0.5,0,96-0.5,50);
  line2->SetLineStyle(2);
  line3 = new TLine(144-0.5,0,144-0.5,50);
  line3->SetLineStyle(2);
  
  line_ccs1 = new TLine(128,0,128,104);
  line_ccs1->SetLineColor(kMagenta);
  line_ccs2 = new TLine(256,0,256,104);
  line_ccs2->SetLineColor(kMagenta);
  line_ccs3 = new TLine(384,0,384,104);
  line_ccs3->SetLineColor(kMagenta);
  line_ccs4 = new TLine(0,26,512,26);
  line_ccs4->SetLineColor(kMagenta);
  line_ccs5 = new TLine(0,52,512,52);
  line_ccs5->SetLineColor(kMagenta);
  line_ccs6 = new TLine(0,78,512,78);
  line_ccs6->SetLineColor(kMagenta);


  for (int i=0; i<12; i++)
    {
      lineROC[i] = new TLine(i*2+.5,0,i*2+.5,7);
      lineROC[i]->SetLineColor(kRed);

      lineROC_time[i] = new TLine(0,i*2+.5,600,i*2+.5);
      lineROC_time[i]->SetLineColor(kBlack);

      lineROC_cw[i] = new TLine(i*2+.5,0,i*2+.5,15.5);
      lineROC_cw[i]->SetLineColor(kRed);

      for (int iarm=0; iarm<NARMS; iarm++)
        {    
          TString ROCMap = iarm==0?TString(ROCMap12[i]):TString(ROCMap34[i]);

          textROC[iarm][i] = new TText(i*2+1.5,0,ROCMap);
          textROC[iarm][i]->SetTextColor(kMagenta);
          textROC[iarm][i]->SetTextAlign(12);
          textROC[iarm][i]->SetTextAngle(90);
          textROC[iarm][i]->SetTextSize(.05);

          textROC_time[iarm][i] = new TText(0,i*2+1.5,ROCMap);
          textROC_time[iarm][i]->SetTextColor(kBlack);
          textROC_time[iarm][i]->SetTextAlign(12);
          textROC_time[iarm][i]->SetTextSize(.05);

          ROCMap = iarm==0?TString(ROCMap12_cw[i]):TString(ROCMap34_cw[i]);

          textROC_cw[iarm][i] = new TText(i*2+1.5,0,ROCMap);
          textROC_cw[iarm][i]->SetTextColor(kMagenta);
          textROC_cw[iarm][i]->SetTextAlign(12);
          textROC_cw[iarm][i]->SetTextAngle(90);
          textROC_cw[iarm][i]->SetTextSize(.05);

        }
    }

  for (int i=0; i<13; i++) text_cw_bit[i] = new TText(-2.65,i-0.5,TString(ControlWordBit[i]) );



  ostringstream armStation;
  for (int iarm=0; iarm<NARMS; iarm++) 
    {
      for (int istation=0; istation<NSTATIONS; istation++) 
        {
          //      ptArmStation[iarm][istation] = new TPaveText(0.2, 0.96, 0.3, 0.99,"NDC");
          ptArmStation[iarm][istation] = new TPaveText(0.35, 0.45, 0.65, 0.55,"NDC");
          ptArmStation[iarm][istation]->SetTextSize(0.07);
          ptArmStation[iarm][istation]->SetFillColor(kWhite);
          ptArmStation[iarm][istation]->SetBorderSize(0);
          armStation.str("");
          armStation << CARM[iarm] << " St. " << istation;
          ptArmStation[iarm][istation]->AddText(armStation.str().c_str());
        }
    }
  //  return ;







  //title : to be checked
  // this TimeOffsetTicks is neccessary to get the time axis right
  TDatime T0(2003, 01, 01, 00, 00, 00);
  TimeOffsetTicks = T0.Convert();
  dbvars = new OnlMonDB(ThisName);
  return;
}

InttMonDraw::~InttMonDraw()
{
  delete pieFvtxDisk;
  delete pieROCname;
  delete pieCenter;
  delete tsta0;
  delete tsta1;
  delete tsta2;
  delete tsta3;
  delete line1;
  delete line2;
  delete line_ccs1; 
  delete line_ccs2; 
  delete line_ccs3; 
  delete line_ccs4; 
  delete line_ccs5; 
  delete line_ccs6; 
  delete line3;
  for (int i=0; i<12; i++)
    {
      delete lineROC[i];
      delete lineROC_time[i];
      delete lineROC_cw[i];
      for (int iarm=0; iarm<NARMS; iarm++)
        {
          delete textROC[iarm][i];
          delete textROC_cw[iarm][i];
          delete textROC_time[iarm][i];
        }
    }
  for (int i=0; i<13; i++) delete text_cw_bit[i];

  delete rundb_odbc;

  return;
}

int InttMonDraw::Init()
{
  return 0;
}

void InttMonDraw::registerHists()
{
  OnlMonClient *cl = OnlMonClient::instance();
  m_run_number = cl->RunNumber();
  m_run_type = rundb_odbc->RunType(m_run_number);
  
  cl->registerHisto("fvtxH1NumEvent", "FVTXMON");
  cl->registerHisto(Form("hFvtxEventNumberVsTime"), "FVTXMON");
  cl->registerHisto(Form("hFvtxEventNumberVsTimeShort"), "FVTXMON");
  
  for (int i = 0; i < NARMS; i++) 
    {
      cl->registerHisto(Form("hFvtxYields%d",i), "FVTXMON");
      cl->registerHisto(Form("hFvtxYieldsByPacket%d",i), "FVTXMON");
      cl->registerHisto(Form("hFvtxYieldsByPacketVsTime%d",i), "FVTXMON");
      cl->registerHisto(Form("hFvtxYieldsByPacketVsTimeShort%d",i), "FVTXMON");
      cl->registerHisto(Form("hFvtxControlWord%d",i), "FVTXMON");
      for (int istation=0; istation<NumberOfStations_temp; istation++) 
        {
          cl->registerHisto(Form("hFvtxDisk%d_%d",i,istation), "FVTXMON");
          cl->registerHisto(Form("hFvtxAdc%d_%d",i,istation), "FVTXMON");
        }
    }
  for (int iroc = 0; iroc < NROCS; iroc++) 
    {
      cl->registerHisto(Form("hFvtxChipChannel_ROC%d",iroc), "FVTXMON");
    }
  return ;

}

int InttMonDraw::MakeCanvas(const std::string &name)
// int InttMonDraw::MakeCanvas(const char *name)
{
  OnlMonClient *cl = OnlMonClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  
  ostringstream tname;
  ostringstream tdesc;

  float xlow = 0.;
  float xup = 0.;
  float ylow = 0.; 
  float yup = 0.;

  for(int icanvas=0; icanvas<ncanvas; icanvas++)
    {
      if( strcmp(name.c_str(),"ALL") && strcmp(name.c_str(),canv_name[icanvas].c_str()) ) continue;

      // CANVAS
      TC[icanvas]=new TCanvas(canv_name[icanvas].c_str(),canv_desc[icanvas].c_str(),-1 - (icanvas%2 * xsize/2), 0, xsize/2, ysize);
      gSystem->ProcessEvents();

      // PAD(s)
      for(int ipad=0; ipad<npad[icanvas]; ipad++)
        {
          tname.str("");
          tname<<"fvtxPad"<<icanvas<<"_"<<ipad;
          if(npad[icanvas]==1)
            {
              xlow = DISTBORD;
              ylow = DISTBORD + PADSIZEYBOTTOM;
              xup = xlow + (PADSIZEX1 - 2 * DISTBORD);
              yup = ylow + (PADSIZEY1 - 3 * DISTBORD);
              TP[icanvas][ipad]=new TPad(tname.str().c_str(),"",xlow,ylow,xup,yup,0.);
            }
          else if(npad[icanvas]==4) // 4 pads
            {
              xlow = DISTBORD + (ipad % 2) * PADSIZEX2;
              ylow = DISTBORD + (2 - ipad / 2) * PADSIZEY2;
              xup = xlow + (PADSIZEX2 - 2 * DISTBORD);
              yup = ylow + (PADSIZEY2 - 3 * DISTBORD);
              TP[icanvas][ipad]=new TPad(tname.str().c_str(),"",xlow,ylow,xup,yup,0.);
            }
          else if(npad[icanvas]==6) // 6 pads
            {
              xlow = DISTBORD + (ipad % 2) * PADSIZEX2;
              ylow = DISTBORD + PADSIZEYBOTTOM + (ipad / 2) * PADSIZEY3;
              xup = xlow + (PADSIZEX2 - 2 * DISTBORD);
              yup = ylow + (PADSIZEY3 - 3 * DISTBORD);
              TP[icanvas][ipad]=new TPad(tname.str().c_str(),"", xlow, ylow, xup, yup, 0.);
            }
          TP[icanvas][ipad]->Draw();
        }

      // TRANSPARENT
      tname.str("");
      tname<<"fvtxTrans"<<icanvas;
      transparent[icanvas]=new TPad(tname.str().c_str(),"this does not show",0,0,1,1);
      transparent[icanvas]->SetFillStyle(4000);
      transparent[icanvas]->Draw();
    }

  // if (name == "InttMon1")
  // {
  // // xpos (-1) negative: do not draw menu bar
  //TC[0] = new TCanvas(name.c_str(), "InttMon Example Monitor", -1, 0, xsize / 2, ysize);
  //// root is pathetic, whenever a new TCanvas is created root piles up
  //// 6kb worth of X11 events which need to be cleared with
  //// gSystem->ProcessEvents(), otherwise your process will grow and
  //// grow and grow but will not show a definitely lost memory leak
  //gSystem->ProcessEvents();
  //Pad[0] = new TPad("inttpad1", "who needs this?", 0.1, 0.5, 0.9, 0.9, 0);
  //Pad[1] = new TPad("inttpad2", "who needs this?", 0.1, 0.05, 0.9, 0.45, 0);
  //Pad[0]->Draw();
  //Pad[1]->Draw();
  //// this one is used to plot the run number on the canvas
  // transparent[0] = new TPad("transparent0", "this does not show", 0, 0, 1, 1);
  // transparent[0]->SetFillStyle(4000);
  //transparent[0]->Draw();
  //TC[0]->SetEditable(0);
  //}
  //else if (name == "InttMon2")
  //{
  //// xpos negative: do not draw menu bar
  // TC[1] = new TCanvas(name.c_str(), "InttMon2 Example Monitor", -xsize / 2, 0, xsize / 2, ysize);
  // gSystem->ProcessEvents();
  //Pad[2] = new TPad("inttpad3", "who needs this?", 0.1, 0.5, 0.9, 0.9, 0);
  //Pad[3] = new TPad("inttpad4", "who needs this?", 0.1, 0.05, 0.9, 0.45, 0);
  //Pad[2]->Draw();
  //Pad[3]->Draw();
  //// this one is used to plot the run number on the canvas
    //transparent[1] = new TPad("transparent1", "this does not show", 0, 0, 1, 1);
    //transparent[1]->SetFillStyle(4000);
    //transparent[1]->Draw();
    //TC[1]->SetEditable(0);
  //}
  //else if (name == "InttMon3")
  //{
  //TC[2] = new TCanvas(name.c_str(), "InttMon3 Example Monitor", xsize / 2, 0, xsize / 2, ysize);
  //gSystem->ProcessEvents();
  //Pad[4] = new TPad("inttpad5", "who needs this?", 0.1, 0.5, 0.9, 0.9, 0);
  //Pad[5] = new TPad("inttpad6", "who needs this?", 0.1, 0.05, 0.9, 0.45, 0);
  //Pad[4]->Draw();
  //Pad[5]->Draw();
  //// this one is used to plot the run number on the canvas
  ////        transparent[2] = new TPad("transparent2", "this does not show", 0, 0, 1, 1);
  ////        transparent[2]->SetFillStyle(4000);
  ////        transparent[2]->Draw();
  ////      TC[2]->SetEditable(0);
  // }
  return 0;
}

int InttMonDraw::Draw(const std::string &what)
{
  /* int iret = 0;
  int idraw = 0;
  if (what == "ALL" || what == "FIRST")
  {
    iret += DrawFirst(what);
    idraw++;
  }
  if (what == "ALL" || what == "SECOND")
  {
    iret += DrawSecond(what);
    idraw++;
  }
  if (what == "ALL" || what == "HISTORY")
  {
    iret += DrawHistory(what);
    idraw++;
  }
  if (!idraw)
  {
    std::cout << PHWHERE << " Unimplemented Drawing option: " << what << std::endl;
    iret = -1;
  }
  return iret;*/

  int iret = 0;
  int idraw = 0;
  string cmd=what;

  // setup, ROOT appearance; keep the old settings and put them back in the end
  TStyle* oldStyle = gStyle;
  TStyle* fvtxStyle = new TStyle(*gStyle);
  gStyle = fvtxStyle;
  gStyle->Reset();

  gStyle->SetOptStat(0);
  gStyle->SetTitleX(0.2);
  gStyle->SetTitleW(0.6);
  gStyle->SetTitleH(0.08);

  gStyle->SetTitleXSize(0.045);
  gStyle->SetTitleYSize(0.045);
  gStyle->SetTitleXOffset(0.99);
  gStyle->SetTitleYOffset(0.95);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);

  gStyle->SetPalette(1,0);
  // end of settings

  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxSummary"))
    {
      iret+=DrawSummary(what,0); // Second argument is arm
      iret+=DrawSummary(what,1); // Second argument is arm
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxYields"))
    {
      iret+=DrawYieldsByWedge(what,0);
      iret+=DrawYieldsByWedge(what,1);
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxYieldsByPacket"))
    {
      iret+=DrawYieldsByPacket(what,0);
      iret+=DrawYieldsByPacket(what,1);
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxAdc"))
    {
      iret+=DrawAdc(what,0);
      iret+=DrawAdc(what,1);
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxChipVsChannelW"))
    {
      iret+=DrawChipVsChannel(what,0,0); // arm, side
      iret+=DrawChipVsChannel(what,1,0); // arm, side
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxChipVsChannelE"))
    {
      iret+=DrawChipVsChannel(what,0,1); // arm, side
      iret+=DrawChipVsChannel(what,1,1); // arm, side
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxControlWord"))
    {
      iret+=DrawControlWord(what,0);
      iret+=DrawControlWord(what,1);
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxYieldsByPacketVsTime"))
    {
      iret+=DrawYieldsByPacketVsTime(what,0);
      iret+=DrawYieldsByPacketVsTime(what,1);
      idraw++;
    }
  if(!strcmp(what.c_str(),"ALL") || !strcmp(what.c_str(),"fvtxYieldsByPacketVsTimeShort"))
    {
      iret+=DrawYieldsByPacketVsTimeShort(what,0);
      iret+=DrawYieldsByPacketVsTimeShort(what,1);
      idraw++;
      }
  if (!idraw)
    {
      cout << PHWHERE << " Unimplemented Drawing option: " << what << endl;
      iret = -1;
    }
  // go back to original settings
  gStyle = oldStyle;
  delete fvtxStyle;


  return iret;

}



int InttMonDraw::DrawYieldsByWedge(const std::string &what, int arm)
{
  OnlMonClient *cl = OnlMonClient::instance();
  TText fvtx_message;
  int eventNum = 0;
  TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");
  what.c_str();
  if (arm==0 && ! gROOT->FindObject("fvtxYieldsSouth"))
    {
      MakeCanvas("fvtxYieldsSouth");
    }
  else if (arm==1 && ! gROOT->FindObject("fvtxYieldsNorth"))
    {
      MakeCanvas("fvtxYieldsNorth");
    }

  if (arm==0) TC[YIELDSS]->Clear("D");
  else if (arm==1) TC[YIELDSN]->Clear("D");

  if (fvtxH1NumEvent) {
    eventNum = (int)fvtxH1NumEvent->GetEntries();
    cout << "event Num " << eventNum << endl;
    if (eventNum == 0) {
      cout << "FVTX : no events have yet been processed" << endl;
      if (arm==0) {
	TC[YIELDSS]->Clear();
	fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
	TC[YIELDSS]->Update();
      }
      else if (arm==1) {
	TC[YIELDSN]->Clear();
	fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
	TC[YIELDSN]->Update();
      }
      return -1;
    }
    else if (eventNum < FVTXMINEVENTS) {
      cout << "FVTX : not enough events yet" << endl;
      if (arm==0) {
	TC[YIELDSS]->Clear();
	fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
	TC[YIELDSS]->Update();
      }
      else if (arm==1) {
	TC[YIELDSN]->Clear();
	fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
	TC[YIELDSN]->Update();
      }
      return -1;
    }
  }
  else // Dead server
    {
      if (arm==0) {
	DrawDeadServer(transparent[YIELDSS]);
	TC[YIELDSS]->Update();
      }
      else if (arm==1) {
	DrawDeadServer(transparent[YIELDSN]);
	TC[YIELDSN]->Update();
      }

      return -1;
    }

  if (arm==0) {
    TC[YIELDSS]->cd();
    TP[YIELDSS][0]->cd();
  }
  else if (arm==1) {
    TC[YIELDSN]->cd();
    TP[YIELDSN][0]->cd();
  }
  TH1F* hFvtxYields = (TH1F*)cl->getHisto(Form("hFvtxYields%d",arm));
  TH1F* hFvtxYieldsScaled = (TH1F*)hFvtxYields->Clone("hFvtxYieldsClone");
  hFvtxYieldsScaled->Scale(1.0/eventNum);
  //hFvtxYieldsScaled->SetMaximum(28);//run-14 200 GeV Au+Au
  //hFvtxYieldsScaled->SetMaximum(4);//run-14 200 GeV He+Au
  //hFvtxYieldsScaled->SetMaximum(1.);//run-15 200 GeV p+p
  hFvtxYieldsScaled->SetMaximum(MAXHITSPERWEDGE[arm]);
  hFvtxYieldsScaled->SetFillColor(kGray);
  hFvtxYieldsScaled->SetLineColor(kBlack);
  hFvtxYieldsScaled->SetFillStyle(1001);
  hFvtxYieldsScaled->GetXaxis()->SetNdivisions(8,4,0,kFALSE);
  hFvtxYieldsScaled->DrawCopy("");
  line1->Draw();
  line2->Draw();
  line3->Draw();
  tsta0->Draw();
  tsta1->Draw();
  tsta2->Draw();
  tsta3->Draw();

  //--
  TLine l;
  l.SetLineStyle(2);
  l.DrawLine(-0.5,CUT_HOT_WEDGE_ST0[arm],47.5,CUT_HOT_WEDGE_ST0[arm]);
  l.DrawLine(-0.5,CUT_COLD_WEDGE_ST0[arm],47.5,CUT_COLD_WEDGE_ST0[arm]);
  l.DrawLine(47.5,CUT_HOT_WEDGE_ST1[arm],191.5,CUT_HOT_WEDGE_ST1[arm]);
  l.DrawLine(47.5,CUT_COLD_WEDGE_ST1[arm],191.5,CUT_COLD_WEDGE_ST1[arm]);
  //--

  // run and time info
  time_t t = cl->EventTime();
  ostringstream posting;
  posting.str("");
  posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
	  << "   Date:" <<  ctime(&t);

  if (arm==0) transparent[YIELDSS]->cd();
  else if (arm==1) transparent[YIELDSN]->cd();

  TText printRun;
  printRun.SetTextFont(62);
  printRun.SetTextSize(TXTSIZE*0.7);
  printRun.SetNDC();
  printRun.DrawText(0.11, 0.30, posting.str().c_str());

  if (arm==0) TC[YIELDSS]->Update();
  else if (arm==1) TC[YIELDSN]->Update();

  system("cat fvtxclientlog.txt");

  delete hFvtxYieldsScaled;
  return 0;
}

int InttMonDraw::DrawSummary(const std::string &/*what*/, int arm)
{

	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");

	if (arm==0 && ! gROOT->FindObject("fvtxSummarySouth"))
	{
		MakeCanvas("fvtxSummarySouth");
	}
	else if (arm==1 && ! gROOT->FindObject("fvtxSummaryNorth"))
	{
		MakeCanvas("fvtxSummaryNorth");
	}

	if (arm==0) TC[SUMMARYS]->Clear("D");
	else if (arm==1) TC[SUMMARYN]->Clear("D");
	if (fvtxH1NumEvent) 
  {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) 
    {
			cout << "FVTX : no events have yet been processed" << endl;
			outfile << "FVTX : no events have yet been processed" << endl;
			if (arm==0) {
				TC[SUMMARYS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[SUMMARYS]->Update();
			}
			else if (arm==1) {
				TC[SUMMARYN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[SUMMARYN]->Update();
			}
			outfile.close();
			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) 
    {
			cout << "FVTX : not enough events yet" << endl;
			outfile << "FVTX : not enough events yet" << endl;
			if (arm==0) 
      {
				TC[SUMMARYS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[SUMMARYS]->Update();
			}
			else if (arm==1)
      {
				TC[SUMMARYN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[SUMMARYN]->Update();
			}
			outfile.close();
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0) 
    {
			DrawDeadServer(transparent[SUMMARYS]);
			TC[SUMMARYS]->Update();
		}
		else if (arm==1) 
    {
			DrawDeadServer(transparent[SUMMARYN]);
			TC[SUMMARYN]->Update();
		}

		return -1;
	}

	for (int istation=0; istation<NumberOfStations_temp; istation++) {
		if (arm==0) 
    {
			TC[SUMMARYS]->cd();
			TP[SUMMARYS][istation]->cd();
		}
		else if (arm==1) 
    {
			TC[SUMMARYN]->cd();
			TP[SUMMARYN][istation]->cd();
		}

		TH1F* hFvtxYields = (TH1F*)cl->getHisto(Form("hFvtxYields%d",arm));
		for (int iwedge=0; iwedge<48; iwedge++) 
    {
			int color = 1; 

			int bin = hFvtxYields->FindBin(istation*48 + iwedge);
			double hitPerEvent = hFvtxYields->GetBinContent(bin)/eventNum;


			/*   ////Put in the disabled wedges here (make them kGray on the pie plots).
			//from Run12 commented out
			// SE5, SW5, SW4 st0 col3, SE0 st1 col3, NE4 st2 col3, NW0 st0 col0, NW1 st0 col0
			// NE3 st3 col0, NE5 st3 col0, NW5 st0 col0
			if (
			(arm==0 && iwedge>43) || (arm==0 && iwedge>19 && iwedge<24) 
			|| (arm==0 && istation==0 && iwedge==19) || (arm==0 && istation==1 && iwedge==27)
			|| (arm==1 && istation==2 && iwedge==27) || (arm==1 && istation==0 && iwedge==0)
			|| (arm==1 && istation==0 && iwedge==4)  || (arm==1 && istation==3 && iwedge==35) 
			|| (arm==1 && istation==3 && iwedge==44) || (arm==1 && istation==0 && iwedge==20) 
			) 
			color = kGray;
			else if (hitPerEvent<24 && hitPerEvent>2) color = kGreen;
			////Put in the disabled wedges here (make them kGray on the pie plots).
			//from Run-13
			if (   
			(arm==1 && iwedge==22) //NW5, all stations, missing fiber connection
			|| (arm==1 && istation==2 && iwedge==(24+3)  ) //NE0 station 2, sector 3, HV off
			|| (arm==1 && istation==3 && iwedge==(24+16) ) //NE4 station 3, sector 16, HV off
			|| (arm==0 && istation==2 && iwedge==(24+11) ) //SE2 station 2 sector 11, HV off
			|| (arm==1 && istation==0 && iwedge==23  )       //NW5 station 0 sector "3" actually (3)+(5*4)=23
			) 
			color = kGray;
			else if (hitPerEvent<1 && hitPerEvent>0.1) color = kGreen;  //// Run-13 p+p 510 GeV, JMD 11 Mar 2013
			//else if (hitPerEvent<24 && hitPerEvent>2) color = kGreen;  //hits condition for Run-12 Cu+Au
			*/

			//for Run-14
			//if (   
			//		(   arm==0                && iwedge==(24+12) ) //SE3 column out
			//		|| (arm==0 && istation==0 && iwedge==(   21) ) //SW5 st0 sector 21 side 0 missing //
			//		|| (arm==0 && istation==0 && iwedge==(   22) ) //SW5 st 0 sector 3: HV disabled elog 4604
			//		|| (arm==0 && istation==0 && iwedge==(24+ 1) ) //SE0 st0 sector 1 side 1 missing //
			//		|| (arm==0 && istation==0 && iwedge==(24+ 6) ) //SE1 st0 sector 6 missing //
			//		|| (arm==0 && istation==0 && iwedge==(24+10) ) //SE2 st0 sector10 missing 
			//		|| (arm==0 && istation==1 && iwedge==(24+10) ) //SE2 st1 sector10 missing
			//		|| (arm==0 && istation==2 && iwedge==(24+ 7) ) //SE1 st2 sector 7 low efficiency //
			//		|| (arm==0 && istation==2 && iwedge==(24+17) ) //SE4 sector17
			//		|| (arm==0 && istation==0 && iwedge==(24+19) ) //SE4 sector19, come and go wedge
			//		|| (arm==0 && istation==3 && iwedge==(24+ 6) ) //SE1 st0 sector 6 missing side 1 //
			//		|| (arm==0 && istation==3 && iwedge==(24+ 7) ) //SE1 st3 sector 7 low efficiency //

			//		|| (arm==1 && istation==0 && iwedge==(   21) ) //NW5 sector 1 st 0 HV problem, elog 4579
			//		|| (arm==1 && istation==0 && iwedge==(   23) )//NW5 sector23 side 0 missing
			//		|| (arm==1                && iwedge==(   21) )//NW5 column out
			//		|| (arm==1                && iwedge==(24+ 4) )//NE1 column out
			//		|| (arm==1 && istation==3 && iwedge==(24+ 9) ) //NE2 st3 sector 9 side 1 missing
			//		|| (arm==1 && istation==3 && iwedge==(24+16) ) //NE4 sector16
			//		|| (arm==1 && istation==3 && iwedge==(24+21) ) //NE4 sector21, low eff
			//		|| (arm==1 && istation==3 && iwedge==(24+23) ) //NE4 sector23, low eff, missing chip  
			//		) 
			//		color = kGray;

			if(
					false
					//Run-15
					//No HV, marked at Feb. 10, 2015
					//||(arm==0 && istation==0 && iwedge==(00+4*0+1) ) //Station0 SW0 Sector1
					//||(arm==0 && istation==2 && iwedge==(24+4*2+3) ) //Station2 SE2 Sector3
					//||(arm==1 && istation==3 && iwedge==(24+4*4+0) ) //Station3 NE4 Sector0
					//Run-16
					//Feb 13, 2016. http://logbook2.phenix.bnl.gov:7815/FVTX/5173
					||(arm==0 && istation==0 && iwedge==(00+4*0+1) )//Station0 SW0 Sector1
					||(arm==0 && istation==0 && iwedge==(00+4*3+0) )//Station0 SW3 Sector0
					||(arm==0 && istation==0 && iwedge==(24+4*1+2) )//Station0 SE1 Sector2
					||(arm==0 && istation==2 && iwedge==(24+4*4+1) )//Station2 SE4 Sector1
					||(arm==1 && istation==0 && iwedge==(00+4*3+2) )//Station0 NW3 Sector2
					||(arm==1 && istation==1 && iwedge==(00+4*4+2) )//Station1 NW4 Sector2
					||(arm==1 && istation==1 && iwedge==(24+4*0+2) )//Station1 NE0 Sector2
				)
				color = kGray;

			//the factor 5/13 reflect the difference in station 0 wedge size
			//else  if ( (istation>0 && hitPerEvent<5 && hitPerEvent>0.5) || (istation==0 && hitPerEvent<(5*5/13) && hitPerEvent>(0.5*5/13)) ) color = kGreen;  //hits condition for Run-14 15 GeV Au+Au
			//else  if ( (istation>0 && hitPerEvent<130 && hitPerEvent>5) || (istation==0 && hitPerEvent<(130*5/13) && hitPerEvent>(5*5/13)) ) color = kGreen;  //hits condition for Run-14 200 GeV Au+Au
			//else  if (
			//		(arm==0 && istation>0 && hitPerEvent<4 && hitPerEvent>0.20) || (arm==0 && istation==0 && hitPerEvent<1.538 && hitPerEvent>0.1100 ) ||  ///S arm, Au going
			//		(arm==1 && istation>0 && hitPerEvent<3 && hitPerEvent>0.15) || (arm==1 && istation==0 && hitPerEvent<1.150 && hitPerEvent>0.0577 )    //N arm, He going
			//		) 
			//	color = kGreen;  //hits condition for Run-14 200 GeV He+Au
			//else  if ( // Run-15 200 GeV p+p
			//		(istation>0 && hitPerEvent<1.0 && hitPerEvent>0.04) || (istation==0 && hitPerEvent<0.5 && hitPerEvent>0.02 )  
			//		) 
			else  if 
      (
					(istation>0 && hitPerEvent<CUT_HOT_WEDGE_ST1[arm] && hitPerEvent>CUT_COLD_WEDGE_ST1[arm]) 
					|| (istation==0 && hitPerEvent<CUT_HOT_WEDGE_ST0[arm] && hitPerEvent>CUT_COLD_WEDGE_ST0[arm] )  
			) 
				color = kGreen;  //hits condition for Run-14 200 GeV He+Au

			else 
			{
				color = kRed; 
				int cage = 0;
				if (iwedge>23) cage = 1;
				int sector = 0;
				sector = iwedge - cage*24;
				outfile << "Red wedge in arm, cage, station, sector: " <<  arm << " " << cage << " " << istation << " " << sector <<", with hits per event = "<<hitPerEvent<<endl;
			}
			pieFvtxDisk->SetEntryFillColor(iwedge,color);
			if (iwedge%6==0) pieFvtxDisk->SetEntryLabel(iwedge,Form("%d",iwedge));
			else pieFvtxDisk->SetEntryLabel(iwedge,"");
		}
		pieFvtxDisk->SetAngularOffset(90.0-arm*180.);   // rotate so up points up
		pieFvtxDisk->Draw();                              // Draw the colored wedges

		for (int iroc=0; iroc<NROCSperSTATION; iroc++) 
    {  // add the ROC names
			pieROCname->SetEntryFillColor(iroc,kWhite);
			stringstream ss;                                // Make labels NW0 through NE5
			string NS = "";
			NS = (arm==1) ? "N":"S";                        // arm 0 = West, arm 1 = East
			if (iroc<6) { ss << NS << "W" << iroc;   }
			else        { ss << NS << "E" << iroc-6; }
			rocnames[iroc] = ss.str();
			pieROCname->GetSlice(iroc)->SetTitle( rocnames[iroc].c_str() );
		}    
		pieROCname->SetAngularOffset(90.0-arm*180.);
		pieROCname->Draw("same");

		for (int iwedge=0; iwedge<2; iwedge++) 
    {          // add blank circle
			pieCenter->SetEntryFillColor(iwedge,kWhite);
			pieCenter->SetEntryLabel(iwedge,"");
		}    
		pieCenter->SetAngularOffset(90.0-arm*180.);
		pieCenter->Draw("same");

		ptArmStation[arm][istation]->Draw();              // station name label

		if (arm==0) TC[SUMMARYS]->Update();
		else if (arm==1) TC[SUMMARYN]->Update();

	} // End loop over stations

	// run and time info
	time_t t = cl->EventTime();
	//  tm * localT = localtime(&t);
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);
	outfile << posting.str().c_str() << endl;

	if (arm==0) transparent[SUMMARYS]->cd();
	else if (arm==1) transparent[SUMMARYN]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	TText colorCodeGreen, colorCodeGray, colorCodeRed;
	colorCodeGreen.SetTextColor(kGreen);
	colorCodeGreen.SetTextSize(TXTSIZE*0.8);
	colorCodeGreen.SetNDC();
	colorCodeGreen.DrawText(0.15, 0.22,"Good");
	colorCodeRed.SetTextColor(kRed);
	colorCodeRed.SetTextSize(TXTSIZE*0.8);
	colorCodeRed.SetNDC();
	colorCodeRed.DrawText(0.35, 0.22,"Low hit rate");
	colorCodeGray.SetTextColor(kGray+2);
	colorCodeGray.SetTextSize(TXTSIZE*0.8);
	colorCodeGray.SetNDC();
	colorCodeGray.DrawText(0.65, 0.22,"Disabled");



	if (arm==0) TC[SUMMARYS]->Update();
	else if (arm==1) TC[SUMMARYN]->Update();


	return 0;
}

int InttMonDraw::DrawYieldsByPacket(const std::string &/*what*/, int arm)
{
	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");

	if (arm==0 && ! gROOT->FindObject("fvtxYieldsByPacketSouth"))
	{
		MakeCanvas("fvtxYieldsByPacketSouth");
	}
	else if (arm==1 && ! gROOT->FindObject("fvtxYieldsByPacketNorth"))
	{
		MakeCanvas("fvtxYieldsByPacketNorth");
	}

	if (arm==0) TC[YIELDSPACKETS]->Clear("D");
	else if (arm==1) TC[YIELDSPACKETN]->Clear("D");

	if (fvtxH1NumEvent) 
  {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) 
    {
			cout << "FVTX : no events have yet been processed" << endl;
			if (arm==0) 
      {
				TC[YIELDSPACKETS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[YIELDSPACKETS]->Update();
			}
			else if (arm==1) 
      {
				TC[YIELDSPACKETN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[YIELDSPACKETN]->Update();
			}
			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) 
    {
			cout << "FVTX : not enough events yet" << endl;
			if (arm==0) 
      {
				TC[YIELDSPACKETS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[YIELDSPACKETS]->Update();
			}
			else if (arm==1) 
      {
				TC[YIELDSPACKETN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[YIELDSPACKETN]->Update();
			}
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0) 
    {
			DrawDeadServer(transparent[YIELDSPACKETS]);
			TC[YIELDSPACKETS]->Update();
		}
		else if (arm==1) 
    {
			DrawDeadServer(transparent[YIELDSPACKETN]);
			TC[YIELDSPACKETN]->Update();
		}

		return -1;
	}

	if (arm==0) 
  {
		TC[YIELDSPACKETS]->cd();
		TP[YIELDSPACKETS][0]->cd();
	}
	else if (arm==1) 
  {
		TC[YIELDSPACKETN]->cd();
		TP[YIELDSPACKETN][0]->cd();
	}

	TH1F* hFvtxYieldsByPacket = (TH1F*)cl->getHisto(Form("hFvtxYieldsByPacket%d",arm));
	TH1F* hFvtxYieldsByPacketScaled = (TH1F*)hFvtxYieldsByPacket->Clone("hFvtxYieldsByPacketClone");
	hFvtxYieldsByPacketScaled->Scale(1.0/eventNum);
	hFvtxYieldsByPacketScaled->SetFillColor(kGray);
	hFvtxYieldsByPacketScaled->SetLineColor(kBlack);
	hFvtxYieldsByPacketScaled->SetFillStyle(1001);
	hFvtxYieldsByPacketScaled->SetMinimum(0);
	hFvtxYieldsByPacketScaled->SetMaximum(MAXHITSPERPACKET[arm]);
	hFvtxYieldsByPacketScaled->DrawCopy("");

	MakeMark(arm);

	//count hot/cold packets
	int nbin = hFvtxYieldsByPacketScaled->GetNbinsX();
	int nhot = 0;
	int ncold = 0;
	TArrow new_hot_indicator(0,0,0,0,0.03,"|>");
	new_hot_indicator.SetLineColor(kRed);
	new_hot_indicator.SetFillColor(kRed);
	TArrow new_cold_indicator(0,0,0,0,0.03,"|>");
	new_cold_indicator.SetLineColor(kBlue);
	new_cold_indicator.SetFillColor(kBlue);

	TArrow known_hot_indicator(0,0,0,0,0.03,"|>");
	known_hot_indicator.SetLineColor(kGreen);
	known_hot_indicator.SetFillColor(kGreen);
	TArrow known_cold_indicator(0,0,0,0,0.03,"|>");
	known_cold_indicator.SetLineColor(kGreen);
	known_cold_indicator.SetFillColor(kGreen);

	for(int i=1;i<=nbin;i++)
	{
		float yield = hFvtxYieldsByPacketScaled->GetBinContent(i);

		if(yield>CUT_HOT_PACKET[arm])
		{
			//known hot
			//Run-15
			//if((arm==0&&(i==11||i==15)) //South
			//		||(arm==1&&(i==11||i==17)) ) //North
			//Run-16
			if(false)
			{
				if(yield>CUT_HOT_PACKET[arm])
					known_hot_indicator.DrawArrow(i,CUT_HOT_PACKET[arm]-(MAXHITSPERPACKET[arm])/6.,i,CUT_HOT_PACKET[arm]);
				continue;
			}
			nhot++;
			new_hot_indicator.DrawArrow(i,CUT_HOT_PACKET[arm]-(MAXHITSPERPACKET[arm])/6.,i,CUT_HOT_PACKET[arm]);
		}
		if(yield<CUT_COLD_PACKET[arm])
		{
			//known cold
			//Run-15
			//if((arm==0&&(i==12||i==18)) //South
			//		||(arm==1&&(i==12||i==14)) ) //North
			//Run-16
			if((arm==0&&(false)) //South
					||(arm==1&&(i==14)) ) //North
			{
				if(yield<CUT_COLD_PACKET[arm])
					known_cold_indicator.DrawArrow(i,CUT_COLD_PACKET[arm]+(MAXHITSPERPACKET[arm])/6.,i,CUT_COLD_PACKET[arm]);
				continue;
			}
			ncold++;
			new_cold_indicator.DrawArrow(i,CUT_COLD_PACKET[arm]+(MAXHITSPERPACKET[arm])/6.,i,CUT_COLD_PACKET[arm]);
		}

	}

	TLine *line_hot = new TLine(0.5,CUT_HOT_PACKET[arm],24.5,CUT_HOT_PACKET[arm]);
	line_hot->SetLineStyle(2);
	line_hot->Draw();
	TLine *line_cold = new TLine(0.5,CUT_COLD_PACKET[arm],24.5,CUT_COLD_PACKET[arm]);
	line_cold->SetLineStyle(2);
	line_cold->Draw();

	if(nhot>0||ncold>0)
	{
		float txt_y = 0.85;
		bool is_expert_mode = false;
		if(is_expert_mode)
		{
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "FVTX is under expert debugging mode.");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "Please ignore the warning messages.");
		}
		//else if(m_run_type != "PHYSICS")
		//else if(m_run_type == "JUNK")
		//{
		//	txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, Form("Run type is %s",m_run_type.data()));
		//	txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "Please ignore the warning messages.");
		//}
		else if(nhot<=2&&ncold<=2)
		{
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "New hot/cold packets:");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "No need to stop this run.");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "If this is PHYSICS run:");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "1) check HV, if HV is OK:");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "2) continue this run, and");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "check if it continues in the next run");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "(NO FEED needed!)");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "3) if it continues, contact the FVTX expert!");
		}
		else
		{
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "Too many hot/cold packets");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "If this is PHYSICS run:");
			fvtx_message.SetTextColor(kRed);
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "1) Stop this run, check the HV:");
			fvtx_message.SetTextColor(kBlack);
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "2) check if it continues in the next run");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "(NO FEED needed!)");
			txt_y-=0.05;fvtx_message.DrawTextNDC(0.2,txt_y, "3) if it continues, contact the FVTX expert!");
		}
	}

	// run and time info
	time_t t = cl->EventTime();
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);

	if (arm==0) transparent[YIELDSPACKETS]->cd();
	else if (arm==1) transparent[YIELDSPACKETN]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	printRun.SetTextColor(kBlue);
	printRun.DrawText(0.11, 0.25, "Blue arrows indicate new cold packets");
	printRun.SetTextColor(kRed);
	printRun.DrawText(0.11, 0.20, "Red arrows indicate new hot packets");
	printRun.SetTextColor(kGreen);
	printRun.DrawText(0.11, 0.15, "Green arrows indicate known hot/cold packets");

	if (arm==0) TC[YIELDSPACKETS]->Update();
	else if (arm==1) TC[YIELDSPACKETN]->Update();

	delete hFvtxYieldsByPacketScaled;

	return 0;
}

int InttMonDraw::DrawAdc(const std::string &/*what*/, int arm)
{
	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");

	if (arm==0 && ! gROOT->FindObject("fvtxAdcSouth"))
	{
		MakeCanvas("fvtxAdcSouth");
	}
	else if (arm==1 && ! gROOT->FindObject("fvtxAdcNorth"))
	{
		MakeCanvas("fvtxAdcNorth");
	}

	if (arm==0) TC[ADCS]->Clear("D");
	else if (arm==1) TC[ADCN]->Clear("D");

	if (fvtxH1NumEvent) {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) {
			cout << "FVTX : no events have yet been processed" << endl;
			if (arm==0) {
				TC[ADCS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[ADCS]->Update();
			}
			else if (arm==1) {
				TC[ADCN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[ADCN]->Update();
			}
			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) {
			cout << "FVTX : not enough events yet" << endl;
			if (arm==0) {
				TC[ADCS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[ADCS]->Update();
			}
			else if (arm==1) {
				TC[ADCN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[ADCN]->Update();
			}
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0) {
			DrawDeadServer(transparent[ADCS]);
			TC[ADCS]->Update();
		}
		else if (arm==1) {
			DrawDeadServer(transparent[ADCN]);
			TC[ADCN]->Update();
		}

		return -1;
	}

	for (int istation=0; istation<NumberOfStations_temp; istation++) {
		if (arm==0) {
			TC[ADCS]->cd();
			TP[ADCS][istation]->cd();
		}
		else if (arm==1) {
			TC[ADCN]->cd();
			TP[ADCN][istation]->cd();
		}

		TH1F* hFvtxAdc = (TH1F*)cl->getHisto(Form("hFvtxAdc%d_%d",arm,istation));  
		hFvtxAdc->Draw();

	}
	// run and time info
	time_t t = cl->EventTime();
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);

	if (arm==0) transparent[ADCS]->cd();
	else if (arm==1) transparent[ADCN]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	if (arm==0) TC[ADCS]->Update();
	else if (arm==1) TC[ADCN]->Update();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
int InttMonDraw::DrawControlWord(const std::string &/*what*/, int arm)
{

	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");


	if (arm==0 && ! gROOT->FindObject("fvtxControlWordS"))
	{
		MakeCanvas("fvtxControlWordS");
	}
	else if (arm==1 && ! gROOT->FindObject("fvtxControlWordN"))
	{
		MakeCanvas("fvtxControlWordN");
	}


	if (arm==0) TC[CONTROLWORDS]->Clear("D");
	else if (arm==1) TC[CONTROLWORDN]->Clear("D");

	if (fvtxH1NumEvent) {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) {
			cout << "FVTX : no events have yet been processed" << endl;
			if (arm==0) {
				TC[CONTROLWORDS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[CONTROLWORDS]->Update();
			}
			else if (arm==1) {
				TC[CONTROLWORDN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[CONTROLWORDN]->Update();
			}
			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) {
			cout << "FVTX : not enough events yet" << endl;
			if (arm==0) {
				TC[CONTROLWORDS]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[CONTROLWORDS]->Update();
			}
			else if (arm==1) {
				TC[CONTROLWORDN]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[CONTROLWORDN]->Update();
			}
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0) {
			DrawDeadServer(transparent[CONTROLWORDS]);
			TC[CONTROLWORDS]->Update();
		}
		else if (arm==1) {
			DrawDeadServer(transparent[CONTROLWORDN]);
			TC[CONTROLWORDN]->Update();
		}

		return -1;
	}

	if (arm==0) {
		TC[CONTROLWORDS]->cd();
		TP[CONTROLWORDS][0]->cd();
	}
	else if (arm==1) {
		TC[CONTROLWORDN]->cd();
		TP[CONTROLWORDN][0]->cd();
	}

	TH2F* hFvtxControlWord = (TH2F*)cl->getHisto(Form("hFvtxControlWord%d",arm));
	TH2F* hFvtxControlWordScaled = (TH2F*)hFvtxControlWord->Clone("hFvtxControlWordClone");
	hFvtxControlWordScaled->Scale(1.0/eventNum);
	hFvtxControlWordScaled->SetFillStyle(1001);
	hFvtxControlWordScaled->SetMinimum(1.0E-5);
	hFvtxControlWordScaled->SetMaximum(1);
	gPad->SetLogz();
	hFvtxControlWordScaled->DrawCopy("col4z");

	MakeMark_cw(arm);

	// run and time info
	time_t t = cl->EventTime();
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);

	if (arm==0) transparent[CONTROLWORDS]->cd();
	else if (arm==1) transparent[CONTROLWORDN]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	if (arm==0) TC[CONTROLWORDS]->Update();
	else if (arm==1) TC[CONTROLWORDN]->Update();

	delete hFvtxControlWordScaled;

	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////

int InttMonDraw::DrawChipVsChannel(const std::string &/*what*/, int arm, int side)
{
	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");

	if (arm==0 && side==0 && ! gROOT->FindObject("fvtxChipVsChannelSouthW"))
	{
		MakeCanvas("fvtxChipVsChannelSouthW");
	}
	else if (arm==0 && side==1 && ! gROOT->FindObject("fvtxChipVsChannelSouthE"))
	{
		MakeCanvas("fvtxChipVsChannelSouthE");
	}
	else if (arm==1 && side==0 && ! gROOT->FindObject("fvtxChipVsChannelNorthW"))
	{
		MakeCanvas("fvtxChipVsChannelNorthW");
	}
	else if (arm==1 && side==1 && ! gROOT->FindObject("fvtxChipVsChannelNorthE"))
	{
		MakeCanvas("fvtxChipVsChannelNorthE");
	}

	if (arm==0 && side==0) TC[CHIPCHANNELSW]->Clear("D");
	else if (arm==0 && side==1) TC[CHIPCHANNELSE]->Clear("D");
	else if (arm==1 && side==0) TC[CHIPCHANNELNW]->Clear("D");
	else if (arm==1 && side==1) TC[CHIPCHANNELNE]->Clear("D");

	if (fvtxH1NumEvent) {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) {
			cout << "FVTX : no events have yet been processed" << endl;
			if (arm==0 && side==0) {
				TC[CHIPCHANNELSW]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[CHIPCHANNELSW]->Update();
			}
			else if (arm==0 && side==1) {
				TC[CHIPCHANNELSE]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[CHIPCHANNELSE]->Update();
			}
			else if (arm==1 && side==0) {
				TC[CHIPCHANNELNW]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[CHIPCHANNELNW]->Update();
			}
			else if (arm==1 && side==1) {
				TC[CHIPCHANNELNE]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[CHIPCHANNELNE]->Update();
			}

			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) {
			cout << "FVTX : not enough events yet" << endl;
			if (arm==0 && side==0) {
				TC[CHIPCHANNELSW]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[CHIPCHANNELSW]->Update();
			}
			else if (arm==0 && side==1) {
				TC[CHIPCHANNELSE]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[CHIPCHANNELSE]->Update();
			}
			else if (arm==1 && side==0) {
				TC[CHIPCHANNELNW]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[CHIPCHANNELNW]->Update();
			}
			else if (arm==1 && side==1) {
				TC[CHIPCHANNELNE]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[CHIPCHANNELNE]->Update();
			}
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0 && side==0) {
			DrawDeadServer(transparent[CHIPCHANNELSW]);
			TC[CHIPCHANNELSW]->Update();
		}
		else if (arm==0 && side==1) {
			DrawDeadServer(transparent[CHIPCHANNELSE]);
			TC[CHIPCHANNELSE]->Update();
		}
		else if (arm==1 && side==0) {
			DrawDeadServer(transparent[CHIPCHANNELNW]);
			TC[CHIPCHANNELNW]->Update();
		}
		else if (arm==1 && side==1) {
			DrawDeadServer(transparent[CHIPCHANNELNE]);
			TC[CHIPCHANNELNE]->Update();
		}

		return -1;
	}

	if (arm==0 && side==0) { // Southwest cage
		TC[CHIPCHANNELSW]->cd();
		int ipad = 0;
		for (int iroc=0; iroc<6; iroc++) {
			TP[CHIPCHANNELSW][ipad]->cd();
			TH2F* hFvtxChipChannel = (TH2F*)cl->getHisto(Form("hFvtxChipChannel_ROC%d",iroc));
			TH2F* hFvtxChipChannelScaled = (TH2F*)hFvtxChipChannel->Clone("hFvtxChipChannelClone");
			hFvtxChipChannelScaled->Scale(1.0/eventNum);
			string tmpTitle = hFvtxChipChannelScaled->GetTitle();
			stringstream tmpTitle_new;
			tmpTitle_new << tmpTitle << "  (SW " << iroc  <<")";
			hFvtxChipChannelScaled->SetTitle(tmpTitle_new.str().c_str() );

			hFvtxChipChannelScaled->SetMaximum(MAX_CHIP_CHANNEL);
			hFvtxChipChannelScaled->GetYaxis()->SetTitle("St. 0     St. 1     St. 2     St. 3");
			hFvtxChipChannelScaled->GetYaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleOffset(0.4);

			hFvtxChipChannelScaled->GetXaxis()->SetLabelSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetNdivisions(4,4,0,kFALSE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitle("Sector 0               Sector 1               Sector 2               Sector 3");
			hFvtxChipChannelScaled->GetXaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleOffset(0.6);
			hFvtxChipChannelScaled->DrawCopy("COLZ");
			line_ccs1->Draw();
			line_ccs2->Draw();
			line_ccs3->Draw();
			line_ccs4->Draw();
			line_ccs5->Draw();
			line_ccs6->Draw();
			delete hFvtxChipChannelScaled;
			ipad++;
		}
	}
	else if (arm==0 && side==1) { // Southeast cage
		TC[CHIPCHANNELSE]->cd();
		int ipad = 0;
		for (int iroc=6; iroc<12; iroc++) {
			TP[CHIPCHANNELSE][ipad]->cd();
			TH2F* hFvtxChipChannel = (TH2F*)cl->getHisto(Form("hFvtxChipChannel_ROC%d",iroc));
			TH2F* hFvtxChipChannelScaled = (TH2F*)hFvtxChipChannel->Clone("hFvtxChipChannelClone");
			hFvtxChipChannelScaled->Scale(1.0/eventNum);
			hFvtxChipChannelScaled->SetMaximum(0.01);
			string tmpTitle = hFvtxChipChannelScaled->GetTitle();
			stringstream tmpTitle_new;
			tmpTitle_new << tmpTitle << "  (SE " << iroc-6  <<")";
			hFvtxChipChannelScaled->SetTitle(tmpTitle_new.str().c_str() );
			hFvtxChipChannelScaled->SetMaximum(MAX_CHIP_CHANNEL);
			hFvtxChipChannelScaled->GetYaxis()->SetTitle("St. 0     St. 1     St. 2     St. 3");
			hFvtxChipChannelScaled->GetYaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleOffset(0.4);
			hFvtxChipChannelScaled->GetXaxis()->SetLabelSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetNdivisions(4,4,0,kFALSE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitle("Sector 0               Sector 1               Sector 2               Sector 3");
			hFvtxChipChannelScaled->GetXaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleOffset(0.6);
			hFvtxChipChannelScaled->DrawCopy("COLZ");
			line_ccs1->Draw();
			line_ccs2->Draw();
			line_ccs3->Draw();
			line_ccs4->Draw();
			line_ccs5->Draw();
			line_ccs6->Draw();
			delete hFvtxChipChannelScaled;
			ipad++;
		}
	}
	else if (arm==1 && side==0) { // Northwest cage
		TC[CHIPCHANNELNW]->cd();
		int ipad = 0;
		for (int iroc=12; iroc<18; iroc++) {
			TP[CHIPCHANNELNW][ipad]->cd();
			TH2F* hFvtxChipChannel = (TH2F*)cl->getHisto(Form("hFvtxChipChannel_ROC%d",iroc));
			TH2F* hFvtxChipChannelScaled = (TH2F*)hFvtxChipChannel->Clone("hFvtxChipChannelClone");
			hFvtxChipChannelScaled->Scale(1.0/eventNum);
			hFvtxChipChannelScaled->SetMaximum(0.01);
			string tmpTitle = hFvtxChipChannelScaled->GetTitle();
			stringstream tmpTitle_new;
			tmpTitle_new << tmpTitle << "  (NW " << iroc-12  <<")";
			hFvtxChipChannelScaled->SetTitle(tmpTitle_new.str().c_str() );
			hFvtxChipChannelScaled->SetMaximum(MAX_CHIP_CHANNEL);
			hFvtxChipChannelScaled->GetYaxis()->SetTitle("St. 0     St. 1     St. 2     St. 3");
			hFvtxChipChannelScaled->GetYaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleOffset(0.4);
			hFvtxChipChannelScaled->GetXaxis()->SetLabelSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetNdivisions(4,4,0,kFALSE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitle("Sector 0               Sector 1               Sector 2               Sector 3");
			hFvtxChipChannelScaled->GetXaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleOffset(0.6);
			hFvtxChipChannelScaled->DrawCopy("COLZ");
			line_ccs1->Draw();
			line_ccs2->Draw();
			line_ccs3->Draw();
			line_ccs4->Draw();
			line_ccs5->Draw();
			line_ccs6->Draw();
			delete hFvtxChipChannelScaled;
			ipad++;
		}
	}
	else if (arm==1 && side==1) { // Northeast cage
		TC[CHIPCHANNELNE]->cd();
		int ipad = 0;
		for (int iroc=18; iroc<NROCS; iroc++) {
			TP[CHIPCHANNELNE][ipad]->cd();
			TH2F* hFvtxChipChannel = (TH2F*)cl->getHisto(Form("hFvtxChipChannel_ROC%d",iroc));
			TH2F* hFvtxChipChannelScaled = (TH2F*)hFvtxChipChannel->Clone("hFvtxChipChannelClone");
			hFvtxChipChannelScaled->Scale(1.0/eventNum);
			hFvtxChipChannelScaled->SetMaximum(0.01);
			string tmpTitle = hFvtxChipChannelScaled->GetTitle();
			stringstream tmpTitle_new;
			tmpTitle_new << tmpTitle << "  (NE " << iroc-18  <<")";
			hFvtxChipChannelScaled->SetTitle(tmpTitle_new.str().c_str() );
			hFvtxChipChannelScaled->SetMaximum(MAX_CHIP_CHANNEL);
			hFvtxChipChannelScaled->GetYaxis()->SetTitle("St. 0     St. 1     St. 2     St. 3");
			hFvtxChipChannelScaled->GetYaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetYaxis()->SetTitleOffset(0.4);
			hFvtxChipChannelScaled->GetXaxis()->SetLabelSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetNdivisions(4,4,0,kFALSE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitle("Sector 0               Sector 1               Sector 2               Sector 3");
			hFvtxChipChannelScaled->GetXaxis()->CenterTitle(kTRUE);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleSize(0.06);
			hFvtxChipChannelScaled->GetXaxis()->SetTitleOffset(0.6);
			hFvtxChipChannelScaled->DrawCopy("COLZ");
			line_ccs1->Draw();
			line_ccs2->Draw();
			line_ccs3->Draw();
			line_ccs4->Draw();
			line_ccs5->Draw();
			line_ccs6->Draw();
			delete hFvtxChipChannelScaled;
			ipad++;
		}
	}

	// run and time info
	time_t t = cl->EventTime();
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);

	if (arm==0 && side==0) transparent[CHIPCHANNELSW]->cd();
	else if (arm==0 && side==1) transparent[CHIPCHANNELSE]->cd();
	else if (arm==1 && side==0) transparent[CHIPCHANNELNW]->cd();
	else if (arm==1 && side==1) transparent[CHIPCHANNELNE]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	if (arm==0 && side==0) TC[CHIPCHANNELSW]->Update();
	else if (arm==0 && side==1) TC[CHIPCHANNELSE]->Update();
	else if (arm==1 && side==0) TC[CHIPCHANNELNW]->Update();
	else if (arm==1 && side==1) TC[CHIPCHANNELNE]->Update();


	return 0;
}

int InttMonDraw::DrawYieldsByPacketVsTime(const std::string &/*what*/, int arm)
{
	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");
	TH1F* hFvtxEventNumberVsTime = (TH1F*)cl->getHisto("hFvtxEventNumberVsTime");

	if (arm==0 && ! gROOT->FindObject("fvtxYieldsByPacketSouthVsTime"))
	{
		MakeCanvas("fvtxYieldsByPacketSouthVsTime");
	}
	else if (arm==1 && ! gROOT->FindObject("fvtxYieldsByPacketNorthVsTime"))
	{
		MakeCanvas("fvtxYieldsByPacketNorthVsTime");
	}

	if (arm==0) TC[YIELDSPACKETSVSTIME]->Clear("D");
	else if (arm==1) TC[YIELDSPACKETNVSTIME]->Clear("D");

	if (fvtxH1NumEvent) {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) {
			cout << "FVTX : no events have yet been processed" << endl;
			if (arm==0) {
				TC[YIELDSPACKETSVSTIME]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[YIELDSPACKETSVSTIME]->Update();
			}
			else if (arm==1) {
				TC[YIELDSPACKETNVSTIME]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[YIELDSPACKETNVSTIME]->Update();
			}
			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) {
			cout << "FVTX : not enough events yet" << endl;
			if (arm==0) {
				TC[YIELDSPACKETSVSTIME]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[YIELDSPACKETSVSTIME]->Update();
			}
			else if (arm==1) {
				TC[YIELDSPACKETNVSTIME]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[YIELDSPACKETNVSTIME]->Update();
			}
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0) {
			DrawDeadServer(transparent[YIELDSPACKETSVSTIME]);
			TC[YIELDSPACKETSVSTIME]->Update();
		}
		else if (arm==1) {
			DrawDeadServer(transparent[YIELDSPACKETNVSTIME]);
			TC[YIELDSPACKETNVSTIME]->Update();
		}

		return -1;
	}

	if (arm==0) {
		TC[YIELDSPACKETSVSTIME]->cd();
		TP[YIELDSPACKETSVSTIME][0]->cd();
		//TP[YIELDSPACKETSVSTIME][0]->SetLogz();
	}
	else if (arm==1) {
		TC[YIELDSPACKETNVSTIME]->cd();
		TP[YIELDSPACKETNVSTIME][0]->cd();
		//TP[YIELDSPACKETNVSTIME][0]->SetLogz();
	}

	TH2F* hFvtxYieldsByPacket = (TH2F*)cl->getHisto(Form("hFvtxYieldsByPacketVsTime%d",arm));
	TH2F* hFvtxYieldsByPacketScaled = (TH2F*)hFvtxYieldsByPacket->Clone("hFvtxYieldsByPacketVsTimeClone");

	int nbinx = hFvtxYieldsByPacketScaled->GetXaxis()->GetNbins();
	int nbiny = hFvtxYieldsByPacketScaled->GetYaxis()->GetNbins();
	if(!hFvtxEventNumberVsTime) return 0;
	else if(hFvtxEventNumberVsTime->GetXaxis()->GetNbins()!=nbinx) return 0;

	float max_time = 0;
	for(int ibinx=1;ibinx<=nbinx;ibinx++){
		eventNum = hFvtxEventNumberVsTime->GetBinContent(ibinx);
		if(eventNum>0){
			for(int ibiny=1;ibiny<=nbiny;ibiny++)
				hFvtxYieldsByPacketScaled->SetBinContent(ibinx,ibiny,
						hFvtxYieldsByPacketScaled->GetBinContent(ibinx,ibiny) / eventNum
						);
			max_time = hFvtxYieldsByPacketScaled->GetXaxis()->GetBinUpEdge(ibinx);

		}else{
			for(int ibiny=1;ibiny<=nbiny;ibiny++)
				hFvtxYieldsByPacketScaled->SetBinContent(ibinx,ibiny,
						0.
						);
		}
	}
	//hFvtxYieldsByPacketScaled->Scale(1.0/eventNum);
	hFvtxYieldsByPacketScaled->GetXaxis()->SetRangeUser(0,max_time);

	hFvtxYieldsByPacketScaled->SetMaximum(MAX_YIELD_PER_PACKET_VS_TIME);
	hFvtxYieldsByPacketScaled->SetMinimum(0.1);
	hFvtxYieldsByPacketScaled->DrawCopy("colz");

	MakeMarkVsTime(arm);

	// run and time info
	time_t t = cl->EventTime();
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);

	if (arm==0) transparent[YIELDSPACKETSVSTIME]->cd();
	else if (arm==1) transparent[YIELDSPACKETNVSTIME]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	if (arm==0) TC[YIELDSPACKETSVSTIME]->Update();
	else if (arm==1) TC[YIELDSPACKETNVSTIME]->Update();

	delete hFvtxYieldsByPacketScaled;

	return 0;
}

int InttMonDraw::DrawYieldsByPacketVsTimeShort(const std::string &/*what*/, int arm)
{
	OnlMonClient *cl = OnlMonClient::instance();
	TText fvtx_message;
	int eventNum = 0;
	TH1F* fvtxH1NumEvent = (TH1F*)cl->getHisto("fvtxH1NumEvent");
	TH1F* hFvtxEventNumberVsTimeShort = (TH1F*)cl->getHisto("hFvtxEventNumberVsTimeShort");

	if (arm==0 && ! gROOT->FindObject("fvtxYieldsByPacketSouthVsTimeShort"))
	{
		MakeCanvas("fvtxYieldsByPacketSouthVsTimeShort");
	}
	else if (arm==1 && ! gROOT->FindObject("fvtxYieldsByPacketNorthVsTimeShort"))
	{
		MakeCanvas("fvtxYieldsByPacketNorthVsTimeShort");
	}

	if (arm==0) TC[YIELDSPACKETSVSTIMESHORT]->Clear("D");
	else if (arm==1) TC[YIELDSPACKETNVSTIMESHORT]->Clear("D");

	if (fvtxH1NumEvent) {
		eventNum = (int)fvtxH1NumEvent->GetEntries();
		cout << "event Num " << eventNum << endl;
		if (eventNum == 0) {
			cout << "FVTX : no events have yet been processed" << endl;
			if (arm==0) {
				TC[YIELDSPACKETSVSTIMESHORT]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : No events processed!");
				TC[YIELDSPACKETSVSTIMESHORT]->Update();
			}
			else if (arm==1) {
				TC[YIELDSPACKETNVSTIMESHORT]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : No events processed!");
				TC[YIELDSPACKETNVSTIMESHORT]->Update();
			}
			return -1;
		}
		else if (eventNum < FVTXMINEVENTS) {
			cout << "FVTX : not enough events yet" << endl;
			if (arm==0) {
				TC[YIELDSPACKETSVSTIMESHORT]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.S : Not enough events!");
				TC[YIELDSPACKETSVSTIMESHORT]->Update();
			}
			else if (arm==1) {
				TC[YIELDSPACKETNVSTIMESHORT]->Clear();
				fvtx_message.DrawTextNDC(0.3, 0.5, "FVTX.N : Not enough events!");
				TC[YIELDSPACKETNVSTIMESHORT]->Update();
			}
			return -1;
		}
	}
	else // Dead server
	{
		if (arm==0) {
			DrawDeadServer(transparent[YIELDSPACKETSVSTIMESHORT]);
			TC[YIELDSPACKETSVSTIMESHORT]->Update();
		}
		else if (arm==1) {
			DrawDeadServer(transparent[YIELDSPACKETNVSTIMESHORT]);
			TC[YIELDSPACKETNVSTIMESHORT]->Update();
		}

		return -1;
	}

	if (arm==0) {
		TC[YIELDSPACKETSVSTIMESHORT]->cd();
		TP[YIELDSPACKETSVSTIMESHORT][0]->cd();
	}
	else if (arm==1) {
		TC[YIELDSPACKETNVSTIMESHORT]->cd();
		TP[YIELDSPACKETNVSTIMESHORT][0]->cd();
	}

	TH2F* hFvtxYieldsByPacket = (TH2F*)cl->getHisto(Form("hFvtxYieldsByPacketVsTimeShort%d",arm));
	TH2F* hFvtxYieldsByPacketScaled = (TH2F*)hFvtxYieldsByPacket->Clone("hFvtxYieldsByPacketVsTimeShortClone");

	int nbinx = hFvtxYieldsByPacketScaled->GetXaxis()->GetNbins();
	int nbiny = hFvtxYieldsByPacketScaled->GetYaxis()->GetNbins();
	if(!hFvtxEventNumberVsTimeShort) return 0;
	else if(hFvtxEventNumberVsTimeShort->GetXaxis()->GetNbins()!=nbinx) return 0;

	float max_time = 0;
	for(int ibinx=1;ibinx<=nbinx;ibinx++){
		eventNum = hFvtxEventNumberVsTimeShort->GetBinContent(ibinx);
		if(eventNum>0){
			for(int ibiny=1;ibiny<=nbiny;ibiny++)
				hFvtxYieldsByPacketScaled->SetBinContent(ibinx,ibiny,
						hFvtxYieldsByPacketScaled->GetBinContent(ibinx,ibiny) / eventNum
						);
			max_time = hFvtxYieldsByPacketScaled->GetXaxis()->GetBinUpEdge(ibinx);

		}else{
			for(int ibiny=1;ibiny<=nbiny;ibiny++)
				hFvtxYieldsByPacketScaled->SetBinContent(ibinx,ibiny,
						0.
						);
		}
	}
	//hFvtxYieldsByPacketScaled->Scale(1.0/eventNum);
	hFvtxYieldsByPacketScaled->GetXaxis()->SetRangeUser(0,max_time);

	hFvtxYieldsByPacketScaled->SetMaximum(MAX_YIELD_PER_PACKET_VS_TIME_SHORT);
	hFvtxYieldsByPacketScaled->SetMinimum(0);
	hFvtxYieldsByPacketScaled->DrawCopy("colz");

	MakeMarkVsTime(arm);

	// run and time info
	time_t t = cl->EventTime();
	ostringstream posting;
	posting.str("");
	posting << "Run# " << cl->RunNumber() << "   Nevt:" << eventNum
		<< "   Date:" << ctime(&t);

	if (arm==0) transparent[YIELDSPACKETSVSTIMESHORT]->cd();
	else if (arm==1) transparent[YIELDSPACKETNVSTIMESHORT]->cd();

	TText printRun;
	printRun.SetTextFont(62);
	printRun.SetTextSize(TXTSIZE*0.7);
	printRun.SetNDC();
	printRun.DrawText(0.11, 0.30, posting.str().c_str());

	if (arm==0) TC[YIELDSPACKETSVSTIMESHORT]->Update();
	else if (arm==1) TC[YIELDSPACKETNVSTIMESHORT]->Update();

	delete hFvtxYieldsByPacketScaled;

	return 0;
}

int InttMonDraw::DrawDeadServer(TPad *transparentpad)
{
  transparentpad->cd();
  TText FatalMsg;
  FatalMsg.SetTextFont(62);
  FatalMsg.SetTextSize(0.1);
  FatalMsg.SetTextColor(4);
  FatalMsg.SetNDC();          // set to normalized coordinates
  FatalMsg.SetTextAlign(23);  // center/top alignment
  FatalMsg.DrawText(0.5, 0.9, "INTT MONITOR");
  FatalMsg.SetTextAlign(22);  // center/center alignment
  FatalMsg.DrawText(0.5, 0.5, "SERVER");
  FatalMsg.SetTextAlign(21);  // center/bottom alignment
  FatalMsg.DrawText(0.5, 0.1, "DEAD");
  transparentpad->Update();
  return 0;
}

void InttMonDraw::MakeMark(const int arm)
{
	for (int i=0; i<12; i++)
	{
		lineROC[i]->Draw();
		textROC[arm][i]->Draw();
	}
}

void InttMonDraw::MakeMarkVsTime(const int arm)
{
	for (int i=0; i<12; i++)
	{
		lineROC_time[i]->Draw();
		textROC_time[arm][i]->Draw();
	}
}

void InttMonDraw::MakeMark_cw(const int arm)
{
	for (int i=0; i<12; i++)
	{
		lineROC_cw[i]->Draw();
		textROC_cw[arm][i]->Draw();
	}
	for (int i=0; i<13; i++)text_cw_bit[i]->Draw();
}


int InttMonDraw::MakePS(const std::string &what)
{
  OnlMonClient *cl = OnlMonClient::instance();
	int iret = Draw(what);
	if (iret)
	{
		return iret;
	}
	for (int icanvas = 0; icanvas < ncanvas; icanvas++)
	{
		ostringstream filename;
		filename << ThisName << "_" << icanvas
			<< "_" << cl->RunNumber() << ".ps";
		string savefile = filename.str();
		TC[icanvas]->Print(savefile.c_str());
	}
	return 0;
}

int InttMonDraw::MakeHtml(const std::string &what)
{
  int iret = Draw(what);
	if (iret)
	{
		return iret;
	}

	OnlMonClient *cl = OnlMonClient::instance();

	const char* names[] ={ "South", "North" };

	// "Shift crew" plots.
	for ( int iarm = 0; iarm < NARMS; iarm++ )
	{
		ostringstream filestring, indexstring;
		// Status summary canvas
		filestring.str("");
		filestring << "fvtxSummary" << names[iarm];
		indexstring.str("");
		indexstring << "0" << names[iarm];
		string ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[SUMMARYS], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[SUMMARYN], ofile);

		// Normalized yield by packet canvas
		filestring.str("");
		filestring << "fvtxYieldsByPacket" << names[iarm];
		indexstring.str("");
		indexstring << "1" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[YIELDSPACKETS], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[YIELDSPACKETN], ofile);

		// Expert plots
		// Normalized yield by wedge canvas
		filestring.str("");
		filestring << "Expert/fvtxYields" << names[iarm];
		indexstring.str("");
		indexstring << "2" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[YIELDSS], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[YIELDSN], ofile);

		// ADC distribution canvas
		filestring.str("");
		filestring << "Expert/fvtxAdc" << names[iarm];
		indexstring.str("");
		indexstring << "3" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[ADCS], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[ADCN], ofile);

		// Chip vs. channel by ROC canvas, West
		filestring.str("");
		filestring << "Expert/fvtxChipVsChannel" << names[iarm] << "W";
		indexstring.str("");
		indexstring << "4" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[CHIPCHANNELSW], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[CHIPCHANNELNW], ofile);

		// Chip vs. channel by ROC canvas, East
		filestring.str("");
		filestring << "Expert/fvtxChipVsChannel" << names[iarm] << "E";
		indexstring.str("");
		indexstring << "5" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[CHIPCHANNELSE], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[CHIPCHANNELNE], ofile);

		// FEM control word canvas
		filestring.str("");
		filestring << "Expert/fvtxControlWord" << names[iarm];
		indexstring.str("");
		indexstring << "6" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[CONTROLWORDS], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[CONTROLWORDN], ofile);

		// Normalized yield by packet Vs. time canvas every 30s
		filestring.str("");
		filestring << "Expert/fvtxYieldsByPacket" << names[iarm] << "VsTime";
		indexstring.str("");
		indexstring << "7" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[YIELDSPACKETSVSTIME], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[YIELDSPACKETNVSTIME], ofile);

		// Normalized yield by packet Vs. time canvas every 1s
		filestring.str("");
		filestring << "Expert/fvtxYieldsByPacket" << names[iarm] << "VsTimeShort";
		indexstring.str("");
		indexstring << "8" << names[iarm];
		ofile = cl->htmlRegisterPage(*this, filestring.str(), indexstring.str(), "png");
		if (iarm==0) cl->CanvasToPng(TC[YIELDSPACKETSVSTIMESHORT], ofile);
		else if (iarm==1) cl->CanvasToPng(TC[YIELDSPACKETNVSTIMESHORT], ofile);
	}

	// Log file.
	string ofile = cl->htmlRegisterPage(*this, "Expert/Log file",
			"clientlog", "txt");
	string clientlogfile = "fvtxclientlog.txt";
	ostringstream cmd;
	cmd << "mv " << clientlogfile << " " << ofile;
	gSystem->Exec(cmd.str().c_str());
	cl->SaveLogFile(*this);

	return 0;
}

// int InttMonDraw::DrawHistory(const std::string & /* what */)
// {
//   int iret = 0;
//   // you need to provide the following vectors
//   // which are filled from the db
//   std::vector<float> var;
//   std::vector<float> varerr;
//   std::vector<time_t> timestamp;
//   std::vector<int> runnumber;
//   std::string varname = "inttmondummy";
//   // this sets the time range from whihc values should be returned
//   time_t begin = 0;            // begin of time (1.1.1970)
//   time_t end = time(nullptr);  // current time (right NOW)
//   iret = dbvars->GetVar(begin, end, varname, timestamp, runnumber, var, varerr);
//   if (iret)
//   {
//     std::cout << PHWHERE << " Error in db access" << std::endl;
//     return iret;
//   }
//   if (!gROOT->FindObject("InttMon3"))
//   {
//     MakeCanvas("InttMon3");
//   }
//   // timestamps come sorted in ascending order
//   float *x = new float[var.size()];
//   float *y = new float[var.size()];
//   float *ex = new float[var.size()];
//   float *ey = new float[var.size()];
//   int n = var.size();
//   for (unsigned int i = 0; i < var.size(); i++)
//   {
//     //       std::cout << "timestamp: " << ctime(&timestamp[i])
//     // 	   << ", run: " << runnumber[i]
//     // 	   << ", var: " << var[i]
//     // 	   << ", varerr: " << varerr[i]
//     // 	   << std::endl;
//     x[i] = timestamp[i] - TimeOffsetTicks;
//     y[i] = var[i];
//     ex[i] = 0;
//     ey[i] = varerr[i];
//   }
//   Pad[4]->cd();
//   if (gr[0])
//   {
//     delete gr[0];
//   }
//   gr[0] = new TGraphErrors(n, x, y, ex, ey);
//   gr[0]->SetMarkerColor(4);
//   gr[0]->SetMarkerStyle(21);
//   gr[0]->Draw("ALP");
//   gr[0]->GetXaxis()->SetTimeDisplay(1);
//   gr[0]->GetXaxis()->SetLabelSize(0.03);
//   // the x axis labeling looks like crap
//   // please help me with this, the SetNdivisions
//   // don't do the trick
//   gr[0]->GetXaxis()->SetNdivisions(-1006);
//   gr[0]->GetXaxis()->SetTimeOffset(TimeOffsetTicks);
//   gr[0]->GetXaxis()->SetTimeFormat("%Y/%m/%d %H:%M");
//   delete[] x;
//   delete[] y;
//   delete[] ex;
//   delete[] ey;

//   varname = "inttmoncount";
//   iret = dbvars->GetVar(begin, end, varname, timestamp, runnumber, var, varerr);
//   if (iret)
//   {
//     std::cout << PHWHERE << " Error in db access" << std::endl;
//     return iret;
//   }
//   x = new float[var.size()];
//   y = new float[var.size()];
//   ex = new float[var.size()];
//   ey = new float[var.size()];
//   n = var.size();
//   for (unsigned int i = 0; i < var.size(); i++)
//   {
//     //       std::cout << "timestamp: " << ctime(&timestamp[i])
//     // 	   << ", run: " << runnumber[i]
//     // 	   << ", var: " << var[i]
//     // 	   << ", varerr: " << varerr[i]
//     // 	   << std::endl;
//     x[i] = timestamp[i] - TimeOffsetTicks;
//     y[i] = var[i];
//     ex[i] = 0;
//     ey[i] = varerr[i];
//   }
//   Pad[5]->cd();
//   if (gr[1])
//   {
//     delete gr[1];
//   }
//   gr[1] = new TGraphErrors(n, x, y, ex, ey);
//   gr[1]->SetMarkerColor(4);
//   gr[1]->SetMarkerStyle(21);
//   gr[1]->Draw("ALP");
//   gr[1]->GetXaxis()->SetTimeDisplay(1);
//   // TC[2]->Update();
//   //    h1->GetXaxis()->SetTimeDisplay(1);
//   //    h1->GetXaxis()->SetLabelSize(0.03);
//   gr[1]->GetXaxis()->SetLabelSize(0.03);
//   gr[1]->GetXaxis()->SetTimeOffset(TimeOffsetTicks);
//   gr[1]->GetXaxis()->SetTimeFormat("%Y/%m/%d %H:%M");
//   //    h1->Draw();
//   delete[] x;
//   delete[] y;
//   delete[] ex;
//   delete[] ey;

//   TC[2]->Update();
//   return 0;
// }
