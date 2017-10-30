#include "fithist.h"
#include "TPolyMarker.h"

using namespace std;

FitHist::FitHist(TH1F *hist)
{
    //h = (TH1F*)hist->Clone();
    h = hist;
    params.nbins_initial = h->GetNbinsX();
    params.nEntries = h->GetEntries();
    //cout << "h->GetEntries() = " << h->GetEntries() << endl;


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
    //printf("You are in FitHist::FindPeaks() (Thread %d) \n", syscall(__NR_gettid));
    cout << __LINE__ << endl;
    params.llimit = llimit;
    params.rlimit = rlimit;
    params.sigma = sigma;

    if(params.llimit == 0 && params.rlimit == 0)
    {
        params.llimit = h->GetXaxis()->GetBinLowEdge(1);
        params.rlimit = h->GetXaxis()->GetBinLowEdge(1) + h->GetBinWidth(1) * /*h->GetNbinsX()*/ params.nbins_initial;
    }

    cout << __LINE__ << endl;
    lbin = ((params.llimit - h->GetXaxis()->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( lbin >= 0 ) lbin++;
    else lbin = 1;
    rbin = ((params.rlimit - h->GetXaxis()->GetBinLowEdge(1)) / h->GetBinWidth(1));
    if( !(rbin >= /*h->GetNbinsX()*/ params.nbins_initial) ) rbin++;
    else rbin = /*h->GetNbinsX()*/ params.nbins_initial;

    params.nbins_user_limits = (rbin - lbin + 1);
    //cout << "nbins = " << params.nbins_user_limits << endl;
    vector<Float_t> source(params.nbins_user_limits);
    vector<Float_t> dest(params.nbins_user_limits);

    for (int i = 0; i < params.nbins_user_limits; i++)
    {
        source[i]=h->GetBinContent(i + lbin);
    }

    cout << __LINE__ << endl;
    params.nfound = s->SearchHighRes(&source[0], &dest[0], params.nbins_user_limits, params.sigma, 2, kFALSE, 3, kTRUE, 3);
    if(params.nfound > 0)
    {
        //cout << "you found " << params.nfound << " peaks" << endl;
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
        std::sort(params.peak_positions_xy.begin(), params.peak_positions_xy.end(), sort_pair_first<Double_t, Double_t>());

        cout << __LINE__ << endl;
        //Show peaks on the hist
        TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
        if (pm)
        {
            h->GetListOfFunctions()->Remove(pm);
            delete pm;
        }
        cout << __LINE__ << endl;
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
        //cout << "you didn't' find any peaks" << endl;
    }
}

void FitHist::FindSpe(bool is_ped_on_the_left)
{
    //if signal was negative, after this operation you will have peaks as after positive signal
    //cout << __LINE__ << endl;

    if(!is_ped_on_the_left)
    {
      std::sort(params.peak_positions_xy.begin(), params.peak_positions_xy.end(),
                sort_pair_first<Double_t, Double_t, std::greater<Double_t> >());

      for (int i = 0; i < params.nfound; ++i)
      {
          (params.peak_positions_xy[i]).first *= -1;
      }
    }

    //cout << __LINE__ << endl;
    TGraph gr(params.nfound);
    for (int i = 0; i < params.nfound; ++i)
    {
        gr.SetPoint(i, i, (params.peak_positions_xy[i]).first);
    }
    gr.Fit("pol1", "Q");//do not use "N" mode (problems with GetFunction may be)
    TF1 *myfunc = gr.GetFunction("pol1");
    Double_t chi2 = myfunc->GetChisquare();
    Double_t p0 = myfunc->GetParameter(0);
    Double_t p1 = myfunc->GetParameter(1);
    Double_t p0_err = myfunc->GetParError(0);
    Double_t p1_err = myfunc->GetParError(1);

//    cout << "chi2 =" << chi2 << endl;
//    cout << "p0 = " << p0 << endl;
//    cout << "p1 = " << p1 << endl;
    params.spe_value = p1;
    params.spe_value_err = p1_err;
    params.pedestal_shift = p0;
    params.pedestal_shift_err = p0_err;

    cout << "params.spe_value = " << params.spe_value << endl;
    cout << "params.pedestal_shift = " << params.pedestal_shift << endl;
    cout << "h->GetBinCenter(1) = " << h->GetBinCenter(1)  << endl;
    cout << "h->GetBinCenter(params.nbins_initial) = " << h->GetBinCenter(params.nbins_initial)  << endl;

    //discrete hist
    //vector<pair<Int_t, Int_t> > hist_discrete_xy_values;
    Int_t min_npe = (h->GetBinCenter(1) - params.pedestal_shift)/(params.spe_value) + 0.5;
    Int_t max_npe = (h->GetBinCenter(params.nbins_initial) - params.pedestal_shift)/(params.spe_value) + 0.5;
    if (!is_ped_on_the_left)
    {
        Int_t tmp = min_npe;
        min_npe = -max_npe;
        max_npe = -tmp;
    }
    cout <<  "min_npe = "<< min_npe << endl;
    cout <<  "max_npe = "<< max_npe << endl;
    vector<Double_t> hist_discrete_x(max_npe - min_npe);

    //in case of negative signal I will use invertied X axis
    Short_t sign = is_ped_on_the_left ? 1 : -1 ;
    for (int i = 0; i < params.nbins_initial; ++i)
    {
        Int_t val = (h->GetBinCenter(i+1)*sign - params.pedestal_shift)/(params.spe_value) + 0.5;
        if(val >= 0)
        {
            hist_discrete_x[val] += h->GetBinContent(i+1);
        }
    }


    for (int i = 0; i < hist_discrete_x.size(); ++i)
    {
        cout << i << " " << hist_discrete_x[i] << endl;
    }
}

void FitHist::ShowFitParameters()
{
    cout << __LINE__ << endl;
    cout << "FitParameters --------------------------------" << endl;

    cout << "nEntries = " << params.nEntries << endl;
    cout << "nbins_initial = " << params.nbins_initial << endl;
    cout << "nbins_user_limits = " << params.nbins_user_limits << endl;
    cout << "llimit = " << params.llimit << endl;
    cout << "rlimit = " << params.rlimit << endl;

    cout << "sigma = " << params.sigma << endl;
    cout << "nfound = " << params.nfound << endl;
    for (int i = 0; i < params.nfound; ++i)
    {
        cout << "\t x = " << (params.peak_positions_xy[i]).first << "; "
             << "y = " << (params.peak_positions_xy[i]).second << endl;
    }

    cout << "end FitParameters --------------------------------" << endl;
}
