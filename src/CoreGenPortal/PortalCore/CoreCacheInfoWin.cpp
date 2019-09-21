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
  EVT_BUTTON(wxID_SAVE, CoreCacheInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CoreCacheInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreCacheInfoWin::CoreCacheInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenCache *Cache)
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,350), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->CacheNode = Cache;

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
  CacheNameSizer = new wxBoxSizer( wxHORIZONTAL );
  CacheNameText = new wxStaticText( Wnd,
                                   5,
                                   wxT("Cache Name"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  CacheNameText->Wrap(-1);
  CacheNameSizer->Add( CacheNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  CacheNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 Cache ? wxString(Cache->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320, 25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("CacheName") );
  CacheNameSizer->Add( CacheNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CacheNameSizer, 0, wxALIGN_CENTER|wxBOTTOM, 5 );

  //-- sets
  CacheSetsSizer = new wxBoxSizer( wxHORIZONTAL );
  SetsText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Cache Sets"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  SetsText->Wrap(-1);
  CacheSetsSizer->Add( SetsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  SetsCtrl = new wxTextCtrl( Wnd,
                             1,
                             Cache ? wxString::Format(wxT("%i"),Cache->GetSets()) : "",
                             wxDefaultPosition,
                             wxSize(320,25),
                             0,
                             wxDefaultValidator,
                             wxT("Sets") );
  CacheSetsSizer->Add( SetsCtrl, 0, wxALL, 0 );
  InnerSizer->Add(CacheSetsSizer, 0, wxALIGN_CENTER|wxALL, 0);

  //-- ways
  CacheWaysSizer = new wxBoxSizer( wxHORIZONTAL );
  WaysText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Cache Ways"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  WaysText->Wrap(-1);
  CacheWaysSizer->Add( WaysText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  WaysCtrl = new wxTextCtrl( Wnd,
                             2,
                             Cache ? wxString::Format(wxT("%i"),Cache->GetWays()) : "",
                             wxDefaultPosition,
                             wxSize(320,25),
                             0,
                             wxDefaultValidator,
                             wxT("Ways") );
  CacheWaysSizer->Add( WaysCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CacheWaysSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- child cache
  ChildCacheSizer = new wxBoxSizer( wxHORIZONTAL );
  ChildCacheText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Child Cache Node"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  ChildCacheText->Wrap(-1);
  ChildCacheSizer->Add( ChildCacheText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  ChildCacheCtrl = new wxTextCtrl( Wnd,
                             3,
                             wxEmptyString,
                             wxDefaultPosition,
                             wxSize(320,25),
                             0,
                             wxDefaultValidator,
                             wxT("ChildCache") );
  if( Cache && Cache->IsParentLevel() ){
    CoreGenCache *Child = Cache->GetSubCache();
    if( Child != nullptr )
      ChildCacheCtrl->AppendText(wxString(Child->GetName()));
  }
  ChildCacheSizer->Add( ChildCacheCtrl, 0, wxALL, 0 );
  InnerSizer->Add( ChildCacheSizer, 0, wxALIGN_CENTER, 5 );

  //-- parent caches
  ParentCacheSizer = new wxBoxSizer( wxHORIZONTAL );
  ParentCacheText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Parent Cache Nodes"),
                                   wxDefaultPosition,
                                   wxSize(160, -1),
                                   0 );
  ParentCacheText->Wrap(-1);
  ParentCacheSizer->Add( ParentCacheText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  ParentCacheCtrl = new wxTextCtrl( Wnd,
                             4,
                             wxEmptyString,
                             wxDefaultPosition,
                             wxSize(320,100),
                             wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                             wxDefaultValidator,
                             wxT("ParentCache") );
  if( Cache && Cache->IsSubLevel() ){
    CoreGenCache *Parent;
    for(unsigned i = 0; i < Cache->GetNumParentCache(); i++){
      Parent = Cache->GetParentCache(i);
      if( Parent != nullptr )
        ParentCacheCtrl->AppendText(wxString(Parent->GetName()) + "\n");
    }
  }
  ParentCacheSizer->Add( ParentCacheCtrl, 0, wxALL, 0 );
  InnerSizer->Add( ParentCacheSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  // add space between info items and static line
  SpacerPanelSizer = new wxBoxSizer( wxVERTICAL );
  SpacerPanelSizer->Add( new wxPanel(this), 1, wxEXPAND, 0 );
  InnerSizer->Add(SpacerPanelSizer, 1, wxEXPAND, 0);

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 0, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(Cache) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreCacheInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreCacheInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->CacheNode, CGCache))
    this->EndModal(wxID_SAVE);
}

void CoreCacheInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->CacheNode, CGCache);
}

CoreCacheInfoWin::~CoreCacheInfoWin(){
}

// EOF
