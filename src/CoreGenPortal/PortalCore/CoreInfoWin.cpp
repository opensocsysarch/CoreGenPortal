//
// _COREINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreInfoWin.h"

CoreInfoWin::CoreInfoWin( wxWindow *parent,
                          wxWindowID id,
                          CoreGenNode *Node ){
  if( Node->GetType() == CGSoc ){
    CoreSocInfoWin *Win = new CoreSocInfoWin( parent, id,
                                              wxString(Node->GetName()),
                                              static_cast<CoreGenSoC *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Node->GetType() == CGCore ){
    CoreCoreInfoWin *Win = new CoreCoreInfoWin( parent, id,
                                                wxString(Node->GetName()),
                                                static_cast<CoreGenCore *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Node->GetType() == CGInstF ){
  }else if( Node->GetType() == CGInst ){
  }else if( Node->GetType() == CGPInst ){
  }else if( Node->GetType() == CGRegC ){
  }else if( Node->GetType() == CGReg ){
  }else if( Node->GetType() == CGISA ){
  }else if( Node->GetType() == CGCache ){
  }else if( Node->GetType() == CGEnc ){
  }else if( Node->GetType() == CGExt ){
  }else if( Node->GetType() == CGComm ){
  }else if( Node->GetType() == CGSpad ){
  }else if( Node->GetType() == CGMCtrl ){
  }else if( Node->GetType() == CGVTP ){
  }else if( Node->GetType() == CGPlugin ){
  }
}

CoreInfoWin::~CoreInfoWin(){
}

// EOF
