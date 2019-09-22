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
  EVT_BUTTON(wxID_SAVE, CoreInstInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CoreInstInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreInstInfoWin::CoreInstInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenInst *Inst )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->InstNode = Inst;
  
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
  InstNameSizer = new wxBoxSizer( wxHORIZONTAL );
  InstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Name"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  InstNameText->Wrap(-1);
  InstNameSizer->Add( InstNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  InstNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 Inst ? wxString(Inst->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("Inst Name") );
  InstNameSizer->Add( InstNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add(InstNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- inst format
  InstFNameSizer = new wxBoxSizer( wxHORIZONTAL );
  InstFNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Format"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  InstFNameText->Wrap(-1);
  InstFNameSizer->Add( InstFNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  InstFNameCtrl = new wxTextCtrl( Wnd,
                                 1,
                                 Inst ? wxString(Inst->GetFormat()->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  InstFNameSizer->Add( InstFNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( InstFNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- isa name
  ISANameSizer = new wxBoxSizer( wxHORIZONTAL );
  ISANameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Set"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  ISANameText->Wrap(-1);
  ISANameSizer->Add( ISANameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  ISANameCtrl = new wxTextCtrl( Wnd,
                                 2,
                                 Inst ? wxString(Inst->GetISA()->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("ISA Name") );
  ISANameSizer->Add( ISANameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( ISANameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- syntax
  SyntaxSizer = new wxBoxSizer( wxHORIZONTAL );
  SyntaxText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Syntax"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  SyntaxText->Wrap(-1);
  SyntaxSizer->Add( SyntaxText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  SyntaxCtrl = new wxTextCtrl( Wnd,
                                 3,
                                 Inst ? wxString(Inst->GetSyntax()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  SyntaxSizer->Add( SyntaxCtrl, 0, wxALL, 0 );
  InnerSizer->Add( SyntaxSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- stone cutter implementation
  StoneCutterSizer = new wxBoxSizer( wxHORIZONTAL );
  StoneCText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("StoneCutter Syntax"),
                                   wxDefaultPosition,
                                   wxSize(160, -1), 
                                   0 );
  StoneCText->Wrap(-1);
  StoneCutterSizer->Add( StoneCText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  StoneCCtrl = new wxTextCtrl( Wnd,
                                 4,
                                 Inst ? wxString(Inst->GetImpl()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,100),
                                 wxTE_MULTILINE|wxHSCROLL,
                                 wxDefaultValidator,
                                 wxT("StoneCutter") );
  StoneCutterSizer->Add( StoneCCtrl, 0, wxALL, 0 );
  InnerSizer->Add( StoneCutterSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- encodings
  EncodingSizer = new wxBoxSizer( wxHORIZONTAL );
  EncText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Inst Field Encodings"),
                              wxDefaultPosition,
                              wxSize(160, -1),
                              0 );
  EncText->Wrap(-1);
  EncodingSizer->Add( EncText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  EncCtrl = new wxTextCtrl( Wnd,
                            5,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(320,100),
                            wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("StoneCutter") );
  if(Inst){
    for( unsigned i=0; i<Inst->GetNumEncodings(); i++ ){
      wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                      Inst->GetEncoding(i)->GetEncoding() );
      EncCtrl->AppendText(wxString(Inst->GetEncoding(i)->GetField()) +
                          wxT(" = ") + tmp + wxT("\n"));
    }
  } 
  EncodingSizer->Add( EncCtrl, 0, wxALL, 0 );
  InnerSizer->Add( EncodingSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(Inst) m_userOK = new wxButton( Wnd, wxID_OK );
  else m_userOK = new wxButton( Wnd, wxID_CANCEL );
  m_userSAVE = new wxButton( Wnd, wxID_SAVE);
  m_socbuttonsizer->SetAffirmativeButton( m_userOK );
  m_socbuttonsizer->SetCancelButton( m_userSAVE );
  m_socbuttonsizer->Realize();
  InnerSizer->Add( m_socbuttonsizer, 0, wxALL, 5 );

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

void CoreInstInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->InstNode, CGInst))
    this->EndModal(wxID_SAVE);
}

void CoreInstInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->InstNode, CGInst);
}

CoreInstInfoWin::~CoreInstInfoWin(){
}

// EOF
