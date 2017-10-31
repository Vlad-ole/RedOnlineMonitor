#ifndef VINOGRADOVPDF_H
#define VINOGRADOVPDF_H

#include "TApplication.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVirtualFitter.h"
#include "TTree.h"
#include "TChain.h"
#include "TList.h"
#include "TFile.h"

//to show thread_id for linux systems
#include <sys/types.h>
#include <sys/syscall.h>

#include "Rtypes.h"

#include <iostream>

//I decided to create new small class, because there are many ways to improve this code

class VinogradovPDF
{
public:
    VinogradovPDF();
    Double_t GetVinogradovPDFValue(UInt_t k, Double_t p, Double_t L);
    Double_t GetBik(UInt_t i, UInt_t k);
    Double_t operator() (Double_t *x, Double_t *p);
private:
    ULong64_t counter;
};

#endif // VINOGRADOVPDF_H
