#include "../dependence/AliceRun3tool.h"
#include "../dependence/AliceTPCpostcalibration.h"

//initialize v0 histogram
TH1* MassGamma;
TH1* MassK0s;
TH1* MassLambda;
TH1* MassAntiLambda;
TH2* NsigmaEle_pin;
TH2* NsigmaPi_pin;
TH2* NsigmaPr_pin;
TH2* NsigmaEle_eta;
TH2* NsigmaPi_eta;
TH2* NsigmaPr_eta;
TH2* Dedx_pin;
TH2* Dedx_pin_El;
TH2* Dedx_pin_Pi;
TH2* Dedx_pin_Pr;
TH2* beta_pin;
TH2* beta_pin_El;
TH2* beta_pin_Pi;
TH2* beta_pin_Pr;
string v0name[4] = {"Gamma","K0s","Lambda","AntiLambda"};
TH2* APplot;

void getusedhisto(TFile*);
void setusedhisto();

void v0check(string dataname,string ischeck,int isfristfit){
    gStyle->SetOptStat(0);
    string filename="input/verson1/"+ dataname +"_"+ ischeck +".root";
    TFile* data = new TFile(filename.c_str(),"READ");
    system(Form("mkdir -p output/%s/%s/v0check",dataname.c_str(),ischeck.c_str()));
    getusedhisto(data);
    setusedhisto();

    //draw v0 unlike-sign mass distribution
    //need to updata!!!!!!!!!!
    TCanvas* c1 = new TCanvas("c1","c1",800,600);
    for (int iv0=0;iv0<4;iv0++){
        c1->cd();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.15);
        if (iv0==0){
            MassGamma->Draw("E");
        }
        else if (iv0==1){
            MassK0s->Draw("E");
        }
        else if (iv0==2){
            MassLambda->Draw("E");
        }
        else if (iv0==3){
            MassAntiLambda->Draw("E");
        }
        drawer::drawLatex(0.2,0.82,Form("ALICE Run3 %s",dataname.c_str()),22,0.05,1);
        drawer::drawLatex(0.2,0.76,Form("%s",v0name[iv0].c_str()),22,0.05,2);
        c1->SaveAs(Form("output/%s/%s/v0check/%s.pdf",dataname.c_str(),ischeck.c_str(),v0name[iv0].c_str()));
        c1->Clear();
    }

    //draw v0 AP plot
    TCanvas* c2 = new TCanvas("c2","c2",800,600);
    c2->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    APplot->Draw("colz");
    c2->SaveAs(Form("output/%s/%s/v0check/AP.pdf",dataname.c_str(),ischeck.c_str()));

    //draw v0 Nsigma plot
    TCanvas* c3 = new TCanvas("c3","c3",800,600);
    c3->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    NsigmaEle_pin->Draw("colz");
    c3->SaveAs(Form("output/%s/%s/v0check/NsigmaEle_pin.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c4 = new TCanvas("c4","c4",800,600);
    c4->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    NsigmaPi_pin->Draw("colz");
    c4->SaveAs(Form("output/%s/%s/v0check/NsigmaPi_pin.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c5 = new TCanvas("c5","c5",800,600);
    c5->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    NsigmaPr_pin->Draw("colz");
    c5->SaveAs(Form("output/%s/%s/v0check/NsigmaPr_pin.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c6 = new TCanvas("c6","c6",800,600);
    c6->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    NsigmaEle_eta->Draw("colz");
    c6->SaveAs(Form("output/%s/%s/v0check/NsigmaEle_eta.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c7 = new TCanvas("c7","c7",800,600);
    c7->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    NsigmaPi_eta->Draw("colz");
    c7->SaveAs(Form("output/%s/%s/v0check/NsigmaPi_eta.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c8 = new TCanvas("c8","c8",800,600);
    c8->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    NsigmaPr_eta->Draw("colz");
    c8->SaveAs(Form("output/%s/%s/v0check/NsigmaPr_eta.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c9 = new TCanvas("c9","c9",800,600);
    c9->cd();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    TH1F* hMassLambda = (TH1F*)MassLambda->Clone("MassLambda");
    TH1F* hMassAntiLambda = (TH1F*)MassAntiLambda->Clone("MassAntiLambda");
    hMassLambda->Scale(1/hMassLambda->Integral());
    hMassAntiLambda->Scale(1/hMassAntiLambda->Integral());
    hMassLambda->SetLineColor(kRed);
    hMassAntiLambda->SetLineColor(kBlue);
    hMassLambda->SetLineWidth(2);
    hMassAntiLambda->SetLineWidth(2);
    hMassLambda->SetLineStyle(2);
    hMassAntiLambda->SetLineStyle(2);
    hMassLambda->SetStats(0);
    hMassAntiLambda->SetStats(0);
    hMassLambda->Draw("hist");
    hMassAntiLambda->Draw("hist same");
    drawer::drawLatex(0.2,0.82,Form("ALICE Run3 %s",dataname.c_str()),22,0.05,1);
    drawer::drawLatex(0.2,0.76,Form("Lambda"),22,0.05,2);
    drawer::drawLatex(0.2,0.7,Form("AntiLambda"),22,0.05,4);
    c9->SaveAs(Form("output/%s/%s/v0check/Lambda_vs_AntiLambda.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c10 = new TCanvas("c10","c10",800,600);
    c10->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    Dedx_pin->Draw("colz");
    drawer::drawLatex(0.5,0.7,Form("track from V0"),22,0.05,1);
    c10->SaveAs(Form("output/%s/%s/v0check/Dedx_pin.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c11 = new TCanvas("c11","c11",800,600);
    c11->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    Dedx_pin_El->Draw("colz");
    drawer::drawLatex(0.5,0.7,Form("Electron from gamma"),22,0.05,1);
    c11->SaveAs(Form("output/%s/%s/v0check/Dedx_pin_El.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c12 = new TCanvas("c12","c12",800,600);
    c12->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    Dedx_pin_Pi->Draw("colz");
    drawer::drawLatex(0.5,0.7,Form("Pion from K0s"),22,0.05,1);
    c12->SaveAs(Form("output/%s/%s/v0check/Dedx_pin_Pi.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c13 = new TCanvas("c13","c13",800,600);
    c13->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    Dedx_pin_Pr->Draw("colz");
    drawer::drawLatex(0.5,0.7,Form("Proton from Lambda/AntiLambda"),22,0.05,1);
    c13->SaveAs(Form("output/%s/%s/v0check/Dedx_pin_Pr.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c14 = new TCanvas("c14","c14",800,600);
    c14->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    beta_pin->Draw("colz");
    drawer::drawLatex(0.5,0.5,Form("track from V0"),22,0.05,1);
    c14->SaveAs(Form("output/%s/%s/v0check/beta_pin.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c15 = new TCanvas("c15","c15",800,600);
    c15->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    beta_pin_El->Draw("colz");
    drawer::drawLatex(0.5,0.5,Form("Electron from gamma"),22,0.05,1);
    c15->SaveAs(Form("output/%s/%s/v0check/beta_pin_El.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c16 = new TCanvas("c16","c16",800,600);
    c16->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    beta_pin_Pi->Draw("colz");
    drawer::drawLatex(0.5,0.5,Form("Pion from K0s"),22,0.05,1);
    c16->SaveAs(Form("output/%s/%s/v0check/beta_pin_Pi.pdf",dataname.c_str(),ischeck.c_str()));

    TCanvas* c17 = new TCanvas("c17","c17",800,600);
    c17->cd();
    gPad->SetLogz();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    beta_pin_Pr->Draw("colz");
    drawer::drawLatex(0.5,0.5,Form("Proton from Lambda/AntiLambda"),22,0.05,1);
    c17->SaveAs(Form("output/%s/%s/v0check/beta_pin_Pr.pdf",dataname.c_str(),ischeck.c_str()));




};

void getusedhisto(TFile* data){
    //intialize v0 histogram in data
    TH2* hMassGamma = (TH2*)data->Get("v0-selector/hMassGamma");
    TH2* hMassK0S = (TH2*)data->Get("v0-selector/hMassK0S");
    TH2* hMassLambda = (TH2*)data->Get("v0-selector/hMassLambda");
    TH2* hMassAntiLambda = (TH2*)data->Get("v0-selector/hMassAntiLambda");
    APplot = (TH2*)data->Get("v0-selector/hV0APplot");
    Dedx_pin = (TH2*)data->Get("track-pid-qa/h2TPCdEdx_Pin");
    Dedx_pin_El = (TH2*)data->Get("track-pid-qa/h2TPCdEdx_Pin_El");
    Dedx_pin_Pi = (TH2*)data->Get("track-pid-qa/h2TPCdEdx_Pin_Pi");
    Dedx_pin_Pr = (TH2*)data->Get("track-pid-qa/h2TPCdEdx_Pin_Pr");
    beta_pin = (TH2*)data->Get("track-pid-qa/h2TOFbeta_Pin");
    beta_pin_El = (TH2*)data->Get("track-pid-qa/h2TOFbeta_Pin_El");
    beta_pin_Pi = (TH2*)data->Get("track-pid-qa/h2TOFbeta_Pin_Pi");
    beta_pin_Pr = (TH2*)data->Get("track-pid-qa/h2TOFbeta_Pin_Pr");


    //coversion to 1D histogram
    MassGamma = (TH1*)hMassGamma->ProjectionY();
    MassK0s = (TH1*)hMassK0S->ProjectionY();
    MassLambda = (TH1*)hMassLambda->ProjectionY();
    MassAntiLambda = (TH1*)hMassAntiLambda->ProjectionY();
    NsigmaEle_pin = reader::get_histo<TH2>(reader::get_trkList(data,postcalibration::cutname[0]),"TPCnSigEle_pIN");
    NsigmaPi_pin = reader::get_histo<TH2>(reader::get_trkList(data,postcalibration::cutname[1]),"TPCnSigPi_pIN");
    NsigmaPr_pin = reader::get_histo<TH2>(reader::get_trkList(data,postcalibration::cutname[2]),"TPCnSigPr_pIN");
    NsigmaEle_eta = reader::get_histo<TH2>(reader::get_trkList(data,postcalibration::cutname[0]),"TPCnSigEl_Corr_Eta");
    NsigmaPi_eta = reader::get_histo<TH2>(reader::get_trkList(data,postcalibration::cutname[1]),"TPCnSigPi_Corr_Eta");
    NsigmaPr_eta = reader::get_histo<TH2>(reader::get_trkList(data,postcalibration::cutname[2]),"TPCnSigPr_Corr_Eta");
};

void setusedhisto(){
    //set v0 histogram style
    setter::sethistoaxis(MassGamma,"m_{#gamma} (GeV/c^{2})","Counts",1.2,1.2,0.05,0.05);
    setter::sethistostyle(MassGamma,1,20,1,1,1,2);
    setter::sethistoranger(MassGamma,0,0.1,MassGamma->GetMinimum()*0.5,MassGamma->GetMaximum()*1.2);
    MassGamma->SetTitle("");

    setter::sethistoaxis(MassK0s,"m_{K^{0}_{S}} (GeV/c^{2})","Counts",1.2,1.2,0.05,0.05);
    setter::sethistostyle(MassK0s,1,20,1,1,1,2);
    setter::sethistoranger(MassK0s,0.4,0.6,MassK0s->GetMinimum()*0.5,MassK0s->GetMaximum()*1.2);
    MassK0s->SetTitle("");
    
    setter::sethistoaxis(MassLambda,"m_{#Lambda} (GeV/c^{2})","Counts",1.2,1.2,0.05,0.05);
    setter::sethistostyle(MassLambda,1,20,1,1,1,2);
    setter::sethistoranger(MassLambda,1.0,1.2,MassLambda->GetMinimum()*0.5,MassLambda->GetMaximum()*1.2);
    MassLambda->SetTitle("");
    
    setter::sethistoaxis(MassAntiLambda,"m_{#bar{#Lambda}} (GeV/c^{2})","Counts",1.2,1.2,0.05,0.05);
    setter::sethistostyle(MassAntiLambda,1,20,1,1,1,2);
    setter::sethistoranger(MassAntiLambda,1.0,1.2,MassAntiLambda->GetMinimum()*0.5,MassAntiLambda->GetMaximum()*1.2);
    MassAntiLambda->SetTitle("");

    APplot->SetTitle("");
    setter::sethistoaxis(APplot,"alpha","qt",1.2,1.2,0.05,0.05);

    //set nsigma histogram style
    setter::sethistoaxis(NsigmaEle_pin,"p_{IN} (GeV/c)","n#sigma_{Electron}^{TPC}",1.2,1.2,0.05,0.05);
    NsigmaEle_pin->SetTitle("");

    setter::sethistoaxis(NsigmaPi_pin,"p_{IN} (GeV/c)","n#sigma_{Pion}^{TPC}",1.2,1.2,0.05,0.05);
    NsigmaPi_pin->SetTitle("");

    setter::sethistoaxis(NsigmaPr_pin,"p_{IN} (GeV/c)","n#sigma_{Proton}^{TPC}",1.2,1.2,0.05,0.05);
    NsigmaPr_pin->SetTitle("");

    setter::sethistoaxis(NsigmaEle_eta,"#eta","n#sigma_{Electron}^{TPC}",1.2,1.2,0.05,0.05);
    NsigmaEle_eta->SetTitle("");

    setter::sethistoaxis(NsigmaPi_eta,"#eta","n#sigma_{Pion}^{TPC}",1.2,1.2,0.05,0.05);
    NsigmaPi_eta->SetTitle("");

    setter::sethistoaxis(NsigmaPr_eta,"#eta","n#sigma_{Proton}^{TPC}",1.2,1.2,0.05,0.05);
    NsigmaPr_eta->SetTitle("");

    //set dedx histogram style
    setter::sethistoaxis(Dedx_pin,"p_{IN} (GeV/c)","dE/dx (a.u.)",1.2,1.2,0.05,0.05);
    Dedx_pin->SetTitle("");

    setter::sethistoaxis(Dedx_pin_El,"p_{IN} (GeV/c)","dE/dx (a.u.)",1.2,1.2,0.05,0.05);
    Dedx_pin_El->SetTitle("");

    setter::sethistoaxis(Dedx_pin_Pi,"p_{IN} (GeV/c)","dE/dx (a.u.)",1.2,1.2,0.05,0.05);
    Dedx_pin_Pi->SetTitle("");

    setter::sethistoaxis(Dedx_pin_Pr,"p_{IN} (GeV/c)","dE/dx (a.u.)",1.2,1.2,0.05,0.05);
    Dedx_pin_Pr->SetTitle("");

    //set beta histogram style
    setter::sethistoaxis(beta_pin,"p_{IN} (GeV/c)","#beta",1.2,1.2,0.05,0.05);
    beta_pin->SetTitle("");

    setter::sethistoaxis(beta_pin_El,"p_{IN} (GeV/c)","#beta",1.2,1.2,0.05,0.05);
    beta_pin_El->SetTitle("");

    setter::sethistoaxis(beta_pin_Pi,"p_{IN} (GeV/c)","#beta",1.2,1.2,0.05,0.05);
    beta_pin_Pi->SetTitle("");

    setter::sethistoaxis(beta_pin_Pr,"p_{IN} (GeV/c)","#beta",1.2,1.2,0.05,0.05);
    beta_pin_Pr->SetTitle("");
};