#include "fithist.h"
#include "TPolyMarker.h"

using namespace std;

FitHist::FitHist(TH1F *hist, bool is_ped_on_the_left) : is_ped_on_the_left(is_ped_on_the_left)
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

void FitHist::FindSpe()
{
    //if signal was negative, after this operation you will have peaks as after positive signal
    if(!is_ped_on_the_left)
    {
      std::sort(params.peak_positions_xy.begin(), params.peak_positions_xy.end(),
                sort_pair_first<Double_t, Double_t, std::greater<Double_t> >());

      for (int i = 0; i < params.nfound; ++i)
      {
          (params.peak_positions_xy[i]).first *= -1;
      }
    }

    TGraph gr(params.nfound);
    for (int i = 0; i < params.nfound; ++i)
    {
        gr.SetPoint(i, i, (params.peak_positions_xy[i]).first);
    }
    gr.Fit("pol1", "Q");//do not use "N" mode (problems with GetFunction may be)
    TF1 *myfunc = gr.GetFunction("pol1");

    params.spe_value = myfunc->GetParameter(1);
    params.spe_value_err = myfunc->GetParError(1);
    params.pedestal_shift = myfunc->GetParameter(0);
    params.pedestal_shift_err = myfunc->GetParError(0);
}

void FitHist::GetLP()
{
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
    //cout <<  "min_npe = "<< min_npe << endl;
    //cout <<  "max_npe = "<< max_npe << endl;
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

    Double_t hist_discrete_x_summ = 0;
    for (int i = 0; i < hist_discrete_x.size(); ++i)
    {
        //cout << i << " " << hist_discrete_x[i] << endl;
        hist_discrete_x_summ += hist_discrete_x[i];
    }

    //normalized (pdf) values
    Double_t f0 = hist_discrete_x[0] / hist_discrete_x_summ;
    Double_t f1 = hist_discrete_x[1] / hist_discrete_x_summ;

    //cout << "f0 = " << f0 << endl;
    //cout << "f1 = " << f1 << endl;

    params.p_01_peaks = 1 + f1 / (f0 * log(f0));
    params.L_01_peaks = - log(f0);

    //cout << "p = " << params.p << endl;
    //cout << "L = " << params.L << endl;

    VinogradovPDF vin_pdf;
    TF1 *fit_func = new TF1("fit",vin_pdf,0,100,2);
    TH1F *h_discrete = new TH1F("h_pdf", "h_pdf", hist_discrete_x.size(), min_npe, max_npe);
    //cout << "h_discrete.GetBinWidth(1) = " << h_discrete->GetBinWidth(1) << endl;
    for (int i = 0; i < hist_discrete_x.size(); ++i)
    {
        h_discrete->SetBinContent(i+1, hist_discrete_x[i]);
    }
    fit_func->SetParLimits(0,0,0.2);

    fit_func->SetParameter(1,0.3);
    fit_func->SetParLimits(1,0.3,0.3);

    h_discrete->Fit("fit", "QN");

    params.p_fit_discrete = fit_func->GetParameter(0);
    params.p_fit_discrete_err = fit_func->GetParError(0);
    params.L_fit_discrete = fit_func->GetParameter(1);
    params.L_fit_discrete_err = fit_func->GetParError(1);
    params.chi2_per_ndf_fit_discrete = ( fit_func->GetChisquare() ) / ( fit_func->GetNDF() );
}

void FitHist::ShowFitParameters()
{
    //cout << __LINE__ << endl;

    cout << "Properties of " << h->GetTitle() << "--------------------------------" << endl;

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

    cout << endl;
    cout  << "Vinogradov approximation:" << endl;
    cout << "p_01_peaks = " << params.p_01_peaks << endl;
    cout << "L_01_peaks = " << params.L_01_peaks << endl;
    cout << "chi2_per_ndf_fit_discrete = " << params.chi2_per_ndf_fit_discrete << endl;
    cout << "p_fit_discrete = " << params.p_fit_discrete << " +- " << params.p_fit_discrete_err << endl;
    cout << "L_fit_discrete = " << params.L_fit_discrete << " +- " << params.L_fit_discrete_err << endl;

    cout << h->GetTitle() << "--------------------------------" << endl;
}
