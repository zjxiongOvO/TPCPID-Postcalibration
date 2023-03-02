#include "AliceRun3tool.h"

#ifndef _ALICE_TPC_POSTCALIBRATION_H_
#define _ALICE_TPC_POSTCALIBRATION_H_
using namespace std;
class postcalibration
{
    public:
        //common text set
        static string pairticle_name[3];
        static string mother_pairticle_name[3];
        static string cutname[3];
        static string histoname[3];
        static string datasets(string);

        //set binning
        // static double nclsbin[2];
        static double nclsbin[3];
        // static double nclsbin[4];
        // static double Pinbin[8];
        static double Pinbin[6];
        // static double Pinbin[5];
        static double etabin[10];
        //static double etabin[4];
        // static double etabin[6];
        static int Nnclsbin;
        static int NPinbin;
        static int Netabin;

        //intermediate histogram and variable
        static int isfristfit;
        static double expercted_par[6];
        static double limit_low_par[6];
        static double limit_high_par[6];
        static TH3F* fit_parameter_signal_mean;
        static TH3F* fit_parameter_signal_sigma;
        static TH3F* fit_parameter_bkg_mean;
        static TH3F* fit_parameter_bkg_sigma;
        static TH3I* fit_parameter_ngauss;
        static TH3F* fit_parameter_chi2;
        static double fitresult[10];
        static void get_fit_type(int);
        static void get_parameter_histo(TFile*,int);
        static void fill_parameter_histo(int,int,int);
        static void set_fit_parameter(int,int,int,int);
        static THn* calibration(string,int);

        //fit tools
        static int ngauss;
        static TH1* projection(THn*,double*,double*);
        static TF1* singlegauss();
        static TF1* doublegauss();
        static TF1* signal_peak();//single gauss for signal peak
        static TF1* background(int);//chooce a background function
        static void standardfit(TH1*);
        static void fit_v1(TH1*,int);
        static void fit_v2(TH1*,int);
        static void refit(TH1*,int);

        //draw and save tools
        static TH1* standard_histo_set(TH1*,int);
        static void standard_canva_set(int pairticletype,string period,double* vars_low,double* vars_high);
        static TH2F* interpolartion(TH2F*);
        static void parameter_savein(TFile*,int);
};

string postcalibration::pairticle_name[3] = {"electron","pion","proton"};
string postcalibration::mother_pairticle_name[3] = {"photo","K_{S}^{0}","Lambda"};
string postcalibration::cutname[3] = {"PIDCalibElectron","PIDCalibPion","PIDCalibProton"};
string postcalibration::histoname[3] = {"nSigmaTPCelectron","nSigmaTPCpion","nSigmaTPCproton"};
string postcalibration::datasets(string period){
    string shouldwrite = "Alice Run3 "+period;
    return shouldwrite;
};

// double postcalibration::nclsbin[2] = {0,160};
double postcalibration::nclsbin[3] = {0,130,160};
// double postcalibration::nclsbin[4] = {0,100,140,160};
// double postcalibration::Pinbin[8] = {0.2,0.4,0.6,0.8,1.0,1.5,2.0,5.0};
double postcalibration::Pinbin[6] = {0.2,0.4,0.6,0.8,1.0,5.0};
// double postcalibration::Pinbin[5] = {0.2,0.6,0.8,1.0,5.0};
// double postcalibration::etabin[4] = {-0.9,-0.3,0.3,0.9};
// double postcalibration::etabin[6] = {-0.9,-0.5,-0.3,0.3,0.5,0.9};
double postcalibration::etabin[10] = {-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9};
// double postcalibration::Pinbin[2] = {0.4,0.6};
// double postcalibration::etabin[2] = {-0.6,-0.4};
int postcalibration::Nnclsbin=sizeof(postcalibration::nclsbin)/sizeof(postcalibration::nclsbin[0])-1;
int postcalibration::NPinbin=sizeof(postcalibration::Pinbin)/sizeof(postcalibration::Pinbin[0])-1;
int postcalibration::Netabin=sizeof(postcalibration::etabin)/sizeof(postcalibration::etabin[0])-1;

int postcalibration::isfristfit = 1;//1 is frist fit,will use interpolated parameter to fit
                                    //0 is not frist fit,will use parameter form histrograms to fit

