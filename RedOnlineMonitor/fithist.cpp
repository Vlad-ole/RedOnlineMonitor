#include "fithist.h"
#include "TPolyMarker.h"

using namespace std;

FitHist::FitHist(TH1F *hist)
{
    //h = (TH1F*)hist->Clone();
    h = hist;
    nbins = h->GetNbinsX();
    cout << "h->GetEntries() = " << h->GetEntries() << endl;


    s = new TSpectrum(100);
    nfound = 0;
    fPositionX.resize(nbins);
    fPositionY.resize(nbins);
}

FitHist::~FitHist()
{
    delete s;
}

void FitHist::FindPeaks(Double_t llimit, Double_t rlimit, Float_t sigma)
{
    printf("You are in FitHist::FindPeaks() (Thread %d) \n", syscall(__NR_gettid));



    //Double_t xmin     = -3;
    //Double_t xmax     = 10;

    Float_t source[nbins], dest[nbins];

    //Double_t fPositionX[100];
    //Double_t fPositionY[100];

    for (int i = 0; i < nbins; i++) source[i]=h->GetBinContent(i + 1);

    nfound = s->SearchHighRes(source, dest, nbins, sigma, 2, kTRUE, 3, kTRUE, 3);
    cout << "nfound = " << nfound << endl;



    //Get peaks positions
    Float_t *xpeaks = s->GetPositionX();
    for (int i = 0; i < nfound; i++)
    {
       Double_t a = xpeaks[i];
       Int_t bin = 1 + Int_t(a + 0.5);
       fPositionX[i] = h->GetBinCenter(bin);
       fPositionY[i] = h->GetBinContent(bin);
       cout << i << " " << fPositionX[i] << " " << fPositionY[i] << endl;
    }

    //Show peaks on the hist
    TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
    cout << "pm = " << pm << endl;
    if (pm)
    {
       h->GetListOfFunctions()->Remove(pm);
       delete pm;
    }
    pm = new TPolyMarker(nfound, &fPositionX[0], &fPositionY[0]);
    pm->SetMarkerStyle(23);
    pm->SetMarkerColor(kRed);
    pm->SetMarkerSize(1.3);
    h->GetListOfFunctions()->Add(pm);
}
