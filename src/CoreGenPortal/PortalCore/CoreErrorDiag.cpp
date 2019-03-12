//
// _COREERRORDIAG_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreErrorDiag.h"


CoreErrorDiag::CoreErrorDiag( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              const wxString inMsg )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  Msg = inMsg;
}

CoreErrorDiag::~CoreErrorDiag(){
}

// EOF
