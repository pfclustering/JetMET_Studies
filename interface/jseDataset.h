#ifndef jseDataset_h
#define jseDataset_h


#include "TFile.h"
#include "TTree.h"
#include <string>



class jseDataset {

 public:
  
  jseDataset( const std::string& prodName, const std::string& name, const std::string& prettyName );


  std::string prodName;
  std::string name;
  std::string prettyName;

  TFile* file;


 private:

};


#endif
