#ifndef BINNING_H
#define BINNING_H
using namespace std;

class binning
{
    public:
        static double nclsbin[2];
        static double Pinbin[5];
        static double etabin[6];
        static int Nnclsbin;
        static int NPinbin;
        static int Netabin;
};

double binning::nclsbin[2] = {0,160};
double binning::Pinbin[5] = {0.2,0.6,0.8,1.0,5.0};
double binning::etabin[6] = {-0.9,-0.5,-0.3,0.3,0.5,0.9};
int binning::Nnclsbin=sizeof(binning::nclsbin)/sizeof(binning::nclsbin[0])-1;
int binning::NPinbin=sizeof(binning::Pinbin)/sizeof(binning::Pinbin[0])-1;
int binning::Netabin=sizeof(binning::etabin)/sizeof(binning::etabin[0])-1;
#endif