#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TF1.h"

#include "../interface/jseDataset.h"
#include "../interface/jseCommon.h"



void drawAllPlots( const std::vector< jseDataset* >& datasets, const std::string& comparisonName );
void drawPlot( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& histoName, const std::string& sorting, const std::string& axisName, bool log_aussi=false );
void drawProfileVsEta( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& sorting, const std::string& axisName, float yMin, float yMax, int iPt=-1 );
void drawProfileVsPhi( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& sorting, const std::string& axisName, float yMin, float yMax, int iPt=-1 );
void drawResponseResolution( const std::string& outdir, const std::vector<jseDataset*>& datasets, const std::string& suffix="" );
TF1* fitResponse( TH1D* h1 );
std::vector<TH1D*> getHistoVector( const std::string& name, const std::vector<float>& ptBins, const std::vector<float>& etaBins );
TH1D* getTruncatedHisto( TH1D* h1, float frac );


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

  if( argc>8 ) {
    std::string noEPS_str(argv[2]);
    if( noEPS_str=="true" || noEPS_str=="True" || noEPS_str=="TRUE" || noEPS_str=="noEPS" || noEPS_str=="fast" ) {
      noEPS=true;
      std::cout << "-> Will not save eps files, only PDF." << std::endl;
    }
  }

    std::vector< jseDataset* > datasets;
    datasets.push_back( new jseDataset( prodName1, datasetName1, legend1));
    datasets.push_back( new jseDataset( prodName2, datasetName2, legend2));
    
    drawAllPlots(datasets, plotsName);

  return 0;

}



void drawAllPlots( const std::vector< jseDataset* >& datasets, const std::string& comparisonName ) {

  std::string outdir( Form("/eos/project/e/ecaldpg/www/JetMET_validation/mguillot/plots/%s", comparisonName.c_str()) );
  system( Form("mkdir -p %s", outdir.c_str()) );

  // drawPlot( outdir, datasets, "nVertex", "Number of Reconstructed Vertexes" );
  // drawPlot( outdir, datasets, "rho", "Pile Up Energy Density #rho [GeV]" );

  drawPlot( outdir, datasets, "Jet_pt", "Jet p_{T} [GeV]", "Jet.1" , true );
  drawPlot( outdir, datasets, "Jet_eta", "Jet #eta","Jet.2" );
  drawPlot( outdir, datasets, "MET_phi", "MET_phi","MET.1");
  drawPlot( outdir, datasets, "MET_pt", "MET_pt","MET.2");
  drawPlot( outdir, datasets, "MET_sumEt", "MET_sumEt","MET.3");

  drawProfileVsEta( outdir, datasets, "phEF","Jet.3" , "Jet Photon Energy Fraction"        ,   0., 0.85 );
  drawProfileVsEta( outdir, datasets, "nhEF","Jet.4" , "Jet Neutral Hadron Energy Fraction", -0.1, 0.5  );
  drawProfileVsEta( outdir, datasets, "chEF","Jet.5" , "Jet Charged Hadron Energy Fraction",   0., 0.85 );

  drawProfileVsEta( outdir, datasets, "phE","Jet.6" , "Jet Photon Energy [GeV]"            , 0.  , 100. );
  drawProfileVsEta( outdir, datasets, "nhE","Jet.7" , "Jet Neutral Hadron Energy [GeV]"    , 0.  , 50.  );
  drawProfileVsEta( outdir, datasets, "chE","Jet.8" , "Jet Charged Hadron Energy [GeV]"    , 0.  , 150. );
  drawProfileVsPhi( outdir, datasets, "MET_pt","MET.4", "MET_pt over phi"        ,   0., 50 );
  drawProfileVsPhi( outdir, datasets, "MET_sumEt","MET.5", "MET_sumEt over phi", 0., 1000 );

  //drawProfileVsEta( outdir, datasets, "phMult", "Jet Photon Multiplicity"         , 0.  , 25.  );
  //drawProfileVsEta( outdir, datasets, "nhMult", "Jet Neutral Hadron Multiplicity" , 0.  , 3.5  );
  // drawProfileVsEta( outdir, datasets, "chMult", "Jet Charged Hadron Multiplicity" , 0.  , 20.  );

  //std::vector<float> ptBins = jseCommon::ptBins();

  //for( unsigned iPt=0; iPt<ptBins.size()-1; ++iPt ) {

  //drawProfileVsEta( outdir, datasets, "phEF", "Jet Photon Energy Fraction"        ,   0., 0.85, iPt );
  //drawProfileVsEta( outdir, datasets, "nhEF", "Jet Neutral Hadron Energy Fraction",   0., 0.85, iPt );
  //drawProfileVsEta( outdir, datasets, "chEF", "Jet Charged Hadron Energy Fraction",   0., 0.85, iPt );
  //
  //drawProfileVsEta( outdir, datasets, "phE", "Jet Photon Energy [GeV]"            , 0.  , ptBins[iPt]   , iPt );
  //drawProfileVsEta( outdir, datasets, "nhE", "Jet Neutral Hadron Energy [GeV]"    , 0.  , TMath::Min(0.9*ptBins[iPt+1], 500.) , iPt );
  //drawProfileVsEta( outdir, datasets, "chE", "Jet Charged Hadron Energy [GeV]"    , 0.  , ptBins[iPt]   , iPt );

    //drawProfileVsEta( outdir, datasets, "phMult", "Jet Photon Multiplicity"         , 0.  , 8.5 + ptBins[iPt]/10. , iPt );
    //drawProfileVsEta( outdir, datasets, "nhMult", "Jet Neutral Hadron Multiplicity" , 0.  , 2.0 + 0.4*iPt , iPt );
    //drawProfileVsEta( outdir, datasets, "chMult", "Jet Charged Hadron Multiplicity" , 0.  , 8.0 + 2.0*iPt , iPt );

  // } // for ipt

  //drawResponseResolution( outdir, datasets );
  //drawResponseResolution( outdir, datasets, "Raw" );

}



