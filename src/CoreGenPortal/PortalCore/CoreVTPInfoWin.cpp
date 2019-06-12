//
// _COREVTPINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreVTPInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreVTPInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreVTPInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreVTPInfoWin::CoreVTPInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenVTP *VTP )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( VTP == nullptr ){
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
  //-- memory controller name
  VTPNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("VTP Controller Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  VTPNameText->Wrap(-1);
  InnerSizer->Add( VTPNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  VTPCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxString(VTP->GetName()),
                            wxDefaultPosition,
                            wxSize(400,25),
                            wxTE_READONLY,
                            wxDefaultValidator,
                            wxT("VTPName") );
  InnerSizer->Add( VTPCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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

void CoreVTPInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreVTPInfoWin::~CoreVTPInfoWin(){
}

// EOF
