#ifndef MYMAINFRAME_H
#define MYMAINFRAME_H

//c++
#include <sstream>

//root cern general
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TRandom.h>
#include "TAxis.h"

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

private:
    //GUI
    TGMainFrame *fMain;
    TRootEmbeddedCanvas **fEcanvas_arr;
    const Int_t n_canvases;

    TGTextButton *button_start;
    bool is_start_button_activated;

    bool is_redraw_hist;

    Pixel_t pixel_t_yellow;
    Pixel_t pixel_t_red;

    //Threads
    static void *ReadoutLoop(void*);
    TThread *slave_thread;

    //
    DataStr data_str;
    TGraph **graphs;
    TH1F *hist;
    Int_t aNrGraphs;
    Int_t n_points;
    TCanvas **aCanvas_arr;

    //
    //TGraph *gr;
};

#endif // MYMAINFRAME_H