void drawPlot( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& histoName, const std::string& axisName, const std::string& sorting, bool log_aussi ) {

  std::vector<int> colors = jseCommon::colors();

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TPaveText* labelTop = jseCommon::getLabelTopSimulation();
  labelTop->Draw();
  TLegend* legend = new TLegend( 0.65, 0.7, 0.9, 0.9 );
  if( histoName == "MET_phi"){
    legend = new TLegend(  0.65, 0.4, 0.5, 0.5 );
  }
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);

  for( unsigned i=0; i<datasets.size(); ++i ) {

    TH1D* thisHisto = (TH1D*)datasets[i]->file->Get( histoName.c_str() );
    thisHisto->SetMarkerSize(1.3);
    thisHisto->SetMarkerStyle(20);
    thisHisto->SetMarkerColor(colors[i]);
    thisHisto->SetLineColor(colors[i]);
    thisHisto->SetXTitle( axisName.c_str() );
    thisHisto->SetYTitle( "Normalized to Unity" );

    if( i==0 )
      thisHisto->DrawNormalized("p");
    else
      thisHisto->DrawNormalized("p same");
    legend->AddEntry( thisHisto, datasets[i]->prettyName.c_str() );

  } // for datasets

  legend->Draw("same");

  gPad->RedrawAxis();
 
  
  c1->SaveAs( Form("%s/%s. %s.png", outdir.c_str(), sorting.c_str(), histoName.c_str()) );
  
  delete c1;

}


