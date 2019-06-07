//
// _COREREGCLASSINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreRegClassInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreRegClassInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreRegClassInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreRegClassInfoWin::CoreRegClassInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenRegClass *RegClass )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( RegClass == nullptr ){
    this->EndModal(wxID_OK);
  }

  // init the internals
  this->SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the outer box sizer
  OuterSizer = new wxBoxSizer( wxVERTICAL );

  // create the scrolled window
  Wnd = new wxScrolledWindow(this,
                             wxID_ANY,
                             wxDefaultPosition,
                             wxDefaultSize,
                             0,
                             wxT("Scroll"));

  // create the inner sizer
  InnerSizer = new wxBoxSizer( wxVERTICAL );

  // add all the interior data
  // -- reg class
  RegClassNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Register Class Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  RegClassNameText->Wrap(-1);
  InnerSizer->Add( RegClassNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  RegClassNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(RegClass->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Reg Class Name") );
  InnerSizer->Add( RegClassNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- registers
  RegNameText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Registers"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  RegNameText->Wrap(-1);
  InnerSizer->Add( RegNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  RegNameCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(400,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("registers") );

  for( unsigned i=0; i<RegClass->GetNumReg(); i++ ){
    RegNameCtrl->AppendText(wxString(RegClass->GetReg(i)->GetName())+wxT("\n") );
  }
  InnerSizer->Add( RegNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( Wnd, wxID_OK );
  m_socbuttonsizer->AddButton( m_userOK );
  m_socbuttonsizer->Realize();
  InnerSizer->Add( m_socbuttonsizer, 1, wxEXPAND, 5 );

  Wnd->SetScrollbars(20,20,50,50);
  Wnd->SetSizer( InnerSizer );
  Wnd->SetAutoLayout(true);
  Wnd->Layout();

  // draw the dialog box until we get more info
  OuterSizer->Add(Wnd, 1, wxEXPAND | wxALL, 5 );
  this->SetSizer( OuterSizer );
  this->SetAutoLayout( true );
  this->Layout();
}

void CoreRegClassInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreRegClassInfoWin::~CoreRegClassInfoWin(){
}

// EOF
