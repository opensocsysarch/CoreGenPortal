//
// _SCCompConfig_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/SCCompConfig.h"

SCCompConfig::SCCompConfig()
  : IsValid(false), IsPipeline(false), IsParse(false), IsKeep(false),
  OptLevel(-1), SigMapFile("") {
  SetupDefaultOptimizations();
}

SCCompConfig::~SCCompConfig(){
}

void SCCompConfig::SetupDefaultOptimizations(){
  SCMsg *Msgs = new SCMsg();
  SCParser *Parser = new SCParser(Msgs);

  std::vector<std::string> P = Parser->GetPassList();

  for( unsigned i=0; i<P.size(); i++ ){
    LLVMPasses.push_back(std::make_pair(P[i],true));
  }

  delete Msgs;
  delete Parser;

  // currently we have to do this manually
  SCPasses.push_back(std::make_pair("SCInstArg",true));
  SCPasses.push_back(std::make_pair("SCInstFormat",true));
  SCPasses.push_back(std::make_pair("SCIOWarn",true));
  SCPasses.push_back(std::make_pair("SCFieldIO",true));

  // default optimization level is -O2
  OptLevel = 2;

  IsValid = true;
  IsPipeline = false;
  IsParse = false;
  IsKeep = false;
}

void SCCompConfig::SetO0(){
  OptLevel = 0;
  IsPipeline = false;
}

void SCCompConfig::SetO1(){
  OptLevel = 1;
  IsPipeline = false;
}

void SCCompConfig::SetO2(){
  OptLevel = 2;
  IsPipeline = false;
}

void SCCompConfig::SetO3(){
  OptLevel = 3;
  IsPipeline = true;
}

wxString SCCompConfig::GetLLVMPassName(unsigned P){
  if( P > (LLVMPasses.size()-1) )
    return wxT("");
  return wxString(LLVMPasses[P].first);
}

wxString SCCompConfig::GetSCPassName(unsigned P){
  if( P > (SCPasses.size()-1) )
    return wxT("");
  return wxString(SCPasses[P].first);
}

bool SCCompConfig::IsLLVMPassEnabled(unsigned P){
  if( P > (LLVMPasses.size()-1) )
    return false;
  return LLVMPasses[P].second;
}

bool SCCompConfig::IsSCPassEnabled(unsigned P){
  if( P > (SCPasses.size()-1) )
    return false;
  return SCPasses[P].second;
}

// EOF
