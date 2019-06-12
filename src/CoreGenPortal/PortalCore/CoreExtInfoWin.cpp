//
// _COREEXTINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreExtInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreExtInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreExtInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreExtInfoWin::CoreExtInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenExt *Ext )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Ext == nullptr ){
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
  //-- extension name
  ExtNameText = new wxStaticText( Wnd,
                                  wxID_ANY,
                                  wxT("Extension Name"),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0 );
  ExtNameText->Wrap(-1);
  InnerSizer->Add( ExtNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  ExtNameCtrl = new wxTextCtrl( Wnd,
                                wxID_ANY,
                                wxString(Ext->GetName()),
                                wxDefaultPosition,
                                wxSize(400,25),
                                wxTE_READONLY,
                                wxDefaultValidator,
                                wxT("ExtName") );
  InnerSizer->Add( ExtNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );


  //-- extension type
  ExtTypeText = new wxStaticText( Wnd,
                                  wxID_ANY,
                                  wxT("Extension Type"),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0 );
  ExtTypeText->Wrap(-1);
  InnerSizer->Add( ExtTypeText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  ExtTypeCtrl = new wxTextCtrl( Wnd,
                                wxID_ANY,
                                wxEmptyString,
                                wxDefaultPosition,
                                wxSize(400,25),
                                wxTE_READONLY,
                                wxDefaultValidator,
                                wxT("ExtType") );
  switch(Ext->GetType()){
  case CGExtTemplate:
    ExtTypeCtrl->AppendText(wxT("Template extension"));
    break;
  case CGExtModule:
    ExtTypeCtrl->AppendText(wxT("Module extension"));
    break;
  case CGExtComm:
    ExtTypeCtrl->AppendText(wxT("Communications extension"));
    break;
  case CGExtUnk:
  default:
    ExtTypeCtrl->AppendText(wxT("Unknown extension"));
    break;
  }
  InnerSizer->Add( ExtTypeCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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

void CoreExtInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreExtInfoWin::~CoreExtInfoWin(){
}

// EOF
