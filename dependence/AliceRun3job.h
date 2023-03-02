#include <TFile.h>
#include <TH1.h>
#include <TList.h>
#include <TLatex.h>
#include <TMath.h>
#include <TH2.h>
#include <TCanvas.h>
#include <THn.h>
#include <TLine.h>


// #ifndef ALICE_RUN3_JOB_H
// #define ALICE_RUN3_JOB_H

using namespace std;
TList* get_List(TFile*, string);
TLatex* drawLatex(double, double, char*, int,double,int);
void fit_single_gaus(TH1*, double*, double*);
void fit_single_gaus_v2(TH1*, double*, double*, double*);
void fit_double_gaus(TH1*, double*, double*, int);
void fit_double_gaus_v2(TH1*, double*, double*, double*, double*);
TH1* SetHistogram(TH1*, string, string, double, double);

TList* get_List(TFile *file, string HashListname,string Listname)
{
    THashList *HashList = (THashList*)file->Get(HashListname.c_str());
    TList *List = (TList*)HashList->FindObject(Listname.c_str());
    return List;
}

TLatex* drawLatex(double x, double y, char* text, int textFont, double textSize, int colorIndex)
{
  TLatex *latex = new TLatex(x,y,text); 
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);   
  latex->SetTextColor(colorIndex); 
  latex->Draw("same");
  return latex;
}

template <class HISTO>
HISTO* get_histo(TList* List, string histoname)
{
    HISTO *histo = (HISTO*)List->FindObject(histoname.c_str());
    return histo;
}

void fit_single_gaus(TH1* h1,double *mean,double* sigma){
    //TF1* gaus = new TF1("gaus","[0]*TMath::Gaus(x,[1],[2],1)",-10,10);
    TF1* gaus = new TF1("gaus","gaus",-10,10);
    double max=h1->GetMaximum();
    double max_bin=h1->GetBinCenter(h1->GetMaximumBin());
    //if max_bin is too close to the edge, then use the mean of the histogram
    if (max_bin<-3||max_bin>3){
        max_bin=mean[0];
        max=h1->GetBinContent(h1->FindBin(max_bin));
    }
    gaus->SetParameters(max,max_bin,sigma[0]);
    gaus->SetParLimits(2,0.5,1.5);
    //h1->Fit("gaus","R","",-3,3);
    // h1->Fit("gaus","MR","",max_bin-3*sigma[0],max_bin+3*sigma[0]);
    h1->Fit("gaus","MR","",-2,2);
    mean[0]=gaus->GetParameter(1);
    sigma[0]=gaus->GetParameter(2);
    mean[1]=gaus->GetParError(1);
    sigma[1]=gaus->GetParError(2);
}

void fit_single_gaus_v2(TH1* h1,double *mean,double* sigma,double* sig_limit){
    //TF1* gaus = new TF1("gaus","[0]*TMath::Gaus(x,[1],[2],1)",-10,10);
    TF1* gaus = new TF1("gaus","gaus+expo(3)",-10,10);
    double max=h1->GetMaximum();
    double max_bin=h1->GetBinCenter(h1->GetMaximumBin());
    //if max_bin is too close to the edge, then use the mean of the histogram
    //gaus->SetParameters(max,max_bin,sigma[0]);
    gaus->SetParameters(0,max);
    gaus->SetParameters(1,max_bin);
    gaus->SetParameters(2,sigma[0]);
    gaus->SetParLimits(1,sig_limit[0],sig_limit[1]);
    h1->Fit("gaus","R","",max_bin-2*sigma[0],max_bin+2*sigma[0]);
    mean[0]=gaus->GetParameter(1);
    sigma[0]=gaus->GetParameter(2);
    mean[1]=gaus->GetParError(1);
    sigma[1]=gaus->GetParError(2);
}