void drawProfileVsPhi( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& sorting, const std::string& axisName, float yMin, float yMax, int iPt ) {


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
  float yMin_leg = 0.18;
  float xMax_leg = 0.70;
  float yMax_leg = 0.40;

  if( varName == "nhE" || varName == "phE" || varName == "nhEF" || varName == "phEF" ) {

    xMin_leg = 0.35;
    yMin_leg = 0.70;
    xMax_leg = 0.70;
    yMax_leg = 0.92;
 
  }


  TLegend* legend = new TLegend( xMin_leg, yMin_leg, xMax_leg, yMax_leg );
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);

  for( unsigned i=0; i<datasets.size(); ++i ) {

    TProfile* thisProfile = (TProfile*)datasets[i]->file->Get( profileName.c_str() );
    thisProfile->SetMarkerSize(1.3);
    thisProfile->SetMarkerStyle(20);
    thisProfile->SetMarkerColor(colors[i]);
    thisProfile->SetLineColor(colors[i]);

    thisProfile->Draw("p same");
    legend->AddEntry( thisProfile, datasets[i]->prettyName.c_str() );

  } // for datasets

  legend->Draw("same");

  gPad->RedrawAxis();
 
  
  c1->SaveAs( Form("%s/%s. %s.png", outdir.c_str(), sorting.c_str(), profileName.c_str()) );

  delete c1;
  delete h2_axes;

}



void drawProfileVsEta( const std::string& outdir, const std::vector< jseDataset* >& datasets, const std::string& varName, const std::string& sorting, const std::string& axisName, float yMin, float yMax, int iPt ) {


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

  TH2D* h2_axes = new TH2D( "axes", "", 10, -3.2, 3.2, 10, yMin, yMax );
  h2_axes->SetXTitle("Jet #eta");
  h2_axes->SetYTitle(axisName.c_str());
  h2_axes->Draw();

  TPaveText* labelTop = jseCommon::getLabelTopSimulation();
  labelTop->Draw("same");


  std::string ptText;
  if( iPt>=0 ) {
    ptText = std::string( Form("%.0f < p_{T} < %.0f GeV", ptBins[iPt], ptBins[iPt+1]) );
  } else {
    ptText = std::string( Form("p_{T} > %.0f GeV", ptBins[0]) );
  }

  float xMin_leg = 0.35;
  float yMin_leg = 0.18;
  float xMax_leg = 0.70;
  float yMax_leg = 0.40;

  if( varName == "nhE" || varName == "phE" || varName == "nhEF" || varName=="phEF" ) {

    xMin_leg = 0.35;
    yMin_leg = 0.70;
    xMax_leg = 0.70;
    yMax_leg = 0.92;
 
  }


  TLegend* legend = new TLegend( xMin_leg, yMin_leg, xMax_leg, yMax_leg, ptText.c_str() );
  legend->SetTextSize(0.03);
  legend->SetFillColor(0);

  for( unsigned i=0; i<datasets.size(); ++i ) {

    TProfile* thisProfile = (TProfile*)datasets[i]->file->Get( profileName.c_str() );
    thisProfile->SetMarkerSize(1.3);
    thisProfile->SetMarkerStyle(20);
    thisProfile->SetMarkerColor(colors[i]);
    thisProfile->SetLineColor(colors[i]);

    thisProfile->Draw("p same");
    legend->AddEntry( thisProfile, datasets[i]->prettyName.c_str() );

  } // for datasets

  legend->Draw("same");

  gPad->RedrawAxis();
 
  
  c1->SaveAs( Form("%s/%s. %s.png", outdir.c_str(),sorting.c_str(), profileName.c_str()) );

  delete c1;
  delete h2_axes;

}



