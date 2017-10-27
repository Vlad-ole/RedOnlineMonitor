#ifndef FITHIST_H
#define FITHIST_H

//c++
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

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

class FitHist
{
public:
    FitHist(TH1F *hist);
    ~FitHist();
    void FindPeaks(Float_t sigma, Double_t llimit = 0, Double_t rlimit = 0); /*unit of sigma is number of bins (can be real number) */
private:
    TH1F *h;
    Int_t nbins;

    TSpectrum *s;
    Int_t nfound;
    std::vector<Double_t> fPositionX;
    std::vector<Double_t> fPositionY;
};

#endif // FITHIST_H
