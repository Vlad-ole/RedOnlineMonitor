#include "fithist.h"
#include "TPolyMarker.h"

using namespace std;

FitHist::FitHist(TH1F *hist)
{
    //h = (TH1F*)hist->Clone();
    h = hist;
    params.nbins_initial = h->GetNbinsX();
    params.nEntries = h->GetEntries();
    cout << "h->GetEntries() = " << h->GetEntries() << endl;


    s = new TSpectrum(100);
    params.nfound = 0;
}

FitHist::~FitHist()
{
    delete s;
}

FitParameters FitHist::GetFitParameters()
{
    return params;
}

void FitHist::FindPeaks(Float_t sigma, Double_t llimit, Double_t rlimit)
{
    printf("You are in FitHist::FindPeaks() (Thread %d) \n", syscall(__NR_gettid));

    params.llimit = llimit;
    params.rlimit = rlimit;
    params.sigma = sigma;

    if(params.llimit == 0 && params.rlimit == 0)
    {
        params.llimit = h->GetXaxis()->GetBinLowEdge(1);
        params.rlimit = h->GetXaxis()->GetBinLowEdge(1) + h->GetBinWidth(1) * /*h->GetNbinsX()*/ params.nbins_initial;
    }

    Int_t lbin = ((params.llimit - h->GetXaxis()->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( lbin >= 0 ) lbin++;
    else lbin = 1;
    Int_t rbin = ((params.rlimit - h->GetXaxis()->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( !(rbin >= /*h->GetNbinsX()*/ params.nbins_initial) ) rbin++;
    else rbin = /*h->GetNbinsX()*/ params.nbins_initial;

    params.nbins_user_limits = (rbin - lbin + 1);
    cout << "nbins = " << params.nbins_user_limits << endl;
    vector<Float_t> source(params.nbins_user_limits);
    vector<Float_t> dest(params.nbins_user_limits);

    for (int i = 0; i < params.nbins_user_limits; i++)
    {
        source[i]=h->GetBinContent(i + lbin);
    }

    //cout << __LINE__ << endl;
    params.nfound = s->SearchHighRes(&source[0], &dest[0], params.nbins_user_limits, params.sigma, 2, kFALSE, 3, kTRUE, 3);
    if(params.nfound > 0)
    {
        cout << "you found " << params.nfound << " peaks" << endl;
        params.peak_positions_xy.resize(params.nfound);
        //fPositionX.resize(params.nfound);
        //fPositionY.resize(params.nfound);
        //cout << __LINE__ << endl;

        //Get peaks positions
        Float_t *xpeaks = s->GetPositionX();
        for (int i = 0; i < params.nfound; i++)
        {
            Double_t a = xpeaks[i];
            Int_t bin = lbin + Int_t(a + 0.5);
            /*fPositionX[i]*/ (params.peak_positions_xy[i]).first = h->GetBinCenter(bin);
            /*fPositionY[i]*/ (params.peak_positions_xy[i]).second = h->GetBinContent(bin);
        }
        //cout << "Get peaks positions finished" << endl;

        //Show peaks on the hist
        TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
        if (pm)
        {
            h->GetListOfFunctions()->Remove(pm);
            delete pm;
        }
        std::vector<Double_t> fPositionX(params.nfound);
        std::vector<Double_t> fPositionY(params.nfound);
        for (int i = 0; i < params.nfound; ++i)
        {
            fPositionX[i] = (params.peak_positions_xy[i]).first;
            fPositionY[i] = (params.peak_positions_xy[i]).second;
        }
        pm = new TPolyMarker(params.nfound, &fPositionX[0], &fPositionY[0]);
        pm->SetMarkerStyle(23);
        pm->SetMarkerColor(kRed);
        pm->SetMarkerSize(1.3);
        h->GetListOfFunctions()->Add(pm);
        //cout << "Show peaks on the hist" << endl;
    }
    else
    {
        cout << "you didn't' find any peaks" << endl;
    }
}
