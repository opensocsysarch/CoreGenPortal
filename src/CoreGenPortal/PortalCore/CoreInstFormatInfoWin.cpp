//
// _COREINSTFORMATINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreInstFormatInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreInstFormatInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreInstFormatInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreInstFormatInfoWin::CoreInstFormatInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenInstFormat *InstF )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxVSCROLL|wxFULL_REPAINT_ON_RESIZE ){
  if( InstF == nullptr ){
    this->EndModal(wxID_OK);
  }

  // init the internals
  //this->SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the outer sizer
  OuterSizer = new wxBoxSizer( wxVERTICAL );

  // create the inner sizer
  InnerSizer = new wxBoxSizer( wxVERTICAL );

  //-- inst format figure
  IFDraw = new CoreDrawInstFormat( this,
                                   wxID_ANY,
                                   wxT("Inst Format Figure"),
                                   InstF );
  IFDraw->SetAutoLayout(true);
  IFDraw->Layout();
  InnerSizer->Add( IFDraw, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND|wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( this, wxID_OK );
  m_socbuttonsizer->AddButton( m_userOK );
  m_socbuttonsizer->Realize();

  InnerSizer->Add( m_socbuttonsizer, 1, wxEXPAND, 5 );

  // draw everything
  OuterSizer->Add(InnerSizer,1,wxEXPAND,5);
  this->SetSizer( OuterSizer );
  this->SetAutoLayout(true);
  this->Layout();
}

void CoreInstFormatInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreInstFormatInfoWin::~CoreInstFormatInfoWin(){
}

// EOF
