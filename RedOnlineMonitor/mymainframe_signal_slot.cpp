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


