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

void FitHist::FindPeaks(Float_t sigma, Double_t llimit, Double_t rlimit)
{
    printf("You are in FitHist::FindPeaks() (Thread %d) \n", syscall(__NR_gettid));

    if(llimit == 0 && rlimit == 0)
    {
        //llimit = h->GetBinLowEdge(1);
        //rlimit = h->GetBinLowEdge(nbins+1);

        llimit = h->GetXaxis()->GetBinLowEdge(1);
        //rlimit = h->GetXaxis()->GetBinLowEdge(nbins+1);
        rlimit = h->GetXaxis()->GetBinLowEdge(1) + h->GetBinWidth(1) * h->GetNbinsX();
    }
    cout << __LINE__ << endl;

    Int_t lbin = ((llimit - h->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( lbin >= 0 ) lbin++;
    else lbin = 1;
    Int_t rbin = ((rlimit - /*h->GetBinLowEdge(1)*/ h->GetXaxis()->GetBinLowEdge(1)) / h->GetBinWidth(1));
    cout << "rlimit =" << rlimit << endl;
    cout << "h->GetXaxis()->GetBinLowEdge(1) =" << h->GetXaxis()->GetBinLowEdge(1) << endl;
    cout << "numerator = " << (rlimit - h->GetXaxis()->GetBinLowEdge(1)) << endl;
    cout << "denominator = " << h->GetBinWidth(1) << endl;
    cout << "rbin before if/else = " << rbin << endl;
    if( !(rbin >= h->GetNbinsX()) ) rbin++;
    else rbin = h->GetNbinsX();

    cout << __LINE__ << endl;
    cout << "lbin = " << lbin << endl;
    cout << "rbin = " << rbin << endl;
    nbins = (rbin - lbin + 1);
    cout << "nbins = " << nbins << endl;
    vector<Float_t> source(nbins);
    vector<Float_t> dest(nbins);
    cout << "source.size() = " << source.size() << endl;
    cout << "dest.size() = " << dest.size() << endl;

    cout << __LINE__ << endl;
    for (int i = 0; i < nbins; i++)
    {
        source[i]=h->GetBinContent(i + lbin);
    }

    cout << __LINE__ << endl;
    nfound = s->SearchHighRes(&source[0], &dest[0], nbins, sigma, 2, kFALSE, 3, kTRUE, 3);
    if(nfound > 0)
    {
        cout << "you found " << nfound << " peaks" << endl;
        fPositionX.resize(nfound);
        fPositionY.resize(nfound);
        //cout << __LINE__ << endl;

        //Get peaks positions
        Float_t *xpeaks = s->GetPositionX();
        for (int i = 0; i < nfound; i++)
        {
            Double_t a = xpeaks[i];
            Int_t bin = lbin + Int_t(a + 0.5);
            fPositionX[i] = h->GetBinCenter(bin);
            fPositionY[i] = h->GetBinContent(bin);
        }
        //cout << "Get peaks positions finished" << endl;

        //Show peaks on the hist
        TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
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
        //cout << "Show peaks on the hist" << endl;
    }
    else
    {
        cout << "you didn't' find any peaks" << endl;
    }
}
