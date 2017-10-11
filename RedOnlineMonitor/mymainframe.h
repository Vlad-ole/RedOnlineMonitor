#ifndef MYMAINFRAME_H
#define MYMAINFRAME_H

//c++


//root cern general
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
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

class MyMainFrame
{
    RQ_OBJECT("MyMainFrame")
public:
    MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
    virtual ~MyMainFrame();

    void Clicked_start_button();

private:
    TGMainFrame *fMain;
    TRootEmbeddedCanvas *fEcanvas, *fEcanvas_ch1, *fEcanvas_ch2, *fEcanvas_ch3, *fEcanvas_ch4, *fEcanvas_ch5;
    TRootEmbeddedCanvas *fEcanvas_evergy_spectrum, *fEcanvas_Npe_vs_time;

    TGTextButton *button_start;
    bool is_start_button_activated;

    Pixel_t pixel_t_yellow;
    Pixel_t pixel_t_red;
};

#endif // MYMAINFRAME_H
