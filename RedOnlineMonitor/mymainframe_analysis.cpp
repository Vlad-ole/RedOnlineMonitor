#include "mymainframe.h"
#include "fithist.h"

void *MyMainFrame::AnalyzeHistsWorker(void *aPtr)
{
    MyMainFrame *p = (MyMainFrame*)aPtr;
    printf("You are in AnalyzeHistsWorker() (Thread %d) \n", syscall(__NR_gettid));

    {
        //pre set
        TThread::Lock();
        std::ostringstream osstr;
        osstr << p->GetCurrentTime() << "Analysis has been started";
        //    p->twStatus_label->AddLine(p->sst_status_label.str().c_str());
        //    p->twStatus_label->ShowBottom();
        //you can't change hist during analysis
        p->EnableFrame(p->tab_frame_cp_hist_opt, kFALSE);
        p->EnableFrame(p->tab_frame_cp_hanalysis, kFALSE);
        p->button_start->SetEnabled(kFALSE);
        TThread::UnLock();
        gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->AddLine(\"%s\");", (ULong_t)p->twStatus_label, osstr.str().c_str()));
        gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->ShowBottom();", (ULong_t)p->twStatus_label));
    }



    //analyze
    for (int i = 0; i < /*p->aNrGraphs*/ 1; ++i)
    {
        FitHist fit_hist(p->hists[i]);
        if( p->IsDownIsEnable(p->Chbt_hanalysis_auto_limits_checkb[i]).first )
        {
            fit_hist.FindPeaks(p->hanalysis_sigma[i]);
        }
        else
        {
            fit_hist.FindPeaks(p->hanalysis_sigma[i], p->hanalysis_lvalues[i], p->hanalysis_rvalues[i]);
        }

        Int_t index = i + p->aNrGraphs;
        p->aCanvas_arr[index]->Modified();
        p->aCanvas_arr[index]->Update();
    }
    //gSystem->Sleep(2000);


    {
        //post set
        TThread::Lock();
        p->EnableFrame(p->tab_frame_cp_hist_opt, kTRUE);
        p->EnableFrame(p->tab_frame_cp_hanalysis, kTRUE);
        p->button_start->SetEnabled(kTRUE);

        std::ostringstream osstr;
        osstr << p->GetCurrentTime() << "Analysis has been finished";
        //p->twStatus_label->AddLine(p->sst_status_label.str().c_str());
        //p->twStatus_label->ShowBottom();
        TThread::UnLock();
        gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->AddLine(\"%s\");", (ULong_t)p->twStatus_label, osstr.str().c_str()));
        gROOT->ProcessLine(Form("((TGTextView *)0x%lx)->ShowBottom();", (ULong_t)p->twStatus_label));
    }

}
