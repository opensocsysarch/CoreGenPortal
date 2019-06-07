//
// _COREPINSTINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CorePInstInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CorePInstInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CorePInstInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CorePInstInfoWin::CorePInstInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenPseudoInst *PInst )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( PInst == nullptr ){
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
  //-- pseudo inst name
  PInstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Pseudo Instruction Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  PInstNameText->Wrap(-1);
  InnerSizer->Add( PInstNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  PInstNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(PInst->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Name") );
  InnerSizer->Add( PInstNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- inst name
  InstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Target Instruction Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  InstNameText->Wrap(-1);
  InnerSizer->Add( InstNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  InstNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(PInst->GetInst()->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Name") );
  InnerSizer->Add( InstNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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
                                 wxString(PInst->GetISA()->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  InnerSizer->Add( ISANameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(400,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("StoneCutter") );

  for( unsigned i=0; i<PInst->GetNumEncodings(); i++ ){
    wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                    PInst->GetEncoding(i)->GetEncoding() );
    EncCtrl->AppendText(wxString(PInst->GetEncoding(i)->GetField()) +
                        wxT(" = ") + tmp + wxT("\n"));
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

void CorePInstInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CorePInstInfoWin::~CorePInstInfoWin(){
}

// EOF
