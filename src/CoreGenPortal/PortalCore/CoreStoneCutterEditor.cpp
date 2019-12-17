//
// _CORESTONECUTTEREDITOR_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreStoneCutterEditor.h"

CoreStoneCutterEditor::CoreStoneCutterEditor( wxWindow *parent, wxWindowID id )
  : wxStyledTextCtrl(parent,id) {

  // Setup all the interior styling
  this->SetMarginWidth(SC_MARGIN_LINE_NUMBERS, 10);
  this->SetTabWidth(3);
  this->SetIndent(3);
  this->SetUseTabs(false);
  this->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
  this->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
  this->SetMarginType(SC_MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
  this->SetWrapMode(wxSTC_WRAP_WORD);
  this->StyleClearAll();
}

CoreStoneCutterEditor::~CoreStoneCutterEditor(){
}

// EOF
