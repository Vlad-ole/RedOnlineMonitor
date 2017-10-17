#include "mymainframe.h"

void MyMainFrame::Clicked_start_button()
{

    if(is_start_button_activated)
    {
        button_start->SetBackgroundColor(pixel_t_red);
        is_start_button_activated = false;
    }
    else
    {
        button_start->SetBackgroundColor(pixel_t_yellow);
        is_start_button_activated = true;

    }
}

void MyMainFrame::RedrawHist()
{
    is_redraw_hist = true;
}

void MyMainFrame::SetDesirableUpdateRate()
{
    desirable_update_rate = NEntr_update_time->GetNumberEntry()->GetNumber();
    cout << "desirable_update_rate =" << desirable_update_rate << endl;
}


void MyMainFrame::ChangeNEventsForAvr()
{
    n_events_for_avr = NEntr_n_events_for_avr->GetNumberEntry()->GetIntNumber();
    cout << "n_events_for_avr =" << n_events_for_avr << endl;
}

void MyMainFrame::SetBaselineGateFrom()
{
    time_baseline_gate_from = NEntr_baseline_gate_from->GetNumberEntry()->GetNumber();
    cout << "time_baseline_gate_from =" << time_baseline_gate_from << endl;
}

void MyMainFrame::SetBaselineGateTo()
{
    time_baseline_gate_to = NEntr_baseline_gate_to->GetNumberEntry()->GetNumber();
    cout << "time_baseline_gate_to =" << time_baseline_gate_to << endl;
}

void MyMainFrame::SetSignalGateFrom()
{
    time_signal_gate_from = NEntr_signal_gate_from->GetNumberEntry()->GetNumber();
    cout << "time_signal_gate_from =" << time_signal_gate_from << endl;
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
