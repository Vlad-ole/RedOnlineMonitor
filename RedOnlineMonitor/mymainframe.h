#ifndef MYMAINFRAME_H
#define MYMAINFRAME_H

//c++
#include <sstream>
#include <iomanip>

//root cern general
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TRandom.h>
#include "TAxis.h"
#include "TStopwatch.h"

//root cern Thread
#include "TThread.h"
#include <Riostream.h>

//root cern GUI
#include <TGClient.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include "TGFrame.h"
#include "TGTab.h"
#include "TGNumberEntry.h"
#include "TGLabel.h"
#include <TQObject.h>
#include <RQ_OBJECT.h>

//to show thread_id for linux systems
#include <sys/types.h>
#include <sys/syscall.h>

struct DataStr
{
    Long_t counter;
    std::vector<std::vector<Float_t> > data_vv;
};

class MyMainFrame
{
    RQ_OBJECT("MyMainFrame")
public:
    MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
    virtual ~MyMainFrame();

    void Clicked_start_button();
    void RunThread();
    void InitGraphs();

    void RedrawHist();
    void ChangeNEventsForAvr();
    void SetDesirableUpdateRate();

private:
    //GUI
    TGMainFrame *fMain;
    TRootEmbeddedCanvas **fEcanvas_arr;
    const Int_t n_canvases;

    TGTextButton *button_start;

    bool is_start_button_activated;
    bool is_can_draw_now;
    bool is_redraw_hist;
    Double_t desirable_update_rate;

    TGLabel *fLabel_income_rate;
    TGLabel *fLabel_update_rate;
    TGGroupFrame *gframe_cp_income_rate;
    TGGroupFrame *gframe_cp_update_rate;

    TGNumberEntry *NEntr_n_events_for_avr;
    TGNumberEntry *NEntr_update_time;
    TGNumberEntry *NEntr_baseline_gate_from;
    TGNumberEntry *NEntr_baseline_gate_to;

    Pixel_t pixel_t_yellow;
    Pixel_t pixel_t_red;


    //Threads
    static void *ReadoutLoop(void*);
    TThread *slave_thread;

    //
    DataStr data_str;
    TGraph **graphs;
    TH1F **hists;
    TH1F *hist;
    Double_t summ_value_hist;
    Int_t n_events_for_avr;

    TGraph *gr_mean;
    std::vector<Double_t> xv_gr_mean;
    std::vector<Double_t> yv_gr_mean;

    Int_t aNrGraphs;
    Int_t n_points;
    TCanvas **aCanvas_arr;

    ULong64_t global_counter;
    //ULong64_t income_counter;
    //Double_t income_rate;

    //
    //TGraph *gr;
};

#endif // MYMAINFRAME_H
