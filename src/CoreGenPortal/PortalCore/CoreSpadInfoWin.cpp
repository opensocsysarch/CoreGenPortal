//
// _CORESPADINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreSpadInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreSpadInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreSpadInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreSpadInfoWin::CoreSpadInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenSpad *Spad )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Spad == nullptr ){
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
  //-- spad name
  SpadNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Scratchpad Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  SpadNameText->Wrap(-1);
  InnerSizer->Add( SpadNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  SpadNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Spad->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("SpadName") );
  InnerSizer->Add( SpadNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- scratchpad size
  SizeNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Scratchpad Size (in bytes)"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  SizeNameText->Wrap(-1);
  InnerSizer->Add( SizeNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  SizeCtrl = new wxTextCtrl( Wnd,
                             wxID_ANY,
                             wxString::Format(wxT("%i"),Spad->GetMemSize()),
                             wxDefaultPosition,
                             wxSize(400,25),
                             wxTE_READONLY,
                             wxDefaultValidator,
                             wxT("SpadSize") );
  InnerSizer->Add( SizeCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- request ports
  RqstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Request Ports"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  RqstNameText->Wrap(-1);
  InnerSizer->Add( RqstNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  RqstCtrl = new wxTextCtrl( Wnd,
                             wxID_ANY,
                             wxString::Format(wxT("%i"),Spad->GetRqstPorts()),
                             wxDefaultPosition,
                             wxSize(400,25),
                             wxTE_READONLY,
                             wxDefaultValidator,
                             wxT("RqstPorts") );
  InnerSizer->Add( RqstCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- response ports
  RspNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Response Ports"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  RspNameText->Wrap(-1);
  InnerSizer->Add( RspNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  RspCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxString::Format(wxT("%i"),Spad->GetRspPorts()),
                            wxDefaultPosition,
                            wxSize(400,25),
                            wxTE_READONLY,
                            wxDefaultValidator,
                            wxT("RspPorts") );
  InnerSizer->Add( RspCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- starting address
  StartNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Starting Memory Address"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  StartNameText->Wrap(-1);
  InnerSizer->Add( StartNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  // TODO: print this in hex
  wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                  Spad->GetStartAddr() );
  StartCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            tmp,
                            wxDefaultPosition,
                            wxSize(400,25),
                            wxTE_READONLY,
                            wxDefaultValidator,
                            wxT("StartAddr") );
  InnerSizer->Add( StartCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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

void CoreSpadInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreSpadInfoWin::~CoreSpadInfoWin(){
}

// EOF
