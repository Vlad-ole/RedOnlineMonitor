#include "mymainframe.h"

void MyMainFrame::Clicked_start_button()
{

    if(is_start_button_activated)
    {
        button_start->SetBackgroundColor(pixel_t_red);
        is_start_button_activated = false;

        sst_status_label.str("");
        sst_status_label << GetCurrentTime() << "Program has been stopped";
        twStatus_label->AddLine(sst_status_label.str().c_str());
    }
    else
    {
        button_start->SetBackgroundColor(pixel_t_yellow);
        is_start_button_activated = true;

        sst_status_label.str("");
        sst_status_label << GetCurrentTime() << "Program is working";
        twStatus_label->AddLine(sst_status_label.str().c_str());
    }
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
}


void MyMainFrame::ChangeNEventsForAvr()
{
    n_events_for_avr = NEntr_n_events_for_avr->GetNumberEntry()->GetIntNumber();
    //cout << "n_events_for_avr =" << n_events_for_avr << endl;

}

void MyMainFrame::SetBaselineGateFrom()
{

    //cout << "time_baseline_gate_from =" << time_baseline_gate_from << endl;

    sst_status_label.str("");

    if ( IsGoodGateValues() )
    {
        time_baseline_gate_from = NEntr_baseline_gate_from->GetNumberEntry()->GetNumber();
        sst_status_label << GetCurrentTime() << "Time_from for baseline gate has been changed to: " << time_baseline_gate_from;
    }
    else
    {
        sst_status_label << GetCurrentTime() << "Sorry, I can't set such values, because:" ;
    }


    twStatus_label->AddLine(sst_status_label.str().c_str());
}

void MyMainFrame::SetBaselineGateTo()
{
    time_baseline_gate_to = NEntr_baseline_gate_to->GetNumberEntry()->GetNumber();
    //cout << "time_baseline_gate_to =" << time_baseline_gate_to << endl;

    sst_status_label.str("");
    sst_status_label << "Time_to for baseline gate has been changed to: " << time_baseline_gate_to;
    //lStatus_label->SetText(sst_status_label.str().c_str());
}

void MyMainFrame::SetSignalGateFrom()
{
    time_signal_gate_from = NEntr_signal_gate_from->GetNumberEntry()->GetNumber();
    //cout << "time_signal_gate_from =" << time_signal_gate_from << endl;

    sst_status_label.str("");
    sst_status_label << "Time_from for signal gate has been changed to: " << time_signal_gate_from;
    //lStatus_label->SetText(sst_status_label.str().c_str());
}

void MyMainFrame::SetSignalGateTo()
{
    time_signal_gate_to = NEntr_signal_gate_to->GetNumberEntry()->GetNumber();
    cout << "time_signal_gate_to =" << time_signal_gate_to << endl;
}
void MyMainFrame::SetSignalGateFastTo()
{
    time_signal_gate_fast_to = NEntr_signal_gate_fast_to->GetNumberEntry()->GetNumber();
    cout << "time_signal_gate_fast_to =" << time_signal_gate_fast_to << endl;
}

std::string MyMainFrame::GetCurrentTime()
{
    std::ostringstream osst;
    tdatime.Set();
    osst << tdatime.GetHour() << "h:" << tdatime.GetMinute() << "m:" << tdatime.GetSecond() << "s; ";

    std::string str = osst.str();
    return str;
}

//has not been finished!
bool MyMainFrame::IsGoodGateValues()
{
    bool is_good_gate_value = true;

    Double_t time_baseline_gate_from_tmp = NEntr_baseline_gate_from->GetNumberEntry()->GetNumber();
    Double_t time_baseline_gate_to_tmp = NEntr_baseline_gate_to->GetNumberEntry()->GetNumber();

    if(time_baseline_gate_from_tmp > time_baseline_gate_to_tmp)
    {
        is_good_gate_value = false;
    }

//    if(time_signal_gate_from > time_signal_gate_to)
//    {
//        is_good_gate_value = false;
//    }

//    if(time_signal_gate_from > time_signal_gate_fast_to)
//    {
//        is_good_gate_value = false;
//    }

//    if(time_signal_gate_fast_to > time_signal_gate_fast_to)
//    {
//        is_good_gate_value = false;
//    }

//    if( (time_baseline_gate_from > time_signal_gate_from) && (time_baseline_gate_from < time_signal_gate_to) )
//    {
//        is_good_gate_value = false;
//    }

//    if ( (time_baseline_gate_to > time_signal_gate_from) && (time_baseline_gate_to < time_signal_gate_to) )
//    {
//        is_good_gate_value = false;
//    }


//    time_baseline_gate_from
//    time_baseline_gate_to
//    time_signal_gate_from
//    time_signal_gate_to
//    time_signal_gate_fast_to

    return is_good_gate_value;
}
