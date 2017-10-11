#include "mymainframe.h"

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h)
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

    // Create a horizontal frame widget with buttons
    TGHorizontalFrame *hframe = new TGHorizontalFrame(vframe_control_panel,200,40);
    TGTextButton *draw = new TGTextButton(hframe,"&Draw");
    //draw->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
    hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
    TGTextButton *exit = new TGTextButton(hframe,"&Exit","gApplication->Terminate(0)");
    hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

    //button_start
    button_start = new TGTextButton(vframe_control_panel,"&Start acquisition");
    button_start->SetBackgroundColor(pixel_t_red);
    is_start_button_activated = false;
    button_start->Connect("Clicked()","MyMainFrame",this,"Clicked_start_button()");
 //   //button_start->Connect("Clicked()","MyWorker",worker,"DataAcquisition_Slot()");

    //set update_time
    TGHorizontalFrame *hframe_update_time = new TGHorizontalFrame(vframe_control_panel,200,40);
    TGNumberEntry *NEntr_update_time = new TGNumberEntry(hframe_update_time, 1000, 6, 1,
             TGNumberFormat::kNESInteger,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             100,5000);                         //limit values

    TGLabel *label_update_time = new TGLabel(hframe_update_time, "Set update time [ms]");
    hframe_update_time->AddFrame(NEntr_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_update_time->AddFrame(label_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));



    vframe_control_panel->AddFrame(button_start, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    vframe_control_panel->AddFrame(hframe_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    vframe_control_panel->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
 //   //--------------end control_panel






    //create the Tab widget
    TGTab *fTab = new TGTab(hframe_control_panel_tab_frame, 300, 300);



    //----------------------------- Wavefoms tab
    //Create Tab_1
    TGCompositeFrame *tab_frame = fTab->AddTab("Waveforms (ch0 - ch1)");


    //---------------- canvases
    //Create vertical frame for 2 rows
    TGVerticalFrame *vframe_canvases = new TGVerticalFrame(tab_frame,w,h);

    //----
    //row 1
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_canvas_row = new TGHorizontalFrame(vframe_canvases,w,h);

    // Create canvas widget
    //ch_0
    fEcanvas = new TRootEmbeddedCanvas("Ecanvas",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(fEcanvas, fL_canvas);

    //ch_1
    fEcanvas_ch1 = new TRootEmbeddedCanvas("Ecanvas_ch1",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(fEcanvas_ch1, fL_canvas);

    //ch_2
    fEcanvas_ch2 = new TRootEmbeddedCanvas("Ecanvas_ch2",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(fEcanvas_ch2, fL_canvas);
    //----



    //----
    //row 2
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_canvas_row2 = new TGHorizontalFrame(vframe_canvases,w,h);

    //ch_3
    fEcanvas_ch3 = new TRootEmbeddedCanvas("Ecanvas_ch3",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(fEcanvas_ch3, fL_canvas);

    //ch_4
    fEcanvas_ch4 = new TRootEmbeddedCanvas("Ecanvas_ch4",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(fEcanvas_ch4, fL_canvas);

    //ch_5
    fEcanvas_ch5 = new TRootEmbeddedCanvas("Ecanvas_ch5",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(fEcanvas_ch5, fL_canvas);
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

    fEcanvas_evergy_spectrum = new TRootEmbeddedCanvas("Ecanvas_evergy_spectrum",hframe_tab2,canvas_w,canvas_h);
    hframe_tab2->AddFrame(fEcanvas_evergy_spectrum, fL_canvas);

    fEcanvas_Npe_vs_time = new TRootEmbeddedCanvas("Ecanvas_Npe_vs_time",hframe_tab2,canvas_w,canvas_h);
    hframe_tab2->AddFrame(fEcanvas_Npe_vs_time, fL_canvas);


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
