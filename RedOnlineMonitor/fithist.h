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
#include "TH1F.h"
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

//this project
#include "vinogradovpdf.h"

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

    //Vinogradov analytically
    Double_t p_01_peaks;
    Double_t L_01_peaks;

    //Vinogradov fit
    Double_t p_fit_discrete;
    Double_t L_fit_discrete;
    Double_t p_fit_discrete_err;
    Double_t L_fit_discrete_err;
    Double_t chi2_per_ndf_fit_discrete;
};

class FitHist
{
public:
    FitHist(TH1F *hist, bool is_ped_on_the_left);
    ~FitHist();
    void FindPeaks(Float_t sigma, Double_t llimit = 0, Double_t rlimit = 0); /*unit of sigma is number of bins (can be real number) */
    void FindSpe();
    FitParameters GetFitParameters();
    void ShowFitParameters();
    void GetLP();
private:
    TH1F *h;
    Int_t lbin;
    Int_t rbin;
    Bool_t is_ped_on_the_left;

    FitParameters params;

    TSpectrum *s;
};

#endif // FITHIST_H
