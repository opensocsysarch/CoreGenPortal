//
// _COREVTPINFOWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreVTPInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreVTPInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreVTPInfoWin::OnPressOk)
  EVT_BUTTON(wxID_SAVE, CoreVTPInfoWin::OnSave)
wxEND_EVENT_TABLE()

CoreVTPInfoWin::CoreVTPInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenVTP *VTP )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,175), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->VTPNode = (CoreGenVTP*)VTP;

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
  NameSizer = new wxBoxSizer( wxHORIZONTAL );
  VTPNameText = new wxStaticText( Wnd,
                                   1,
                                   wxT("VTP Controller Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  VTPNameText->Wrap(-1);
  NameSizer->Add( VTPNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  VTPCtrl = new wxTextCtrl( Wnd,
                            0,
                            VTP ? wxString(VTP->GetName()) : "",
                            wxDefaultPosition,
                            wxSize(320,25),
                            0,
                            wxDefaultValidator,
                            wxT("VTPName") );
  NameSizer->Add( VTPCtrl, 0, wxALL, 0 );
  InnerSizer->Add( NameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(VTP) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreVTPInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreVTPInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->VTPNode, CGVTP))
    this->EndModal(wxID_SAVE);
}

CoreVTPInfoWin::~CoreVTPInfoWin(){
}

// EOF
