#ifndef INTT_INTTMONDRAW_H
#define INTT_INTTMONDRAW_H

#include <onlmon/OnlMonDraw.h>
#include <FvtxMonConsts.h>
#include <FvtxMonDrawConsts.h>

#include <string>  // for allocator, string
class TPie;
class TLatex;
class TLine;
class TText;
class TPaveText;
class TH1;
class TH2;
class OnlMonDB;
class TCanvas;
class TGraphErrors;
class TPad;

class RunDBodbc;

class InttMonDraw : public OnlMonDraw
{
  public:
    InttMonDraw(const std::string &name = "INTTMON"); // same name as server!

    virtual ~InttMonDraw() override; //override {};

    int Init() override;
    void registerHists();
    int Draw(const std::string &what = "ALL") override;
    int MakePS(const std::string &what = "ALL") override;
    int MakeHtml(const std::string &what = "ALL") override;
    static const int NROCSperSTATION=12; 
    std::string rocnames[NROCSperSTATION];
  
  // in the original FVTXMonDraw.h, it's private 
  protected:
    // int MakeCanvas(const std::string &name);
    int MakeCanvas(const std::string &name);
    int DrawSummary(const  std::string &what = "ALL", int arm = 0);
    int DrawYieldsByWedge(const std::string &what = "ALL", int arm = 0);
    int DrawYieldsByPacket(const std::string &what = "ALL", int arm = 0);
    int DrawYieldsByPacketVsTime(const std::string &what = "ALL", int arm = 0);
    int DrawYieldsByPacketVsTimeShort(const std::string &what = "ALL", int arm = 0);
    int DrawAdc(const std::string &what = "ALL", int arm = 0);
    int DrawChipVsChannel(const std::string &what = "ALL", int arm = 0, int side = 0);
    int DrawControlWord(const std::string &what = "ALL", int arm = 0);

    int DrawFirst(const std::string &what = "ALL");
    int DrawSecond(const std::string &what = "ALL");
    // int DrawHistory(const std::string &what = "ALL");
    int DrawDeadServer(TPad *transparent);
  
    void MakeMark(const int DCM = 0);
    void MakeMark_cw(const int DCM = 0);
    void MakeMarkVsTime(const int DCM = 0);

    static const int ncanvas = 18;//add 2 for FEM control word =14
    int npad[ncanvas];
    static const int nmaxpad = 6;
    TCanvas * TC[ncanvas];
    TPad *TP[ncanvas][nmaxpad];
    TPad *transparent[ncanvas];
    std::string canv_name[ncanvas]; // Global name (must be unique)
    std::string canv_desc[ncanvas]; // Canvas description
    
    TPie *pieFvtxDisk;
    TPie *pieROCname;
    TPie *pieCenter;
    TPaveText *ptArmStation[NARMS][NSTATIONS];
    TPaveText *tsta0;
    TPaveText *tsta1;
    TPaveText *tsta2;
    TPaveText *tsta3;
    TLine *line1;
    TLine *line2;
    TLine *line3; 
    TLine *line_ccs1; 
    TLine *line_ccs2; 
    TLine *line_ccs3; 
    TLine *line_ccs4; 
    TLine *line_ccs5; 
    TLine *line_ccs6; 
    TLine *lineROC[12];
    TText *textROC[NARMS][12];

    TLine *lineROC_cw[12];
    TText *textROC_cw[NARMS][12];
    TText *text_cw_bit[13];

    TLine *lineROC_time[12];
    TText *textROC_time[NARMS][12];

    int m_run_number;
    std::string m_run_type;

    RunDBodbc *rundb_odbc;




    // the lines below are from the original INTTMonDraw.h
    //title : to be checked
    int TimeOffsetTicks = -1;
    // TCanvas *TC[3] = {nullptr};
    // TPad *transparent[3] = {nullptr};
    TPad *Pad[6] = {nullptr};
    TGraphErrors *gr[2] = {nullptr};
    OnlMonDB *dbvars = nullptr;
};

#endif /* INTT_INTTMONDRAW_H */
