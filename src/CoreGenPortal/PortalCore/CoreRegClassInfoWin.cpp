//
// _COREREGCLASSINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreRegClassInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreRegClassInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreRegClassInfoWin::OnPressOk)
  EVT_TEXT_ENTER(wxID_ANY, CoreRegClassInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreRegClassInfoWin::CoreRegClassInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenRegClass *RegClass )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,250), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  RegClassNode = RegClass;

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
  // -- reg class
  RegClassNameSizer = new wxBoxSizer( wxHORIZONTAL );
  RegClassNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Register Class Name"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  RegClassNameText->Wrap(-1);
  RegClassNameSizer->Add( RegClassNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  RegClassNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 RegClass ? wxString(RegClass->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 wxTE_PROCESS_ENTER,
                                 wxDefaultValidator,
                                 wxT("Reg Class Name") );
  RegClassNameSizer->Add( RegClassNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( RegClassNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- registers
  RegNameSizer = new wxBoxSizer( wxHORIZONTAL );
  RegNameText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Registers"),
                              wxDefaultPosition,
                              wxSize(160,-1),
                              0 );
  RegNameText->Wrap(-1);
  RegNameSizer->Add( RegNameText, 0, wxALIGN_CENTER|wxALL, 0 );

  RegNameCtrl = new wxTextCtrl( Wnd,
                            1,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(320,100),
                            wxTE_PROCESS_ENTER|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("registers") );
  if(RegClass){
    for( unsigned i=0; i<RegClass->GetNumReg(); i++ ){
      RegNameCtrl->AppendText(wxString(RegClass->GetReg(i)->GetName())+wxT("\n") );
    }
  }
  RegNameSizer->Add( RegNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( RegNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

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

void CoreRegClassInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreRegClassInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->RegClassNode, CGRegC);
}

CoreRegClassInfoWin::~CoreRegClassInfoWin(){
}

// EOF
