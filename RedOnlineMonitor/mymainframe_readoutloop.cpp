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
    vector<Double_t> integral_fast(p->aNrGraphs);
    vector<Double_t> integral_combined_hists(4);

    double total_integral;

    bool is_good_baseline_calc = true;
    bool is_good_integral_calc = true;
    bool is_first_error = true;

    TRandom3 rnd3;
    Double_t L = 3;
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
                Double_t amp = rnd3.PoissonD(L);
                for (int j = 0; j < p->n_points; ++j)
                {
                    Double_t val = rnd.Uniform(-10, 10) + baseline;
                    Double_t tmp_time = j * time_step;
                    if(tmp_time > 1.500 && tmp_time < 10.000)
                    {
                        //val -= 1000 * ( TMath::Gaus(tmp_time, 2000, 100, kTRUE) ) ;
                        val -= (amp) * 50 * TMath::Exp( - (tmp_time - t0) / tau_slow) /* /  tau_slow*/;
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
                //check time gates
                UInt_t point_baseline_gate_from = (p->time_baseline_gate_from / time_step);
                UInt_t point_baseline_gate_to = (p->time_baseline_gate_to / time_step);
                if( (point_baseline_gate_from >= point_baseline_gate_to) ||
                        (point_baseline_gate_to >= (p->n_points - 1)) ) // incorrect situation
                {
                    is_good_baseline_calc = false;
                    break;
                }
                UInt_t point_signal_gate_from = (p->time_signal_gate_from / time_step);
                UInt_t point_signal_gate_to = (p->time_signal_gate_to / time_step);
                if( (point_signal_gate_from >= point_signal_gate_to) ||
                        (point_signal_gate_to >= (p->n_points - 1)) ) // incorrect situation
                {
                    is_good_integral_calc = false;
                    break;
                }
                UInt_t point_signal_gate_fast_to = (p->time_signal_gate_fast_to / time_step);
                if( (point_signal_gate_from >= point_signal_gate_fast_to) ||
                       (point_signal_gate_fast_to >= (p->n_points - 1)) ) // incorrect situation
                {
                    is_good_integral_calc = false;
                    break;
                }

                is_good_baseline_calc = true;
                is_good_integral_calc = true;

                //calc baseline
                Double_t baseline_cacl = 0;
                // standart situation
                for (int j = point_baseline_gate_from; j < point_baseline_gate_to; ++j)
                {
                    baseline_cacl += yvv[i][j];
                }
                baseline_cacl /= (point_baseline_gate_to - point_baseline_gate_from);


                //calc integral
                integral[i] = 0;
                for (int j = point_signal_gate_from; j < point_signal_gate_to; ++j)
                {
                    integral[i] += (yvv[i][j] - baseline_cacl);
                }
                integral[i] *= time_step;
                //cout << "for ch_" << i <<  " integral = " << integral[i] << endl;//test
                //gSystem->Sleep(300);//test


                //calc fast integral
                integral_fast[i] = 0;
                for (int j = point_signal_gate_from; j < point_signal_gate_fast_to; ++j)
                {
                    integral_fast[i] += (yvv[i][j] - baseline_cacl);
                }
                integral_fast[i] *= time_step;
                //cout << "for ch_" << i <<  " integral_fast = " << integral_fast[i] << endl;//test
                //2gSystem->Sleep(300);//test


            }
            //cout << endl;//test

            total_integral = 0;
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                total_integral += integral[i];
            }



            //--------------- There is an error with twStatus_label here. I can't understand the exact reason
            if(p->is_can_draw_now && !is_good_baseline_calc && is_first_error)
            {
                is_first_error = false;
                std::ostringstream osstr;
                osstr << p->GetCurrentTime() << "Error! Too big or too small baseline gate";

                //variant1 - there are problems
                //p->twStatus_label->AddLine(osstr.str().c_str());
                //p->twStatus_label->ShowBottom();

                //variant2: gROOT->ProcessLine will call code in main thread - there are problems too
                gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->AddLine(\"%s\");", (ULong_t)p->twStatus_label, osstr.str().c_str()));
                gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->ShowBottom();", (ULong_t)p->twStatus_label));

                p->gframe_status_label->SetTextColor(p->pixel_t_red);
            }
            if(p->is_can_draw_now && !is_good_integral_calc && is_first_error)
            {
                is_first_error = false;
                std::ostringstream osstr;
                osstr << p->GetCurrentTime() << "Error! Too big or too small integral gate";

                gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->AddLine(\"%s\");", (ULong_t)p->twStatus_label, osstr.str().c_str()));
                gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->ShowBottom();", (ULong_t)p->twStatus_label));

                p->gframe_status_label->SetTextColor(p->pixel_t_red);
            }
            //---------------
            //let's minimize consequence of this error
            if(p->is_can_draw_now && is_good_baseline_calc && is_good_integral_calc && !is_first_error)
            {
                is_first_error = true;
                std::ostringstream osstr;
                osstr << p->GetCurrentTime() << "Baseline & integral gates are correct";

                gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->AddLine(\"%s\");", (ULong_t)p->twStatus_label, osstr.str().c_str()));
                gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->ShowBottom();", (ULong_t)p->twStatus_label));

                p->gframe_status_label->SetTextColor(p->pixel_t_green);
            }






            //================================================================================
            //Global mutex to avoid data race
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

            //gate lines
            if(p->is_can_draw_now)
            {
                for (int i = 0; i < p->aNrGraphs; ++i)
                {
                    Double_t y_min = p->graphs[i]->GetYaxis()->GetXmin();
                    Double_t y_max = p->graphs[i]->GetYaxis()->GetXmax();

                    p->line_baseline_gate_from[i]->SetX1(p->time_baseline_gate_from);
                    p->line_baseline_gate_from[i]->SetX2(p->time_baseline_gate_from);
                    p->line_baseline_gate_from[i]->SetY1(y_min);
                    p->line_baseline_gate_from[i]->SetY2(y_max);

                    p->line_baseline_gate_to[i]->SetX1(p->time_baseline_gate_to);
                    p->line_baseline_gate_to[i]->SetX2(p->time_baseline_gate_to);
                    p->line_baseline_gate_to[i]->SetY1(y_min);
                    p->line_baseline_gate_to[i]->SetY2(y_max);

                    p->line_signal_gate_from[i]->SetX1(p->time_signal_gate_from);
                    p->line_signal_gate_from[i]->SetX2(p->time_signal_gate_from);
                    p->line_signal_gate_from[i]->SetY1(y_min);
                    p->line_signal_gate_from[i]->SetY2(y_max);

                    p->line_signal_gate_fast_to[i]->SetX1(p->time_signal_gate_fast_to);
                    p->line_signal_gate_fast_to[i]->SetX2(p->time_signal_gate_fast_to);
                    p->line_signal_gate_fast_to[i]->SetY1(y_min);
                    p->line_signal_gate_fast_to[i]->SetY2(y_max);

                    p->line_signal_gate_to[i]->SetX1(p->time_signal_gate_to);
                    p->line_signal_gate_to[i]->SetX2(p->time_signal_gate_to);
                    p->line_signal_gate_to[i]->SetY1(y_min);
                    p->line_signal_gate_to[i]->SetY2(y_max);
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
                    p->hists[i]->GetXaxis()->SetLimits(p->hlimits_lvalues[i], p->hlimits_rvalues[i]);
                    p->hists[i]->GetXaxis()->SetRangeUser(p->hlimits_lvalues[i], p->hlimits_rvalues[i]);
                    p->hists[i]->GetXaxis()->Set(p->hlimits_n_bins[i], p->hlimits_lvalues[i], p->hlimits_rvalues[i]);

                    if (p->check_button_hlimits[i]->IsDown()) p->hists[i]->SetBit(TH1::kCanRebin, kTRUE);
                    else p->hists[i]->SetBit(TH1::kCanRebin, kFALSE);
                }
            }
            for (int i = 0; i < p->aNrGraphs; ++i)
            {
                p->hists[i]->Fill(integral[i]);
            }


            //combined hists
            if(p->is_redraw_hist)
            {
                for (int i = 0; i < 4; ++i)
                {
                    const int n_bins = p->hists_combined_hists[i]->GetNbinsX();
                    for (int j = 0; j < n_bins + 1; ++j)
                    {
                        p->hists_combined_hists[i]->SetBinContent(j, 0);
                    }
                    p->hists_combined_hists[i]->SetEntries(0);
                    const int index = i + p->aNrGraphs;
                    p->hists_combined_hists[i]->GetXaxis()->SetLimits(p->hlimits_lvalues[index], p->hlimits_rvalues[index]);
                    p->hists_combined_hists[i]->GetXaxis()->SetRangeUser(p->hlimits_lvalues[index], p->hlimits_rvalues[index]);
                    p->hists_combined_hists[i]->GetXaxis()->Set(p->hlimits_n_bins[index], p->hlimits_lvalues[index], p->hlimits_rvalues[index]);

                    if (p->check_button_hlimits[index]->IsDown()) p->hists_combined_hists[i]->SetBit(TH1::kCanRebin, kTRUE);
                    else p->hists_combined_hists[i]->SetBit(TH1::kCanRebin, kFALSE);
                }
            }
            for (int i = 0; i < 4; ++i)
            {
                integral_combined_hists[i] = 0;
                bool is_down;
                for (int j = 0; j < p->aNrGraphs; ++j)
                {
                    if(i < 2)
                        is_down = p->check_button_combined_hists_row1[j]->IsDown();
                    else
                        is_down = p->check_button_combined_hists_row2[j]->IsDown();

                    if( ((i==0) || (i==2)) && is_down)
                    {
                        integral_combined_hists[i] += integral[j];
                    }

                    if( ((i==1) || (i==3)) && is_down)
                    {
                        integral_combined_hists[i] += (integral_fast[j] / integral[j]);
                    }
                }
                p->hists_combined_hists[i]->Fill(integral_combined_hists[i]);
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
        }


    }
}