void drawResponseResolution( const std::string& outdir, const std::vector<jseDataset*>& datasets, const std::string& suffix  ) {


  std::vector<float> ptBins  = jseCommon::ptBins();
  std::vector<float> etaBins = jseCommon::etaBins();

  Double_t etaBins_lower[etaBins.size()];
  for( unsigned i=0; i<etaBins.size(); ++i ) etaBins_lower[i] = etaBins[i];

  std::vector<int> colors = jseCommon::colors();

  TFile* fileHistos = TFile::Open( Form("%s/%s.root", outdir.c_str(), suffix.c_str()), "RECREATE" );
  fileHistos->cd();


  for( unsigned iPt=0; iPt<ptBins.size()-2; ++iPt ) {


    std::string ptText( Form("%.0f < p_{T} < %.0f GeV", ptBins[iPt], ptBins[iPt+1]) );

    float xMin = etaBins[0];
    float xMax = etaBins[etaBins.size()-1];


    // prepare response plot

    TCanvas* c1_resp = new TCanvas( "c1_resp", "", 600, 600 );
    c1_resp->cd();

    float yMinResp = 0.75;
    float yMaxResp = 1.3;

    TH2D* h2_axes_resp = new TH2D( "axes_resp", "", 10, xMin, xMax, 10, yMinResp, yMaxResp );
    h2_axes_resp->SetXTitle( "Jet #eta" );
    if( suffix=="Raw" )
      h2_axes_resp->SetYTitle( "Jet Raw Response" );
    else
      h2_axes_resp->SetYTitle( "Jet Response" );
    h2_axes_resp->Draw();

    TLine* lineOne = new TLine( xMin, 1., xMax, 1. );
    lineOne->Draw("same");

    TPaveText* labelTop = jseCommon::getLabelTopSimulation();
    labelTop->Draw("same");

    TLegend* legend_resp;
    if( suffix=="Raw" ) legend_resp = new TLegend( 0.3, 0.7, 0.7, 0.9, ptText.c_str() );
    else                legend_resp = new TLegend( 0.3, 0.2, 0.7, 0.4, ptText.c_str() );
    legend_resp->SetTextSize(0.038);
    legend_resp->SetFillColor(0);


    // response fit

    TCanvas* c1_respFit = new TCanvas( "c1_respFit", "", 600, 600 );
    c1_respFit->cd();

    TH2D* h2_axes_respFit = new TH2D( "axes_respFit", "", 10, xMin, xMax, 10, yMinResp, yMaxResp );
    h2_axes_respFit->SetXTitle( "Jet #eta" );
    if( suffix=="Raw" )
      h2_axes_respFit->SetYTitle( "Jet Raw Response (fit)" );
    else
      h2_axes_respFit->SetYTitle( "Jet Response (fit)" );
    h2_axes_respFit->Draw();

    labelTop->Draw("same");

    lineOne->Draw("same");


    // resolution plot

    TCanvas* c1_reso = new TCanvas( "c1_reso", "", 600, 600 );
    c1_reso->cd();

    TH2D* h2_axes_reso = new TH2D( "axes_reso", "", 10, xMin, xMax, 10, 0., 0.5 );
    h2_axes_reso->SetXTitle( "Jet #eta" );
    h2_axes_reso->SetYTitle( "Jet Resolution" );
    if( suffix=="Raw" )
      h2_axes_reso->SetYTitle( "Jet Raw Resolution" );
    else
      h2_axes_reso->SetYTitle( "Jet Resolution" );
    h2_axes_reso->Draw();

    labelTop->Draw("same");

    TLegend* legend_reso = new TLegend( 0.3, 0.7, 0.7, 0.9, ptText.c_str() );
    legend_reso->SetTextSize(0.038);
    legend_reso->SetFillColor(0);


    // resolution fit 

    TCanvas* c1_resoFit = new TCanvas( "c1_resoFit", "", 600, 600 );
    c1_resoFit->cd();

    TH2D* h2_axes_resoFit = new TH2D( "axes_resoFit", "", 10, xMin, xMax, 10, 0., 0.5 );
    h2_axes_resoFit->SetXTitle( "Jet #eta" );
    if( suffix=="Raw" )
      h2_axes_resoFit->SetYTitle( "Jet Raw Resolution (fit)" );
    else
      h2_axes_resoFit->SetYTitle( "Jet Resolution (fit)" );
    h2_axes_resoFit->Draw();

    labelTop->Draw("same");



    for( unsigned iDataset=0; iDataset<datasets.size(); ++iDataset ) {

      TFile* file = datasets[iDataset]->file;

      std::string respDir( Form("%s/resp%s/%s", outdir.c_str(), suffix.c_str(), datasets[iDataset]->name.c_str()) );

      if( iPt==0 )
        system( Form("mkdir -p %s", respDir.c_str()) );


      TH1D* h1_resp_vs_eta    = new TH1D( Form("resp%s_vs_eta_%s_pt%d"   , suffix.c_str(), datasets[iDataset]->name.c_str(), iPt), "", etaBins.size()-1, etaBins_lower );
      TH1D* h1_reso_vs_eta    = new TH1D( Form("reso%s_vs_eta_%s_pt%d"   , suffix.c_str(), datasets[iDataset]->name.c_str(), iPt), "", etaBins.size()-1, etaBins_lower );
      TH1D* h1_respFit_vs_eta = new TH1D( Form("resp%sFit_vs_eta_%s_pt%d", suffix.c_str(), datasets[iDataset]->name.c_str(), iPt), "", etaBins.size()-1, etaBins_lower );
      TH1D* h1_resoFit_vs_eta = new TH1D( Form("reso%sFit_vs_eta_%s_pt%d", suffix.c_str(), datasets[iDataset]->name.c_str(), iPt), "", etaBins.size()-1, etaBins_lower );

      for( unsigned iEta=0; iEta<etaBins.size()-1; ++iEta ) {

        std::string thisHistoName( jseCommon::getHistoName( Form("resp%s", suffix.c_str()), iPt, iEta ) ); 
        TH1D* h1_response = (TH1D*)file->Get( thisHistoName.c_str() );
        if( h1_response==0 ) continue;
        if( h1_response->GetEntries()<5 ) continue;
        float truncFrac = 0.99;
        TH1D* h1_responseTrunc = getTruncatedHisto( h1_response, truncFrac );

        float xMin = h1_response->GetXaxis()->GetXmin();
        float xMax = h1_response->GetXaxis()->GetXmax();

        TF1* f1_resp = fitResponse(h1_responseTrunc);

        float mean    = h1_responseTrunc->GetMean();
        float meanErr = h1_responseTrunc->GetMeanError();
        float rms     = h1_responseTrunc->GetRMS();
        float rmsErr  = h1_responseTrunc->GetRMSError();

        float reso = rms/mean;
        float resoErr = sqrt( rmsErr*rmsErr/(mean*mean) + meanErr*meanErr*rms*rms/(mean*mean*mean*mean) );

        float mu = 0.;
        float muErr = 0.;
        float sigma = 0.;
        float sigmaErr = 0.;
        float resoFit = 0.;
        float resoFitErr = 0.;

        if( f1_resp != 0 ) {

          mu    = f1_resp->GetParameter(1);
          muErr = f1_resp->GetParError (1);
   
          sigma    = f1_resp->GetParameter(2);
          sigmaErr = f1_resp->GetParError (2);
   
          resoFit = sigma/mu;
          resoFitErr = sqrt( sigmaErr*sigmaErr/(mu*mu) + muErr*muErr*sigma*sigma/(mu*mu*mu*mu) );

        }

        // first plot

        TCanvas* c1 = new TCanvas( "c1_tmp", "", 600, 600 );
        c1->cd();

        float yMin = 0.;
        float yMax = 1.1*h1_response->GetMaximum();

        TH2D* h2_axes = new TH2D("axes_tmp", "", 10, xMin, xMax, 10, yMin, yMax);
        h2_axes->SetXTitle( "Reco p_{T} / Gen p_{T}" );
        h2_axes->SetYTitle( "Entries" );
        h2_axes->Draw();

        h1_response->SetLineColor(kBlack);
        h1_response->SetFillColor(kWhite);
        h1_response->SetLineWidth(2);

        h1_responseTrunc->SetFillStyle(3004);
        h1_responseTrunc->SetFillColor(46);
        h1_responseTrunc->SetLineColor(46);
        h1_responseTrunc->SetLineWidth(2);

        //TLegend* legend = new TLegend( 0.7, 0.75, 0.9, 0.9 );
        //legend->SetFillColor(0);
        //legend->SetTextSize(0.035);
        //legend->AddEntry( h1_response, "All", "F" );
        //legend->AddEntry( h1_responseTrunc, Form("%.1f%%", truncFrac*100.), "F" );
        //legend->Draw("same");


        TLine* lineResp = new TLine( mean, yMin, mean, yMax );
        lineResp->SetLineColor( 46 );
        lineResp->Draw("same");
        
        labelTop->Draw("same");

        TPaveText* labelEta = new TPaveText( 0.65, 0.85, 0.9, 0.9, "brNDC" );
        labelEta->SetTextSize( 0.035 );
        labelEta->SetFillColor(0);
        labelEta->AddText( Form("%.1f < #eta < %.1f", etaBins[iEta], etaBins[iEta+1]) );
        labelEta->Draw("same");

        TPaveText* labelPt = new TPaveText( 0.65, 0.8, 0.9, 0.85, "brNDC" );
        labelPt->SetTextSize( 0.035 );
        labelPt->SetFillColor(0);
        labelPt->AddText( Form("%.0f < p_{T} < %.0f GeV", ptBins[iPt], ptBins[iPt+1]) );
        labelPt->Draw("same");

        TPaveText* labelResp = new TPaveText( 0.65, 0.68, 0.9, 0.77, "brNDC" );
        labelResp->SetTextSize( 0.035 );
        labelResp->SetFillColor(0);
        labelResp->AddText( Form("Mean = %.2f", mean) );
        labelResp->AddText( Form("RMS = %.2f", rms) );
        labelResp->Draw("same");

        TPaveText* labelFit = new TPaveText( 0.65, 0.55, 0.9, 0.64, "brNDC" );
        labelFit->SetTextSize( 0.035 );
        labelFit->SetFillColor(0);
        labelFit->AddText( Form("#mu = %.2f", mu) );
        labelFit->AddText( Form("#sigma = %.2f", sigma) );
        labelFit->Draw("same");

        h1_response->Draw("same");
        h1_responseTrunc->Draw("same");
        f1_resp->Draw("same");

        gPad->RedrawAxis();
  
        if( !noEPS )
          c1->SaveAs( Form("%s/resp%s_pt%d_eta%d.eps", respDir.c_str(), suffix.c_str(), iPt, iEta) );
        c1->SaveAs( Form("%s/resp%s_pt%d_eta%d.pdf", respDir.c_str(), suffix.c_str(), iPt, iEta) );

        delete c1;
        delete h2_axes;

        // end plotting 

        h1_resp_vs_eta   ->SetBinContent( iEta+1, mean );
        h1_resp_vs_eta   ->SetBinError  ( iEta+1, meanErr );

        h1_reso_vs_eta   ->SetBinContent( iEta+1, reso );
        h1_reso_vs_eta   ->SetBinError  ( iEta+1, resoErr );

        h1_respFit_vs_eta->SetBinContent( iEta+1, mu );
        h1_respFit_vs_eta->SetBinError  ( iEta+1, muErr );

        h1_resoFit_vs_eta->SetBinContent( iEta+1, resoFit );
        h1_resoFit_vs_eta->SetBinError  ( iEta+1, resoFitErr );

        delete h1_responseTrunc;
 
      }  // eta

      c1_resp->cd();

      h1_resp_vs_eta->SetMarkerStyle(20);
      h1_resp_vs_eta->SetMarkerSize(1.3);
      h1_resp_vs_eta->SetMarkerColor(colors[iDataset]);
      h1_resp_vs_eta->SetLineColor(colors[iDataset]);
      h1_resp_vs_eta->Draw("p same");

      legend_resp->AddEntry( h1_resp_vs_eta, datasets[iDataset]->prettyName.c_str(), "P" );

      c1_respFit->cd();

      h1_respFit_vs_eta->SetMarkerStyle(20);
      h1_respFit_vs_eta->SetMarkerSize(1.3);
      h1_respFit_vs_eta->SetMarkerColor(colors[iDataset]);
      h1_respFit_vs_eta->SetLineColor(colors[iDataset]);
      h1_respFit_vs_eta->Draw("p same");

      c1_reso->cd();

      h1_reso_vs_eta->SetMarkerStyle(20);
      h1_reso_vs_eta->SetMarkerSize(1.3);
      h1_reso_vs_eta->SetMarkerColor(colors[iDataset]);
      h1_reso_vs_eta->SetLineColor(colors[iDataset]);
      h1_reso_vs_eta->Draw("p same");

      legend_reso->AddEntry( h1_reso_vs_eta, datasets[iDataset]->prettyName.c_str(), "P" );

      c1_resoFit->cd();

      h1_resoFit_vs_eta->SetMarkerStyle(20);
      h1_resoFit_vs_eta->SetMarkerSize(1.3);
      h1_resoFit_vs_eta->SetMarkerColor(colors[iDataset]);
      h1_resoFit_vs_eta->SetLineColor(colors[iDataset]);
      h1_resoFit_vs_eta->Draw("p same");

      fileHistos->cd();

      h1_resp_vs_eta   ->Write();
      h1_reso_vs_eta   ->Write();
      h1_respFit_vs_eta->Write();
      h1_resoFit_vs_eta->Write();
 

    } // dataset

    c1_resp->cd();

    legend_resp->Draw("same");

    gPad->RedrawAxis();

    if( !noEPS )
      c1_resp->SaveAs( Form( "%s/resp%s_vs_eta_pt%d.eps", outdir.c_str(), suffix.c_str(), iPt ) );
    c1_resp->SaveAs( Form( "%s/resp%s_vs_eta_pt%d.pdf" , outdir.c_str(), suffix.c_str(), iPt ) );


    c1_respFit->cd();

    legend_resp->Draw("same");

    gPad->RedrawAxis();

    if( !noEPS )
      c1_respFit->SaveAs( Form( "%s/respFit%s_vs_eta_pt%d.eps", outdir.c_str(), suffix.c_str(), iPt ) );
    c1_respFit->SaveAs( Form( "%s/respFit%s_vs_eta_pt%d.pdf", outdir.c_str(), suffix.c_str(), iPt ) );


    c1_reso->cd();

    legend_reso->Draw("same");

    gPad->RedrawAxis();

    if( !noEPS )
      c1_reso->SaveAs( Form( "%s/reso%s_vs_eta_pt%d.eps", outdir.c_str(), suffix.c_str(), iPt ) );
    c1_reso->SaveAs( Form( "%s/reso%s_vs_eta_pt%d.pdf", outdir.c_str(), suffix.c_str(), iPt ) );


    c1_resoFit->cd();

    legend_reso->Draw("same");

    gPad->RedrawAxis();

    if( !noEPS )
      c1_resoFit->SaveAs( Form( "%s/resoFit%s_vs_eta_pt%d.eps", outdir.c_str(), suffix.c_str(), iPt ) );
    c1_resoFit->SaveAs( Form( "%s/resoFit%s_vs_eta_pt%d.pdf", outdir.c_str(), suffix.c_str(), iPt ) );


    delete c1_reso;
    delete c1_resp;
    delete c1_resoFit;
    delete c1_respFit;
    delete h2_axes_reso;
    delete h2_axes_resp;
    delete h2_axes_resoFit;
    delete h2_axes_respFit;

  } // pt

  fileHistos->Close();

}


