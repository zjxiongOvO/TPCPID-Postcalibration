# include "TFile.h"
# include "../dependence/AliceRun3job.h"


double nclsbinning[7]={50,70,90,110,130,140,160};
double pinbinning[7]={0.2,0.4,0.6,0.8,1.0,1.2,5.0};
double etabinning[10]={-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9};
string axisname[3]={"ncls","pin","eta"};

int nbin[3]={sizeof(nclsbinning)/sizeof(nclsbinning[0])-1,sizeof(pinbinning)/sizeof(pinbinning[0])-1,sizeof(etabinning)/sizeof(etabinning[0])-1};
int ndim=sizeof(axisname)/sizeof(axisname[0]);
double* binning[3]={nclsbinning,pinbinning,etabinning};
void get_dependence_dimbydim(string datasetname,string ischeck){
    gStyle->SetOptStat(0);
    // string datasetname="LHC22f_pass2_v2";
    system(Form("mkdir -p output/%s/%s/dimbydim",datasetname.c_str(),ischeck.c_str()));
    TFile *filename=new TFile(Form("input/verson1/%s_%s.root",datasetname.c_str(),ischeck.c_str()),"read");
    string hashlistname="analysis-track-selection/output";
    string listname[3]={"TrackBarrel_PIDCalibElectron","TrackBarrel_PIDCalibPion","TrackBarrel_PIDCalibProton"};
    string histoname[3]={"nSigmaTPCelectron","nSigmaTPCpion","nSigmaTPCproton"};
    string particle[3]={"Electron","Pion","Proton"};
    TList* pidlist[3];
    THn* nSigmaTPC[3];
    int nlist=sizeof(listname)/sizeof(listname[0]);
    for(int ilist=0;ilist<nlist;ilist++){
        pidlist[ilist]=get_List(filename,hashlistname,listname[ilist]);
        nSigmaTPC[ilist]=get_histo<THn>(pidlist[ilist],histoname[ilist]);
    }
    //standard set for all histograms##########################################

    //loop over all dimensions and all particles
    for (int iparticle=0;iparticle<nlist;iparticle++){
        string outputfilename="output/"+datasetname+"/"+ischeck+"/dimbydim/test4postcalib_"+particle[iparticle]+".root";
        TFile* outputfile = new TFile(outputfilename.c_str(),"RECREATE");
        TH1F *histo4fit_mean[ndim];
        TH1F *histo4fit_sigma[ndim];
        TCanvas *c1 = new TCanvas("c1","c1",1600,1600);
        for (int idim=0;idim<ndim;idim++){
            c1->Divide(3,3);
            string histoname_mean="histo4fit_mean_"+axisname[idim];
            string histoname_sigma="histo4fit_sigma_"+axisname[idim];
            histo4fit_mean[idim]=new TH1F(histoname_mean.c_str(),histoname_mean.c_str(),nbin[idim],binning[idim]);
            histo4fit_sigma[idim]=new TH1F(histoname_sigma.c_str(),histoname_sigma.c_str(),nbin[idim],binning[idim]);
            //loop over all bins of the dimension
            double pid_mean3[4]={2,0,-4,0};
            double pid_sigma3[4]={1,0,1,0};
            double sig_limit[2]={-2,4}; //{-1.5,1.5}; for 22f fit range is -6 to 4
            double bkg_limit[2]={-99,-2};//{-99,-4} for 22f
            if (ischeck=="check"){
                pid_mean3[0]=0;
                pid_mean3[2]=-3;
                sig_limit[0]=-1;
                sig_limit[1]=1;
            }
            for (int ibin=0;ibin<nbin[idim];ibin++){
                c1->cd(ibin+1);
                gPad->SetLeftMargin(0.14);
                gPad->SetRightMargin(0.05);
                gPad->SetBottomMargin(0.14);

                //get the histogram for the current bin
                double lowedge=binning[idim][ibin];
                double highedge=binning[idim][ibin+1];
                THn* testhisto_clone = (THn*)nSigmaTPC[iparticle]->Clone("testhisto_clone");
                testhisto_clone->SetName(Form("testhisto_clone_%d",ibin));
                testhisto_clone->GetAxis(idim+1)->SetRangeUser(lowedge,highedge);
                //project the histogram to 1D
                TH1D* testhisto = testhisto_clone->Projection(0);
                testhisto->SetName(Form("nsigmahisto_%d",ibin));
                //just for fix the bug of Nsigma = 0 
                //get the max bin
                int maxbin = testhisto->GetMaximumBin();
                //set this bin to null
                // testhisto->SetBinContent(maxbin,(testhisto->GetBinContent(maxbin-1)+testhisto->GetBinContent(maxbin+1))/2);
                // testhisto->SetBinContent(maxbin,0);
                // testhisto->SetBinError(maxbin,0);                

                //
                testhisto->GetXaxis()->SetTitle(Form("n#sigma^{TPC}_{%s}",particle[iparticle].c_str()));
                testhisto->GetYaxis()->SetTitle("counts");
                testhisto->SetTitle("");
                testhisto->Draw();
                //fit the histogram
                if (iparticle==0){
                    //use double gaussian for electrons
                    // fit_double_gaus_v2(testhisto,pid_mean3,pid_sigma3,sig_limit,bkg_limit);
                        fit_single_gaus(testhisto,pid_mean3,pid_sigma3);
                }
                    else{
                        fit_single_gaus(testhisto,pid_mean3,pid_sigma3);
                    }
                //store the fit parameters
                histo4fit_mean[idim]->SetBinContent(ibin+1,pid_mean3[0]);
                histo4fit_mean[idim]->SetBinError(ibin+1,pid_mean3[1]);
                // histo4fit_mean[idim]->SetBinError(ibin+1,0);
                histo4fit_sigma[idim]->SetBinContent(ibin+1,pid_sigma3[0]);
                histo4fit_sigma[idim]->SetBinError(ibin+1,pid_sigma3[1]);
                //draw other information
                drawLatex(0.2,0.8,Form("mean=%.2f",pid_mean3[0]),22,0.05,1);
                drawLatex(0.2,0.7,Form("sigma=%.2f",pid_sigma3[0]),22,0.05,1);
                drawLatex(0.2,0.6,Form("%.1f < %s < %.1f",binning[idim][ibin],axisname[idim].c_str(),binning[idim][ibin+1]),22,0.05,1);
            }
            //store histograms to root file
            outputfile->cd();
            histo4fit_mean[idim]->Write();
            histo4fit_sigma[idim]->Write();
            //save the canvas
            c1->SaveAs(Form("output/%s/%s/dimbydim/test4postcalib_%s_%s.pdf",datasetname.c_str(),ischeck.c_str(),particle[iparticle].c_str(),axisname[idim].c_str()));
            c1->Clear();
            //reset the root file 
        }//loop over all dimensions 
        outputfile->Close();
    } 
}
