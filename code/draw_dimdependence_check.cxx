#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TH3.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
TLatex* drawLatex(double, double, char*, int,double,int);
TH1F* sethistogram(TH1F*,string,string,int,int);


void draw_dimdependence_check(string datasetname){
    string nsigmaname[3]={"Electron","Pion","Proton"};
    string mothername[3]={"photo", "K0s", "Lambda"};    
    string axisname[3]={"ncls","pin","eta"};
    string pname[3]={"e","#pi","proton"};
    string axisunit[3]={};
    string axisxunit[3]={"ncls","P_{in} (GeV/c)","#eta"};
    int ndim=sizeof(axisname)/sizeof(axisname[0]);
    for (int nsigmatype=0;nsigmatype<3;nsigmatype++){
        string filename="output/"+datasetname+"/check/dimbydim/test4postcalib_"+nsigmaname[nsigmatype]+".root";
        TFile* postcalibfile = new TFile(filename.c_str());

        string checkfilename="output/"+datasetname+"/map/dimbydim/test4postcalib_"+nsigmaname[nsigmatype]+".root";
        //string checkfilename2="output/"+checkname2+"/test4postcalib_"+nsigmaname[nsigmatype]+".root";        

        TFile* checkfile = new TFile(checkfilename.c_str());
        //TFile* checkfile2 = new TFile(checkfilename2.c_str());
        TCanvas *c1 = new TCanvas("c1","c1",800,600);
        for (int idim=0;idim<ndim;idim++){
            //set y grid style
            c1->SetGridy();
            gPad->SetBottomMargin(0.13);
            gPad->SetLeftMargin(0.13);
            gPad->SetRightMargin(0.02);
            gPad->SetTopMargin(0.02);
            string histoname_mean = Form("histo4fit_mean_%s",axisname[idim].c_str());
            TH1F* test= (TH1F*)postcalibfile->Get(histoname_mean.c_str());
            TH1F* checkwith = (TH1F*)checkfile->Get(histoname_mean.c_str());
             //TH1F* checkwith2 = (TH1F*)checkfile2->Get(histoname_mean.c_str());
            TH1F* testafter = sethistogram(test,Form("%s",axisxunit[idim].c_str()),Form("n#sigma^{TPC}_{%s} mean",pname[nsigmatype].c_str()),1,2);

            TH1F* checkafter = sethistogram(checkwith,axisname[idim],"TPCnSigma mean",1,4);
             //TH1F* checkafter2 = sethistogram(checkwith2,axisname[idim],"TPCnSigma mean",1,2);
            testafter->Draw();
            //h1_check[nsigmatype]->Draw("same");
            checkafter->Draw("same");
             //checkafter2->Draw("same");
            //
            TLegend *leg = new TLegend(0.6,0.7,0.8,0.85);
            leg->SetTextFont(42);
            leg->SetTextSize(0.06);
            leg->AddEntry(testafter,"after correction","P");
            leg->AddEntry(checkafter,"before correction","P");    
            leg->SetFillStyle(0);
            leg->SetBorderSize(0);
            leg->Draw();
            TLatex* latex = drawLatex(0.2,0.90,Form("%s (Run3), pp@13.6 TeV",datasetname.c_str()),22,0.06,1);
            string savepdfname = "output/"+datasetname+"/check/dimbydim/"+nsigmaname[nsigmatype]+"_mean_check_"+axisname[idim]+".pdf";
            c1->SaveAs(savepdfname.c_str());
            test->Reset();
            testafter->Reset();
            checkwith->Reset();
            checkafter->Reset();
            c1->Clear();
        }	

        for (int idim=0;idim<ndim;idim++){
            //set y grid style
            c1->SetGridy();
            string histoname_mean = Form("histo4fit_sigma_%s",axisname[idim].c_str());
            TH1F* test= (TH1F*)postcalibfile->Get(histoname_mean.c_str());
            TH1F* testafter = sethistogram(test,Form("%s",axisxunit[idim].c_str()),Form("n#sigma^{TPC}_{%s} sigma",pname[nsigmatype].c_str()),0,2);
            //
            TH1F* checkwith = (TH1F*)checkfile->Get(histoname_mean.c_str());
            TH1F* checkafter = sethistogram(checkwith,axisname[idim],"TPCnSigma sigma",0,4);
            //
            testafter->Draw();
            checkafter->Draw("same");

            TLegend *leg = new TLegend(0.6,0.7,0.8,0.85);
            leg->SetTextFont(42);
            leg->SetTextSize(0.06);
            leg->AddEntry(testafter,"after correction","P");
            leg->AddEntry(checkafter,"before correction","P");    
            leg->SetFillStyle(0);
            leg->SetBorderSize(0);
            leg->Draw();
             TLatex* latex = drawLatex(0.2,0.90,Form("%s (Run3), pp@13.6 TeV",datasetname.c_str()),22,0.06,1);
            string savepdfname = "output/"+datasetname+"/check/dimbydim/"+nsigmaname[nsigmatype]+"_sigma_check_"+axisname[idim]+".pdf";
            c1->SaveAs(savepdfname.c_str());
            test->Reset();
            testafter->Reset();
            c1->Clear();
        }
        c1->Close();
        postcalibfile->Close();
    }

}

TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex)
{
  TLatex *latex = new TLatex(x,y,text); 
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);   
  latex->SetTextColor(colorIndex); 
  latex->Draw("same");
  return latex;
}

TH1F* sethistogram(TH1F* objecthisto,string xaxisname,string yaxisname,int ismean,int color){
    objecthisto->SetStats(0);
    if (ismean==1){
        objecthisto->GetYaxis()->SetRangeUser(-1,3);
    }
    else{
    objecthisto->GetYaxis()->SetRangeUser(0.5,2);
    }
    objecthisto->GetXaxis()->SetTitle(xaxisname.c_str());
    objecthisto->GetXaxis()->SetTitleSize(0.05);
    objecthisto->GetXaxis()->SetTitleOffset(1.1);
    objecthisto->GetXaxis()->SetLabelSize(0.05);
    objecthisto->GetXaxis()->SetLabelOffset(0.015);
    objecthisto->GetYaxis()->SetTitle(yaxisname.c_str());
    objecthisto->GetYaxis()->SetTitleSize(0.05);
    objecthisto->GetYaxis()->SetTitleOffset(1.1);
    objecthisto->GetYaxis()->SetLabelSize(0.05);
    objecthisto->GetYaxis()->SetLabelOffset(0.015);
    objecthisto->GetYaxis()->SetNdivisions(508);
    //objecthisto->GetYaxis()->SetMaxDigits(5);
    objecthisto->SetMarkerStyle(20);
    objecthisto->SetMarkerSize(2);
    //objecthisto->SetMarkerColor(kBlack);
    objecthisto->SetLineWidth(2);
    //objecthisto->SetLineColor(kBlack);
    objecthisto->SetLineColor(color);
    objecthisto->SetMarkerColor(color);
    //dot draw title
    objecthisto->SetTitle("");
    return objecthisto;
}
