    #include <iostream>    

    #include "TFile.h"
    #include "TH1.h"
    #include "TH2.h"
    #include "TProfile.h"
    #include "TRandom.h"
    #include "TTree.h"
    #include "TLeaf.h"
    #include "TCanvas.h"
    //__________________________________________________________________________

    main(int argc, char *argv[])
    {

    if( argc<4 ) {
      std::cout << "Usage: ./my_analyzer [productionName] [datasetName]" << std::endl;
    exit(1);
    }
    

    std::string prodName(argv[1]);
    std::string datasetName(argv[2]);
    std::string targetDir(argv[3]);

    std::cout << "-> Starting analysis for prod: " << prodName << " dataset: " << datasetName << std::endl;
    
    TFile *file = TFile::Open(Form("root://cms-xrd-global.cern.ch/%s",prodName.c_str()));
    TTree *tree; 
    file->GetObject("Events", tree);
    
    //Uncomment to add relevant branches of the former tree in the histoFile
    //tree->SetBranchStatus("*", 0);
    //for (auto activeBranchName : {"MET_phi", "MET_pt", "MET_sumEt"})
    //  tree->SetBranchStatus(activeBranchName, 1);
    
    system(Form("rm %s/histoFiles/%s", targetDir.c_str(), datasetName.c_str()));
    system(Form( "mkdir -p %s/histoFiles", targetDir.c_str()) );
    
    TFile* outfile = TFile::Open( Form("%s/histoFiles/%s", targetDir.c_str(), datasetName.c_str()), "RECREATE" );
    
    //Uncomment to add relevant branches of the former tree in the histoFile
    //auto newtree = tree->CloneTree();


    TH1D *met_phi = new TH1D("MET_phi", "", 64, -3.2, 3.2);
    TH1D *met_pt = new TH1D("MET_pt", "", 64, 0, 500);
    TH1D *met_sumEt = new TH1D("MET_sumEt", "", 64, 0, 10000);
    TH1D *jet_pt  = new TH1D( "Jet_pt" , "", 100, 0., 500. );
    TH1D *jet_eta = new TH1D( "Jet_eta", "", 100, -5., 5. );

    float etaMax = 3.2;
    int nBins_eta = 64;
    float phiMax = 3.1416;
    int nBins_phi = 64;

    TProfile* chEF_vs_eta      = new TProfile( "chEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax );
    TProfile* nhEF_vs_eta      = new TProfile( "nhEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax );
    TProfile* phEF_vs_eta      = new TProfile( "phEF_vs_eta"     , "", nBins_eta, -etaMax, etaMax );
 
    TProfile* chE_vs_eta       = new TProfile( "chE_vs_eta"      , "", nBins_eta, -etaMax, etaMax );
    TProfile* nhE_vs_eta       = new TProfile( "nhE_vs_eta"      , "", nBins_eta, -etaMax, etaMax );
    TProfile* phE_vs_eta       = new TProfile( "phE_vs_eta"      , "", nBins_eta, -etaMax, etaMax );
 
    TProfile* MET_pt_vs_phi    = new TProfile( "MET_pt_vs_phi"   , "", nBins_phi, -phiMax, phiMax );
    TProfile* MET_sumEt_vs_phi = new TProfile( "MET_sumEt_vs_phi", "", nBins_phi, -phiMax, phiMax );


    

    tree->Draw("MET_phi>>MET_phi");
    tree->Draw("MET_pt>>MET_pt");
    tree->Draw("Jet_eta>>Jet_eta");
    tree->Draw("Jet_pt>>Jet_pt");
    tree->Draw("MET_sumEt>>MET_sumEt");

    tree->Draw("Jet_chHEF:Jet_eta>>chEF_vs_eta");
    tree->Draw("Jet_neHEF:Jet_eta>>nhEF_vs_eta");
    tree->Draw("Jet_neEmEF:Jet_eta>>phEF_vs_eta");

    tree->Draw("Jet_chHEF*Jet_pt:Jet_eta>>chE_vs_eta");
    tree->Draw("Jet_neHEF*Jet_pt:Jet_eta>>nhE_vs_eta");
    tree->Draw("Jet_neEmEF*Jet_pt:Jet_eta>>phE_vs_eta");

    tree->Draw("MET_pt:MET_phi>>MET_pt_vs_phi");
    tree->Draw("MET_sumEt:MET_phi>>MET_sumEt_vs_phi");
    
    
    // Save all objects in this file
    outfile->Write();
    
    return 0;
}
