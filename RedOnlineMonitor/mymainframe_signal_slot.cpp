#include "mymainframe.h"

void MyMainFrame::Clicked_start_button()
{
    sst_status_label.str("");

    if(is_start_button_activated)
    {
        button_start->SetBackgroundColor(pixel_t_red);
        is_start_button_activated = false;


        sst_status_label << GetCurrentTime() << "Program has been stopped";
        twStatus_label->AddLine(sst_status_label.str().c_str());
    }
    else
    {
        button_start->SetBackgroundColor(pixel_t_yellow);
        is_start_button_activated = true;

        sst_status_label << GetCurrentTime() << "Program is working";
        twStatus_label->AddLine(sst_status_label.str().c_str());
    }

    twStatus_label->ShowBottom();
}

void MyMainFrame::RedrawHist()
{
    is_redraw_hist = true;
}

void MyMainFrame::SetDesirableUpdateRate()
{
    desirable_update_rate = NEntr_update_time->GetNumberEntry()->GetNumber();
    //cout << "desirable_update_rate =" << desirable_update_rate << endl;

    sst_status_label.str("");
    sst_status_label << GetCurrentTime() << "Desirable update rate has been changed to: " << desirable_update_rate;
    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}


void MyMainFrame::ChangeNEventsForAvr()
{
    n_events_for_avr = NEntr_n_events_for_avr->GetNumberEntry()->GetIntNumber();
    //cout << "n_events_for_avr =" << n_events_for_avr << endl;

}