TF1* fitResponse( TH1D* h1 ) {

  TF1* f1 = new TF1( Form("fit_%s", h1->GetName()), "gaus" );
  
  float histMean = h1->GetMean();
  float histRMS  = h1->GetRMS();
 
  f1->SetParameter(0, h1->GetMaximum());
  f1->SetParameter(1, histMean);
  f1->SetParameter(2, histRMS);


  if( histRMS==0. ) return 0;

  f1->SetParLimits(1, 0., h1->GetXaxis()->GetXmax());

  float lowerBound = histMean - histRMS;
  float upperBound = histMean + histRMS;

  f1->SetRange( lowerBound, upperBound );
  f1->SetLineColor(46);
 
  h1->Fit(f1, "QRN");

  int n_iter = 3;

  for(int i=0; i<n_iter; ++i) {

    lowerBound = f1->GetParameter(1) - 1.5*f1->GetParameter(2);
    upperBound = f1->GetParameter(1) + 1.0*f1->GetParameter(2);

    f1->SetRange( lowerBound, upperBound );

    h1->Fit(f1, "QRN");

  }

  return f1;

}


std::vector<TH1D*> getHistoVector( const std::string& name, const std::vector<float>& ptBins, const std::vector<float>& etaBins ) {

  std::vector<TH1D*> vh1;

  Double_t etaBins_lower[etaBins.size()];
  for( unsigned i=0; i<etaBins.size(); ++i ) etaBins_lower[i] = etaBins[i];

  for( unsigned iPt=0; iPt<ptBins.size()-1; ++iPt ) {
     TH1D* newhisto = new TH1D( Form("%s_vs_eta_pt%d", name.c_str(), iPt), "", etaBins.size()-1, etaBins_lower );
     vh1.push_back(newhisto);
  }

  return vh1;

}


