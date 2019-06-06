//
// _COREINSTINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreInstInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreInstInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreInstInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreInstInfoWin::CoreInstInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenInst *Inst )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Inst == nullptr ){
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
  //-- inst name
  InstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  InstNameText->Wrap(-1);
  InnerSizer->Add( InstNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  InstNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Inst->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Name") );
  InnerSizer->Add( InstNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- inst format
  InstFNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Format"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  InstFNameText->Wrap(-1);
  InnerSizer->Add( InstFNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  InstFNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Inst->GetFormat()->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  InnerSizer->Add( InstFNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- isa name
  ISANameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Set"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  ISANameText->Wrap(-1);
  InnerSizer->Add( ISANameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  ISANameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Inst->GetISA()->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  InnerSizer->Add( ISANameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- syntax
  SyntaxText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Syntax"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  SyntaxText->Wrap(-1);
  InnerSizer->Add( SyntaxText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  SyntaxCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Inst->GetSyntax()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  InnerSizer->Add( SyntaxCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- stone cutter implementation
  StoneCText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("StoneCutter Syntax"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  StoneCText->Wrap(-1);
  InnerSizer->Add( StoneCText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  StoneCCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Inst->GetImpl()),
                                 wxDefaultPosition,
                                 wxSize(400,100),
                                 wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                                 wxDefaultValidator,
                                 wxT("StoneCutter") );
  InnerSizer->Add( StoneCCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- encodings
  EncText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Instruction Field Encodings"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  EncText->Wrap(-1);
  InnerSizer->Add( EncText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  EncCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxString(Inst->GetImpl()),
                            wxDefaultPosition,
                            wxSize(400,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("StoneCutter") );

  for( unsigned i=0; i<Inst->GetNumEncodings(); i++ ){
    wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                    Inst->GetEncoding(i)->GetEncoding() );
    EncCtrl->AppendText(wxString(Inst->GetEncoding(i)->GetField()) +
                        wxT(" = ") + tmp );
  }
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

void CoreInstInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreInstInfoWin::~CoreInstInfoWin(){
}

// EOF
