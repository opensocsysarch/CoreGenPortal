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
  EVT_BUTTON(wxID_SAVE, CorePInstInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CorePInstInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CorePInstInfoWin::CorePInstInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenPseudoInst *PInst )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,350), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->PInstNode = PInst;

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
  PInstNameSizer = new wxBoxSizer( wxHORIZONTAL );
  PInstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Pseudo Instruction Name"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  PInstNameText->Wrap(-1);
  PInstNameSizer->Add( PInstNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  PInstNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 PInst ? wxString(PInst->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 wxTE_PROCESS_ENTER,
                                 wxDefaultValidator,
                                 wxT("Inst Name") );
  PInstNameSizer->Add( PInstNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( PInstNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- inst name
  InstNameSizer = new wxBoxSizer( wxHORIZONTAL );
  InstNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Target Instruction Name"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  InstNameText->Wrap(-1);
  InstNameSizer->Add( InstNameText, 0, wxALIGN_CENTER|wxALL, 0 );

  InstNameCtrl = new wxTextCtrl( Wnd,
                                 1,
                                 PInst ? wxString(PInst->GetInst()->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 wxTE_PROCESS_ENTER,
                                 wxDefaultValidator,
                                 wxT("Inst Name") );
  InstNameSizer->Add( InstNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( InstNameSizer, 0, wxALIGN_CENTER|wxALL, 0 );

  //-- isa name
  ISANameSizer = new wxBoxSizer( wxHORIZONTAL );
  ISANameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Instruction Set"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  ISANameText->Wrap(-1);
  ISANameSizer->Add( ISANameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  ISANameCtrl = new wxTextCtrl( Wnd,
                                 2,
                                 PInst ? wxString(PInst->GetISA()->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Inst Format Name") );
  ISANameSizer->Add( ISANameCtrl, 0, wxALL, 0 );
  InnerSizer->Add(ISANameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- encodings
  EncodingSizer = new wxBoxSizer( wxHORIZONTAL );
  EncText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Instruction Field Encodings"),
                              wxDefaultPosition,
                              wxSize(160,-1),
                              0 );
  EncText->Wrap(-1);
  EncodingSizer->Add( EncText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  EncCtrl = new wxTextCtrl( Wnd,
                            3,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(320,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("StoneCutter") );
  if(PInst){
    for( unsigned i=0; i<PInst->GetNumEncodings(); i++ ){
      wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                      PInst->GetEncoding(i)->GetEncoding() );
      EncCtrl->AppendText(wxString(PInst->GetEncoding(i)->GetField()) +
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
  if(PInst) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CorePInstInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CorePInstInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->PInstNode, CGPInst))
    this->EndModal(wxID_SAVE);
}

void CorePInstInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->PInstNode, CGPInst);
}

CorePInstInfoWin::~CorePInstInfoWin(){
}

// EOF
