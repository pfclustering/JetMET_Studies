#include "../interface/jseCommon.h"

#include <iostream>
#include <math.h>
 
#include "TStyle.h"
#include "TColor.h"



TPaveText* jseCommon::getLabelTop( float lumi ) {

  char text[300];
  if( lumi>0. )
    sprintf( text, "%s (13 TeV)", getLumiText(lumi).c_str() );
  else
    sprintf( text, "13 TeV" );
  std::string text_str(text);
  return getLabelTop(text_str);

}


TPaveText* jseCommon::getLabelTopSimulation( float lumi ) {

  char text[300];
  sprintf( text, "CMS Simulation, %.1f fb^{-1} at #sqrt{s} = 13 TeV", lumi );
  std::string text_str(text);
  return getLabelTopSimulation(text_str);

}

TPaveText* jseCommon::getLabelTop( const std::string& text ) {

  TPaveText* label_top = new TPaveText(0.4,0.959,0.975,0.963, "brNDC");
  //  TPaveText* label_top = new TPaveText(0.4,0.953,0.975,0.975, "brNDC");
  label_top->SetBorderSize(0);
  label_top->SetFillColor(kWhite);
  label_top->SetTextSize(0.038);
  label_top->SetTextAlign(31); // align right
  label_top->SetTextFont(42);  // label_top->SetTextFont(62);
  label_top->AddText(text.c_str());

  return label_top;

}


TPaveText* jseCommon::getLabelTopSimulation( const std::string& text ) {

  TPaveText* label_top = new TPaveText(0.4,0.953,0.975,0.975, "brNDC");
  label_top->SetBorderSize(0);
  label_top->SetFillColor(kWhite);
  label_top->SetTextSize(0.038);
  label_top->SetTextAlign(31); // align right                                                                                                                                        
  label_top->SetTextFont(42);
  label_top->AddText(text.c_str());

  return label_top;

}


TPaveText* jseCommon::getLabelCMS( const std::string& text ) {

  TPaveText* label_cms = new TPaveText(0.143,0.96,0.27,0.965, "brNDC");
  label_cms->SetBorderSize(0);
  label_cms->SetFillColor(kWhite);
  label_cms->SetTextSize(0.042);
  label_cms->SetTextAlign(11); // align left
  label_cms->SetTextFont(42);
  label_cms->AddText( text.c_str() );

  return label_cms;

}


std::string jseCommon::getLumiText( float lumi ) {

  std::string returnText;
  if( lumi>=1.0 )
    returnText = (std::string)Form(" %.1f fb^{-1}", lumi);
  else if( lumi>0.01 )
    returnText = (std::string)Form(" %.0f pb^{-1}", 1000.*lumi);
  else 
    returnText = (std::string)Form(" %.1f pb^{-1}", 1000.*lumi);

  return returnText;

}


std::vector<float> jseCommon::ptBins() {

  std::vector<float> ptBins;

  ptBins.push_back( 20. );
  ptBins.push_back( 25. );
  ptBins.push_back( 30. );
  ptBins.push_back( 40. );
  ptBins.push_back( 55. );
  ptBins.push_back( 80. );
  ptBins.push_back( 130. );
  ptBins.push_back( 250. );
  ptBins.push_back( 500. );
  ptBins.push_back( 13000. );

  return ptBins;

}


std::vector<float> jseCommon::etaBins() {

  std::vector<float> etaBins;

  etaBins.push_back( -3.2 );
  etaBins.push_back( -2.8 );
  etaBins.push_back( -2.5 );
  etaBins.push_back( -2.2 );
  etaBins.push_back( -2.0 );
  etaBins.push_back( -1.6 );
  etaBins.push_back( -1.4 );
  etaBins.push_back( -1.0 );
  etaBins.push_back( -0.5 );
  etaBins.push_back(  0.0 );

  jseCommon::symmetrize(etaBins);

  return etaBins;

}


std::vector<float> jseCommon::phiBins() {

  std::vector<float> phiBins;

  for( float phi=0.; phi<3.2; phi+=0.2 ){
    phiBins.push_back( phi );
  }

  return phiBins;

}


void jseCommon::symmetrize( std::vector<float>& vec ) {

  std::vector<float> initialVector = vec;

  while( initialVector.size()>0 ) {

    float lastOne = initialVector.back();
    if( lastOne != 0. )
      vec.push_back( -lastOne );

    initialVector.pop_back();

  } // while

}


//std::string jseCommon::getHistoName( const std::string& name, float ptMin, float ptMax, float etaMin, float etaMax ) {
//
//  return std::string( Form( "%s_pt%dto%d_eta%sto%s", name.c_str(), (int)ptMin, (int)ptMax, jseCommon::etaText(etaMin).c_str(), jseCommon::etaText(etaMax).c_str()) );
//
//}


std::string jseCommon::getHistoName( const std::string& name, int ptBin, int etaBin ) {

  return std::string( Form( "%s_pt%d_eta%d", name.c_str(), ptBin, etaBin) );

}


std::string jseCommon::getHistoName( const std::string& name, int ptBin, int etaBin, int phiBin ) {

  return std::string( Form( "%s_pt%d_eta%d_phi%d", name.c_str(), ptBin, etaBin, phiBin) );

}



std::string jseCommon::getProfileVsEtaName( const std::string& name, int ptBin ) {

  return std::string( Form( "%s_vs_eta_pt%d", name.c_str(), ptBin) );

}



