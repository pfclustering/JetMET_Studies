#include <iostream>    
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TCut.h"

using namespace std;

//__________________________________________________________________________

main(int argc, char *argv[])
{

   if( argc<4 ) {
      std::cout << "Usage: ./my_analyzer [productionName] [datasetName] [nanoFileDir] [targetDir] [nEvtsPerFile] [cutOff]" << std::endl;
      exit(1);
   }


   std::string prodName(argv[1]);
   std::string datasetName(argv[2]);
   std::string fileDir(argv[3]);
   std::string targetDir(argv[4]);
   std::string nEvts(argv[5]);
   std::string cutOffstr(argv[6]);

   int cutOff = std::stoi(cutOffstr);
   int nEvt = std::stoi(nEvts);

   std::cout << "-> Starting analysis for dataset: " << datasetName << " on " << nEvt << " events and cutOff " << cutOff << std::endl;

   //TFile *file = TFile::Open(Form("root://cms-xrd-global.cern.ch/%s",prodName.c_str()));
   string name;
   if(prodName!="private"){
      name = "file";
   }
   else{
      name = "merged";
   }
   TFile *file;
   if(TFile::Open(Form("%s/%s/%s.root", "root://t3dcachedb.psi.ch:1094/", fileDir.c_str(), name.c_str()))){
      file = TFile::Open(Form("%s/%s/%s.root", "root://t3dcachedb.psi.ch:1094/", fileDir.c_str(), name.c_str()));
   }
   else{
      cout << "file not found" << endl;
      //return 0;
      exit(11);
   }

   TTree *tree; 
   file->GetObject("Events", tree);

   cout << "got the Events tree" << endl;

   //Uncomment to add relevant branches of the former tree in the histoFile
   //tree->SetBranchStatus("*", 0);
   //for (auto activeBranchName : {"MET_phi", "MET_pt", "MET_sumEt", "PV_npvs", "Jet_pt", "Jet_eta", "Jet_chHEF", "Jet_neHEF", "Jet_neEmEF", "Jet_chEmEF", "Jet_muEF" })
   //   tree->SetBranchStatus(activeBranchName, 1);

   //system(Form("rm %s/histoFiles/%s", targetDir.c_str(), datasetName.c_str()));
   system(Form( "mkdir -p %s/histoFiles", targetDir.c_str()) );

   if(prodName == "private"){
      datasetName += ".root";
   }
   TFile* outfile = TFile::Open( Form("%s/histoFiles/%s", targetDir.c_str(), datasetName.c_str()), "RECREATE" );

   //Uncomment to add relevant branches of the former tree in the histoFile
   auto newtree = tree->CloneTree();

   cout << "will set the branch addresses" << endl;

   int nVert;
   tree->SetBranchAddress("PV_npvs", &nVert );

   UInt_t njet;
   tree->SetBranchAddress("nJet", &njet );
   float jet_pt[999];
   tree->SetBranchAddress("Jet_pt", &jet_pt );
   float jet_eta[999];
   tree->SetBranchAddress("Jet_eta", &jet_eta );
   float jet_phi[999];
   tree->SetBranchAddress("Jet_phi", &jet_phi );
   float jet_puid[999];
   tree->SetBranchAddress("Jet_puId", &jet_puid );


   float MET_phi;
   tree->SetBranchAddress("MET_phi", &MET_phi );
   float MET_pt;
   tree->SetBranchAddress("MET_pt", &MET_pt );
   float MET_sumEt;
   tree->SetBranchAddress("MET_sumEt", &MET_sumEt );

   float jet_chEF[999];
   tree->SetBranchAddress("Jet_chHEF", &jet_chEF );
   float jet_chEFJerc[999];
   tree->SetBranchAddress("Jet_jercCHF", &jet_chEFJerc );
   float jet_nhEF[999];
   tree->SetBranchAddress("Jet_neHEF", &jet_nhEF );
   float jet_phEF[999];
   tree->SetBranchAddress("Jet_neEmEF", &jet_phEF );
   float jet_elEF[999];
   tree->SetBranchAddress("Jet_chEmEF", &jet_elEF);
   float jet_muEF[999];
   tree->SetBranchAddress("Jet_muEF", &jet_muEF);


   TH1D *h1_PV_npvs = new TH1D("nVertex", "", 100, 0, 100);
   TH1D *h1_met_phi = new TH1D("MET_phi", "", 64, -3.2, 3.2);
   TH1D *h1_met_pt = new TH1D("MET_pt", "", 64, 0, 500);
   TH1D *h1_met_sumEt = new TH1D("MET_sumEt", "", 64, 0, 10000);
   TH1D *h1_jet_pt  = new TH1D( "Jet_pt" , "", 100, 0., 500. );
   TH1D *h1_jet_eta = new TH1D( "Jet_eta", "", 100, -5., 5. );
   TH1D *h1_totEF = new TH1D("totEF", "", 100, 0, 1.05);

   float etaMax = 5;
   int nBins_eta = 64;
   float phiMax = 3.1416;
   int nBins_phi = 64;

   TH2D* h2_Jet_pt_vs_eta     = new TH2D( "h2_Jet_pt_vs_eta"    , "", nBins_eta, -etaMax, etaMax, 100, 0., 500.  );
   //TProfile* hp_Jetpt_vs_eta = new TProfile( "Jet_pt_vs_eta"     , "", nBins_eta, -etaMax, etaMax );

   //TProfile* hp_chEF_vs_eta      = new TProfile( "chEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax );
   //TProfile* hp_nhEF_vs_eta      = new TProfile( "nhEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax );
   //TProfile* hp_phEF_vs_eta      = new TProfile( "phEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax );

   //TProfile* hp_chE_vs_eta       = new TProfile( "chE_vs_eta"      , "", nBins_eta, -etaMax, etaMax );
   //TProfile* hp_nhE_vs_eta       = new TProfile( "nhE_vs_eta"      , "", nBins_eta, -etaMax, etaMax );
   //TProfile* hp_phE_vs_eta       = new TProfile( "phE_vs_eta"      , "", nBins_eta, -etaMax, etaMax );

   //TProfile* hp_MET_pt_vs_phi    = new TProfile( "MET_pt_vs_phi"   , "", nBins_phi, -phiMax, phiMax );
   //TProfile* hp_MET_sumEt_vs_phi = new TProfile( "MET_sumEt_vs_phi", "", nBins_phi, -phiMax, phiMax );

   TH2D* h2_chEF_vs_eta      = new TH2D( "h2_chEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax, 100, 0., 1.);
   TH2D* h2_chEFJerc_vs_eta  = new TH2D( "h2_chEFJerc_vs_eta" , "", nBins_eta, -etaMax, etaMax, 100, 0., 1.);
   TH2D* h2_nhEF_vs_eta      = new TH2D( "h2_nhEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax, 100, 0., 1. );
   TH2D* h2_phEF_vs_eta      = new TH2D( "h2_phEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax, 100, 0., 1. );
   TH2D* h2_elEF_vs_eta      = new TH2D( "h2_elEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax, 100, 0., 1. );
   TH2D* h2_muEF_vs_eta      = new TH2D( "h2_muEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax, 100, 0., 1. );
   TH2D* h2_totEF_vs_eta     = new TH2D( "h2_totEF_vs_eta"    , "", nBins_eta, -etaMax, etaMax, 100, 0., 1. );
   TH2D* h2_totEF_vs_pt      = new TH2D( "h2_totEF_vs_pt"    , "",  100, 0, 500, 100, 0., 1. );

   TH2D* h2_chE_vs_eta       = new TH2D( "h2_chE_vs_eta"      , "", nBins_eta, -etaMax, etaMax, 100, 0., 500 );
   TH2D* h2_nhE_vs_eta       = new TH2D( "h2_nhE_vs_eta"      , "", nBins_eta, -etaMax, etaMax, 100, 0., 500 );
   TH2D* h2_phE_vs_eta       = new TH2D( "h2_phE_vs_eta"      , "", nBins_eta, -etaMax, etaMax, 100, 0., 500 );

   TH2D* h2_MET_pt_vs_phi    = new TH2D( "h2_MET_pt_vs_phi"   , "", nBins_phi, -phiMax, phiMax, 100, 0., 500 );
   TH2D* h2_MET_sumEt_vs_phi = new TH2D( "h2_MET_sumEt_vs_phi", "", nBins_phi, -phiMax, phiMax, 100, 0., 10000. );


   int nentries = tree->GetEntries();
   if(nentries < nEvt) nEvt = nentries;

   for( unsigned iEntry=0; iEntry<nEvt; ++iEntry ) {
      tree->GetEntry( iEntry );

      if( iEntry % 1000 == 0 ) std::cout << "  Entry: " << iEntry << " / " << nentries << std::endl;

      h1_PV_npvs->Fill( nVert );
      h1_met_phi->Fill( MET_phi );
      h1_met_sumEt->Fill( MET_sumEt );
      h1_met_pt->Fill( MET_pt );

      h2_MET_pt_vs_phi->Fill(MET_phi, MET_pt);
      h2_MET_sumEt_vs_phi->Fill(MET_phi, MET_sumEt);

      for( unsigned ijet=0; ijet<njet; ++ijet ) {
         //if(jet_pt[ijet]>cutOff && nVert>16 && nVert<20) 
         if(jet_pt[ijet]>cutOff) {
            h1_jet_pt ->Fill( jet_pt [ijet] );
            h1_jet_eta->Fill( jet_eta[ijet] );

            h2_Jet_pt_vs_eta->Fill( jet_eta[ijet], jet_pt[ijet]    );

            h2_chEF_vs_eta     ->Fill( jet_eta[ijet], jet_chEF[ijet]    );
            h2_chEFJerc_vs_eta ->Fill( jet_eta[ijet], jet_chEFJerc[ijet]);
            h2_nhEF_vs_eta     ->Fill( jet_eta[ijet], jet_nhEF[ijet]    );
            h2_phEF_vs_eta     ->Fill( jet_eta[ijet], jet_phEF[ijet]    );
            h2_elEF_vs_eta     ->Fill( jet_eta[ijet], jet_elEF[ijet]    );
            h2_muEF_vs_eta     ->Fill( jet_eta[ijet], jet_muEF[ijet]    );
            h2_totEF_vs_eta    ->Fill( jet_eta[ijet], jet_chEF[ijet]+jet_nhEF[ijet]+jet_phEF[ijet]+jet_elEF[ijet]+jet_muEF[ijet] );
            h2_totEF_vs_pt     ->Fill( jet_pt[ijet], jet_chEF[ijet]+jet_nhEF[ijet]+jet_phEF[ijet]+jet_elEF[ijet]+jet_muEF[ijet] );

            h2_chE_vs_eta      ->Fill( jet_eta[ijet], jet_chEF[ijet]    * jet_pt[ijet] );
            h2_nhE_vs_eta      ->Fill( jet_eta[ijet], jet_nhEF[ijet]    * jet_pt[ijet] );
            h2_phE_vs_eta      ->Fill( jet_eta[ijet], jet_phEF[ijet]    * jet_pt[ijet] );
         }
         
         //filling histo for sum of EF for jet_eta < 1
         if(abs(jet_eta[ijet]<1)){
            h1_totEF->Fill(jet_chEF[ijet]+jet_nhEF[ijet]+jet_phEF[ijet]+jet_elEF[ijet]+jet_muEF[ijet] );
         }
        //if(jet_chEF[ijet]+jet_nhEF[ijet]+jet_phEF[ijet]+jet_elEF[ijet]+jet_muEF[ijet]<0.8){
        //    h2_Jet_pt_vs_eta->Fill( jet_eta[ijet], jet_pt[ijet]    );
        //}
      }
   }
   h2_Jet_pt_vs_eta->ProfileX("Jet_pt_vs_eta");
   h2_chEF_vs_eta->ProfileX("chEF_vs_eta");
   h2_chEFJerc_vs_eta->ProfileX("chEFJerc_vs_eta");
   h2_nhEF_vs_eta->ProfileX("nhEF_vs_eta");
   h2_phEF_vs_eta->ProfileX("phEF_vs_eta");
   h2_elEF_vs_eta->ProfileX("elEF_vs_eta");
   h2_muEF_vs_eta->ProfileX("muEF_vs_eta");
   h2_totEF_vs_eta->ProfileX("totEF_vs_eta");
   h2_totEF_vs_pt->ProfileX("totEF_vs_pt");

   h2_chE_vs_eta->ProfileX("chE_vs_eta");
   h2_nhE_vs_eta->ProfileX("nhE_vs_eta");
   h2_phE_vs_eta->ProfileX("phE_vs_eta");

   h2_MET_pt_vs_phi->ProfileX("MET_pt_vs_phi");
   h2_MET_sumEt_vs_phi->ProfileX("MET_sumEt_vs_phi");
   // Save all objects in this file
   outfile->Write();

   file->Close();
   delete file;


   return 0;
}
