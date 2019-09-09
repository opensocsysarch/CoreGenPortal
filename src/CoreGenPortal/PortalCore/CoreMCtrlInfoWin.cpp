//
// _COREMCTRLINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreMCtrlInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreMCtrlInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreMCtrlInfoWin::OnPressOk)
  EVT_BUTTON(wxID_SAVE, CoreMCtrlInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CoreMCtrlInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreMCtrlInfoWin::CoreMCtrlInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenMCtrl *MCtrl )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,250), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->MCtrlNode = MCtrl;

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
  MCtrlNameSizer = new wxBoxSizer( wxHORIZONTAL );
  MCtrlNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Memory Controller Name"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  MCtrlNameText->Wrap(-1);
  MCtrlNameSizer->Add( MCtrlNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  MCtrlCtrl = new wxTextCtrl( Wnd,
                              0,
                              MCtrl ? wxString(MCtrl->GetName()) : "",
                              wxDefaultPosition,
                              wxSize(320,25),
                              wxTE_PROCESS_ENTER,
                              wxDefaultValidator,
                              wxT("MCtrlName") );
  MCtrlNameSizer->Add( MCtrlCtrl, 0, wxALL, 0 );
  InnerSizer->Add(MCtrlNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- input ports
  InputPortSizer = new wxBoxSizer( wxHORIZONTAL );
  InputPortText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Input Ports"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  InputPortText->Wrap(-1);
  InputPortSizer->Add( InputPortText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  InputPortCtrl = new wxTextCtrl( Wnd,
                                  1,
                                  MCtrl ? wxString::Format(wxT("%i"),MCtrl->GetNumInputPorts()) : "",
                                  wxDefaultPosition,
                                  wxSize(320,25),
                                  wxTE_PROCESS_ENTER,
                                  wxDefaultValidator,
                                  wxT("InputPorts") );
  InputPortSizer->Add( InputPortCtrl, 0, wxALL, 0 );
  InnerSizer->Add( InputPortSizer, 0, wxALIGN_CENTER|wxALL, 0);

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(MCtrl) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreMCtrlInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreMCtrlInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->MCtrlNode, CGMCtrl))
    this->EndModal(wxID_SAVE);
}

void CoreMCtrlInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->MCtrlNode, CGMCtrl);
}

CoreMCtrlInfoWin::~CoreMCtrlInfoWin(){
}

// EOF
