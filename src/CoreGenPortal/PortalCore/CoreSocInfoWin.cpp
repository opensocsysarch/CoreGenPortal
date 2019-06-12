//
// _CORESOCINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreSocInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreSocInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreSocInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreSocInfoWin::CoreSocInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenSoC *Soc )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Soc == nullptr ){
    this->EndModal(wxID_OK);
  }

  // init the internals
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the box sizers
  wxBoxSizer *bSizer2 = new wxBoxSizer( wxVERTICAL );

  m_panel1 = new wxPanel( this,
                          wxID_ANY,
                          wxDefaultPosition,
                          wxDefaultSize,
                          wxTAB_TRAVERSAL );
  bSizer2->Add( m_panel1, 1, wxEXPAND|wxALL, 5);

  // init all the options
  //-- soc name
  SocNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("SoC Name"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  SocNameText->Wrap(-1);
  bSizer2->Add( SocNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- soc name box
  SocNameCtrl = new wxTextCtrl(this,
                               wxID_ANY,
                               wxString(Soc->GetName()),
                               wxDefaultPosition,
                               wxSize(400,25),
                               wxTE_READONLY,
                               wxDefaultValidator,
                               wxT("SoC Name") );
  bSizer2->Add( SocNameCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- core name
  CoreNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("Cores"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  CoreNameText->Wrap(-1);
  bSizer2->Add( CoreNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- cores
  CoreNameCtrl = new wxTextCtrl(this,
                               wxID_ANY,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxSize(400,100),
                               wxTE_MULTILINE|wxTE_READONLY,
                               wxDefaultValidator,
                               wxT("Cores") );
  for( unsigned i=0; i<Soc->GetNumCores(); i++ ){
    CoreNameCtrl->AppendText( wxString(Soc->GetCore(i)->GetName()) + wxT("\n"));
  }
  bSizer2->Add( CoreNameCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  bSizer2->Add( FinalStaticLine, 1, wxSHAPED|wxALL, 5 );

  // setup all the buttons
  wxBoxSizer *bSizer3 = new wxBoxSizer( wxVERTICAL );

  m_socbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( this, wxID_OK );
  m_socbuttonsizer->AddButton( m_userOK );
  m_socbuttonsizer->Realize();

  bSizer3->Add( m_socbuttonsizer, 1, wxSHAPED, 5 );
  bSizer2->Add( bSizer3, 1, wxSHAPED, 5 );

  // draw the dialog box until we get more info
  this->SetSizer( bSizer2 );
  this->Layout();
  bSizer2->Fit(this);
  this->Centre(wxBOTH);
}

void CoreSocInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreSocInfoWin::~CoreSocInfoWin(){
}

// EOF
