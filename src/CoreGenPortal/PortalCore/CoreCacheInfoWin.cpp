//
// _CORECACHEINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreCacheInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreCacheInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreCacheInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreCacheInfoWin::CoreCacheInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenCache *Cache)
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Cache == nullptr ){
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
  //-- cache
  CacheNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Cache Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  CacheNameText->Wrap(-1);
  InnerSizer->Add( CacheNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  CacheNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Cache->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("CacheName") );
  InnerSizer->Add( CacheNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- sets
  SetsText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Cache Sets"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  SetsText->Wrap(-1);
  InnerSizer->Add( SetsText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  SetsCtrl = new wxTextCtrl( Wnd,
                             wxID_ANY,
                             wxString::Format(wxT("%i"),Cache->GetSets()),
                             wxDefaultPosition,
                             wxSize(400,25),
                             wxTE_READONLY,
                             wxDefaultValidator,
                             wxT("Sets") );
  InnerSizer->Add( SetsCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- ways
  WaysText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Cache Ways"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  WaysText->Wrap(-1);
  InnerSizer->Add( WaysText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  WaysCtrl = new wxTextCtrl( Wnd,
                             wxID_ANY,
                             wxString::Format(wxT("%i"),Cache->GetWays()),
                             wxDefaultPosition,
                             wxSize(400,25),
                             wxTE_READONLY,
                             wxDefaultValidator,
                             wxT("Ways") );
  InnerSizer->Add( WaysCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- parent cache
  ParentCacheText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Child Cache Node"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  ParentCacheText->Wrap(-1);
  InnerSizer->Add( ParentCacheText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  ParentCacheCtrl = new wxTextCtrl( Wnd,
                             wxID_ANY,
                             wxEmptyString,
                             wxDefaultPosition,
                             wxSize(400,25),
                             wxTE_READONLY,
                             wxDefaultValidator,
                             wxT("ParentCache") );
  if( Cache->IsParentLevel() ){
    CoreGenCache *Parent = Cache->GetSubCache();
    if( Parent != nullptr )
      ParentCacheCtrl->AppendText(wxString(Parent->GetName()));
  }
  InnerSizer->Add( ParentCacheCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- child cache
  ChildCacheText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Parent Cache Node"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  ChildCacheText->Wrap(-1);
  InnerSizer->Add( ChildCacheText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  ChildCacheCtrl = new wxTextCtrl( Wnd,
                             wxID_ANY,
                             wxEmptyString,
                             wxDefaultPosition,
                             wxSize(400,25),
                             wxTE_READONLY,
                             wxDefaultValidator,
                             wxT("ChildCache") );
  if( Cache->IsSubLevel() ){
    CoreGenCache *Child = Cache->GetParentCache();
    if( Child != nullptr )
      ChildCacheCtrl->AppendText(wxString(Child->GetName()));
  }
  InnerSizer->Add( ChildCacheCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

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

void CoreCacheInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreCacheInfoWin::~CoreCacheInfoWin(){
}

// EOF