double postcalibration::expercted_par[6] = {10000,2,1,100,-4,1};
double postcalibration::limit_low_par[6] = {0,-2,0.5,0,-10,0};
double postcalibration::limit_high_par[6] = {1e6,3.5,1.5,1e6,-1.5,10};

TH3F* postcalibration::fit_parameter_signal_mean = nullptr;
TH3F* postcalibration::fit_parameter_signal_sigma = nullptr;
TH3F* postcalibration::fit_parameter_bkg_mean = nullptr;
TH3F* postcalibration::fit_parameter_bkg_sigma = nullptr;
TH3I* postcalibration::fit_parameter_ngauss = nullptr;
TH3F* postcalibration::fit_parameter_chi2 = nullptr;

int postcalibration::ngauss = 1;

double postcalibration::fitresult[10] = {0,0,0,0,0,0,0,0,0,0};//0:signal mean,1:signal mean error 2:signal sigma,3:signal sigma error 4:bkg mean,5:bkg mean error 6:bkg sigma,7:bkg sigma error 8:number of gauss,9:chi2/ndf
//just set fit type for all analysis
void postcalibration::get_fit_type(int fittype){
    if(fittype==1){
        isfristfit = 1;
    }
    else{
        isfristfit = 0;
    }
};

//set parameter histogram,if is frist fit,will set a new histogram as parameter_histogram
//if not frist fit,will get parameter histogram from file
void postcalibration::get_parameter_histo(TFile* file,int iparticle){
    if(isfristfit==1){
        fit_parameter_signal_mean = new TH3F("fit_parameter_signal_mean","fit_parameter_signal_mean",Nnclsbin,nclsbin,NPinbin,Pinbin,Netabin,etabin);
        fit_parameter_signal_sigma = new TH3F("fit_parameter_signal_sigma","fit_parameter_signal_sigma",Nnclsbin,nclsbin,NPinbin,Pinbin,Netabin,etabin);
        fit_parameter_bkg_mean = new TH3F("fit_parameter_bkg_mean","fit_parameter_bkg_mean",Nnclsbin,nclsbin,NPinbin,Pinbin,Netabin,etabin);
        fit_parameter_bkg_sigma = new TH3F("fit_parameter_bkg_sigma","fit_parameter_bkg_sigma",Nnclsbin,nclsbin,NPinbin,Pinbin,Netabin,etabin);
        fit_parameter_ngauss = new TH3I("fit_parameter_ngauss","fit_parameter_ngauss",Nnclsbin,nclsbin,NPinbin,Pinbin,Netabin,etabin);
        fit_parameter_chi2 = new TH3F("fit_parameter_chi2","fit_parameter_chi2",Nnclsbin,nclsbin,NPinbin,Pinbin,Netabin,etabin);
    }
    else{
        fit_parameter_signal_mean = (TH3F*)file->Get(Form("fit_parameter_signal_mean_%s",pairticle_name[iparticle].c_str()))->Clone();
        fit_parameter_signal_sigma = (TH3F*)file->Get(Form("fit_parameter_signal_sigma_%s",pairticle_name[iparticle].c_str()))->Clone();
        fit_parameter_bkg_mean = (TH3F*)file->Get(Form("fit_parameter_bkg_mean_%s",pairticle_name[iparticle].c_str()))->Clone();
        fit_parameter_bkg_sigma = (TH3F*)file->Get(Form("fit_parameter_bkg_sigma_%s",pairticle_name[iparticle].c_str()))->Clone();
        fit_parameter_ngauss = (TH3I*)file->Get(Form("fit_parameter_ngauss_%s",pairticle_name[iparticle].c_str()))->Clone();
        fit_parameter_chi2 = (TH3F*)file->Get(Form("fit_parameter_chi2_%s",pairticle_name[iparticle].c_str()))->Clone();
    }
};

