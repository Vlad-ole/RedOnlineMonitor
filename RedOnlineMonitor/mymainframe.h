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
#include "TMath.h"
#include "TLine.h"
#include "TDatime.h"

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
#include "TGTextView.h"
#include <Getline.h>

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

    void RunThread();
    void InitGraphs();

    //slots
    void Clicked_start_button();
    void RedrawHist();
    void ChangeNEventsForAvr();
    void SetDesirableUpdateRate();
    void SetBaselineGateFrom();
    void SetBaselineGateTo();
    void SetSignalGateFrom();
    void SetSignalGateTo();
    void SetSignalGateFastTo();


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

    Double_t time_baseline_gate_from;
    Double_t time_baseline_gate_to;
    Double_t time_signal_gate_from;
    Double_t time_signal_gate_to;
    Double_t time_signal_gate_fast_to;

    //rates
    TGLabel *fLabel_income_rate;
    TGLabel *fLabel_update_rate;
    TGGroupFrame *gframe_cp_income_rate;
    TGGroupFrame *gframe_cp_update_rate;

    TGNumberEntry *NEntr_n_events_for_avr;
    TGNumberEntry *NEntr_update_time;

    //status label
    std::string GetCurrentTime();
    TGTextView *twStatus_label;
    //TDatime tdatime;
    TGGroupFrame *gframe_status_label;
    std::ostringstream sst_status_label;

    //gates
    std::string IsGoodBaselineGateValues();
    std::string IsGoodSignalGateValues();
    TGNumberEntry *NEntr_baseline_gate_from;
    TGNumberEntry *NEntr_baseline_gate_to;
    TGNumberEntry *NEntr_signal_gate_from;
    TGNumberEntry *NEntr_signal_gate_to;
    TGNumberEntry *NEntr_signal_gate_fast_to;

    //hist limits

    Double_t *hist_l_limits;
    Double_t *hist_r_limits;

    Pixel_t pixel_t_yellow;
    Pixel_t pixel_t_red;
    Pixel_t pixel_t_blue;
    Pixel_t pixel_t_orange;
    Pixel_t pixel_t_magenta;
    Pixel_t pixel_t_green;
    Pixel_t pixel_t_white;


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

    TLine **line_baseline_gate_from;
    TLine **line_baseline_gate_to;
    TLine **line_signal_gate_from;
    TLine **line_signal_gate_to;
    TLine **line_signal_gate_fast_to;

    TGraph *gr_mean;
    std::vector<Double_t> xv_gr_mean;
    std::vector<Double_t> yv_gr_mean;

    Int_t aNrGraphs;
    Int_t n_points;
    TCanvas **aCanvas_arr;

    ULong64_t global_counter;

};

#endif // MYMAINFRAME_H
