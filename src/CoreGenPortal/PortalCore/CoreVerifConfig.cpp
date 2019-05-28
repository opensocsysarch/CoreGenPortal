//
// _CoreVerifConfig_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreVerifConfig.h"

CoreVerifConfig::CoreVerifConfig(){
  SetupPasses();
  IsValid = true;
}

CoreVerifConfig::CoreVerifConfig(wxString Config){
  SetupPasses();
  IsValid = true;
}

CoreVerifConfig::~CoreVerifConfig(){
}

void CoreVerifConfig::SetupPasses(){
  CoreGenBackend *CG = new CoreGenBackend( "VERIF", "VERIF", "VERIF", CGSocProj );

  // insert a dummy isa to make the dag happy
  CG->InsertISA("dummy.isa");
  CG->BuildDAG();
  CG->InitPassMgr();
  std::vector<std::string> PInfo = CG->GetPasses();

  for( unsigned i=0; i<PInfo.size(); i++ ){
    Passes.push_back(std::make_pair(PInfo[i],true));
  }

  delete CG;
}

wxString CoreVerifConfig::GetPassName( unsigned i ){
  if( i > (Passes.size()-1) )
    return wxT("");

  return wxString(Passes[i].first);
}

bool CoreVerifConfig::IsPassEnabled( unsigned i ){
  if( i > (Passes.size()-1) )
    return false;
  return Passes[i].second;
}

bool CoreVerifConfig::IsPassEnabled( wxString P ){
  std::string S = std::string(P.mb_str());
  for( unsigned i=0; i<Passes.size(); i++ ){
    if( Passes[i].first == S )
      return Passes[i].second;
  }
  return false;
}

// EOF