//fill parameter histogram
//if is frist fit,will fill parameter histogram
//if not frist fit,will not fill parameter histogram
void postcalibration::fill_parameter_histo(int ncls,int pin,int eta){
    if(isfristfit==1){
        fit_parameter_signal_mean->SetBinContent(ncls+1,pin+1,eta+1,fitresult[0]);
        fit_parameter_signal_mean->SetBinError(ncls+1,pin+1,eta+1,fitresult[1]);

        fit_parameter_signal_sigma->SetBinContent(ncls+1,pin+1,eta+1,fitresult[2]);
        fit_parameter_signal_sigma->SetBinError(ncls+1,pin+1,eta+1,fitresult[3]);

        fit_parameter_bkg_mean->SetBinContent(ncls+1,pin+1,eta+1,fitresult[4]);
        fit_parameter_bkg_mean->SetBinError(ncls+1,pin+1,eta+1,fitresult[5]);

        fit_parameter_bkg_sigma->SetBinContent(ncls+1,pin+1,eta+1,fitresult[6]);
        fit_parameter_bkg_sigma->SetBinError(ncls+1,pin+1,eta+1,fitresult[7]);

        fit_parameter_ngauss->SetBinContent(ncls+1,pin+1,eta+1,fitresult[8]);
        fit_parameter_chi2->SetBinContent(ncls+1,pin+1,eta+1,fitresult[9]);
    }
};

//set fit parameter for each bin;
//if is frist fit,will use interpolated parameter to fit
//if not frist fit,will use parameter form histrograms to fit
void postcalibration::set_fit_parameter(int ncls,int pin, int eta, int iparticle){
    if (isfristfit==1){
        int binx = pin+1;
        int biny = eta+1;
        expercted_par[1]=fitresult[0];
        expercted_par[2]=fitresult[2];
        expercted_par[4]=fitresult[4];
        expercted_par[5]=fitresult[6];
        if (biny==1){
            cout<<"using the initial fit parameters default!!!!!!"<<endl;
            expercted_par[1] = 1;
            expercted_par[2] = 1;
            expercted_par[4] = -5;
            expercted_par[5] = 1;
            ngauss = 2;
            if (iparticle==1){
                expercted_par[1] = 0;
                expercted_par[2] = 1;
                expercted_par[4] = -6;
                expercted_par[5] = 1;
                ngauss = 1;
            }
            if (iparticle==2){
                expercted_par[1] = 0;
                expercted_par[2] = 1;
                expercted_par[4] = 1;
                expercted_par[5] = 1;
                ngauss = 1;
            }
        }
        //set the fit range
        limit_low_par[1] = expercted_par[1]-2;
        limit_high_par[1] = expercted_par[1]+2;
        limit_low_par[2] = expercted_par[2]-0.2;
        limit_high_par[2] = expercted_par[2]+0.2;
        // limit_low_par[2] = 1-0.3;
        // limit_high_par[2] = 1+0.2;
        limit_low_par[4] = expercted_par[4]-2;
        limit_high_par[4] = expercted_par[4]+2;
        limit_low_par[5] = expercted_par[5]-0.5;
        limit_high_par[5] = expercted_par[5]+0.5;

    }else {
        int binx = ncls+1;
        int biny = pin+1;
        int binz = eta+1;

        expercted_par[1] = fit_parameter_signal_mean->GetBinContent(binx,biny,binz);
        expercted_par[2] = fit_parameter_signal_sigma->GetBinContent(binx,biny,binz);
        expercted_par[4] = fit_parameter_bkg_mean->GetBinContent(binx,biny,binz);
        expercted_par[5] = fit_parameter_bkg_sigma->GetBinContent(binx,biny,binz);
        //set the fit range
        limit_low_par[1] = expercted_par[1]-2;
        limit_high_par[1] = expercted_par[1]+2;//mean of signal should not move too much
        limit_low_par[2] = expercted_par[2]-0.2;
        limit_high_par[2] = expercted_par[2]+0.2;
        limit_low_par[4] = expercted_par[4]-2;
        limit_high_par[4] = expercted_par[4]+2;
        limit_low_par[5] = expercted_par[5]-0.5;
        limit_high_par[5] = expercted_par[5]+0.5;
        ngauss = fit_parameter_ngauss->GetBinContent(binx,biny,binz);
    }
};