TH1D* getTruncatedHisto( TH1D* h1, float frac ) {

  int nBins = h1->GetXaxis()->GetNbins();
  float xMin = h1->GetXaxis()->GetXmin();
  float xMax = h1->GetXaxis()->GetXmax();

  TH1D* h1_trunc = new TH1D( Form("%s_trunc", h1->GetName()), "", nBins, xMin, xMax );

  int binMax = h1->GetMaximumBin();

  float intTot = h1->Integral();

  int currentBin = binMax;

  int sign=1;
  int delta_bin=1;

  bool stopJumping = false;
  bool stopGoingRight = false;
  bool stopGoingLeft = false;

  //std::cout << "lll4" << std::endl;
  //std::cout << "h1_trunc->Integral(): " << h1_trunc->Integral() << std::endl;
  //std::cout << "frac: " << frac << std::endl;
  //std::cout << "intTot: " << intTot << std::endl;
  //std::cout << "frac*intTot: " << frac*intTot << std::endl;
  //std::cout << "currentBin: " << currentBin << std::endl;
  while( h1_trunc->Integral()<frac*intTot ) {

    //std::cout << "---------" << std::endl;
    //std::cout << "currentBin: " << currentBin << std::endl;
    //std::cout << "stopJumping: " << stopJumping << std::endl;
    //std::cout << "stopGoingRight: " << stopGoingRight << std::endl;
    //std::cout << "stopGoingLeft: " << stopGoingLeft << std::endl;
    h1_trunc->SetBinContent( currentBin, h1->GetBinContent(currentBin) );

    if( stopGoingRight ) currentBin-=1;
    else if( stopGoingLeft  ) currentBin+=1; 
    else currentBin += sign*delta_bin;

    if( currentBin <= 1 ) stopGoingLeft = true;
    if( currentBin >= nBins-1 ) stopGoingRight = true;

    if( stopGoingLeft && stopGoingRight ) break;

    if( !stopJumping ) {

      delta_bin += 1;
      sign *= -1; // makes it add one bin to the right, one to the left

      if( stopGoingLeft || stopGoingRight ) stopJumping = true;

    } // if stop jumping

  }

  return h1_trunc;

} 
