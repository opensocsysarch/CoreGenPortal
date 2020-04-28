//
// _COREDATAPATHINFOWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreDataPathInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreDataPathInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreDataPathInfoWin::OnPressOk)
  EVT_BUTTON(wxID_SAVE, CoreDataPathInfoWin::OnSave)
wxEND_EVENT_TABLE()

CoreDataPathInfoWin::CoreDataPathInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenDataPath *DP )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->CoreGenDPath = DP;

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

  // init all the options
  //-- data path name
  DPathNameSizer = new wxBoxSizer( wxHORIZONTAL );
  DPathNameText = new wxStaticText(Wnd,
                                 6,
                                 wxT("DataPath Name"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  DPathNameText->Wrap(-1);
  DPathNameSizer->Add( DPathNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  // -- data path name box
  DPathNameCtrl = new wxTextCtrl(Wnd,
                               0,
                               CoreGenDPath ? wxString(CoreGenDPath->GetName()) : "",
                               wxDefaultPosition,
                               wxSize(320,25),
                               0,
                               wxDefaultValidator,
                               wxT("DataPath Name") );
  DPathNameSizer->Add( DPathNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( DPathNameSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- style
  StyleNameSizer = new wxBoxSizer( wxHORIZONTAL );
  StyleNameText = new wxStaticText(Wnd,
                                 6,
                                 wxT("Style"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  StyleNameText->Wrap(-1);
  StyleNameSizer->Add( StyleNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  // -- style name box
  StyleNameCtrl = new wxTextCtrl(Wnd,
                               0,
                               CoreGenDPath ? wxString(CoreGenDPath->GetStyle()) : "",
                               wxDefaultPosition,
                               wxSize(320,25),
                               0,
                               wxDefaultValidator,
                               wxT("DataPath Style") );
  StyleNameSizer->Add( StyleNameCtrl,0, wxALL, 0 );
  InnerSizer->Add( StyleNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(CoreGenDPath) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreDataPathInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreDataPathInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->CoreGenDPath, CGDPath))
    this->EndModal(wxID_SAVE);
}

CoreDataPathInfoWin::~CoreDataPathInfoWin(){
}

// EOF
