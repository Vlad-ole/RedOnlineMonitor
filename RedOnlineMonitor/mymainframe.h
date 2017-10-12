#ifndef MYMAINFRAME_H
#define MYMAINFRAME_H

//c++
#include <sstream>

//root cern general
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TRandom.h>

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

private:
    //GUI
    TGMainFrame *fMain;
//    TRootEmbeddedCanvas *fEcanvas, *fEcanvas_ch1, *fEcanvas_ch2, *fEcanvas_ch3, *fEcanvas_ch4, *fEcanvas_ch5;
//    TRootEmbeddedCanvas *fEcanvas_evergy_spectrum, *fEcanvas_Npe_vs_time;

    TRootEmbeddedCanvas **fEcanvas_arr;
    const Int_t n_canvases;

    TGTextButton *button_start;
    bool is_start_button_activated;

    Pixel_t pixel_t_yellow;
    Pixel_t pixel_t_red;

    //Threads
    static void *ReadoutLoop(void*);
    TThread *slave_thread;

    //
    DataStr data_str;
    TGraph **graphs;
    Int_t aNrGraphs;
    Int_t n_points;
    //TGraph *gr;
};

#endif // MYMAINFRAME_H
