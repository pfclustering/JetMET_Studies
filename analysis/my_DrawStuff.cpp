#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TF1.h"
#include "TPaveLabel.h"

#include "../interface/jseDataset.h"
#include "../interface/jseCommon.h"



void drawAllPlots( const std::vector< jseDataset* >& datasets, const std::string& comparisonName );
void drawPlot( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& histoName, const std::string& axisName, bool log_aussi=false );
void drawProfileVsEta( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& axisName, float yMin, float yMax, int iPt=-1 );
void drawProfileVsPhi( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& axisName, float yMin, float yMax, int iPt=-1 );

bool noEPS = false;

int main( int argc, char* argv[] ) {
  
  if( argc<2 ) {
    std::cout << "Usage: ./jseDrawStuff [productionName] [noEPS=false]" << std::endl;
    exit(1);
  }

  jseCommon::setStyle();
  std::string prodName1(argv[1]);
  std::string datasetName1(argv[2]);
  std::string prodName2(argv[3]);
  std::string datasetName2(argv[4]);
  std::string legend1(argv[5]);
  std::string legend2(argv[6]);
  std::string plotsName(argv[7]);
  

    std::vector< jseDataset* > datasets;
    datasets.push_back( new jseDataset( prodName1, datasetName1, legend1));
    datasets.push_back( new jseDataset( prodName2, datasetName2, legend2));
    
    drawAllPlots(datasets, plotsName);

  return 0;

}



void drawAllPlots( const std::vector< jseDataset* >& datasets, const std::string& comparisonName ) {

  //std::string outdir( Form("/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/%s", comparisonName.c_str()) );
  std::string outdir( Form("plots/%s", comparisonName.c_str()) );
  system( Form("mkdir -p %s", outdir.c_str()) );

  // drawPlot( outdir, datasets, "nVertex", "Number of Reconstructed Vertexes" );
  // drawPlot( outdir, datasets, "rho", "Pile Up Energy Density #rho [GeV]" );

  drawPlot( outdir, datasets, "Jet_pt", "Jet p_{T} [GeV]", true );
  drawPlot( outdir, datasets, "Jet_eta", "Jet #eta");
  drawPlot( outdir, datasets, "nVertex", "nVertex");
  drawPlot( outdir, datasets, "MET_phi", "MET_phi");
  drawPlot( outdir, datasets, "MET_pt", "MET_pt");
  drawPlot( outdir, datasets, "MET_sumEt", "MET_sumEt");

  drawProfileVsEta( outdir, datasets, "Jet_pt", "Jet p_{T} [GeV]", 0., 180);

  drawProfileVsEta( outdir, datasets, "phEF", "Jet Photon Energy Fraction"        ,   0., 0.85 );
  drawProfileVsEta( outdir, datasets, "nhEF", "Jet Neutral Hadron Energy Fraction", 0., 1  );
  drawProfileVsEta( outdir, datasets, "chEF", "Jet Charged Hadron Energy Fraction",   0., 0.85 );
  drawProfileVsEta( outdir, datasets, "chEFJerc", "Jet Charged Hadron Energy Fraction with JERC definition", 0., 0.85 );
  drawProfileVsEta( outdir, datasets, "elEF", "Jet Electron Energy Fraction",   0., 0.85 );
  drawProfileVsEta( outdir, datasets, "muEF", "Jet Muon Energy Fraction",   0., 0.85 );
  drawProfileVsEta( outdir, datasets, "totEF", "Sum of Energy Fractions in Jet",   0.8, 1.2 );

  drawProfileVsEta( outdir, datasets, "phE", "Jet Photon Energy [GeV]"            , 0.  , 100. );
  drawProfileVsEta( outdir, datasets, "nhE", "Jet Neutral Hadron Energy [GeV]"    , 0.  , 150.  );
  drawProfileVsEta( outdir, datasets, "chE", "Jet Charged Hadron Energy [GeV]"    , 0.  , 150. );
  drawProfileVsPhi( outdir, datasets, "MET_pt", "MET_pt over phi"        ,   0., 100 );
  drawProfileVsPhi( outdir, datasets, "MET_sumEt", "MET_sumEt over phi", 0., 3000 );

}



