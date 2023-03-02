#include "../dependence/AliceRun3tool.h"
#include "../dependence/AliceTPCpostcalibration.h"

TH2F* mean_map[3]={nullptr};
TH2F* sigma_map[3]={nullptr};
TH3F* mean_map_3d[3]={nullptr};
TH3F* sigma_map_3d[3]={nullptr};

void drawmap(string map,string ischeck){
    gStyle->SetOptStat(0);
    // string map = "LHC22f_pass2_v2";
    TFile* file = new TFile(Form("output/%s/%s/binbybin/postcalibration_%s.root",map.c_str(),ischeck.c_str(),map.c_str()),"READ");
    for (int iparticle=0;iparticle<3;iparticle++){
        mean_map_3d[iparticle] = (TH3F*)file->Get(Form("Postcalib_TpcnSigma_mean_%s",postcalibration::pairticle_name[iparticle].c_str()));
        sigma_map_3d[iparticle] = (TH3F*)file->Get(Form("Postcalib_TpcnSigma_sigma_%s",postcalibration::pairticle_name[iparticle].c_str()));
        //project to 2d
        mean_map[iparticle] = (TH2F*)mean_map_3d[iparticle]->Project3D("zy");
        sigma_map[iparticle] = (TH2F*)sigma_map_3d[iparticle]->Project3D("zy");
        mean_map[iparticle]->SetTitle("");
        sigma_map[iparticle]->SetTitle("");
    }

    //set histogram
    for (int iparticle=0;iparticle<3;iparticle++){
        setter::sethistoaxis(mean_map[iparticle],"P_{in} (GeV/c)","#eta",1.2,1.2,0.05,0.05);
        setter::sethistoaxis(sigma_map[iparticle],"P_{in} (GeV/c)","#eta",1.2,1.2,0.05,0.05);
    }

    TCanvas* c1 = new TCanvas("c1","c1",800,600);
    for (int iparticle=0;iparticle<3;iparticle++){
        c1->cd();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.15);
        mean_map[iparticle]->Draw("colz");
        c1->SaveAs(Form("output/%s/%s/binbybin/TPCnSigma_%s_mean.pdf",map.c_str(),ischeck.c_str(),postcalibration::pairticle_name[iparticle].c_str()));
    }

    TCanvas* c2 = new TCanvas("c2","c2",800,600);
    for (int iparticle=0;iparticle<3;iparticle++){
        c2->cd();
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.15);
        sigma_map[iparticle]->Draw("colz");
        c2->SaveAs(Form("output/%s/%s/binbybin/TPCnSigma_%s_width.pdf",map.c_str(),ischeck.c_str(),postcalibration::pairticle_name[iparticle].c_str()));
    }
}
