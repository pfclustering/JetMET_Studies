#ifndef jseCommon_h
#define jseCommon_h


#include "TPaveText.h"


class jseCommon {

 public:

  jseCommon();
  ~jseCommon();

  static void setStyle();

  static TPaveText* getLabelTop( float lumi );
  static TPaveText* getLabelTopSimulation( float lumi );
  static TPaveText* getLabelCMS( const std::string& text="CMS" );
  static TPaveText* getLabelTop( const std::string& text="CMS Preliminary, #sqrt{s} = 13 TeV" );
  static TPaveText* getLabelTopSimulation( const std::string& text="CMS Simulation, #sqrt{s} = 13 TeV" );

  static std::string getLumiText( float lumi );


  static std::vector<float> ptBins();
  static std::vector<float> etaBins();
  static std::vector<float> phiBins();

  static std::string getHistoName( const std::string& name, int ptBin, int etaBin );
  static std::string getHistoName( const std::string& name, int ptBin, int etaBin, int phiBin );
  static std::string getProfileVsEtaName( const std::string& name, int ptBin );
  static std::string getProfileVsPhiName( const std::string& name, int phiBin );

  static std::string findHistoName( const std::string& name, float pt, float eta );
  static std::string findHistoName( const std::string& name, float pt, float eta, float phi );

  static std::string etaText( float eta );
  static int findBin( float value, const std::vector<float> bins, bool overflow=true );
  static void symmetrize( std::vector<float>& vec );

  static std::vector<int> colors();

};

#endif
