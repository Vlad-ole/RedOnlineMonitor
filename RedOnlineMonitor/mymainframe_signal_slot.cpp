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

}

void MyMainFrame::SetBaselineGateTo()
{

}

void MyMainFrame::SetSignalGateFrom()
{

}

void MyMainFrame::SetSignalGateTo()
{

}
void MyMainFrame::SetSignalFastTo()
{

}
