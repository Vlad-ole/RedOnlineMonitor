#include "mymainframe.h"

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) : n_canvases(8)
{
    // Create the main frame
    fMain = new TGMainFrame(p,w,h);

    //set size
    const UInt_t canvas_w = 500;//in pixel
    const UInt_t canvas_h = 500;//in pixel

    //colors
    gClient->GetColorByName("yellow",pixel_t_yellow);
    gClient->GetColorByName("red", pixel_t_red);


    //Different LayoutHints
    TGLayoutHints *fL_canvas = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX |kLHintsExpandY, 5, 5, 5, 5);
    TGLayoutHints *fL_ExpandY = new TGLayoutHints(kLHintsExpandY);
    TGLayoutHints *fL_control_panel = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 5, 5, 5, 5);
    TGLayoutHints *fL_fMain = new TGLayoutHints(kLHintsTop | kLHintsRight | kLHintsExpandX |kLHintsExpandY, 5, 5, 5, 5);

    //hframe for control_panel and tab_frame
    TGHorizontalFrame *hframe_control_panel_tab_frame = new TGHorizontalFrame(fMain,200,40);


    //-------------- control_panel
    //Create a vertical frame for control_panel
    TGVerticalFrame *vframe_control_panel = new TGVerticalFrame(hframe_control_panel_tab_frame,300,900);

    TGGroupFrame *gframe_control_panel = new TGGroupFrame(vframe_control_panel,"Control panel",kVerticalFrame);
    gframe_control_panel->SetTitlePos(TGGroupFrame::kLeft);

    TGGroupFrame *gframe_cp_common_opt = new TGGroupFrame(gframe_control_panel,"Common options",kVerticalFrame);
    gframe_cp_common_opt->SetTitlePos(TGGroupFrame::kCenter);

    TGGroupFrame *gframe_cp_hist_opt = new TGGroupFrame(gframe_control_panel,"Hist options",kVerticalFrame);
    gframe_cp_hist_opt->SetTitlePos(TGGroupFrame::kCenter);

    //========== common opt
    // Create a horizontal frame widget with buttons
    TGHorizontalFrame *hframe = new TGHorizontalFrame(gframe_cp_common_opt,200,40);
    TGTextButton *draw = new TGTextButton(hframe,"&Draw");
    //draw->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
    hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
    TGTextButton *exit = new TGTextButton(hframe,"&Exit","gApplication->Terminate(0)");
    hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

    //button_start
    button_start = new TGTextButton(gframe_cp_common_opt,"&Start acquisition");
    button_start->SetBackgroundColor(pixel_t_red);
    is_start_button_activated = false;
    button_start->Connect("Clicked()","MyMainFrame",this,"Clicked_start_button()");
 //   //button_start->Connect("Clicked()","MyWorker",worker,"DataAcquisition_Slot()");

    //set update_time
    TGHorizontalFrame *hframe_update_time = new TGHorizontalFrame(gframe_cp_common_opt,200,40);
    TGNumberEntry *NEntr_update_time = new TGNumberEntry(hframe_update_time, 1000, 6, 1,
             TGNumberFormat::kNESInteger,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             100,5000);                         //limit values

    TGLabel *label_update_time = new TGLabel(hframe_update_time, "Set update time [ms]");
    hframe_update_time->AddFrame(NEntr_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_update_time->AddFrame(label_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    //========== end common opt



    //========== Hist options
    TGTextButton *redraw_button = new TGTextButton(gframe_cp_hist_opt,"&Redraw hist");
    redraw_button->Connect("Clicked()","MyMainFrame",this,"RedrawHist()");

    //========== end Hist options



    gframe_cp_common_opt->AddFrame(button_start, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    gframe_cp_common_opt->AddFrame(hframe_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    gframe_cp_common_opt->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    gframe_cp_hist_opt->AddFrame(redraw_button, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    gframe_control_panel->AddFrame(gframe_cp_common_opt, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    gframe_control_panel->AddFrame(gframe_cp_hist_opt, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    vframe_control_panel->AddFrame(gframe_control_panel, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
 //   //--------------end control_panel






    //create the Tab widget
    TGTab *fTab = new TGTab(hframe_control_panel_tab_frame, 300, 300);



    //----------------------------- Wavefoms tab
    //Create Tab_1
    TGCompositeFrame *tab_frame = fTab->AddTab("Waveforms (ch0 - ch1)");


    //---------------- canvases
    fEcanvas_arr = new TRootEmbeddedCanvas*[ n_canvases ];

    //Create vertical frame for 2 rows
    TGVerticalFrame *vframe_canvases = new TGVerticalFrame(tab_frame,w,h);

    //----
    //row 1
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_canvas_row = new TGHorizontalFrame(vframe_canvases,w,h);

    // Create canvas widget
    //ch_0
    /*fEcanvas*/fEcanvas_arr[0]  = new TRootEmbeddedCanvas("Ecanvas",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(/*fEcanvas*/ fEcanvas_arr[0], fL_canvas);

    //ch_1
    /*fEcanvas_ch1*/ fEcanvas_arr[1] = new TRootEmbeddedCanvas("Ecanvas_ch1",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(/*fEcanvas_ch1*/ fEcanvas_arr[1], fL_canvas);

    //ch_2
    /*fEcanvas_ch2*/ fEcanvas_arr[2] = new TRootEmbeddedCanvas("Ecanvas_ch2",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(/*fEcanvas_ch2*/ fEcanvas_arr[2], fL_canvas);
    //----



    //----
    //row 2
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_canvas_row2 = new TGHorizontalFrame(vframe_canvases,w,h);

    //ch_3
    /*fEcanvas_ch3*/ fEcanvas_arr[3] = new TRootEmbeddedCanvas("Ecanvas_ch3",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(/*fEcanvas_ch3*/ fEcanvas_arr[3], fL_canvas);

    //ch_4
    /*fEcanvas_ch4*/ fEcanvas_arr[4] = new TRootEmbeddedCanvas("Ecanvas_ch4",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(/*fEcanvas_ch4*/ fEcanvas_arr[4], fL_canvas);

    //ch_5
    /*fEcanvas_ch5*/ fEcanvas_arr[5] = new TRootEmbeddedCanvas("Ecanvas_ch5",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(/*fEcanvas_ch5*/ fEcanvas_arr[5], fL_canvas);
    //----
    //---------------- end canvases


    //Fill vframe_canvases frame with 2 rows
    vframe_canvases->AddFrame(hframe_canvas_row, fL_canvas);
    vframe_canvases->AddFrame(hframe_canvas_row2, fL_canvas);

    tab_frame->AddFrame(vframe_canvases, fL_canvas);
    //-----------------------------end Wavefoms tab





    //----------------------------- Evergy_information tab
    //Create Tab_1
    tab_frame = fTab->AddTab("Energy info");

    TGHorizontalFrame *hframe_tab2 = new TGHorizontalFrame(tab_frame,200,40);

    /*fEcanvas_evergy_spectrum*/ fEcanvas_arr[6] = new TRootEmbeddedCanvas("Ecanvas_evergy_spectrum",hframe_tab2,canvas_w,canvas_h);
    hframe_tab2->AddFrame(/*fEcanvas_evergy_spectrum*/ fEcanvas_arr[6], fL_canvas);

    /*fEcanvas_Npe_vs_time*/ fEcanvas_arr[7] = new TRootEmbeddedCanvas("Ecanvas_Npe_vs_time",hframe_tab2,canvas_w,canvas_h);
    hframe_tab2->AddFrame(/*fEcanvas_Npe_vs_time*/ fEcanvas_arr[7], fL_canvas);


    tab_frame->AddFrame(hframe_tab2, fL_canvas);
    //----------------------------- end Evergy_information tab








    hframe_control_panel_tab_frame->AddFrame(vframe_control_panel, fL_control_panel);
    hframe_control_panel_tab_frame->AddFrame(fTab, fL_canvas);
    fMain->AddFrame(hframe_control_panel_tab_frame, fL_fMain);

    // Set a name to the main frame
    fMain->SetWindowName("Online monitor for ReD experiment");

    // Map all subwindows of main frame
    fMain->MapSubwindows();

    // Initialize the layout algorithm
    fMain->Resize(fMain->GetDefaultSize());

    // Map main frame
    fMain->MapWindow();

//    //init DataStr
//    data_str.counter = 0;
//    data_str.data_vv.resize(6);
//    const int n_points = 100;
//    for (int i = 0; i < data_str.data_vv.size(); ++i)
//    {
//        data_str.data_vv[i].resize(n_points);
//    }

    //Set params
    aNrGraphs = 6;
    n_points = 100;
    is_redraw_hist = true;
}

MyMainFrame::~MyMainFrame()
{
   // Clean up used widgets: frames, buttons, layout hints
   fMain->Cleanup();
   delete fMain;
}

void MyMainFrame::Clicked_start_button()
{

    if(is_start_button_activated)
    {
        button_start->SetBackgroundColor(pixel_t_red);
//        button_start->SetText("Start acquisition: inactive");
        is_start_button_activated = false;

    }
    else
    {
        button_start->SetBackgroundColor(pixel_t_yellow);
//        button_start->SetText("Start acquisition: active");
        is_start_button_activated = true;

    }
}

void MyMainFrame::InitGraphs()
{
    graphs = new TGraph*[ aNrGraphs ];

    for (int i = 0; i < aNrGraphs; i++)
    {
       graphs[i] = new TGraph( n_points );
       for(Int_t j = 0; j < n_points; j++)
       {
          graphs[i]->SetPoint(j, j, i);
       }

       graphs[i]->SetMarkerColor(2);
       graphs[i]->SetLineColor(2);
       graphs[i]->SetMarkerStyle(21);
       graphs[i]->SetMarkerSize(0.3);
    }

    hist = new TH1F("h1", "N_pe histogram", 100, -1, 1);
    hist->SetBit(TH1::kCanRebin);
    TRandom rnd;
    for (int i = 0; i < 1000; ++i)
    {
        hist->Fill(rnd.Uniform(-1, 1));
    }

    char y_axis_name[] = "Voltage [mV]";

    aCanvas_arr = new TCanvas*[n_canvases];
    for (int i = 0; i < n_canvases; ++i)
    {
        aCanvas_arr[i] = fEcanvas_arr[i]->GetCanvas();
        aCanvas_arr[i]->cd();

        //waveforms
        if (i < 6)
        {
            graphs[i]->Draw();

            std::ostringstream oss;
            oss << "ch_" << i;
            graphs[i]->SetTitle(oss.str().c_str());

            //graphs[i]->GetYaxis()->SetTitle(y_axis_name);//Axis titles do not work for slave thread. I do not know why

        }
        else if (i == 6)
        {
            hist->Draw();
        }

        aCanvas_arr[i]->Update();
    }

}

void MyMainFrame::RunThread()
{
    //Threads
    slave_thread = new TThread("slave_thread", (void(*) (void *))ReadoutLoop, (void*) this);
    slave_thread->Run();
}

void MyMainFrame::RedrawHist()
{
    is_redraw_hist = true;
}

void *MyMainFrame::ReadoutLoop(void *aPtr)
{
    //the whole func ReadoutLoop will be in slave thread
    printf("You are in MyMainFrame::ReadoutLoop() (Thread %d) \n", syscall(__NR_gettid));

    MyMainFrame *p = (MyMainFrame*)aPtr;

    TRandom rnd;
    const int baseline = 4000;
    vector<Float_t> xv(p->n_points);
    for (int i = 0; i < p->n_points; ++i)
    {
        xv[i] = i;
    }

    vector< vector<Float_t> > yvv;
    yvv.resize(p->aNrGraphs);
    for (int i = 0; i < p->aNrGraphs; ++i)
    {
        yvv[i].resize(p->n_points);
    }

    vector<Double_t> integral(p->aNrGraphs);
    const int time_step = 1;//ns
    double total_integral;;

    while(1)
    {
        if (p->is_start_button_activated)
        {
            //GetData
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                for (int j = 0; j < p->n_points; ++j)
                {
                    yvv[i][j] = rnd.Uniform(-10, 10) + baseline;
                }
            }

            //analize data ( you can move this code in slave_2 thread if you know how to do this:) )
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                integral[i] = 0;
                for (int j = 0; j < p->n_points; ++j)
                {
                    integral[i] += (yvv[i][j] - baseline);
                }

                integral[i] *= time_step;
            }

            total_integral = 0;
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                total_integral += integral[i];
            }


            //Global mutex to avoid data race (it is not so important in my case, but it is better do not remove mutex)
            TThread::Lock();

            //graphs
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                for (int j = 0; j < p->n_points; ++j)
                {
                    p->graphs[i]->SetPoint(j, xv[j], yvv[i][j]);
                }
            }

            //hist
            if(p->is_redraw_hist)
            {
                const int n_bins = p->hist->GetNbinsX();
                for (int i = 0; i < n_bins + 1; ++i)
                {
                    p->hist->SetBinContent(i, 0);
                }
                p->hist->SetEntries(0);
                p->is_redraw_hist = false;
            }

            p->hist->Fill(total_integral);

//            for (int i = 0; i < total; ++i)
//            {
//                p->hist->Fill();
//            }

            //canvases
            for (int i = 0; i < p->n_canvases; ++i)
            {
                p->aCanvas_arr[i] = p->fEcanvas_arr[i]->GetCanvas();
                p->aCanvas_arr[i]->Modified();
                p->aCanvas_arr[i]->Update();
            }

            TThread::UnLock();

            gSystem->Sleep(500);//dummy
        }
        else
        {
            //Wait permission to read data
            gSystem->Sleep(300);
        }


    }
}