//get calibration histo from file
THn* postcalibration::calibration(string filename,int pairticletype){
    TFile *file = new TFile(filename.c_str());
    if (file->IsZombie()){
        cout<<"file open failed"<<endl;
        return 0;
    }
    if (pairticletype<0||pairticletype>2){
        cout<<"don't have this pairticle type in file"<<endl;
        return 0;
    }
    THn* histo;
    histo = reader::get_histo<THn>(reader::get_trkList(file,postcalibration::cutname[pairticletype]),postcalibration::histoname[pairticletype]);
    THn* histo_clone = (THn*)histo->Clone();
    return histo_clone;
};

//project a THn to a TH1
TH1* postcalibration::projection(THn* objecthisto,double* vars_low,double* vars_high){
    for (int idim=0;idim<3;idim++){
        objecthisto->GetAxis(idim+1)->SetRangeUser(vars_low[idim]+0.0000001,vars_high[idim]-0.0000001);
    }
    TH1* histo = objecthisto->Projection(0);
    return histo;
};

//fit function
TF1* postcalibration::singlegauss(){
    TF1* f1 = new TF1("f1","gaus",-7,7);
    f1->SetLineColor(kBlue);
    f1->SetLineWidth(1);
    f1->SetLineStyle(1);
    return f1;
};
TF1* postcalibration::doublegauss(){
    TF1* f1 = new TF1("f1","gaus(0)+gaus(3)",-5,5);
    cout<<f1<<endl;
    f1->SetLineColor(kRed);
    f1->SetLineWidth(2);
    f1->SetLineStyle(2);
    f1->SetParameters(expercted_par);
    f1->SetParLimits(0,limit_low_par[0],limit_high_par[0]);
    f1->SetParLimits(1,limit_low_par[1],limit_high_par[1]);
    f1->SetParLimits(2,limit_low_par[2],limit_high_par[2]);
    f1->SetParLimits(3,limit_low_par[3],limit_high_par[3]);
    f1->SetParLimits(4,limit_low_par[4],limit_high_par[4]);
    f1->SetParLimits(5,limit_low_par[5],limit_high_par[5]);
    // cout<<limit_low_par[0]<<endl;
    // cout<<limit_high_par[0]<<endl;
    // cout<<limit_low_par[1]<<endl;
    // cout<<limit_high_par[1]<<endl;
    // cout<<limit_low_par[2]<<endl;
    // cout<<limit_high_par[2]<<endl;
    return f1;
};

//signal peak function
TF1* postcalibration::signal_peak(){
    TF1* f1 = new TF1("f1","gaus",-7,7);
    f1->SetLineColor(kRed);
    f1->SetLineWidth(2);
    f1->SetLineStyle(2);
    f1->SetParameters(expercted_par);
    f1->SetParLimits(0,limit_low_par[0],limit_high_par[0]);
    f1->SetParLimits(1,limit_low_par[1],limit_high_par[1]);
    f1->SetParLimits(2,limit_low_par[2],limit_high_par[2]);
    return f1;
};

//background peak function
TF1* postcalibration::background(int ngauss){
    TF1* f1;
    f1->SetLineColor(kBlue);
    f1->SetLineWidth(2);
    f1->SetLineStyle(2);
    if (ngauss==0){
        f1= new TF1("f1","expo",-7,7);
    }
    else if (ngauss==1){
        f1= new TF1("f1","gaus",-7,7);
        f1->SetParameters(expercted_par[3],expercted_par[4],expercted_par[5]);
        f1->SetParLimits(0,limit_low_par[3],limit_high_par[3]);
        f1->SetParLimits(1,limit_low_par[4],limit_high_par[4]);
        f1->SetParLimits(2,limit_low_par[5],limit_high_par[5]);
    }
    else if (ngauss==2){
        f1= new TF1("f1","gaus(0)+gaus(3)",-7,7);
        f1->SetParameters(expercted_par[3],expercted_par[4],expercted_par[5],1,expercted_par[1]+1,1);
        f1->SetParLimits(0,limit_low_par[3],limit_high_par[3]);
        f1->SetParLimits(1,limit_low_par[4],limit_high_par[4]);
        f1->SetParLimits(2,limit_low_par[5],limit_high_par[5]);
        f1->SetParLimits(3,limit_low_par[3],limit_high_par[3]);
        f1->SetParLimits(4,limit_high_par[1],10);
        f1->SetParLimits(5,limit_low_par[5],limit_high_par[5]);
    }
    return f1;
};

