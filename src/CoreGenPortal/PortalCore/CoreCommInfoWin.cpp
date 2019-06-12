//
// _CORECOMMINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreCommInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreCommInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreCommInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreCommInfoWin::CoreCommInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenComm *Comm )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Comm == nullptr ){
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
  //-- comm
  CommNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Communication Node Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  CommNameText->Wrap(-1);
  InnerSizer->Add( CommNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  CommNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Comm->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("CommName") );
  InnerSizer->Add( CommNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- comm type
  CommTypeText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Communication Node Type"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  CommTypeText->Wrap(-1);
  InnerSizer->Add( CommTypeText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  CommTypeCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxEmptyString,
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("CommType") );
  switch( Comm->GetCommType() ){
  case CGCommP2P:
    CommTypeCtrl->AppendText(wxT("Point-to-Point"));
    break;
  case CGCommBus:
    CommTypeCtrl->AppendText(wxT("Bus"));
    break;
  case CGCommNoc:
    CommTypeCtrl->AppendText(wxT("Network on Chip"));
    break;
  case CGCommUnk:
  default:
    CommTypeCtrl->AppendText(wxT("Unknown"));
    break;
  }
  InnerSizer->Add( CommTypeCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- width
  WidthText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Communication Channel Width"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  WidthText->Wrap(-1);
  InnerSizer->Add( CommNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  WidthCtrl = new wxTextCtrl( Wnd,
                              wxID_ANY,
                              wxString::Format(wxT("%i"),Comm->GetWidth()),
                              wxDefaultPosition,
                              wxSize(400,25),
                              wxTE_READONLY,
                              wxDefaultValidator,
                              wxT("CommWidth") );
  InnerSizer->Add( WidthCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- endpoints
  EndpointText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Endpoints"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  EndpointText->Wrap(-1);
  InnerSizer->Add( EndpointText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  EndpointCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(400,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("endpoints") );

  for( unsigned i=0; i<Comm->GetNumEndpoints(); i++ ){
    EndpointCtrl->AppendText(wxString(Comm->GetEndpoint(i)->GetName())+wxT("\n") );
  }
  InnerSizer->Add( EndpointCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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

void CoreCommInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreCommInfoWin::~CoreCommInfoWin(){
}

// EOF
