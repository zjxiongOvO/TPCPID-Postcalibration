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
TH1F* sethistogram(TH1F*,string,string,int);


void draw_dimdependence(string datasetname,string ischeck){
    string nsigmaname[3]={"Electron","Pion","Proton"};    
    string axisname[3]={"ncls","pin","eta"};
    string axisunit[3]={};
    int ndim=sizeof(axisname)/sizeof(axisname[0]);
    for (int nsigmatype=0;nsigmatype<3;nsigmatype++){
        string filename="output/"+datasetname+"/"+ischeck+"/dimbydim/test4postcalib_"+nsigmaname[nsigmatype]+".root";
        TFile* postcalibfile = new TFile(filename.c_str());
        TCanvas *c1 = new TCanvas("c1","c1",800,600);
        for (int idim=0;idim<ndim;idim++){
            //set y grid style
            c1->SetGridy();
            string histoname_mean = Form("histo4fit_mean_%s",axisname[idim].c_str());
            TH1F* test= (TH1F*)postcalibfile->Get(histoname_mean.c_str());
            TH1F* testafter = sethistogram(test,axisname[idim],"TPCnSigma mean",1);
            testafter->Draw();
            TLatex* latex = drawLatex(0.2,0.8,Form("pp 13.6 TeV %s (Run3)",datasetname.c_str()),22,0.05,1);
            char *text = Form("%s",nsigmaname[nsigmatype].c_str());
            TLatex* latex2 = drawLatex(0.2,0.7,text,22,0.05,2);
            latex->Draw();
            string savepdfname = "output/"+datasetname+"/"+ischeck+"/dimbydim/"+nsigmaname[nsigmatype]+"_mean_"+axisname[idim]+".pdf";
            c1->SaveAs(savepdfname.c_str());
            //reset
            test->Reset();
            testafter->Reset();
            c1->Clear();
        }	

        for (int idim=0;idim<ndim;idim++){
            //set y grid style
            c1->SetGridy();
            string histoname_mean = Form("histo4fit_sigma_%s",axisname[idim].c_str());
            TH1F* test= (TH1F*)postcalibfile->Get(histoname_mean.c_str());
            TH1F* testafter = sethistogram(test,axisname[idim],"TPCnSigma sigma",0);
            testafter->Draw();
            TLatex* latex = drawLatex(0.2,0.8,Form("pp 13.6 TeV %s (Run3)",datasetname.c_str()),22,0.05,1);
            char *text = Form("%s",nsigmaname[nsigmatype].c_str());
            TLatex* latex2 = drawLatex(0.2,0.7,text,22,0.05,2);
            latex->Draw();
            string savepdfname = "output/"+datasetname+"/"+ischeck+"/dimbydim/"+nsigmaname[nsigmatype]+"_sigma_"+axisname[idim]+".pdf";
            c1->SaveAs(savepdfname.c_str());
            //reset
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

TH1F* sethistogram(TH1F* objecthisto,string xaxisname,string yaxisname,int ismean){
    objecthisto->SetStats(0);
    // if (ismean==1){
    //     objecthisto->GetYaxis()->SetRangeUser(-1,3.5);
    // }
    // else{
    // objecthisto->GetYaxis()->SetRangeUser(0,2);
    // }
    objecthisto->GetYaxis()->SetRangeUser(objecthisto->GetMinimum()-1,objecthisto->GetMaximum()+1);

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
    //dot draw title
    objecthisto->SetTitle("");
    return objecthisto;
}