void fit_double_gaus(TH1* h1,double *mean,double* sigma,int fitmode){
    //mean[4]={mean1,mean_err1,mean2,mean_err2}
    //sigma[4]={sigma1,sigma_err1,sigma2,sigma_err2}
    TF1* double_gaus = new TF1("double_gaus","[0]*TMath::Gaus(x,[1],[2],1)+[3]*TMath::Gaus(x,[4],[5],1)",-10,10);
    double_gaus->SetParameters(1,mean[0],sigma[0],1,mean[2],sigma[2]);
    double_gaus->SetParLimits(1,-2,3);
    double_gaus->SetParLimits(2,0.6,1.3);
    double_gaus->SetParLimits(4,-10,-3);
    //h1->Fit("double_gaus","R","",mean[0]-3.5*sigma[0],3);
    h1->Fit("double_gaus","R","",mean[0]-3.5*sigma[0],mean[0]+3.5*sigma[0]);
    double p0=double_gaus->GetParameter(0);
    double p1= double_gaus->GetParameter(1);
    double p3=double_gaus->GetParameter(3);
    double p4=double_gaus->GetParameter(4);
    if (fitmode==1){//choose the peak which the mean close to 0 as the first peak
        if (std::abs(p4)>std::abs(p1)){
            mean[0]=p1;
            mean[1]=double_gaus->GetParError(1);
            sigma[0]=double_gaus->GetParameter(2);
            sigma[1]=double_gaus->GetParError(2);
            mean[2]=p4;
            mean[3]=double_gaus->GetParError(4);
            sigma[2]=double_gaus->GetParameter(5);
            sigma[3]=double_gaus->GetParError(5);
        }
        else{
            mean[0]=p4;
            mean[1]=double_gaus->GetParError(4);
            sigma[0]=double_gaus->GetParameter(5);
            sigma[1]=double_gaus->GetParError(5);
            mean[2]=p1;
            mean[3]=double_gaus->GetParError(1);
            sigma[2]=double_gaus->GetParameter(2);
            sigma[3]=double_gaus->GetParError(2);
        }
    }
    else if (fitmode==2){//choose the peak which higher as the first peak
        if (p0>=p3){
            mean[0]=double_gaus->GetParameter(1);
            sigma[0]=double_gaus->GetParameter(2);
            mean[1]=double_gaus->GetParError(1);
            sigma[1]=double_gaus->GetParError(2);
            mean[2]=double_gaus->GetParameter(4);
            sigma[2]=double_gaus->GetParameter(5);
            mean[3]=double_gaus->GetParError(4);
            sigma[3]=double_gaus->GetParError(5);
        }
        else{
            mean[0]=double_gaus->GetParameter(4);
            sigma[0]=double_gaus->GetParameter(5);
            mean[1]=double_gaus->GetParError(4);
            sigma[1]=double_gaus->GetParError(5);
            mean[2]=double_gaus->GetParameter(1);
            sigma[2]=double_gaus->GetParameter(2);
            mean[3]=double_gaus->GetParError(1);
            sigma[3]=double_gaus->GetParError(2);
        }
  }
}

