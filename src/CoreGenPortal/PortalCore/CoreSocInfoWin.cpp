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
  EVT_TEXT_ENTER(wxID_ANY, CoreSocInfoWin::OnPressEnter)
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

  SoCNode = Soc;
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
  SoCNameSizer = new wxBoxSizer( wxHORIZONTAL );
  SocNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("SoC Name"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  SocNameText->Wrap(-1);
  SoCNameSizer->Add( SocNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- soc name box
  SocNameCtrl = new wxTextCtrl(this,
                               0,
                               wxString(Soc->GetName()),
                               wxDefaultPosition,
                               wxSize(320,25),
                               wxTE_PROCESS_ENTER,
                               wxDefaultValidator,
                               wxT("SoC Name") );
  SoCNameSizer->Add( SocNameCtrl, 0, wxALL, 0 );
  bSizer2->Add( SoCNameSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- core name
  CoreNameSizer = new wxBoxSizer( wxHORIZONTAL );
  CoreNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("Cores"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  CoreNameText->Wrap(-1);
  CoreNameSizer->Add( CoreNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- cores
  CoreNameCtrl = new wxTextCtrl(this,
                               1,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxSize(320,100),
                               wxTE_MULTILINE|wxTE_PROCESS_ENTER,
                               wxDefaultValidator,
                               wxT("Cores") );
  for( unsigned i=0; i<Soc->GetNumCores(); i++ ){
    CoreNameCtrl->AppendText( wxString(Soc->GetCore(i)->GetName()) + wxT("\n"));
  }
  CoreNameSizer->Add( CoreNameCtrl, 0, wxALL, 0 );
  bSizer2->Add( CoreNameSizer, 0, wxALIGN_CENTER|wxALL, 0 );

  // add the static line
  FinalStaticLine = new wxStaticLine( this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  bSizer2->Add( FinalStaticLine, 1, wxEXPAND|wxALL, 5 );

  // setup all the buttons
  wxBoxSizer *bSizer3 = new wxBoxSizer( wxVERTICAL );

  m_socbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( this, wxID_OK );
  m_socbuttonsizer->AddButton( m_userOK );
  m_socbuttonsizer->Realize();

  bSizer2->Add( m_socbuttonsizer, 1, wxEXPAND, 5 );
  bSizer3->Add( bSizer2, 1, wxEXPAND|wxALL, 5 );

  // draw the dialog box until we get more info
  this->SetSizer( bSizer3 );
  this->Layout();
  bSizer3->Fit(this);
  //this->Centre(wxBOTH);
}

void CoreSocInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreSocInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->SoCNode, CGSoc);
}

CoreSocInfoWin::~CoreSocInfoWin(){
}

// EOF
