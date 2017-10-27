#include "mymainframe.h"
#include "fithist.h"

void *MyMainFrame::AnalyzeHistsWorker(void *aPtr)
{
    MyMainFrame *p = (MyMainFrame*)aPtr;
    printf("You are in AnalyzeHistsWorker() (Thread %d) \n", syscall(__NR_gettid));

    //pre set
    TThread::Lock();
    p->sst_status_label.str("");
    p->sst_status_label << p->GetCurrentTime() << "Analysis has been started";
    p->twStatus_label->AddLine(p->sst_status_label.str().c_str());
    p->twStatus_label->ShowBottom();

    //you can't change hist during analysis
    p->EnableFrame(p->tab_frame_cp_hist_opt, kFALSE);
    p->EnableFrame(p->tab_frame_cp_hanalysis, kFALSE);
    p->button_start->SetEnabled(kFALSE);
    TThread::UnLock();




    for (int i = 0; i < p->aNrGraphs; ++i)
    {
        FitHist fit_hist(p->hists[i]);
        fit_hist.FindPeaks(p->hanalysis_lvalues[i], p->hanalysis_rvalues[i], p->hanalysis_sigma[i]);

        Int_t index = i + p->aNrGraphs;
        p->aCanvas_arr[index]->Modified();
        p->aCanvas_arr[index]->Update();
    }


    //gSystem->Sleep(2000);






    //post set
    TThread::Lock();
    p->EnableFrame(p->tab_frame_cp_hist_opt, kTRUE);
    p->EnableFrame(p->tab_frame_cp_hanalysis, kTRUE);
    p->button_start->SetEnabled(kTRUE);

    p->sst_status_label.str("");
    p->sst_status_label << p->GetCurrentTime() << "Analysis has been finished";
    p->twStatus_label->AddLine(p->sst_status_label.str().c_str());
    p->twStatus_label->ShowBottom();
    TThread::UnLock();

}
