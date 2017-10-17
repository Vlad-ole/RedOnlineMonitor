#include "mymainframe.h"

void *MyMainFrame::ReadoutLoop(void *aPtr)
{
    //the whole func ReadoutLoop will be in slave thread
    printf("You are in MyMainFrame::ReadoutLoop() (Thread %d) \n", syscall(__NR_gettid));

    MyMainFrame *p = (MyMainFrame*)aPtr;

    TStopwatch t_update_rate;
    Double_t real_update_time = 10;//in sec

    TStopwatch t_income_rate;
    Double_t accumulated_income_time = 0;
    ULong64_t income_counter = 0;

    //----------- prepare local data
    TRandom rnd;
    const int baseline = 4000;
    const double time_step = 0.010;//ns
    vector<Float_t> xv(p->n_points);
    for (int i = 0; i < p->n_points; ++i)
    {
        xv[i] = i * time_step;
    }

    vector< vector<Float_t> > yvv;
    yvv.resize(p->aNrGraphs);
    for (int i = 0; i < p->aNrGraphs; ++i)
    {
        yvv[i].resize(p->n_points);
    }

    vector<Double_t> integral(p->aNrGraphs);

    double total_integral;
    //-----------


    while(1)
    {
        if (p->is_start_button_activated)
        {
            t_update_rate.Start();
            p->global_counter++;


            //is_can_draw_now
            if(real_update_time > (1.0 / p->desirable_update_rate) )
            {
                p->is_can_draw_now = true;
                //real_update_time = 0;
            }
            else
            {
                p->is_can_draw_now = false;
            }


            //GetData
            t_income_rate.Start();

            const double tau_slow = 1.700;
            const double t0 = 1.500;
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                for (int j = 0; j < p->n_points; ++j)
                {
                    Double_t val = rnd.Uniform(-10, 10) + baseline;
                    Double_t tmp_time = j * time_step;
                    if(tmp_time > 1.500 && tmp_time < 10.000)
                    {
                        //val -= 1000 * ( TMath::Gaus(tmp_time, 2000, 100, kTRUE) ) ;
                        val -= 50 * TMath::Exp( - (tmp_time - t0) / tau_slow) /* /  tau_slow*/;
                    }

                    yvv[i][j] = val;
                }
            }
            gSystem->Sleep(5);//test
            t_income_rate.Stop();
            accumulated_income_time += t_income_rate.RealTime();
            income_counter++;

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


            //================================================================================
            //Global mutex to avoid data race (it is not so important in my case, but it is better do not remove mutex)
            TThread::Lock();


            //graphs
            if(p->is_can_draw_now)
            {
                for (int i = 0; i < p->aNrGraphs; ++i)
                {
                    for (int j = 0; j < p->n_points; ++j)
                    {
                        p->graphs[i]->SetPoint(j, xv[j], yvv[i][j]);
                    }
                }
            }


            //hists
            if(p->is_redraw_hist)
            {
                for (int i = 0; i < p->aNrGraphs; ++i)
                {
                    const int n_bins = p->hists[i]->GetNbinsX();
                    for (int j = 0; j < n_bins + 1; ++j)
                    {
                        p->hists[i]->SetBinContent(j, 0);
                    }
                    p->hists[i]->SetEntries(0);
                }
            }
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                p->hists[i]->Fill(integral[i]);
            }

            //hist
            if(p->is_can_draw_now) //you should decide what to do in this case: there are several variants
            {
                if(p->is_redraw_hist)
                {
                    const int n_bins = p->hist->GetNbinsX();
                    for (int i = 0; i < n_bins + 1; ++i)
                    {
                        p->hist->SetBinContent(i, 0);
                    }
                    p->hist->SetEntries(0);
                    p->is_redraw_hist = false;
                    p->summ_value_hist = 0;
                }
            }
            p->hist->Fill(total_integral);

            //gr_mean
            //if(global_counter % n_events_for_avr == 0) p->summ_value_hist = 0;
            p->summ_value_hist += total_integral;
            Double_t mean_value_hist = p->summ_value_hist / p->hist->GetEntries();

//            //shit right
//            p->yv_gr_mean.pop_back();
//            p->yv_gr_mean.insert(p->yv_gr_mean.begin(), mean_value_hist);
            //shift left
            p->yv_gr_mean.erase(p->yv_gr_mean.begin());
            p->yv_gr_mean.insert( p->yv_gr_mean.end(), mean_value_hist );

            if(p->is_can_draw_now)
            {
                for (int j = 0; j < p->yv_gr_mean.size(); ++j)
                {
                    p->gr_mean->SetPoint(j, p->xv_gr_mean[j], p->yv_gr_mean[j]);
                }
            }


            //canvases
            if(p->is_can_draw_now)
            {
                for (int i = 0; i < p->n_canvases; ++i)
                {
                    p->aCanvas_arr[i] = p->fEcanvas_arr[i]->GetCanvas();
                    p->aCanvas_arr[i]->Modified();
                    p->aCanvas_arr[i]->Update();
                }
            }



            //rate
            if(p->is_can_draw_now)
            {
                //income rate
                double rate = income_counter / accumulated_income_time;
                std::ostringstream strs_income_rate;
                strs_income_rate << std::setprecision(4) << rate;
                std::string str_income_rate = strs_income_rate.str();

                p->fLabel_income_rate->SetText( str_income_rate.c_str()  );
                p->gframe_cp_income_rate->Layout(); // Layout() method will redraw the label
                income_counter = 0;
                accumulated_income_time = 0;

                //update rate
                std::ostringstream strs_update_rate;
                //Double_t tmp_update_rate = (t_update_rate.RealTime()) > 0 ? (1 / t_update_rate.RealTime()) : 0;
                Double_t tmp_update_rate = 1 / real_update_time;
                strs_update_rate << std::setprecision(4) << tmp_update_rate;
                std::string str_update_rate = strs_update_rate.str();

                p->fLabel_update_rate->SetText( str_update_rate.c_str()  );
                p->gframe_cp_update_rate->Layout(); // Layout() method will redraw the label

                real_update_time = 0;
            }

            t_update_rate.Stop();
            real_update_time += t_update_rate.RealTime();

            TThread::UnLock();
            //================================================================================


        }
        else
        {
            //Wait permission to read data
            gSystem->Sleep(300);

//            TThread::Lock();
//            p->fLabel_income_rate->SetText( "No input."  );
//            p->fLabel_update_rate->SetText( "No input."  );
//            TThread::UnLock();
        }


    }
}