//standard fit
void postcalibration::standardfit(TH1* histo){
    // if (histo->GetEntries()<500){
    //     cout<<"not enough entries"<<endl;
    //     return;
    // };
    TF1* fitfunc;
    if (postcalibration::ngauss==1){
        fitfunc=postcalibration::singlegauss();
    }
    else if (postcalibration::ngauss==2){
        fitfunc=postcalibration::doublegauss();
    }
    else {
        cout<<"wrong number of gaussians"<<endl;
        return;
    }
    TF1* signal_peak = new TF1("signal_peak","gaus",-5,5);
    TF1* background = new TF1("background","gaus",-5,5);
    histo->Fit(fitfunc,"R");
    fitfunc->SetLineColor(kRed);
    fitfunc->SetLineWidth(2);
    fitfunc->SetLineStyle(2);
    fitfunc->Draw("same");
    TLatex* latex = new TLatex();
    latex->SetTextSize(0.035);
    latex->SetTextFont(22);
    latex->SetTextColor(kRed);
    latex->DrawLatexNDC(0.75,0.77,Form("#mu = %.2f",fitfunc->GetParameter(1)));
    latex->DrawLatexNDC(0.75,0.73,Form("#sigma = %.2f",fitfunc->GetParameter(2)));
    latex->DrawLatexNDC(0.75,0.69,Form("N = %.0f",fitfunc->GetParameter(0)));
    latex->DrawLatexNDC(0.75,0.65,Form("chi^{2}/ndf = %.2f",fitfunc->GetChisquare()/fitfunc->GetNDF()));
    //latex->DrawLatexNDC(0.75,0.73,Form("chi^{2}/ndf = %.2f",fitfunc->GetChisquare()/fitfunc->GetNDF()));
    //set fitresult parameters and errors
    fitresult[0] = fitfunc->GetParameter(1);
    fitresult[1] = fitfunc->GetParError(1);
    fitresult[2] = fitfunc->GetParameter(2);
    fitresult[3] = fitfunc->GetParError(2);
    //
    fitresult[4] = fitfunc->GetParameter(4);
    fitresult[5] = fitfunc->GetParError(4);
    fitresult[6] = fitfunc->GetParameter(5);
    fitresult[7] = fitfunc->GetParError(5);
    fitresult[8] = ngauss;
    fitresult[9] = fitfunc->GetChisquare()/fitfunc->GetNDF();
    
    //is refit needed?
    // postcalibration::refit(histo,1);

    //is need to change signal peak and background
    if (fitresult[0]<fitresult[4]){
        signal_peak->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
        background->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
    }
    else{
        signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
        background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
    }

    //draw signal peak
    // signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
    signal_peak->SetLineColor(kBlue);
    signal_peak->SetLineWidth(1);
    signal_peak->SetLineStyle(1);
    signal_peak->Draw("same");
    if (ngauss==2){
    //draw background
    // background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
    background->SetLineColor(kGreen);
    background->SetLineWidth(1);
    background->SetLineStyle(1);
    background->Draw("same");
}
};