void drawPlot( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& histoName, const std::string& axisName, bool log_aussi ) {

  std::vector<int> colors = jseCommon::colors();

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TPaveText* labelTop = jseCommon::getLabelTopSimulation();
  labelTop->Draw();
  TLegend* legend = new TLegend( 0.55, 0.7, 0.8, 0.80 );
  if( histoName == "MET_phi"){
    legend = new TLegend(  0.75, 0.5, 0.9, 0.7 );
  }
  if( histoName == "Jet_eta"){
    legend = new TLegend(  0.4, 0.7, 0.6, 0.9 );
  }
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);
 
  TH1D* thisHisto = (TH1D*)datasets[0]->file->Get( histoName.c_str() );
  TH1D* thatHisto = (TH1D*)datasets[1]->file->Get( histoName.c_str() );

  TPad *pad1 = new TPad("pad1", "The pad with the function",0.,0.2,1.,1.);
  TPad *pad2 = new TPad("pad2", "The pad with the histogram",0.,0.,1.,0.20);
  
  pad1->Draw();
  pad2->Draw();

  pad1->cd();
  
  thisHisto->SetMarkerSize(1.3);
  thisHisto->SetMarkerStyle(kFullTriangleUp);
  thisHisto->SetMarkerColor(colors[0]);
  thisHisto->SetLineColor(colors[0]);
  thisHisto->SetXTitle( axisName.c_str() );
  thisHisto->SetYTitle( "Normalized to Unity" );

  thatHisto->SetMarkerSize(1.3);
  thatHisto->SetMarkerStyle(kFullTriangleUp);
  thatHisto->SetMarkerColor(colors[1]);
  thatHisto->SetLineColor(colors[1]);
  thatHisto->SetXTitle( axisName.c_str() );
  thatHisto->SetYTitle( "Normalized to Unity" );

  thisHisto->DrawNormalized("p");
  thatHisto->DrawNormalized("p same");

  //thisHisto->Draw("p");
  //thatHisto->Draw("p same");

  pad2->cd();
  TH1D *rp = (TH1D*)thisHisto->Clone("rp");
  float nentries1 = thisHisto->GetEntries();
  float nentries2 = thatHisto->GetEntries();
  rp->Divide(thatHisto);
  rp->SetMarkerSize(0.5);
  rp->SetMarkerStyle(20);
  rp->SetMarkerColor(1);
  rp->SetLineColor(0);
  rp->Scale(nentries2/nentries1);
  rp->SetMinimum(0.);
  rp->SetMaximum(2.);
  pad2->SetGridy();
  rp->Draw("e p"); 

  rp->GetYaxis()->SetTitle("            ");
  rp->GetYaxis()->SetNdivisions(505);
  rp->GetYaxis()->SetTitleSize(20);
  rp->GetYaxis()->SetTitleFont(43);
  rp->GetYaxis()->SetTitleOffset(1.55);
  rp->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  rp->GetYaxis()->SetLabelSize(15);

  c1->cd();

  legend->AddEntry( thisHisto, datasets[0]->prettyName.c_str() );
  legend->AddEntry( thatHisto, datasets[1]->prettyName.c_str() );

  legend->Draw("same");

  if( log_aussi ) {
    pad1->SetLogy();}

  pad1->RedrawAxis();
 
  
  c1->SaveAs( Form("%s/%s.png", outdir.c_str(), histoName.c_str()) );
  
  delete c1;

}


void drawProfileVsPhi( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& axisName, float yMin, float yMax, int iPt ) {


  std::vector<int> colors = jseCommon::colors();
  std::vector<float> ptBins  = jseCommon::ptBins();


  std::string profileName;
  profileName = std::string(Form("%s_vs_phi", varName.c_str()));

 
  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, -3.2, 3.2, 10, yMin, yMax );
  h2_axes->SetXTitle("MET #phi");
  h2_axes->SetYTitle(axisName.c_str());
  h2_axes->Draw();

  TPaveText* labelTop = jseCommon::getLabelTopSimulation();
  labelTop->Draw("same");

  float xMin_leg = 0.35;
  float yMin_leg = 0.20;
  float xMax_leg = 0.70;
  float yMax_leg = 0.30;

  TLegend* legend = new TLegend( xMin_leg, yMin_leg, xMax_leg, yMax_leg );
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);

  for( unsigned i=0; i<datasets.size(); ++i ) {

    TProfile* thisProfile = (TProfile*)datasets[i]->file->Get( profileName.c_str() );
    thisProfile->SetMarkerSize(1.3);
    thisProfile->SetMarkerStyle(kFullTriangleUp);
    thisProfile->SetMarkerColor(colors[i]);
    thisProfile->SetLineColor(colors[i]);

    thisProfile->Draw("p same");
    legend->AddEntry( thisProfile, datasets[i]->prettyName.c_str() );

  } // for datasets

  legend->Draw("same");

  gPad->RedrawAxis();
 
  
  c1->SaveAs( Form("%s/%s.png", outdir.c_str(), profileName.c_str()) );

  delete c1;
  delete h2_axes;

}



void drawProfileVsEta( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& axisName, float yMin, float yMax, int iPt ) {


  std::vector<int> colors = jseCommon::colors();
  std::vector<float> ptBins  = jseCommon::ptBins();


  std::string profileName;
  if( iPt>=0 ) {
    profileName = std::string(Form("%s_vs_eta_pt%d", varName.c_str(), iPt));
  } else {
    profileName = std::string(Form("%s_vs_eta", varName.c_str()));
  }

 
  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, -5., 5., 10, yMin, yMax );
  h2_axes->SetXTitle("Jet #eta");
  h2_axes->SetYTitle(axisName.c_str());
  h2_axes->Draw();

  TPaveText* labelTop = jseCommon::getLabelTopSimulation();
  labelTop->Draw("same");
  
  std::string ptText;
  ptText = std::string( Form("p_{T} > %.0f GeV", ptBins[0]) );

  float xMin_leg = 0.35;
  float yMin_leg = 0.20;
  float xMax_leg = 0.70;
  float yMax_leg = 0.30;

  if( varName == "nhE" || varName == "nhEF" || varName=="phEF" || varName=="totEF" || varName=="phE" || varName=="nhE" ) {

    xMin_leg = 0.35;
    yMin_leg = 0.70;
    xMax_leg = 0.70;
    yMax_leg = 0.82;
 
  }


  TLegend* legend = new TLegend( xMin_leg, yMin_leg, xMax_leg, yMax_leg );
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);

  for( unsigned i=0; i<datasets.size(); ++i ) {

    TProfile* thisProfile = (TProfile*)datasets[i]->file->Get( profileName.c_str() );
    thisProfile->SetMarkerSize(1.3);
    thisProfile->SetMarkerStyle(kFullTriangleUp);
    thisProfile->SetMarkerColor(colors[i]);
    thisProfile->SetLineColor(colors[i]);

    thisProfile->Draw("p same ");
    legend->AddEntry( thisProfile, datasets[i]->prettyName.c_str() );

  } // for datasets

  legend->Draw("same");

  gPad->RedrawAxis();
 
  
  c1->SaveAs( Form("%s/%s.png", outdir.c_str(), profileName.c_str()) );

  delete c1;
  delete h2_axes;

}
