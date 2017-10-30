#ifndef FITHIST_H
#define FITHIST_H

//c++
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

//root cern general
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TRandom.h>
#include "TAxis.h"
#include "TStopwatch.h"
#include "TMath.h"
#include "TLine.h"
#include "TDatime.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TSpectrum.h"

//to show thread_id for linux systems
#include <sys/types.h>
#include <sys/syscall.h>

template <class T1, class T2, class Pred = std::less<T2> >
struct sort_pair_first
{
    bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right)
    {
        Pred p;
        return p(left.first, right.first);
    }
};

struct FitParameters
{
    //hist param
    Double_t nEntries;
    Int_t nbins_initial;
    Int_t nbins_user_limits;
    Double_t llimit;
    Double_t rlimit;


    //peak founder params
    Float_t sigma;
    Int_t nfound;
    std::vector<std::pair <Double_t, Double_t> > peak_positions_xy;

    //analysis of hist
    Double_t spe_value;
    Double_t spe_value_err;
    Double_t pedestal_shift;
    Double_t pedestal_shift_err;

    //Vinogradov fit
    Double_t p;
    Double_t L;
};

class FitHist
{
public:
    FitHist(TH1F *hist);
    ~FitHist();
    void FindPeaks(Float_t sigma, Double_t llimit = 0, Double_t rlimit = 0); /*unit of sigma is number of bins (can be real number) */
    void FindSpe(bool is_ped_on_the_left);
    FitParameters GetFitParameters();
    void ShowFitParameters();
private:
    TH1F *h;
    Int_t lbin;
    Int_t rbin;

    FitParameters params;

    TSpectrum *s;
    //Int_t nfound;
    //std::vector<Double_t> fPositionX;
    //std::vector<Double_t> fPositionY;
};

#endif // FITHIST_H
