#include "vinogradovpdf.h"

using namespace std;

VinogradovPDF::VinogradovPDF() : counter(0)
{

}

Double_t VinogradovPDF::operator() (Double_t *x, Double_t *param)
{
    return GetVinogradovPDFValue(x[0], param[0], param[1]);
}

Double_t VinogradovPDF::GetVinogradovPDFValue(UInt_t k, Double_t p, Double_t L)
{
    //if(counter % 1000 == 0)
//    {
//        //printf("You are in GetVinogradovPDFValue (Thread %d) \n", syscall(__NR_gettid));
//        cout << counter << endl ;
//    }

    Double_t f = 0;

    if(p)
    {
        for (UInt_t i = 0; i <= k; ++i)
        {
            f += GetBik(i, k) * pow(L * (1-p), i) * pow(p, k-i);
        }
    }
    else
    {
        f = pow(L, k);
    }

    counter++;

    return (f * TMath::Exp(-L) / TMath::Factorial(k) );
}

Double_t  VinogradovPDF::GetBik(UInt_t i, UInt_t k)
{
    Double_t Bik;

    if(i == 0 && k == 0)
    {
        Bik = 1;
    }
    else if (i == 0 && k > 0)
    {
        Bik = 0;
    }
    else
    {
       Bik = ( TMath::Factorial(k) * TMath::Factorial(k-1) ) /
               (TMath::Factorial(i) * TMath::Factorial(i-1) * TMath::Factorial(k-i) );
    }

    return Bik;
}
