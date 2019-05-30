//
// _PORTALUSERPREFWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "PortalUserPrefWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(PortalUserPrefWin, wxDialog)
  EVT_BUTTON(wxID_OK, PortalUserPrefWin::OnPressOk)
  EVT_BUTTON(wxID_CANCEL, PortalUserPrefWin::OnPressCancel)
wxEND_EVENT_TABLE()

PortalUserPrefWin::PortalUserPrefWin( wxWindow *parent,
                                      wxWindowID id,
                                      const wxString& title,
                                      const wxPoint& pos,
                                      const wxSize& size,
                                      long style,
                                      CoreUserConfig *U): wxDialog( parent,
                                                                       id,
                                                                       title,
                                                                       pos,
                                                                       size,
                                                                       style ),
                                                           User(U) {
  // init the internals
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the box sizers
  wxBoxSizer *bSizer1 = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer *bSizer2 = new wxBoxSizer( wxVERTICAL );

  m_panel1 = new wxPanel( this,
                          wxID_ANY,
                          wxDefaultPosition,
                          wxDefaultSize,
                          wxTAB_TRAVERSAL );
  bSizer2->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );

  // init all the options
  // -- project directory static text
  ProjectDirText = new wxStaticText( this,
                                     wxID_ANY,
                                     wxT("Default project directory"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     0 );
  ProjectDirText->Wrap(-1);
  bSizer2->Add( ProjectDirText, 0, wxALIGN_CENTER|wxALL, 5 );

  // -- project directory input box
  ProjectDirCtrl = new wxTextCtrl( this,
                                   wxID_ANY,
                                   User->wxGetProjectDir(),
                                   wxDefaultPosition,
                                   wxSize(400,25),
                                   0,
                                   wxDefaultValidator,
                                   wxT("ProjectDirectory") );
  bSizer2->Add( ProjectDirCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  // -- archive directory static text
  ArchiveDirText = new wxStaticText( this,
                                     wxID_ANY,
                                     wxT("Default archive directory"),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     0 );
  ArchiveDirText->Wrap(-1);
  bSizer2->Add( ArchiveDirText, 0, wxALIGN_CENTER|wxALL, 5 );

  // -- archive directory input box
  ArchiveDirCtrl = new wxTextCtrl( this,
                                   wxID_ANY,
                                   User->wxGetArchiveDir(),
                                   wxDefaultPosition,
                                   wxSize(400,25),
                                   0,
                                   wxDefaultValidator,
                                   wxT("Archive Directory") );
  bSizer2->Add( ArchiveDirCtrl, 0, wxALIGN_CENTER|wxALL, 5 );


  // add the static line
  FinalStaticLine = new wxStaticLine( this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  bSizer2->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );
  bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

  // setup all the buttons
  wxBoxSizer *bSizer3 = new wxBoxSizer( wxVERTICAL );

  m_userbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( this, wxID_OK );
  m_userbuttonsizer->AddButton( m_userOK );
  m_userCancel = new wxButton( this, wxID_CANCEL );
  m_userbuttonsizer->AddButton( m_userCancel );
  m_userbuttonsizer->Realize();

  bSizer3->Add( m_userbuttonsizer, 1, wxEXPAND, 5 );

  bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );

  // draw the diag box until we get more info
  this->SetSizer( bSizer1 );
  this->Layout();
  bSizer1->Fit(this);
  this->Centre( wxBOTH );
}

void PortalUserPrefWin::OnPressOk( wxCommandEvent& ok ){
  // user pressed 'ok', walk through all the options and update
  // the configuration

  User->SetProjectDir(ProjectDirCtrl->GetValue());
  User->SetArchiveDir(ArchiveDirCtrl->GetValue());

  User->WriteConfig();

  this->EndModal( wxID_OK );
}

void PortalUserPrefWin::OnPressCancel( wxCommandEvent& ok ){
  // cancel everything and close the window
  this->EndModal(wxID_CANCEL);
}

PortalUserPrefWin::~PortalUserPrefWin(){
}

// EOF