void postcalibration::fit_v1(TH1* histo,int particletype){
    TF1* fitfunc;
    if (postcalibration::ngauss==1){
        fitfunc=postcalibration::singlegauss();
    }
    else if (postcalibration::ngauss==2){
        fitfunc=postcalibration::doublegauss();
    }
    else {
        cout<<"wrong number of gaussians"<<endl;
        return;
    }
    TF1* signal_peak = new TF1("signal_peak","gaus",-5,5);
    TF1* background = new TF1("background","gaus",-5,5);
    histo->Fit(fitfunc,"R");
    fitfunc->SetLineColor(kRed);
    fitfunc->SetLineWidth(2);
    fitfunc->SetLineStyle(2);
    fitfunc->Draw("same");
    TLatex* latex = new TLatex();
    latex->SetTextSize(0.035);
    latex->SetTextFont(22);
    latex->SetTextColor(kRed);

    //set fitresult parameters and errors
    fitresult[0] = fitfunc->GetParameter(1);
    fitresult[1] = fitfunc->GetParError(1);
    fitresult[2] = fitfunc->GetParameter(2);
    fitresult[3] = fitfunc->GetParError(2);
    //
    fitresult[4] = fitfunc->GetParameter(4);
    fitresult[5] = fitfunc->GetParError(4);
    fitresult[6] = fitfunc->GetParameter(5);
    fitresult[7] = fitfunc->GetParError(5);
    fitresult[8] = ngauss;
    fitresult[9] = fitfunc->GetChisquare()/fitfunc->GetNDF();
    
    //is refit needed?
    // postcalibration::refit(histo,1);

    //is need to change signal peak and background
    if (ngauss==2){
        if (particletype==0){
            if (fitresult[0]<fitresult[4]){
                signal_peak->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
                background->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
            }
            else{
                signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
                background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
            }
        }
        else if (particletype==2){
            if (std::abs(fitresult[0]-fitresult[4])<3){
                if(fitfunc->GetParameter(0)>fitfunc->GetParameter(3)){
                    signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
                    background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
                }
                else{
                    signal_peak->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
                    background->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
                }
            }
            else {
                if (fitresult[0]<fitresult[4]){
                signal_peak->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
                background->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
            }
            else{
                signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
                background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
            }
            }
            // if (fitresult[0]>fitresult[4]){
            //     signal_peak->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
            //     background->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
            // }
            // else{
            //     signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
            //     background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
            // }
        }
    }
    else if (ngauss==1){
        signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
    }
    //draw signal peak
    // signal_peak->SetParameters(fitfunc->GetParameter(0),fitfunc->GetParameter(1),fitfunc->GetParameter(2));
    signal_peak->SetLineColor(kBlue);
    signal_peak->SetLineWidth(1);
    signal_peak->SetLineStyle(1);
    signal_peak->Draw("same");
    if (ngauss==2){
    //draw background
    // background->SetParameters(fitfunc->GetParameter(3),fitfunc->GetParameter(4),fitfunc->GetParameter(5));
    background->SetLineColor(kGreen);
    background->SetLineWidth(1);
    background->SetLineStyle(1);
    background->Draw("same");


}
    latex->DrawLatexNDC(0.75,0.77,Form("#mu = %.2f",signal_peak->GetParameter(1)));
    latex->DrawLatexNDC(0.75,0.73,Form("#sigma = %.2f",signal_peak->GetParameter(2)));
    latex->DrawLatexNDC(0.75,0.69,Form("N = %.0f",signal_peak->GetParameter(0)));
    latex->DrawLatexNDC(0.75,0.65,Form("chi^{2}/ndf = %.2f",fitfunc->GetChisquare()/fitfunc->GetNDF()));

};
//fit v2
void postcalibration::fit_v2(TH1* histo,int particletype){

};

//refit 
void postcalibration::refit(TH1* histo,int ngauss){
    TF1* refitfunc;
    if (ngauss==1){
        refitfunc = postcalibration::singlegauss();
    }
    else if (ngauss==2){
        refitfunc = postcalibration::doublegauss();
    }
    else{
        cout<<"wrong ngauss"<<endl;
        return;
    }
    histo->Fit(refitfunc,"R","",fitresult[0]-1.5*fitresult[2],fitresult[0]+1.5*fitresult[2]);
    refitfunc->SetLineColor(kBlue);
    refitfunc->SetLineWidth(1);
    refitfunc->SetLineStyle(1);
    refitfunc->Draw("same");
    TLatex* latex = new TLatex();
    latex->SetTextSize(0.035);
    latex->SetTextFont(22);
    latex->SetTextColor(kBlue);
    latex->DrawLatexNDC(0.75,0.69,Form("#mu = %.2f",refitfunc->GetParameter(1)));
    latex->DrawLatexNDC(0.75,0.65,Form("#sigma = %.2f",refitfunc->GetParameter(2)));
    latex->DrawLatexNDC(0.75,0.61,Form("N = %.0f",refitfunc->GetParameter(0)));
    latex->DrawLatexNDC(0.75,0.57,Form("chi^{2}/ndf = %.2f",refitfunc->GetChisquare()/refitfunc->GetNDF()));
    //set fitresult parameters and errors
    // fitresult[0] = refitfunc->GetParameter(1);
    // fitresult[1] = refitfunc->GetParError(1);
    // fitresult[2] = refitfunc->GetParameter(2);
    // fitresult[3] = refitfunc->GetParError(2);
}