std::string jseCommon::etaText( float eta ) {

  float fractpart, intpart;
  fractpart = modf ( fabs(eta) , &intpart);

  return std::string( Form( "%dp%d", (int)intpart, (int)(100.*fractpart)) );

}


std::string jseCommon::findHistoName( const std::string& name, float pt, float eta ) {

  std::vector<float> ptBins  = jseCommon::ptBins();
  std::vector<float> etaBins = jseCommon::etaBins();

  int ptBin  = findBin( pt , ptBins );
  int etaBin = findBin( eta, etaBins, false );

  return jseCommon::getHistoName( name, ptBin, etaBin );
   
}


std::string jseCommon::findHistoName( const std::string& name, float pt, float eta, float phi ) {

  std::vector<float> ptBins  = jseCommon::ptBins();
  std::vector<float> etaBins = jseCommon::etaBins();
  std::vector<float> phiBins = jseCommon::phiBins();

  int ptBin  = findBin( pt , ptBins );
  int etaBin = findBin( eta, etaBins, false );
  int phiBin = findBin( phi, phiBins, false );

  return jseCommon::getHistoName( name, ptBin, etaBin, phiBin );
   
}


//std::string jseCommon::findHistoName( const std::string& name, float pt, float eta ) {
//
//  std::vector<float> ptBins  = jseCommon::ptBins();
//  std::vector<float> etaBins = jseCommon::etaBins();
//
//  int ptBin  = findBin( pt , ptBins );
//  int etaBin = findBin( eta, etaBins, false );
//
//  return jseCommon::getHistoName( name, ptBins[ptBin], ptBins[ptBin+1], etaBins[etaBin], etaBins[etaBin+1] );
//   
//}


int jseCommon::findBin( float value, const std::vector<float> bins, bool overflow ) {

  int iBin=-1;
  int lastBin = bins.size()-1;

  if( overflow && value>bins[lastBin] ) {
    
    iBin = lastBin;

  } else {

    for( unsigned i=0; i<bins.size()-1; ++i ) {

      if( value>=bins[i] && value<bins[i+1] ) {
        iBin = i;
        break;
      }

    } // for bins

  }

  return iBin;

}


std::vector<int> jseCommon::colors() {

  std::vector<int> colors;
  
  colors.push_back( 38 );
  colors.push_back( 46 );
  colors.push_back( 29 );
  colors.push_back( kGray+1 );

  return colors;

}


void jseCommon::setStyle() {

  // set the TStyle
  TStyle* style = new TStyle("DrawBaseStyle", "");
  style->SetCanvasColor(0);
  style->SetPadColor(0);
  style->SetFrameFillColor(0);
  style->SetStatColor(0);
  style->SetOptStat(0);
  style->SetOptFit(0);
  style->SetTitleFillColor(0);
  style->SetCanvasBorderMode(0);
  style->SetPadBorderMode(0);
  style->SetFrameBorderMode(0);
  style->SetPadBottomMargin(0.12);
  style->SetPadLeftMargin(0.12);
  style->cd();
  // For the canvas:
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(kWhite);
  style->SetCanvasDefH(600); //Height of canvas
  style->SetCanvasDefW(600); //Width of canvas
  style->SetCanvasDefX(0); //POsition on screen
  style->SetCanvasDefY(0);
  // For the Pad:
  style->SetPadBorderMode(0);
  style->SetPadColor(kWhite);
  style->SetPadGridX(false);
  style->SetPadGridY(false);
  style->SetGridColor(0);
  style->SetGridStyle(3);
  style->SetGridWidth(1);
  // For the frame:
  style->SetFrameBorderMode(0);
  style->SetFrameBorderSize(1);
  style->SetFrameFillColor(0);
  style->SetFrameFillStyle(0);
  style->SetFrameLineColor(1);
  style->SetFrameLineStyle(1);
  style->SetFrameLineWidth(1);
  // Margins:
  style->SetPadTopMargin(0.05);
  style->SetPadBottomMargin(0.15);//0.13);
  style->SetPadLeftMargin(0.15);//0.16);
  style->SetPadRightMargin(0.05);//0.02);
  // For the Global title:
  style->SetOptTitle(0);
  style->SetTitleFont(42);
  style->SetTitleColor(1);
  style->SetTitleTextColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleFontSize(0.05);
  // For the axis titles:
  style->SetTitleColor(1, "XYZ");
  style->SetTitleFont(42, "XYZ");
  style->SetTitleSize(0.05, "XYZ");
  style->SetTitleXOffset(1.15);//0.9);
  style->SetTitleYOffset(1.5); // => 1.15 if exponents
  // For the axis labels:
  style->SetLabelColor(1, "XYZ");
  style->SetLabelFont(42, "XYZ");
  style->SetLabelOffset(0.007, "XYZ");
  style->SetLabelSize(0.045, "XYZ");
  // For the axis:
  style->SetAxisColor(1, "XYZ");
  style->SetStripDecimals(kTRUE);
  style->SetTickLength(0.03, "XYZ");
  style->SetNdivisions(510, "XYZ");
  style->SetPadTickX(1); // To get tick marks on the opposite side of the frame
  style->SetPadTickY(1);
  // for histograms:
  style->SetHistLineColor(1);
  // for the pallete
  Double_t stops[5] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red  [5] = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[5] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue [5] = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(5, stops, red, green, blue, 100);
  style->SetNumberContours(100);

  style->cd();

}

