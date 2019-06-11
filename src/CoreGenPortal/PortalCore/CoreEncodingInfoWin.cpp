//
// _COREENCODINGINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreEncodingInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreEncodingInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreEncodingInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreEncodingInfoWin::CoreEncodingInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenEncoding *Enc)
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Enc == nullptr ){
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
  //-- encoding name
  EncodingNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Encoding Node Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  EncodingNameText->Wrap(-1);
  InnerSizer->Add( EncodingNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  EncNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Enc->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("EncName") );
  InnerSizer->Add( EncNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- field name
  FieldNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Encoding Field"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  FieldNameText->Wrap(-1);
  InnerSizer->Add( FieldNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  FieldNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Enc->GetField()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("EncField") );
  InnerSizer->Add( FieldNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- field length
  FieldLenText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Encoding Length (in bits)"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  FieldLenText->Wrap(-1);
  InnerSizer->Add( FieldLenText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  FieldLenCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString::Format(wxT("%i"),Enc->GetLength()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("EncLen") );
  InnerSizer->Add( FieldLenCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- encoding
  EncText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Encoding"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  EncText->Wrap(-1);
  InnerSizer->Add( EncText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                    Enc->GetEncoding() );
  EncCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            tmp,
                            wxDefaultPosition,
                            wxSize(400,25),
                            wxTE_READONLY,
                            wxDefaultValidator,
                            wxT("Enc") );
  InnerSizer->Add( EncCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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

void CoreEncodingInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreEncodingInfoWin::~CoreEncodingInfoWin(){
}

// EOF