void MyMainFrame::SetBaselineGateFrom()
{
    sst_status_label.str("");
    std:string result = IsGoodBaselineGateValues();

    if ( result.compare("good values") == 0 )
    {
        time_baseline_gate_from = NEntr_baseline_gate_from->GetNumberEntry()->GetNumber();
        sst_status_label << GetCurrentTime() << "Time_from for baseline gate has been changed to: " << time_baseline_gate_from;
    }
    else
    {
        sst_status_label << GetCurrentTime() << "Sorry, I can't set such values, because: " << result ;
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}

void MyMainFrame::SetBaselineGateTo()
{
    sst_status_label.str("");
    std:string result = IsGoodBaselineGateValues();

    if ( result.compare("good values") == 0 )
    {
        time_baseline_gate_to = NEntr_baseline_gate_to->GetNumberEntry()->GetNumber();
        sst_status_label << GetCurrentTime() << "Time_to for baseline gate has been changed to: " << time_baseline_gate_to;
    }
    else
    {
        sst_status_label << GetCurrentTime() << "Sorry, I can't set such values, because: " << result ;
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();

}

void MyMainFrame::SetSignalGateFrom()
{
    sst_status_label.str("");
    std:string result = IsGoodSignalGateValues();

    if ( result.compare("good values") == 0 )
    {
        time_signal_gate_from = NEntr_signal_gate_from->GetNumberEntry()->GetNumber();
        sst_status_label << GetCurrentTime() << "Time_from for signal gate has been changed to: " << time_signal_gate_from;
    }
    else
    {
        sst_status_label << GetCurrentTime() << "Sorry, I can't set such values, because: " << result ;
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}

void MyMainFrame::SetSignalGateTo()
{
    sst_status_label.str("");
    std:string result = IsGoodSignalGateValues();

    if ( result.compare("good values") == 0 )
    {
        time_signal_gate_to = NEntr_signal_gate_to->GetNumberEntry()->GetNumber();
        sst_status_label << GetCurrentTime()  << "Time_to for signal gate has been changed to: " << time_signal_gate_to;
    }
    else
    {
        sst_status_label << GetCurrentTime() << "Sorry, I can't set such values, because: " << result ;
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();

}

void MyMainFrame::SetSignalGateFastTo()
{
    sst_status_label.str("");
    std:string result = IsGoodSignalGateValues();

    if ( result.compare("good values") == 0 )
    {
        time_signal_gate_fast_to = NEntr_signal_gate_fast_to->GetNumberEntry()->GetNumber();
        sst_status_label << GetCurrentTime() << "Time_to for signal fast gate has been changed to: " << time_signal_gate_fast_to;
    }
    else
    {
        sst_status_label << GetCurrentTime() << "Sorry, I can't set such values, because: " << result ;
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();

}

std::string MyMainFrame::GetCurrentTime()
{
    std::ostringstream osst;
    TDatime tdatime;
    //tdatime.Set();
    osst << twStatus_label->ReturnLineCount() << "; ";
    osst << tdatime.GetHour() << "h:" << tdatime.GetMinute() << "m:" << tdatime.GetSecond() << "s; ";

    std::string str = osst.str();
    return str;
}


std::string MyMainFrame::IsGoodBaselineGateValues()
{
    std::string str_status = "good values";

    Double_t time_baseline_gate_from_tmp = NEntr_baseline_gate_from->GetNumberEntry()->GetNumber();
    Double_t time_baseline_gate_to_tmp = NEntr_baseline_gate_to->GetNumberEntry()->GetNumber();

    if(time_baseline_gate_from_tmp >= time_baseline_gate_to_tmp)
    {
        str_status.clear();
        str_status = "time_baseline_gate_from >= time_baseline_gate_to";
    }

    return str_status;
}

std::string MyMainFrame::IsGoodSignalGateValues()
{
    std::string str_status = "good values";

    Double_t time_signal_gate_from_tmp = NEntr_signal_gate_from->GetNumberEntry()->GetNumber();
    Double_t time_signal_gate_to_tmp = NEntr_signal_gate_to->GetNumberEntry()->GetNumber();
    Double_t time_signal_gate_fast_to_tmp = NEntr_signal_gate_fast_to->GetNumberEntry()->GetNumber();

    if(time_signal_gate_from_tmp >= time_signal_gate_to_tmp)
    {
        str_status.clear();
        str_status = "time_signal_gate_from >= time_signal_gate_to";
    }

    if(time_signal_gate_from_tmp > time_signal_gate_fast_to_tmp) // they can equal and it will not be erro
    {
        str_status.clear();
        str_status = "time_signal_gate_from > time_signal_gate_fast_to";
    }

    if(time_signal_gate_fast_to_tmp > time_signal_gate_to_tmp) // they can equal and it will not be error
    {
        str_status.clear();
        str_status = "time_signal_gate_fast_to > time_signal_gate_to";
    }


    return str_status;
}

void MyMainFrame::SetHistLimits()
{
    sst_status_label.str("");
    bool is_good_values = true;

    const UInt_t n = hlimits_lvalues.size();
    std::vector<Double_t> hlimits_lvalues_tmp(n);
    std::vector<Double_t> hlimits_rvalues_tmp(n);

    for (int i = 0; i < n; ++i)
    {
        hlimits_lvalues_tmp[i] = NEntr_hframe_cp_hist_l_limits[i]->GetNumberEntry()->GetNumber();
        hlimits_rvalues_tmp[i] = NEntr_hframe_cp_hist_r_limits[i]->GetNumberEntry()->GetNumber();
    }

    for (int i = 0; i < n; ++i)
    {
        if(hlimits_lvalues_tmp[i] >= hlimits_rvalues_tmp[i])
        {
            sst_status_label << GetCurrentTime() << "Incorrect hist limits values for " <<  *(hlimits_labels[i]->GetText());
            is_good_values = false;
            break;
        }
    }

    if ( is_good_values )
    {
        for (int i = 0; i < n; ++i)
        {
            hlimits_lvalues[i] = hlimits_lvalues_tmp[i];
            hlimits_rvalues[i] = hlimits_rvalues_tmp[i];
        }

        sst_status_label << GetCurrentTime() << "Hist limits has been changed correctly";
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}

void MyMainFrame::SetHistNBins()
{
    sst_status_label.str("");

    const UInt_t n = hlimits_n_bins.size();
    for (int i = 0; i < n; ++i)
    {
        hlimits_n_bins[i] = NEntr_hframe_cp_hist_n_bins[i]->GetNumberEntry()->GetNumber();
    }

    sst_status_label << GetCurrentTime() << "N_bins has been changed";

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}

void MyMainFrame::AnalyzeHistsSlot()
{
    //Threads
    slave2_thread = new TThread("slave2_thread", (void(*) (void *)) AnalyzeHistsWorker, (void*) this);
    slave2_thread->Run();
}

void *MyMainFrame::AnalyzeHistsWorker(void *aPtr)
{
    MyMainFrame *p = (MyMainFrame*)aPtr;
    printf("You are in AnalyzeHistsWorker() (Thread %d) \n", syscall(__NR_gettid));


}

void MyMainFrame::SetAnalysisHistLimits()
{
    sst_status_label.str("");
    bool is_good_values = true;

    const UInt_t n = hanalysis_lvalues.size();
    std::vector<Double_t> hanalysis_lvalues_tmp(n);
    std::vector<Double_t> hanalysis_rvalues_tmp(n);

    for (int i = 0; i < n; ++i)
    {
        hanalysis_lvalues_tmp[i] = NEntr_hanalysis_lvalues[i]->GetNumberEntry()->GetNumber();
        hanalysis_rvalues_tmp[i] = NEntr_hanalysis_rvalues[i]->GetNumberEntry()->GetNumber();
    }

    for (int i = 0; i < n; ++i)
    {
        if(hanalysis_lvalues_tmp[i] >= hanalysis_rvalues_tmp[i])
        {
            sst_status_label << GetCurrentTime() << "Incorrect hist analysis gate values for " <<  *(hlimits_labels[i]->GetText());
            is_good_values = false;
            break;
        }
    }

    if ( is_good_values )
    {
        for (int i = 0; i < n; ++i)
        {
            hanalysis_lvalues[i] = hanalysis_lvalues_tmp[i];
            hanalysis_rvalues[i] = hanalysis_rvalues_tmp[i];
        }

        sst_status_label << GetCurrentTime() << "Hist analysis gate values has been changed correctly";
    }

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}

void MyMainFrame::SetAnalysisHistSigma()
{
    sst_status_label.str("");

    const UInt_t n = hanalysis_sigma.size();
    for (int i = 0; i < n; ++i)
    {
        hanalysis_sigma[i] = NEntr_hanalysis_sigma[i]->GetNumberEntry()->GetNumber();
    }

    sst_status_label << GetCurrentTime() << "Sigma has been changed";

    twStatus_label->AddLine(sst_status_label.str().c_str());
    twStatus_label->ShowBottom();
}

void MyMainFrame::fTab_cp_hist_selected(Int_t val)
{
    cout << "current cp_hist tab = " << val << endl;
}

void MyMainFrame::fTab_selected(Int_t val)
{
    cout << "current canvas tab = " << val << endl;
}

void MyMainFrame::EnableFrame(TGCompositeFrame *frame, Bool_t is_enabled)
{
    //cout << endl;
    EnableListRecursive(frame->GetList(), is_enabled);
}

//I did it!
void MyMainFrame::EnableListRecursive(TList *list, Bool_t is_enabled)
{
    TGFrameElement *el;
    TIter next( list );
    //cout << "list->GetSize()" << list->GetSize() << endl;

    while ( ( el = (TGFrameElement *) next() ) )
    {
        if (el->fFrame->InheritsFrom("TGTextButton") )
        {
            ((TGTextButton *)el->fFrame)->SetEnabled(is_enabled);
        }
        else if (el->fFrame->InheritsFrom("TGNumberEntry") )
        {
            ((TGNumberEntry *)el->fFrame)->SetState(is_enabled);
        }
        else if(el->fFrame->InheritsFrom("TGCompositeFrame"))
        {
            EnableListRecursive( ((TGGroupFrame *)el->fFrame)->GetList(), is_enabled);
        }
    }
}




