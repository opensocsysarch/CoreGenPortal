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
  EVT_BUTTON(wxID_SAVE, CoreSpadInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CoreSpadInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreSpadInfoWin::CoreSpadInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenSpad *Spad )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,325), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  SpadNode = Spad;

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
  SpadNameSizer = new wxBoxSizer( wxHORIZONTAL );
  SpadNameText = new wxStaticText( Wnd,
                                   5,
                                   wxT("Scratchpad Name"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  SpadNameText->Wrap(-1);
  SpadNameSizer->Add( SpadNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  SpadNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 Spad ? wxString(Spad->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("SpadName") );
  SpadNameSizer->Add( SpadNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( SpadNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- scratchpad size
  SizeNameSizer = new wxBoxSizer( wxHORIZONTAL );
  SizeNameText = new wxStaticText( Wnd,
                                   6,
                                   wxT("Scratchpad Size (bytes)"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  SizeNameText->Wrap(-1);
  SizeNameSizer->Add( SizeNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  SizeCtrl = new wxTextCtrl( Wnd,
                             1,
                             Spad ? wxString::Format(wxT("%i"),Spad->GetMemSize()) : "",
                             wxDefaultPosition,
                             wxSize(320,25),
                             0,
                             wxDefaultValidator,
                             wxT("SpadSize") );
  SizeNameSizer->Add( SizeCtrl, 0, wxALL, 0 );
  InnerSizer->Add(SizeNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- request ports
  RqstNameSizer = new wxBoxSizer( wxHORIZONTAL );
  RqstNameText = new wxStaticText( Wnd,
                                   7,
                                   wxT("Request Ports"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  RqstNameText->Wrap(-1);
  RqstNameSizer->Add( RqstNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  RqstCtrl = new wxTextCtrl( Wnd,
                             2,
                             Spad ? wxString::Format(wxT("%i"),Spad->GetRqstPorts()) : "",
                             wxDefaultPosition,
                             wxSize(320,25),
                             0,
                             wxDefaultValidator,
                             wxT("RqstPorts") );
  RqstNameSizer->Add( RqstCtrl, 0, wxALL, 0 );
  InnerSizer->Add( RqstNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- response ports
  RspNameSizer = new wxBoxSizer( wxHORIZONTAL );
  RspNameText = new wxStaticText( Wnd,
                                   8,
                                   wxT("Response Ports"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  RspNameText->Wrap(-1);
  RspNameSizer->Add( RspNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  RspCtrl = new wxTextCtrl( Wnd,
                            3,
                            Spad ? wxString::Format(wxT("%i"),Spad->GetRspPorts()) : "",
                            wxDefaultPosition,
                            wxSize(320,25),
                            0,
                            wxDefaultValidator,
                            wxT("RspPorts") );
  RspNameSizer->Add( RspCtrl, 0, wxALL, 0 );
  InnerSizer->Add( RspNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- starting address
  StartNameSizer = new wxBoxSizer( wxHORIZONTAL );
  StartNameText = new wxStaticText( Wnd,
                                   9,
                                   wxT("Starting Mem Address"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  StartNameText->Wrap(-1);
  StartNameSizer->Add( StartNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  // TODO: print this in hex
  wxString tmp;
  if(Spad){
    wxString tmp = wxString::Format("%" wxLongLongFmtSpec "u",
                                  Spad->GetStartAddr() );
  }
  else{
    tmp = "";
  }
  StartCtrl = new wxTextCtrl( Wnd,
                            4,
                            tmp,
                            wxDefaultPosition,
                            wxSize(320,25),
                            0,
                            wxDefaultValidator,
                            wxT("StartAddr") );
  StartNameSizer->Add( StartCtrl, 0, wxALL, 0 );
  InnerSizer->Add( StartNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(Spad) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreSpadInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreSpadInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->SpadNode, CGSpad))
    this->EndModal(wxID_SAVE);
}

void CoreSpadInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->SpadNode, CGSpad);
}

CoreSpadInfoWin::~CoreSpadInfoWin(){
}

// EOF
