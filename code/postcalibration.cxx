#include "../dependence/AliceRun3tool.h"
#include "../dependence/AliceTPCpostcalibration.h"

void postcalibration(string dataname,string ischeck,int isfristfit){
    string filename="input/verson1/"+ dataname +"_"+ ischeck +".root"; //input file name
    //set parameter histogram and post-calibration map histogram
    TH1* histo;
    THn* objecthisto;
    THn* objecthisto_clone;
    postcalibration::get_fit_type(isfristfit);//if is first fit,the map file is not exist,so we need to create a new one
    cout<<"is frist fit: "<<postcalibration::isfristfit<<endl;

    system(Form("mkdir -p output/%s/%s/binbybin/fitdetail",dataname.c_str(),ischeck.c_str()));
    TFile* file = new TFile(Form("output/%s/%s/binbybin/postcalibration_%s.root",dataname.c_str(),ischeck.c_str(),dataname.c_str()),"RECREATE");

    TFile* parameter_file;
    if (postcalibration::isfristfit==1){
        parameter_file = new TFile(Form("output/%s/%s/binbybin/parameter.root",dataname.c_str(),ischeck.c_str()),"RECREATE");
    }
    else {
        parameter_file = new TFile(Form("output/%s/%s/binbybin/parameter.root",dataname.c_str(),ischeck.c_str()),"READ");
    }
    // cout<<"open file: "<<file<<endl;
    cout<<"parameter file:"<<parameter_file<<endl;

    //_______________________________________________________________________________________________________________________

    for (int iparticle=0;iparticle<3;iparticle++){
        //get THn from file
        objecthisto = postcalibration::calibration(filename,iparticle);
        //get parameter histogram
        postcalibration::get_parameter_histo(parameter_file,iparticle);
        cout<<postcalibration::fit_parameter_signal_mean->GetNbinsX()<<endl;
        //set canvas
        TCanvas* c1 = new TCanvas("c1","c1",800*postcalibration::NPinbin,600*postcalibration::Netabin);

        TCanvas* c2 = new TCanvas("c2","c2",800,600);

        //set TH3F to store post-calibration map
        TH3F* mean_map = new TH3F(Form("mean_map_%s",postcalibration::pairticle_name[iparticle].c_str()),Form("mean_map_%s",postcalibration::pairticle_name[iparticle].c_str()),
                            postcalibration::Nnclsbin,postcalibration::nclsbin,postcalibration::NPinbin,postcalibration::Pinbin,postcalibration::Netabin,postcalibration::etabin);
        TH3F* sigma_map = new TH3F(Form("sigma_map_%s",postcalibration::pairticle_name[iparticle].c_str()),Form("sigma_map_%s",postcalibration::pairticle_name[iparticle].c_str()),
                            postcalibration::Nnclsbin,postcalibration::nclsbin,postcalibration::NPinbin,postcalibration::Pinbin,postcalibration::Netabin,postcalibration::etabin);
        
        for (int incls=0;incls<postcalibration::Nnclsbin;incls++){
            c1->Divide(postcalibration::NPinbin,postcalibration::Netabin);
            for (int ipin=0;ipin<postcalibration::NPinbin;ipin++){
                for (int ieta=0;ieta<postcalibration::Netabin;ieta++){
                    c1->cd(postcalibration::NPinbin*ieta+ipin+1);
                    gPad->SetLeftMargin(0.15);
                    gPad->SetBottomMargin(0.15);
                    double vars_low[3] = {postcalibration::nclsbin[incls],postcalibration::Pinbin[ipin],postcalibration::etabin[ieta]};
                    double vars_high[3] = {postcalibration::nclsbin[incls+1],postcalibration::Pinbin[ipin+1],postcalibration::etabin[ieta+1]};
                    //set fit parameter
                    postcalibration::set_fit_parameter(incls,ipin,ieta,iparticle);
                    //get TH1 from THn
                    objecthisto_clone = (THn*)objecthisto->Clone();
                    histo = postcalibration::projection(objecthisto_clone,vars_low,vars_high);
                    histo->SetName(Form("histo_%d_%d_%d",incls,ipin,ieta));
                    //just for fix the bug of Nsigma = 0 
                    //get the max bin
                    // int maxbin = histo->GetMaximumBin();
                    // histo->SetBinContent(maxbin,(histo->GetBinContent(maxbin-1)+histo->GetBinContent(maxbin+1))/2);
                    //
                    histo->Draw("");
                    postcalibration::standard_histo_set(histo,iparticle); 
                    //fit
                    // postcalibration::standardfit(histo);
                    postcalibration::fit_v1(histo,iparticle);//how to fit
                    postcalibration::standard_canva_set(iparticle,dataname,vars_low,vars_high);
                    //fill parameter histogram
                    postcalibration::fill_parameter_histo(incls,ipin,ieta);
                    //fill post-calibration map
                    mean_map->SetBinContent(incls+1,ipin+1,ieta+1,postcalibration::fitresult[0]);
                    mean_map->SetBinError(incls+1,ipin+1,ieta+1,postcalibration::fitresult[1]);
                    sigma_map->SetBinContent(incls+1,ipin+1,ieta+1,postcalibration::fitresult[2]);
                    sigma_map->SetBinError(incls+1,ipin+1,ieta+1,postcalibration::fitresult[3]);
                    //for avoid nsigmapr error
                    // if (iparticle==2){
                    //     sigma_map->SetBinContent(incls+1,ipin+1,ieta+1,1);
                    //     sigma_map->SetBinError(incls+1,ipin+1,ieta+1,0);
                    // }

                    //draw one plot
                    c2->cd();
                    gPad->SetLeftMargin(0.15);
                    gPad->SetBottomMargin(0.15);
                    histo->Draw("");
                    postcalibration::standard_histo_set(histo,iparticle);
                    postcalibration::fit_v1(histo,iparticle);//how to fit

                    postcalibration::standard_canva_set(iparticle,dataname,vars_low,vars_high);
                    c2->SaveAs(Form("output/%s/%s/binbybin/fitdetail/%s_%s_%d_%d_%d.pdf",dataname.c_str(),ischeck.c_str(),dataname.c_str(),postcalibration::pairticle_name[iparticle].c_str(),incls,ipin,ieta));
                    c2->Clear(); 
                }//loop over eta bin
            }//loop over pin bin 
            c1->SaveAs(Form("output/%s/%s/binbybin/%s_%s_%d_fitting.pdf",dataname.c_str(),ischeck.c_str(),dataname.c_str(),postcalibration::pairticle_name[iparticle].c_str(),incls));
            c1->Clear();
        }//loop over ncls bin
        //save fit result
 
        file->cd();
        mean_map->Write(Form("Postcalib_TpcnSigma_mean_%s",postcalibration::pairticle_name[iparticle].c_str()));
        sigma_map->Write(Form("Postcalib_TpcnSigma_sigma_%s",postcalibration::pairticle_name[iparticle].c_str()));
        //save parameter histogram
        postcalibration::parameter_savein(parameter_file,iparticle);
    }//loop over electron, pion, proton

}