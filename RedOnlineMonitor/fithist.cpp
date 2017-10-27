#include "fithist.h"
#include "TPolyMarker.h"

using namespace std;

FitHist::FitHist(TH1F *hist)
{
    //h = (TH1F*)hist->Clone();
    h = hist;
    //nbins = h->GetNbinsX();
    cout << "h->GetEntries() = " << h->GetEntries() << endl;


    s = new TSpectrum(100);
    nfound = 0;
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


    //use the whole spectrum
    //nbins = h->GetNbinsX();
    //Float_t source[nbins], dest[nbins];
    //for (int i = 0; i < nbins; i++) source[i]=h->GetBinContent(i + 1);

    //-----------------------------
    //use used-defined limits
    //find left and right bins corresponding llimit and rlimit

//    llimit = h->GetBinLowEdge(1) * 0.1;
//    rlimit = h->GetBinLowEdge(h->GetNbinsX());

//    cout << "llimit = " << llimit << endl;
//    cout << "rlimit = " << rlimit << endl;
//    cout << "real llimit = " << h->GetBinLowEdge(1) << endl;
//    cout << "real rlimit = " << h->GetBinLowEdge(h->GetNbinsX()) << endl;

    Int_t lbin = ((llimit - h->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( lbin >= 0 ) lbin++;
    else lbin = 1;
    Int_t rbin = ((rlimit - h->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( !(rbin >= h->GetNbinsX()) ) rbin++;
    else rbin = h->GetNbinsX();

    nbins = (rbin - lbin + 1);
    vector<Float_t> source(nbins);
    vector<Float_t> dest(nbins);

    for (int i = 0; i < nbins; i++)
    {
        source[i]=h->GetBinContent(i + lbin);
    }
    //-----------------------------

    nfound = s->SearchHighRes(&source[0], &dest[0], nbins, sigma, 2, kFALSE, 3, kTRUE, 3);
    cout << "nfound = " << nfound << endl;
    fPositionX.resize(nfound);
    fPositionY.resize(nfound);


    //Get peaks positions
    Float_t *xpeaks = s->GetPositionX();
    for (int i = 0; i < nfound; i++)
    {
       Double_t a = xpeaks[i];
       Int_t bin = lbin + Int_t(a + 0.5);
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