void fit_double_gaus_v2(TH1* h1,double *mean,double* sigma,double* sig_limit,double* bkg_limit)
{
    //TF1* double_gaus = new TF1("double_gaus","[0]*TMath::Gaus(x,[1],[2],1)+[3]*TMath::Gaus(x,[4],[5],1)",-10,10);
    //TF1* double_gaus = new TF1("double_gaus","gaus+gaus(3)",-10,10);
    TF1* double_gaus = new TF1("double_gaus","gaus+gaus(3)",-10,10);
    double_gaus->SetParameters(1,mean[0],sigma[0],1,mean[2],sigma[2]);
    double_gaus->SetParLimits(1,sig_limit[0],sig_limit[1]);
    double_gaus->SetParLimits(2,0.5,1.8);
    double_gaus->SetParLimits(4,bkg_limit[0],bkg_limit[1]);
    double_gaus->SetParLimits(0,0,10e8);
    double_gaus->SetParLimits(3,0,10e8);

    TFitResultPtr  r=h1->Fit("double_gaus","MS","",-4,5);
    // TLegend* leg = new TLegend(0.6,0.6,0.9,0.9);
    // leg->AddEntry(double_gaus,"double gauss","l");
    // leg->AddEntry((TObject*)0,Form("p0=%.2f#pm%.2f",double_gaus->GetParameter(0),double_gaus->GetParError(0)),"");
    // leg->AddEntry((TObject*)0,Form("p1=%.2f#pm%.2f",double_gaus->GetParameter(1),double_gaus->GetParError(1)),"");
    // leg->AddEntry((TObject*)0,Form("p2=%.2f#pm%.2f",double_gaus->GetParameter(2),double_gaus->GetParError(2)),"");
    // leg->AddEntry((TObject*)0,Form("p3=%.2f#pm%.2f",double_gaus->GetParameter(3),double_gaus->GetParError(3)),"");
    // leg->AddEntry((TObject*)0,Form("p4=%.2f#pm%.2f",double_gaus->GetParameter(4),double_gaus->GetParError(4)),"");
    // leg->AddEntry((TObject*)0,Form("p5=%.2f#pm%.2f",double_gaus->GetParameter(5),double_gaus->GetParError(5)),"");
    // leg->AddEntry((TObject*)0,Form("p6=%.2f#pm%.2f",double_gaus->GetParameter(6),double_gaus->GetParError(6)),"");
    // leg->AddEntry((TObject*)0,Form("p7=%.2f#pm%.2f",double_gaus->GetParameter(7),double_gaus->GetParError(7)),"");
    // leg->AddEntry((TObject*)0,Form("chi2/ndf=%.2f",r->Chi2()/r->Ndf()),"");
    // leg->Draw();
    //draw the fit result
    //h1->Fit("double_gaus","M","",-6,4);
    mean[0]=double_gaus->GetParameter(1);
    mean[1]=double_gaus->GetParError(1);
    sigma[0]=double_gaus->GetParameter(2);
    sigma[1]=double_gaus->GetParError(2);
    mean[2]=double_gaus->GetParameter(4);
    mean[3]=double_gaus->GetParError(4);
    sigma[2]=double_gaus->GetParameter(5);
    sigma[3]=double_gaus->GetParError(5);
}

TH1* sethistogram(TH1* objecthisto,string xaxisname,string yaxisname,double yrangelow,double yrangehigh){
    objecthisto->SetStats(0);
    objecthisto->GetYaxis()->SetRangeUser(yrangelow,yrangehigh);
    objecthisto->GetXaxis()->SetTitle(xaxisname.c_str());
    objecthisto->GetXaxis()->SetTitleSize(0.05);
    objecthisto->GetXaxis()->SetTitleOffset(0.9);
    objecthisto->GetXaxis()->SetLabelSize(0.05);
    objecthisto->GetYaxis()->SetTitle(yaxisname.c_str());
    objecthisto->GetYaxis()->SetTitleSize(0.05);
    objecthisto->GetYaxis()->SetTitleOffset(0.9);
    objecthisto->GetYaxis()->SetLabelSize(0.05);
    objecthisto->SetMarkerStyle(20);
    objecthisto->SetMarkerSize(0.7);
    objecthisto->SetMarkerColor(kBlack);
    objecthisto->SetLineWidth(2);
    objecthisto->SetLineColor(kBlack);
    objecthisto->SetTitle("");
    return objecthisto;
}

TLine * drawLine(Double_t x_low=0,Double_t x_high=10,Double_t y_low=0,Double_t y_high=10,Double_t kLineColor=2, Double_t kLineStyle=1, Double_t kLineWith=1.0)
{
   TLine *line = new TLine(x_low,y_low,x_high,y_high);
   line->SetLineColor(kLineColor);
   line->SetLineStyle(kLineStyle);
   line->SetLineWidth(kLineWith);
   line->Draw("same");
   return line;
}
