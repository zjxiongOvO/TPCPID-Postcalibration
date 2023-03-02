#include <TFile.h>
#include <TH1.h>
#include <TList.h>
#include <TLatex.h>
#include <TMath.h>
#include <TH2.h>
#include <TCanvas.h>
#include <THn.h>
#include <TLine.h>
#include <iostream>

#ifndef _ALICE_RUN3_TOOL_H_
#define _ALICE_RUN3_TOOL_H_
using namespace std;

class reader
{
    public:
        static TList* get_trkList(TFile*, string);
        static TList* get_makerList(TFile*, string);
        static TList* get_pairList(TFile*, string);
        template <class HISTO>
        static HISTO* get_histo(TList* List, string histoname);
    private:
        // THashList *HashList;
        // TList *List;
        // string Listname;
};

class drawer
{
    public:
        static TLatex* drawLatex(double, double, char*, int,double,int);
        static TLine* drawLine(double, double, double, double, int, int, int);
    private:
};

class setter
{
    public:
        static TH1* sethistoranger(TH1*, double, double, double, double);
        static TH1* sethistoaxis(TH1*, string, string, double, double, double, double);
        static TH2* sethistoaxis(TH2*, string, string, double, double, double, double);
        static TH1* sethistostyle(TH1*, int, int, int, int, int, int);
    private:
};

TList* reader::get_trkList(TFile *file,string cutname)
{
    string HashListname_trk = "analysis-track-selection/output";
    string Listtitle_trk = "TrackBarrel_";
    string listname = Listtitle_trk + cutname;
    THashList *HashList = (THashList*)file->Get(HashListname_trk.c_str());
    TList *List = (TList*)HashList->FindObject(listname.c_str());
    return List;
};

TList* reader::get_makerList(TFile *file,string cutname)
{
    string HashListname_maker = "table-maker/output";
    string Listtitle_maker = "TrackBarrel_";
    string listname = Listtitle_maker + cutname;
    THashList *HashList = (THashList*)file->Get(HashListname_maker.c_str());
    TList *List = (TList*)HashList->FindObject(listname.c_str());
    return List;
};

TList* reader::get_pairList(TFile *file,string cutname)
{
    string HashListname_pair = "analysis-same-event-pairing/output";
    string Listtitle_pair = "PairsBarrelSEPM_";
    string listname = Listtitle_pair + cutname;
    THashList *HashList = (THashList*)file->Get(HashListname_pair.c_str());
    TList *List = (TList*)HashList->FindObject(listname.c_str());
    return List;
};

template <class HISTO>
HISTO* reader::get_histo(TList* List, string histoname)
{
    HISTO *histo = (HISTO*)List->FindObject(histoname.c_str());
    return histo;
};

TLatex* drawer::drawLatex(double x, double y, char* text, int textFont, double textSize, int colorIndex)
{
  TLatex *latex = new TLatex(x,y,text); 
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);   
  latex->SetTextColor(colorIndex); 
  latex->Draw("same");
  return latex;
};

TLine* drawer::drawLine(double x1, double y1, double x2, double y2, int colorIndex, int style, int width)
{
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineColor(colorIndex);
  line->SetLineStyle(style);
  line->SetLineWidth(width);
  line->Draw("same");
  return line;
};

TH1* setter::sethistoranger(TH1* histo, double xmin, double xmax, double ymin, double ymax)
{
    histo->GetXaxis()->SetRangeUser(xmin,xmax);
    histo->GetYaxis()->SetRangeUser(ymin,ymax);
    return histo;
};

TH1* setter::sethistoaxis(TH1* histo, string xtitle, string ytitle,double xoffset, double yoffset, double xsize, double ysize)
{
    histo->GetXaxis()->SetTitle(xtitle.c_str());
    histo->GetYaxis()->SetTitle(ytitle.c_str());
    histo->GetXaxis()->SetTitleOffset(xoffset);
    histo->GetYaxis()->SetTitleOffset(yoffset);
    histo->GetXaxis()->SetTitleSize(xsize);
    histo->GetYaxis()->SetTitleSize(ysize);
    histo->GetXaxis()->SetLabelSize(0.05);
    histo->GetXaxis()->SetLabelOffset(0.015);
    histo->GetYaxis()->SetLabelSize(0.05);
    histo->GetYaxis()->SetLabelOffset(0.015);
    return histo;
};

TH2* setter::sethistoaxis(TH2* histo, string xtitle, string ytitle,double xoffset, double yoffset, double xsize, double ysize)
{
    histo->GetXaxis()->SetTitle(xtitle.c_str());
    histo->GetYaxis()->SetTitle(ytitle.c_str());
    histo->GetXaxis()->SetTitleOffset(xoffset);
    histo->GetYaxis()->SetTitleOffset(yoffset);
    histo->GetXaxis()->SetTitleSize(xsize);
    histo->GetYaxis()->SetTitleSize(ysize);
    histo->GetXaxis()->SetLabelSize(0.05);
    histo->GetXaxis()->SetLabelOffset(0.015);
    histo->GetYaxis()->SetLabelSize(0.05);
    histo->GetYaxis()->SetLabelOffset(0.015);
    return histo;
};

TH1* setter::sethistostyle(TH1* histo, int colorIndex, int markerStyle, int markerSize, int markerColor, int lineStyle, int lineWidth)
{
    histo->SetMarkerColor(colorIndex);
    histo->SetMarkerStyle(markerStyle);
    histo->SetMarkerSize(markerSize);
    histo->SetLineColor(markerColor);
    histo->SetLineStyle(lineStyle);
    histo->SetLineWidth(lineWidth);
    return histo;
};

#endif