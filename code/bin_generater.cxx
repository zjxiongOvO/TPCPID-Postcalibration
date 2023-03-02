#include <iostream>
#include <fstream>
#include <string>

void bin_generater(std::string dataname){
    std::string filename="output/"+ dataname +"/binning.h";
    //if this file is exist,continue
    std::ifstream file(filename);
    if (file){
        std::cout<<"file is exist,continue"<<std::endl;
        return;
    }
    //if this file is not exist,create a new one
    system(Form("cp dependence/binning.h output/%s/.",dataname.c_str()));
}