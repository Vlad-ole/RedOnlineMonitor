#include "mymainframe.h"

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) : n_canvases(18), aNrGraphs(6)
{
    // Create the main frame
    fMain = new TGMainFrame(p,w,h);

    //PreIntin
    time_baseline_gate_from = 0;
    time_baseline_gate_to = 1;
    time_signal_gate_from = 1.4;
    time_signal_gate_to = 8;
    time_signal_gate_fast_to = 1.6;



    //set size
    const UInt_t canvas_w = 500;//in pixel
    const UInt_t canvas_h = 500;//in pixel

    //colors
    gClient->GetColorByName("yellow",pixel_t_yellow);
    gClient->GetColorByName("red", pixel_t_red);
    gClient->GetColorByName("blue", pixel_t_blue);
    gClient->GetColorByName("orange", pixel_t_orange);
    gClient->GetColorByName("magenta", pixel_t_magenta);
    gClient->GetColorByName("green", pixel_t_green);
    gClient->GetColorByName("white",pixel_t_white);



    //Different LayoutHints
    TGLayoutHints *fL_canvas = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX |kLHintsExpandY, 5, 5, 5, 5);
    TGLayoutHints *fL_ExpandY = new TGLayoutHints(kLHintsExpandY);
    TGLayoutHints *fL_control_panel = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 5, 5, 5, 5);
    TGLayoutHints *fL_TL = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
    TGLayoutHints *fL_fMain = new TGLayoutHints(kLHintsTop | kLHintsRight | kLHintsExpandX |kLHintsExpandY, 5, 5, 5, 5);

    //hframe for control_panel and tab_frame
    TGHorizontalFrame *hframe_control_panel_tab_frame = new TGHorizontalFrame(fMain,200,40);


    //-------------- control_panel
    //Create a vertical frame for control_panel
    TGVerticalFrame *vframe_control_panel = new TGVerticalFrame(hframe_control_panel_tab_frame,300,900);

    TGGroupFrame *gframe_control_panel = new TGGroupFrame(vframe_control_panel,"Control panel",kVerticalFrame);
    gframe_control_panel->SetTitlePos(TGGroupFrame::kLeft);

    TGGroupFrame *gframe_cp_stability_gr_opt = new TGGroupFrame(gframe_control_panel,"Stability graph options",kVerticalFrame);
    gframe_cp_stability_gr_opt->SetTitlePos(TGGroupFrame::kCenter);



    //============================================== common opt
    TGGroupFrame *gframe_cp_common_opt = new TGGroupFrame(gframe_control_panel,"Common options",kVerticalFrame);
    gframe_cp_common_opt->SetTitlePos(TGGroupFrame::kCenter);



    // Create a horizontal frame widget with buttons
    TGHorizontalFrame *hframe = new TGHorizontalFrame(gframe_cp_common_opt,200,40);
    TGTextButton *exit = new TGTextButton(hframe,"&Exit","gApplication->Terminate(0)");
    hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

    //button_start
    TGHorizontalFrame *hframe_start = new TGHorizontalFrame(gframe_cp_common_opt,200,40);
    button_start = new TGTextButton(hframe_start,"&Start acquisition");
    button_start->SetBackgroundColor(pixel_t_red);
    is_start_button_activated = false;
    button_start->Connect("Clicked()","MyMainFrame",this,"Clicked_start_button()");
    hframe_start->AddFrame(button_start, fL_control_panel);

    //signle_run
    TGTextButton *signle_run = new TGTextButton(hframe_start,"Single&Run");
    signle_run->SetEnabled(kFALSE);
    //signle_run->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
    hframe_start->AddFrame(signle_run, fL_control_panel);



    //set update_time
    TGHorizontalFrame *hframe_update_time = new TGHorizontalFrame(gframe_cp_common_opt,200,40);
    NEntr_update_time = new TGNumberEntry(hframe_update_time, 1, 6, 1,
             TGNumberFormat::kNESReal,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             0.1,100);                         //limit values
    //NEntr_update_time->SetState(kFALSE);
    NEntr_update_time->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetDesirableUpdateRate()");
    //(NEntr_update_time->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetDesirableUpdateRate()");

    TGLabel *label_update_time = new TGLabel(hframe_update_time, "Set desirable update rate [Hz]");
    hframe_update_time->AddFrame(NEntr_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_update_time->AddFrame(label_update_time, new TGLayoutHints(kLHintsCenterX,2,2,2,2));






    //---------gates
    hframe_gates = new TGHorizontalFrame(gframe_cp_common_opt,200,40);

    //baseline gate
    TGGroupFrame *gframe_cp_common_opt_baseline_gate = new TGGroupFrame(hframe_gates,"Baseline gate",kVerticalFrame);
    TGVerticalFrame *vframe_cp_copt_bgate_gframe = new TGVerticalFrame(gframe_cp_common_opt_baseline_gate,300,900);


    //row1
    TGHorizontalFrame *hframe_cp_copt_bgate_gframe_row1 = new TGHorizontalFrame(vframe_cp_copt_bgate_gframe,200,40);
    NEntr_baseline_gate_from = new TGNumberEntry(hframe_cp_copt_bgate_gframe_row1, time_baseline_gate_from, 6, 2,
             TGNumberFormat::kNESReal,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             0,1E6);
    NEntr_baseline_gate_from->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetBaselineGateFrom()");

    TGLabel *label_bgate_row1 = new TGLabel(hframe_cp_copt_bgate_gframe_row1, "t_from [us]");
    label_bgate_row1->SetTextColor(pixel_t_red);
    //hframe_cp_copt_bgate_gframe_row1->AddFrame(cbutt_bgate_from, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_cp_copt_bgate_gframe_row1->AddFrame(NEntr_baseline_gate_from, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_cp_copt_bgate_gframe_row1->AddFrame(label_bgate_row1, new TGLayoutHints(kLHintsLeft));


    //row2
    TGHorizontalFrame *hframe_cp_copt_bgate_gframe_row2 = new TGHorizontalFrame(vframe_cp_copt_bgate_gframe,200,40);
    NEntr_baseline_gate_to = new TGNumberEntry(hframe_cp_copt_bgate_gframe_row2, time_baseline_gate_to, 6, 3,
             TGNumberFormat::kNESReal,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             0,1E6);
    NEntr_baseline_gate_to->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetBaselineGateTo()");
    //(NEntr_baseline_gate_to->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetBaselineGateTo()");

    TGLabel *label_bgate_row2 = new TGLabel(hframe_cp_copt_bgate_gframe_row2, "t_to [us]");
    label_bgate_row2->SetTextColor(pixel_t_green);
    hframe_cp_copt_bgate_gframe_row2->AddFrame(NEntr_baseline_gate_to, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_cp_copt_bgate_gframe_row2->AddFrame(label_bgate_row2, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    vframe_cp_copt_bgate_gframe->AddFrame(hframe_cp_copt_bgate_gframe_row1, new TGLayoutHints(kLHintsLeft,2,2,2,2));
    vframe_cp_copt_bgate_gframe->AddFrame(hframe_cp_copt_bgate_gframe_row2, new TGLayoutHints(kLHintsLeft,2,2,2,2));
    gframe_cp_common_opt_baseline_gate->AddFrame(vframe_cp_copt_bgate_gframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_gates->AddFrame(gframe_cp_common_opt_baseline_gate, new TGLayoutHints(kLHintsCenterX,2,2,2,2));


    //signal gate
    TGGroupFrame *gframe_cp_common_opt_signal_gate = new TGGroupFrame(hframe_gates,"Signal gate",kVerticalFrame);
    TGVerticalFrame *vframe_cp_copt_sgate_gframe = new TGVerticalFrame(gframe_cp_common_opt_signal_gate,300,900);
    //row1
    TGHorizontalFrame *hframe_cp_copt_sgate_gframe_row1 = new TGHorizontalFrame(vframe_cp_copt_sgate_gframe,200,40);
    NEntr_signal_gate_from = new TGNumberEntry(hframe_cp_copt_sgate_gframe_row1, time_signal_gate_from, 6, 4,
             TGNumberFormat::kNESReal,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             0,1E6);
    NEntr_signal_gate_from->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetSignalGateFrom()");
    //(NEntr_signal_gate_from->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetSignalGateFrom()");

    TGLabel *label_sgate_row1 = new TGLabel(hframe_cp_copt_sgate_gframe_row1, "t_from [us]");
    label_sgate_row1->SetTextColor(pixel_t_blue);
    hframe_cp_copt_sgate_gframe_row1->AddFrame(NEntr_signal_gate_from, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_cp_copt_sgate_gframe_row1->AddFrame(label_sgate_row1, new TGLayoutHints(kLHintsCenterX,2,2,2,2));


    //row2
    TGHorizontalFrame *hframe_cp_copt_sgate_gframe_row2 = new TGHorizontalFrame(vframe_cp_copt_sgate_gframe,200,40);
    NEntr_signal_gate_to = new TGNumberEntry(hframe_cp_copt_sgate_gframe_row2, time_signal_gate_to, 6, 5,
             TGNumberFormat::kNESReal,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             0,1E6);
    NEntr_signal_gate_to->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetSignalGateTo()");
    //(NEntr_signal_gate_to->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetSignalGateTo()");

    TGLabel *label_sgate_row2 = new TGLabel(hframe_cp_copt_sgate_gframe_row2, "t_to [us]");
    label_sgate_row2->SetTextColor(pixel_t_magenta);
    hframe_cp_copt_sgate_gframe_row2->AddFrame(NEntr_signal_gate_to, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_cp_copt_sgate_gframe_row2->AddFrame(label_sgate_row2, new TGLayoutHints(kLHintsCenterX,2,2,2,2));


    //row3
    TGHorizontalFrame *hframe_cp_copt_sgate_gframe_row3 = new TGHorizontalFrame(vframe_cp_copt_sgate_gframe,200,40);
    NEntr_signal_gate_fast_to = new TGNumberEntry(hframe_cp_copt_sgate_gframe_row3, time_signal_gate_fast_to, 6, 6,
             TGNumberFormat::kNESReal,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             0,1E6);
    NEntr_signal_gate_fast_to->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetSignalGateFastTo()");
    //(NEntr_signal_gate_fast_to->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetSignalGateFastTo()");

    TGLabel *label_sgate_row3 = new TGLabel(hframe_cp_copt_sgate_gframe_row3, "t_fast_to [us]");
    label_sgate_row3->SetTextColor(pixel_t_orange);
    hframe_cp_copt_sgate_gframe_row3->AddFrame(NEntr_signal_gate_fast_to, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_cp_copt_sgate_gframe_row3->AddFrame(label_sgate_row3, new TGLayoutHints(kLHintsCenterX,2,2,2,2));


    vframe_cp_copt_sgate_gframe->AddFrame(hframe_cp_copt_sgate_gframe_row1, new TGLayoutHints(kLHintsLeft,2,2,2,2));
    vframe_cp_copt_sgate_gframe->AddFrame(hframe_cp_copt_sgate_gframe_row2, new TGLayoutHints(kLHintsLeft,2,2,2,2));
    vframe_cp_copt_sgate_gframe->AddFrame(hframe_cp_copt_sgate_gframe_row3, new TGLayoutHints(kLHintsLeft,2,2,2,2));
    gframe_cp_common_opt_signal_gate->AddFrame(vframe_cp_copt_sgate_gframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_gates->AddFrame(gframe_cp_common_opt_signal_gate, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    //---------end gates





    //============================================== end common opt



    //========== Hist options
    //create the Tab widget for hists
    TGTab *fTab_cp_hist = new TGTab(gframe_control_panel, 300, 300);

    //Create Hist Tab_1
    tab_frame_cp_hist_opt = fTab_cp_hist->AddTab("Hist options");
    //TGGroupFrame *gframe_cp_hist_opt = new TGGroupFrame(gframe_control_panel,"Hist options",kVerticalFrame);
    //gframe_cp_hist_opt->SetTitlePos(TGGroupFrame::kCenter);

    TGTextButton *redraw_button = new TGTextButton(tab_frame_cp_hist_opt,"&Redraw hist");
    redraw_button->Connect("Clicked()","MyMainFrame",this,"RedrawHist()");

    //hist limits
    TGGroupFrame *gframe_cp_hist_opt_hist_limits = new TGGroupFrame(tab_frame_cp_hist_opt,"Set left/right limits, n_bins, auto_rebin",kHorizontalFrame);
    TGVerticalFrame *vframe_hlimits_labels = new TGVerticalFrame(gframe_cp_hist_opt_hist_limits,200,40);
    TGVerticalFrame *vframe_hlimits_llimits = new TGVerticalFrame(gframe_cp_hist_opt_hist_limits,200,40);
    TGVerticalFrame *vframe_hlimits_rlimits = new TGVerticalFrame(gframe_cp_hist_opt_hist_limits,200,40);
    TGVerticalFrame *vframe_hlimits_n_bins = new TGVerticalFrame(gframe_cp_hist_opt_hist_limits,200,40);
    TGVerticalFrame *vframe_hlimits_check_buttons = new TGVerticalFrame(gframe_cp_hist_opt_hist_limits,200,40);
    const UInt_t n_rows = aNrGraphs + 4;
    hlimits_labels = new TGLabel*[n_rows];
    NEntr_hframe_cp_hist_l_limits = new TGNumberEntry*[n_rows];
    NEntr_hframe_cp_hist_r_limits = new TGNumberEntry*[n_rows];
    NEntr_hframe_cp_hist_n_bins = new TGNumberEntry*[n_rows];
    check_button_hlimits = new TGCheckButton*[n_rows];
    hlimits_lvalues.resize(n_rows, -100);
    hlimits_rvalues.resize(n_rows, 100);
    hlimits_n_bins.resize(n_rows, 1000);

    for (int i = 0; i < n_rows; ++i)
    {
        const int pad = 0;

        std::ostringstream osstr;
        if (i < aNrGraphs)
        {
            osstr << "ch_" << i;
        }
        else if ( i == (aNrGraphs) ) osstr << "CombH1TotInt";
        else if ( i == (aNrGraphs + 1) ) osstr << "CombH1FasInt";
        else if ( i == (aNrGraphs + 2) ) osstr << "CombH2TotInt";
        else if ( i == (aNrGraphs + 3) ) osstr << "CombH2FasInt";
        else osstr << "unknown";



        hlimits_labels[i] = new TGLabel(vframe_hlimits_labels, osstr.str().c_str());
        vframe_hlimits_labels->AddFrame(hlimits_labels[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));

        //l_limit
        NEntr_hframe_cp_hist_l_limits[i] = new TGNumberEntry(vframe_hlimits_llimits, hlimits_lvalues[i], 6, 20 + i,
                                                             TGNumberFormat::kNESReal,   //style
                                                             TGNumberFormat::kNEAAnyNumber,   //input value filter
                                                             TGNumberFormat::kNELNoLimits //specify limits
                                                             );
        NEntr_hframe_cp_hist_l_limits[i]->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetHistLimits()");
        vframe_hlimits_llimits->AddFrame(NEntr_hframe_cp_hist_l_limits[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));


        //r_limit
        NEntr_hframe_cp_hist_r_limits[i] = new TGNumberEntry(vframe_hlimits_rlimits, hlimits_rvalues[i], 6, 50 + i,
                                                             TGNumberFormat::kNESReal,   //style
                                                             TGNumberFormat::kNEAAnyNumber,   //input value filter
                                                             TGNumberFormat::kNELNoLimits //specify limits
                                                             );
        NEntr_hframe_cp_hist_r_limits[i]->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetHistLimits()");
        vframe_hlimits_rlimits->AddFrame(NEntr_hframe_cp_hist_r_limits[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));


        //n_bins
        NEntr_hframe_cp_hist_n_bins[i] = new TGNumberEntry(vframe_hlimits_n_bins, hlimits_n_bins[i], 6, 70 + i,
                                                             TGNumberFormat::kNESInteger,   //style
                                                             TGNumberFormat::kNEAPositive,   //input value filter
                                                             TGNumberFormat::kNELNoLimits //specify limits
                                                             );
        NEntr_hframe_cp_hist_n_bins[i]->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetHistNBins()");
        vframe_hlimits_n_bins->AddFrame(NEntr_hframe_cp_hist_n_bins[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));

        //auto rebin check_buttons
        check_button_hlimits[i] = new TGCheckButton(vframe_hlimits_check_buttons,"", 100 + i);
        check_button_hlimits[i]->SetState(kButtonDown);
        vframe_hlimits_check_buttons->AddFrame(check_button_hlimits[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));
    }

    //vframe_hlimits_labels->SetBackgroundColor(pixel_t_red);//debug
    //vframe_hlimits_check_buttons->SetBackgroundColor(pixel_t_red);//debug

    gframe_cp_hist_opt_hist_limits->AddFrame(vframe_hlimits_labels, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hist_opt_hist_limits->AddFrame(vframe_hlimits_llimits, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hist_opt_hist_limits->AddFrame(vframe_hlimits_rlimits, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hist_opt_hist_limits->AddFrame(vframe_hlimits_n_bins, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hist_opt_hist_limits->AddFrame(vframe_hlimits_check_buttons, new TGLayoutHints(kLHintsExpandY,2,2,2,2));

    tab_frame_cp_hist_opt->AddFrame(redraw_button, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    tab_frame_cp_hist_opt->AddFrame(gframe_cp_hist_opt_hist_limits, new TGLayoutHints(kLHintsCenterX,2,2,2,2));




    //Create Hist Tab_2
    tab_frame_cp_hanalysis = fTab_cp_hist->AddTab("Hist analysis");
    //tab_frame_cp_hanalysis->SetEditable(kFALSE);
    TGTextButton *analyze_button = new TGTextButton(tab_frame_cp_hanalysis,"&Analyze");
    analyze_button->Connect("Clicked()","MyMainFrame",this,"AnalyzeHistsSlot()");
    tab_frame_cp_hanalysis->AddFrame(analyze_button, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

//    //hist analysis options
    TGGroupFrame *gframe_cp_hanalysis = new TGGroupFrame(tab_frame_cp_hanalysis,"Set left/right limits, sigma[bins]",kHorizontalFrame);
    TGVerticalFrame *vframe_hanalysis_labels = new TGVerticalFrame(gframe_cp_hanalysis,200,40);
//    vframe_hanalysis_labels->SetBackgroundColor(pixel_t_red);
    TGVerticalFrame *vframe_hanalysis_auto_limits_checkb = new TGVerticalFrame(gframe_cp_hanalysis,200,40);
    vframe_hanalysis_llimits = new TGVerticalFrame(gframe_cp_hanalysis,200,40);
    vframe_hanalysis_rlimits = new TGVerticalFrame(gframe_cp_hanalysis,200,40);
    TGVerticalFrame *vframe_hanalysis_sigma = new TGVerticalFrame(gframe_cp_hanalysis,200,40);

    hanalysis_lvalues.resize(aNrGraphs, -100);
    hanalysis_rvalues.resize(aNrGraphs, 100);
    hanalysis_sigma.resize(aNrGraphs, 8);
    TGLabel **analysis_options_labels = new TGLabel*[aNrGraphs];
    NEntr_hanalysis_lvalues = new TGNumberEntry*[aNrGraphs];
    NEntr_hanalysis_rvalues = new TGNumberEntry*[aNrGraphs];
    NEntr_hanalysis_sigma = new TGNumberEntry*[aNrGraphs];
    Chbt_hanalysis_auto_limits_checkb = new TGCheckButton*[aNrGraphs];

    for (int i = 0; i < aNrGraphs; ++i)
    {
        const int pad = 0;

//        //labels
        analysis_options_labels[i] = new TGLabel(vframe_hanalysis_labels, new TGString(hlimits_labels[i]->GetText()) );
        vframe_hanalysis_labels->AddFrame(analysis_options_labels[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));

        //auto_limits_checkb
        Chbt_hanalysis_auto_limits_checkb[i] = new TGCheckButton(vframe_hanalysis_auto_limits_checkb, "");
        vframe_hanalysis_auto_limits_checkb->AddFrame(Chbt_hanalysis_auto_limits_checkb[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));
        Chbt_hanalysis_auto_limits_checkb[i]->SetState(kButtonDown);


//        //llimits
        NEntr_hanalysis_lvalues[i] = new TGNumberEntry(vframe_hanalysis_llimits, hanalysis_lvalues[i], 6, 200 + i,
                                                             TGNumberFormat::kNESReal,   //style
                                                             TGNumberFormat::kNEAAnyNumber,   //input value filter
                                                             TGNumberFormat::kNELNoLimits //specify limits
                                                             );
        NEntr_hanalysis_lvalues[i]->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetAnalysisHistLimits()");
        vframe_hanalysis_llimits->AddFrame(NEntr_hanalysis_lvalues[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));
        NEntr_hanalysis_lvalues[i]->SetState(kFALSE);

        //rlimits
        NEntr_hanalysis_rvalues[i] = new TGNumberEntry(vframe_hanalysis_rlimits, hanalysis_rvalues[i], 6, 250 + i,
                                                             TGNumberFormat::kNESReal,   //style
                                                             TGNumberFormat::kNEAAnyNumber,   //input value filter
                                                             TGNumberFormat::kNELNoLimits //specify limits
                                                             );
        NEntr_hanalysis_rvalues[i]->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetAnalysisHistLimits()");
        vframe_hanalysis_rlimits->AddFrame(NEntr_hanalysis_rvalues[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));
        NEntr_hanalysis_rvalues[i]->SetState(kFALSE);


        //sigma
        NEntr_hanalysis_sigma[i] = new TGNumberEntry(vframe_hanalysis_sigma, hanalysis_sigma[i], 6, 300 + i,
                                                             TGNumberFormat::kNESReal,   //style
                                                             TGNumberFormat::kNEAPositive,   //input value filter
                                                             TGNumberFormat::kNELLimitMinMax, //specify limits
                                                             1, 30000);
        NEntr_hanalysis_sigma[i]->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetAnalysisHistSigma()");
        vframe_hanalysis_sigma->AddFrame(NEntr_hanalysis_sigma[i], new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,pad,pad,pad,pad));

    }

    gframe_cp_hanalysis->AddFrame(vframe_hanalysis_labels, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hanalysis->AddFrame(vframe_hanalysis_auto_limits_checkb, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hanalysis->AddFrame(vframe_hanalysis_llimits, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hanalysis->AddFrame(vframe_hanalysis_rlimits, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    gframe_cp_hanalysis->AddFrame(vframe_hanalysis_sigma, new TGLayoutHints(kLHintsExpandY,2,2,2,2));
    tab_frame_cp_hanalysis->AddFrame(gframe_cp_hanalysis, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY,2,2,2,2));

    fTab_cp_hist->Connect("Selected(Int_t)", "MyMainFrame", this, "fTab_cp_hist_selected(Int_t)");

    //gframe_cp_hanalysis->Activate(kFALSE);
    //gframe_cp_hanalysis->GetList();
    //========== end Hist options



    //========== Stability graph options
    TGHorizontalFrame *hframe_n_events_for_avr = new TGHorizontalFrame(gframe_cp_stability_gr_opt,200,40);
    NEntr_n_events_for_avr = new TGNumberEntry(hframe_n_events_for_avr, 1, 6, 2,
             TGNumberFormat::kNESInteger,   //style
             TGNumberFormat::kNEAPositive,   //input value filter
             TGNumberFormat::kNELLimitMinMax, //specify limits
             1,5000);
    NEntr_n_events_for_avr->Connect("ValueSet(Long_t)", "MyMainFrame", this, "ChangeNEventsForAvr()");
    NEntr_n_events_for_avr->SetState(kFALSE);


    TGLabel *label_n_events_for_avr = new TGLabel(hframe_n_events_for_avr, "Set n_event for avr");
    hframe_n_events_for_avr->AddFrame(NEntr_n_events_for_avr, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_n_events_for_avr->AddFrame(label_n_events_for_avr, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    //========== end Stability graph options



    //========== rate
    TGHorizontalFrame *hframe_rate = new TGHorizontalFrame(gframe_control_panel,200,40);

    gframe_cp_income_rate = new TGGroupFrame(hframe_rate,"Income rate [Hz]",kVerticalFrame);
    gframe_cp_income_rate->SetTitlePos(TGGroupFrame::kLeft);

    gframe_cp_update_rate = new TGGroupFrame(hframe_rate,"Update rate [Hz]",kVerticalFrame);
    gframe_cp_update_rate->SetTitlePos(TGGroupFrame::kLeft);


    fLabel_income_rate = new TGLabel(gframe_cp_income_rate, "No input.");
    gframe_cp_income_rate->AddFrame(fLabel_income_rate, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    fLabel_update_rate = new TGLabel(gframe_cp_update_rate, "No input.");
    gframe_cp_update_rate->AddFrame(fLabel_update_rate, new TGLayoutHints(kLHintsCenterX,2,2,2,2));


    hframe_rate->AddFrame(gframe_cp_income_rate, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    hframe_rate->AddFrame(gframe_cp_update_rate, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    //========== end rate


    //========== status label
    gframe_status_label = new TGGroupFrame(gframe_control_panel,"Status box",kFitWidth);
    gframe_status_label->SetTextColor(pixel_t_green);
    //gframe_status_label->SetBackgroundColor(pixel_t_white);
    twStatus_label = new TGTextView(gframe_status_label, 400, 150);
    sst_status_label << GetCurrentTime() << "Press \"Start acquisition\" to get, integral and draw data";
    twStatus_label->AddLine(sst_status_label.str().c_str());
    sst_status_label.str("");
    sst_status_label<< GetCurrentTime() << "Note: to analyze data \"acquisition\" should be stopped";
    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
    gframe_status_label->AddFrame(twStatus_label, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    //========== end status label



    gframe_cp_common_opt->AddFrame(hframe_start, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    gframe_cp_common_opt->AddFrame(hframe_update_time, new TGLayoutHints(kLHintsLeft,0,0,0,0));
    gframe_cp_common_opt->AddFrame(hframe_gates, new TGLayoutHints(kLHintsLeft));
    gframe_cp_common_opt->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    gframe_cp_stability_gr_opt->AddFrame(hframe_n_events_for_avr, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    gframe_control_panel->AddFrame(gframe_cp_common_opt, new TGLayoutHints(kLHintsLeft));
    gframe_control_panel->AddFrame(fTab_cp_hist, new TGLayoutHints(kLHintsLeft));
    gframe_control_panel->AddFrame(gframe_cp_stability_gr_opt, new TGLayoutHints(kLHintsLeft));
    gframe_control_panel->AddFrame(hframe_rate, new TGLayoutHints(kLHintsLeft));
    gframe_control_panel->AddFrame(gframe_status_label, new TGLayoutHints(kLHintsLeft));


    vframe_control_panel->AddFrame(gframe_control_panel, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
 //   //--------------end control_panel






    //create the Tab widget for canvases
    fTab = new TGTab(hframe_control_panel_tab_frame, 300, 300);

    //---------------- canvases
    fEcanvas_arr = new TRootEmbeddedCanvas*[ n_canvases ];




    //----------------------------- Wavefoms tab
    //Create Tab_1
    TGCompositeFrame *tab_frame = fTab->AddTab("Waveforms (ch0 - ch5)");

    //Create vertical frame for 2 rows
    TGVerticalFrame *vframe_canvases = new TGVerticalFrame(tab_frame,w,h);

    //----
    //row 1
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_canvas_row = new TGHorizontalFrame(vframe_canvases,w,h);

    // Create canvas widget
    //ch_0
    fEcanvas_arr[0]  = new TRootEmbeddedCanvas("Ecanvas",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(fEcanvas_arr[0], fL_canvas);

    //ch_1
    fEcanvas_arr[1] = new TRootEmbeddedCanvas("Ecanvas_ch1",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(fEcanvas_arr[1], fL_canvas);

    //ch_2
    fEcanvas_arr[2] = new TRootEmbeddedCanvas("Ecanvas_ch2",hframe_canvas_row,canvas_w,canvas_h);
    hframe_canvas_row->AddFrame(fEcanvas_arr[2], fL_canvas);
    //----



    //----
    //row 2
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_canvas_row2 = new TGHorizontalFrame(vframe_canvases,w,h);

    //ch_3
    fEcanvas_arr[3] = new TRootEmbeddedCanvas("Ecanvas_ch3",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(fEcanvas_arr[3], fL_canvas);

    //ch_4
    fEcanvas_arr[4] = new TRootEmbeddedCanvas("Ecanvas_ch4",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(fEcanvas_arr[4], fL_canvas);

    //ch_5
    fEcanvas_arr[5] = new TRootEmbeddedCanvas("Ecanvas_ch5",hframe_canvas_row2,canvas_w,canvas_h);
    hframe_canvas_row2->AddFrame(fEcanvas_arr[5], fL_canvas);
    //----
    //---------------- end canvases


    //Fill vframe_canvases frame with 2 rows
    vframe_canvases->AddFrame(hframe_canvas_row, fL_canvas);
    vframe_canvases->AddFrame(hframe_canvas_row2, fL_canvas);

    tab_frame->AddFrame(vframe_canvases, fL_canvas);
    //-----------------------------end Wavefoms tab







    //----------------------------- Hists tab
    tab_frame = fTab->AddTab("Hists (ch0 - ch5)");

    //Create vertical frame for 2 rows
    TGVerticalFrame *vframe_hists = new TGVerticalFrame(tab_frame,w,h);

    //----
    //row 1
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_hists_row = new TGHorizontalFrame(vframe_hists,w,h);

    // Create canvas widgets
    //ch_0
    fEcanvas_arr[6]  = new TRootEmbeddedCanvas("Ecanvas_hist_ch0",hframe_hists_row,canvas_w,canvas_h);
    hframe_hists_row->AddFrame(fEcanvas_arr[6], fL_canvas);

    //ch_1
    fEcanvas_arr[7] = new TRootEmbeddedCanvas("Ecanvas_hist_ch1",hframe_hists_row,canvas_w,canvas_h);
    hframe_hists_row->AddFrame(fEcanvas_arr[7], fL_canvas);

    //ch_2
    fEcanvas_arr[8] = new TRootEmbeddedCanvas("Ecanvas_hist_ch2",hframe_hists_row,canvas_w,canvas_h);
    hframe_hists_row->AddFrame(fEcanvas_arr[8], fL_canvas);
    //----


    //----
    //row 2
    //Create a horizontal frame widget with Ecanvases
    TGHorizontalFrame *hframe_hists_row2 = new TGHorizontalFrame(vframe_hists,w,h);

    // Create canvas widgets
    //ch_3
    fEcanvas_arr[9]  = new TRootEmbeddedCanvas("Ecanvas_hist_ch3",hframe_hists_row2,canvas_w,canvas_h);
    hframe_hists_row2->AddFrame(fEcanvas_arr[9], fL_canvas);

    //ch_4
    fEcanvas_arr[10] = new TRootEmbeddedCanvas("Ecanvas_hist_ch4",hframe_hists_row2,canvas_w,canvas_h);
    hframe_hists_row2->AddFrame(fEcanvas_arr[10], fL_canvas);

    //ch_5
    fEcanvas_arr[11] = new TRootEmbeddedCanvas("Ecanvas_hist_ch5",hframe_hists_row2,canvas_w,canvas_h);
    hframe_hists_row2->AddFrame(fEcanvas_arr[11], fL_canvas);
    //----


    //Fill vframe_hists frame with 2 rows
    vframe_hists->AddFrame(hframe_hists_row, fL_canvas);
    vframe_hists->AddFrame(hframe_hists_row2, fL_canvas);

    tab_frame->AddFrame(vframe_hists, fL_canvas);

    //----------------------------- end Hists tab




    //----------------------------- Combined hists tab
    tab_frame = fTab->AddTab("Combined hists");

    TGVerticalFrame *vframe_combined_hists = new TGVerticalFrame(tab_frame,200,40);

    //row1
    TGHorizontalFrame *hframe_combined_hists_row1 = new TGHorizontalFrame(vframe_combined_hists,200,40);
    //panel
    TGGroupFrame *gframe_combined_hists_panel1 = new TGGroupFrame(hframe_combined_hists_row1,"Combine hists:",kVerticalFrame);
    check_button_combined_hists_row1 = new TGCheckButton*[aNrGraphs];
    for (int i = 0; i < aNrGraphs; ++i)
    {
        std::ostringstream osst;
        osst << "ch_" << i;
        check_button_combined_hists_row1[i] = new TGCheckButton(gframe_combined_hists_panel1, osst.str().c_str(), i);
        check_button_combined_hists_row1[i]->SetState(kButtonDown);
        gframe_combined_hists_panel1->AddFrame(check_button_combined_hists_row1[i], fL_TL);
    }
    hframe_combined_hists_row1->AddFrame(gframe_combined_hists_panel1, fL_TL);

    fEcanvas_arr[12] = new TRootEmbeddedCanvas("Combined hist 1: total integral",hframe_combined_hists_row1,canvas_w,canvas_h);
    hframe_combined_hists_row1->AddFrame(fEcanvas_arr[12], fL_canvas);

    fEcanvas_arr[13] = new TRootEmbeddedCanvas("Combined hist 1: fast integral",hframe_combined_hists_row1,canvas_w,canvas_h);
    hframe_combined_hists_row1->AddFrame(fEcanvas_arr[13], fL_canvas);

    //row2
    TGHorizontalFrame *hframe_combined_hists_row2 = new TGHorizontalFrame(vframe_combined_hists,200,40);
    //panel
    TGGroupFrame *gframe_combined_hists_panel2 = new TGGroupFrame(hframe_combined_hists_row2,"Combine hists:",kVerticalFrame);
    check_button_combined_hists_row2 = new TGCheckButton*[aNrGraphs];
    for (int i = 0; i < aNrGraphs; ++i)
    {
        std::ostringstream osst;
        osst << "ch_" << i;
        check_button_combined_hists_row2[i] = new TGCheckButton(gframe_combined_hists_panel2, osst.str().c_str(), i + aNrGraphs);
        check_button_combined_hists_row2[i]->SetState(kButtonDown);
        gframe_combined_hists_panel2->AddFrame(check_button_combined_hists_row2[i], fL_TL);
    }
    hframe_combined_hists_row2->AddFrame(gframe_combined_hists_panel2, fL_TL);

    fEcanvas_arr[14] = new TRootEmbeddedCanvas("Combined hist 2: total integral",hframe_combined_hists_row2,canvas_w,canvas_h);
    hframe_combined_hists_row2->AddFrame(fEcanvas_arr[14], fL_canvas);

    fEcanvas_arr[15] = new TRootEmbeddedCanvas("Combined hist 2: fast integral",hframe_combined_hists_row2,canvas_w,canvas_h);
    hframe_combined_hists_row2->AddFrame(fEcanvas_arr[15], fL_canvas);


    vframe_combined_hists->AddFrame(hframe_combined_hists_row1, fL_canvas);
    vframe_combined_hists->AddFrame(hframe_combined_hists_row2, fL_canvas);
    tab_frame->AddFrame(vframe_combined_hists, fL_canvas);
    //----------------------------- end Combined hists tab






    //----------------------------- Evergy_information tab
    //Create Tab
    tab_frame = fTab->AddTab("Energy info");

    TGHorizontalFrame *hframe_tab2 = new TGHorizontalFrame(tab_frame,200,40);

    fEcanvas_arr[16] = new TRootEmbeddedCanvas("Ecanvas_evergy_spectrum",hframe_tab2,canvas_w,canvas_h);
    hframe_tab2->AddFrame(fEcanvas_arr[16], fL_canvas);

    fEcanvas_arr[17] = new TRootEmbeddedCanvas("Ecanvas_Npe_vs_time",hframe_tab2,canvas_w,canvas_h);
    hframe_tab2->AddFrame(fEcanvas_arr[17], fL_canvas);


    tab_frame->AddFrame(hframe_tab2, fL_canvas);
    //----------------------------- end Evergy_information tab

    fTab->Connect("Selected(Int_t)", "MyMainFrame", this, "fTab_selected(Int_t)");


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

    //Enable/Disable panels
    EnableFrame(tab_frame_cp_hist_opt, kFALSE);
    EnableFrame(tab_frame_cp_hanalysis, kFALSE);

    //Set params
    n_points = 1000;
    is_redraw_hist = true;
    global_counter = 0;
    //income_counter = 0;
    is_can_draw_now = true;
    desirable_update_rate = 1;
}

MyMainFrame::~MyMainFrame()
{
   // Clean up used widgets: frames, buttons, layout hints
   fMain->Cleanup();
   delete fMain;
}


void MyMainFrame::InitGraphs()
{
    //graphs
    graphs = new TGraph*[ aNrGraphs ];
    for (int i = 0; i < aNrGraphs; i++)
    {
       graphs[i] = new TGraph( n_points );
       for(Int_t j = 0; j < n_points; j++)
       {
          graphs[i]->SetPoint(j, j, i);
       }

       graphs[i]->SetMarkerColor(2);
       //graphs[i]->SetLineColor(2);
       graphs[i]->SetMarkerStyle(21);
       graphs[i]->SetMarkerSize(0.3);
    }


    //---------------- gate lines
    Double_t x_baseline_gate_from = 100;
    Double_t x_baseline_gate_to = 200;
    Double_t x_signal_gate_from = 300;
    Double_t x_signal_gate_fast_to = 400;
    Double_t x_signal_gate_to = 500;

    line_baseline_gate_from = new TLine*[ aNrGraphs ];
    line_baseline_gate_to = new TLine*[ aNrGraphs ];
    line_signal_gate_from = new TLine*[ aNrGraphs ];
    line_signal_gate_fast_to = new TLine*[ aNrGraphs ];
    line_signal_gate_to = new TLine*[ aNrGraphs ];
    for (int i = 0; i < aNrGraphs; i++)
    {
        Double_t y_min = graphs[i]->GetYaxis()->GetXmin();
        Double_t y_max = graphs[i]->GetYaxis()->GetXmax();

        line_baseline_gate_from[i] = new TLine(x_baseline_gate_from, y_min, x_baseline_gate_from, y_max);
        line_baseline_gate_from[i]->SetLineColor(kRed);

        line_baseline_gate_to[i] = new TLine(x_baseline_gate_to, y_min, x_baseline_gate_to, y_max);
        line_baseline_gate_to[i]->SetLineColor(kGreen);

        line_signal_gate_from[i] = new TLine(x_signal_gate_from, y_min, x_signal_gate_from, y_max);
        line_signal_gate_from[i]->SetLineColor(kBlue);

        line_signal_gate_fast_to[i] = new TLine(x_signal_gate_fast_to, y_min, x_signal_gate_fast_to, y_max);
        line_signal_gate_fast_to[i]->SetLineColor(kOrange);

        line_signal_gate_to[i] = new TLine(x_signal_gate_to, y_min, x_signal_gate_to, y_max);
        line_signal_gate_to[i]->SetLineColor(kMagenta);
    }
    //---------------- end gate lines




    //gr_mean
    const int n_points_gr_mean = 200;
    xv_gr_mean.resize(n_points_gr_mean);
    for (int i = 0; i < n_points_gr_mean; ++i)
    {
        xv_gr_mean[i] = i;
    }
    yv_gr_mean.resize(n_points_gr_mean);
    gr_mean = new TGraph(n_points_gr_mean, &xv_gr_mean[0], &yv_gr_mean[0]);
    gr_mean->SetLineColor(kGreen);
    gr_mean->SetTitle("<N_pe> vs time");
    gr_mean->SetFillColor(kGreen);

    //hists individual ch
    TRandom rnd;
    hists = new TH1F*[ aNrGraphs ];
    for (int i = 0; i < aNrGraphs; i++)
    {
        std::ostringstream oss;
        oss << "ch_" << i;

        hists[i] = new TH1F( oss.str().c_str(), oss.str().c_str(), hlimits_n_bins[i], -1, 1 );
        hists[i]->SetBit(TH1::kCanRebin);
        for(Int_t j = 0; j < n_points; j++)
        {
            hists[i]->Fill(rnd.Uniform(-0.5 + i, 0.5 + i));
        }
    }

    //combined hists
    hists_combined_hists = new TH1F*[4];
    for (int i = 0; i < 4; i++)
    {
        std::ostringstream oss;

        switch (i)
        {
        case 0:
            oss << "Combined hist 1: total integral";
            break;
        case 1:
            oss << "Combined hist 1: fast/total ratio";
            break;
        case 2:
            oss << "Combined hist 2: total integral";
            break;
        case 3:
            oss << "Combined hist 2: fast/total ratio";
            break;
        default:
            break;
        }

        hists_combined_hists[i] = new TH1F( oss.str().c_str(), oss.str().c_str(), hlimits_n_bins[i + aNrGraphs], -1, 1 );
        hists_combined_hists[i]->SetBit(TH1::kCanRebin);
        hists_combined_hists[i]->Fill(rnd.Uniform(-0.5 + i, 0.5 + i));
    }



    //hist
    //TH1::SetDefaultSumw2(kTRUE);
    summ_value_hist = 0;
    hist = new TH1F("h1", "N_pe histogram", 100, -1, 1);
    hist->SetBit(TH1::kCanRebin);//The histogram bit TH1::kCanRebin has been removed in root 6. I should add #ifdef
    for (int i = 0; i < 1000; ++i)
    {
        hist->Fill(rnd.Uniform(-1, 1));
    }


    //char y_axis_name[] = "Voltage [mV]";

    //TCanvas
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

            line_baseline_gate_from[i]->Draw();
            line_baseline_gate_to[i]->Draw();
            line_signal_gate_from[i]->Draw();
            line_signal_gate_fast_to[i]->Draw();
            line_signal_gate_to[i]->Draw();
        }
        else if (i >= 6 && i < 12)
        {
            hists[i-6]->Draw();
        }
        else if (i >= 12 && i < 16)
        {
            hists_combined_hists[i - 2 *aNrGraphs]->Draw();
        }
        else if (i == 16)
        {
            hist->Draw();
        }
        else if (i == 17)
        {
            gr_mean->Draw(/*"AB"*/);
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