//set standard style for fitting histogram
TH1* postcalibration::standard_histo_set(TH1* histo,int pairticletype){
    histo->SetStats(0);
    histo->GetXaxis()->SetTitle(Form("n#sigma^{TPC}_{%s}",postcalibration::pairticle_name[pairticletype].c_str()));
    histo->GetYaxis()->SetTitle("counts");
    histo->GetXaxis()->SetTitleOffset(1.0);
    histo->GetYaxis()->SetTitleOffset(1.0);
    histo->GetXaxis()->SetTitleSize(0.05);
    histo->GetYaxis()->SetTitleSize(0.05);
    histo->GetXaxis()->SetLabelSize(0.04);
    histo->GetYaxis()->SetLabelSize(0.04);
    histo->GetYaxis()->SetRangeUser(0.1,histo->GetMaximum()*1.2);
    histo->SetMarkerStyle(20);
    histo->SetMarkerSize(0.4);
    histo->SetMarkerColor(kBlack);
    histo->SetLineColor(kBlack);
    histo->SetLineWidth(2);
    histo->SetTitle("");
    return histo;
};

//set standard style for canvas
void postcalibration::standard_canva_set(int pairticletype,string period,double* vars_low,double* vars_high){
    drawer::drawLatex(0.2,0.85,Form("%s",postcalibration::datasets(period).c_str()),22,0.05,1);
    drawer::drawLatex(0.2,0.8,Form("%s from %s",postcalibration::pairticle_name[pairticletype].c_str(),postcalibration::mother_pairticle_name[pairticletype].c_str()),22,0.035,4);
    drawer::drawLatex(0.2,0.75,Form("%.1f < ncls < %.1f",vars_low[0],vars_high[0]),22,0.035,1);
    drawer::drawLatex(0.2,0.7,Form("%.1f < P_{in} < %.1f GeV/c",vars_low[1],vars_high[1]),22,0.035,1);
    drawer::drawLatex(0.2,0.65,Form("%.1f < #eta < %.1f",vars_low[2],vars_high[2]),22,0.035,1);
};

//interpolate the post-calibration histogram
TH2F* postcalibration::interpolartion(TH2F* objectedhisto){
    int nxbin,nybin;
    nxbin = 24;
    nybin = 18;
    TH2F* afterhisto = new TH2F("afterhisto","afterhisto",nxbin,0.2,5,nybin,-0.9,0.9);
    for (int i=1;i<=nxbin;i++){
        for (int j=1;j<=nybin;j++){
            afterhisto->SetBinContent(i,j,objectedhisto->Interpolate(afterhisto->GetXaxis()->GetBinCenter(i),afterhisto->GetYaxis()->GetBinCenter(j)));
        };
    };
    return afterhisto;
};

//save parameters to root file
void postcalibration::parameter_savein(TFile* file,int iparticle){
    TH3F* h1 = (TH3F*)fit_parameter_signal_mean->Clone();
    TH3F* h2 = (TH3F*)fit_parameter_signal_sigma->Clone();
    TH3F* h3 = (TH3F*)fit_parameter_bkg_mean->Clone();
    TH3F* h4 = (TH3F*)fit_parameter_bkg_sigma->Clone();
    TH3I* h5 = (TH3I*)fit_parameter_ngauss->Clone();
    TH3F* h6 = (TH3F*)fit_parameter_chi2->Clone();
    file->cd();
    h1->Write(Form("fit_parameter_signal_mean_%s",postcalibration::pairticle_name[iparticle].c_str()));
    h2->Write(Form("fit_parameter_signal_sigma_%s",postcalibration::pairticle_name[iparticle].c_str()));
    h3->Write(Form("fit_parameter_bkg_mean_%s",postcalibration::pairticle_name[iparticle].c_str()));
    h4->Write(Form("fit_parameter_bkg_sigma_%s",postcalibration::pairticle_name[iparticle].c_str()));
    h5->Write(Form("fit_parameter_ngauss_%s",postcalibration::pairticle_name[iparticle].c_str()));
    h6->Write(Form("fit_parameter_chi2_%s",postcalibration::pairticle_name[iparticle].c_str()));
};


#endif