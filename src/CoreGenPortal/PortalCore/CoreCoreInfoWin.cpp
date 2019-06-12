//
// _CORECOREINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreCoreInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreCoreInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreCoreInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreCoreInfoWin::CoreCoreInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenCore *Core )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Core == nullptr ){
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
  //-- core name
  CoreNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("Core Name"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  CoreNameText->Wrap(-1);
  bSizer2->Add( CoreNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- core name box
  CoreNameCtrl = new wxTextCtrl(this,
                               wxID_ANY,
                               wxString(Core->GetName()),
                               wxDefaultPosition,
                               wxSize(400,25),
                               wxTE_READONLY,
                               wxDefaultValidator,
                               wxT("Core Name") );
  bSizer2->Add( CoreNameCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- thread unit name
  ThreadUnitText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("Thread Units"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  ThreadUnitText->Wrap(-1);
  bSizer2->Add( ThreadUnitText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- thread unit number
  ThreadUnitCtrl = new wxTextCtrl(this,
                               wxID_ANY,
                               wxString::Format(wxT("%i"),Core->GetNumThreadUnits()),
                               wxDefaultPosition,
                               wxSize(400,25),
                               wxTE_READONLY,
                               wxDefaultValidator,
                               wxT("Thread Units") );
  bSizer2->Add( ThreadUnitCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- isa name
  ISANameText= new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("ISA Name"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  ISANameText->Wrap(-1);
  bSizer2->Add( ISANameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- isa name ctrl
  ISACtrl = new wxTextCtrl(this,
                           wxID_ANY,
                           wxString(Core->GetISA()->GetName()),
                           wxDefaultPosition,
                           wxSize(400,25),
                           wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("ISA") );
  bSizer2->Add( ISACtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- cache name
  CacheNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("Cache Name"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  CacheNameText->Wrap(-1);
  bSizer2->Add( CacheNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- cache name ctrl
  CacheCtrl = new wxTextCtrl(this,
                           wxID_ANY,
                           wxString(Core->GetCache()->GetName()),
                           wxDefaultPosition,
                           wxSize(400,25),
                           wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("Cache") );
  bSizer2->Add( CacheCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- regclass name
  RegClassNameText = new wxStaticText(this,
                                 wxID_ANY,
                                 wxT("Register Classes"),
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 0 );
  RegClassNameText->Wrap(-1);
  bSizer2->Add( RegClassNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- reg classes
  RegClassCtrl = new wxTextCtrl(this,
                           wxID_ANY,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(400,100),
                           wxTE_MULTILINE|wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("Reg Classes") );
  bSizer2->Add( RegClassCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
  for( unsigned i=0; i<Core->GetNumRegClass(); i++ ){
    RegClassCtrl->AppendText(wxString(Core->GetRegClass(i)->GetName()));
  }

  //-- ext name
  ExtNameText = new wxStaticText(this,
                             wxID_ANY,
                             wxT("Extensions"),
                             wxDefaultPosition,
                             wxDefaultSize,
                             0 );
  ExtNameText->Wrap(-1);
  bSizer2->Add( ExtNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- ext's
  ExtCtrl = new wxTextCtrl(this,
                           wxID_ANY,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(400,100),
                           wxTE_MULTILINE|wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("Extensions") );
  bSizer2->Add( ExtCtrl, 0, wxALIGN_CENTER|wxALL, 5 );
  for( unsigned i=0; i<Core->GetNumExt(); i++ ){
    ExtCtrl->AppendText(wxString(Core->GetExt(i)->GetName()));
  }

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

void CoreCoreInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreCoreInfoWin::~CoreCoreInfoWin(){
}

// EOF
