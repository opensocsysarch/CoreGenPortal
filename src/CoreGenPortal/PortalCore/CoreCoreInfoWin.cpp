//
// _CORECOREINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreCoreInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreCoreInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreCoreInfoWin::OnPressOk)
  EVT_BUTTON(wxID_SAVE, CoreCoreInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CoreCoreInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreCoreInfoWin::CoreCoreInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenCore *Core )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->CoreNode = Core;

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
  //-- core name
  CoreNameSizer = new wxBoxSizer( wxHORIZONTAL );
  CoreNameText = new wxStaticText(Wnd,
                                 6,
                                 wxT("Core Name"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  CoreNameText->Wrap(-1);
  CoreNameSizer->Add( CoreNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- core name box
  CoreNameCtrl = new wxTextCtrl(Wnd,
                               0,
                               Core ? wxString(Core->GetName()) : "",
                               wxDefaultPosition,
                               wxSize(320,25),
                               wxTE_PROCESS_ENTER,
                               wxDefaultValidator,
                               wxT("Core Name") );
  CoreNameSizer->Add( CoreNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CoreNameSizer, 0, wxALIGN_CENTER|wxALL, 5);
 
  //-- thread unit name
  ThreadUnitSizer = new wxBoxSizer( wxHORIZONTAL );
  ThreadUnitText = new wxStaticText(Wnd,
                                 7,
                                 wxT("Thread Units"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  ThreadUnitText->Wrap(-1);
  ThreadUnitSizer->Add( ThreadUnitText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- thread unit number
  ThreadUnitCtrl = new wxTextCtrl(Wnd,
                               1,
                               Core ? wxString::Format(wxT("%i"),Core->GetNumThreadUnits()) : "",
                               wxDefaultPosition,
                               wxSize(320,25),
                               wxTE_PROCESS_ENTER,
                               wxDefaultValidator,
                               wxT("Thread Units") );
  ThreadUnitSizer->Add( ThreadUnitCtrl, 0, wxALL, 0 );
  InnerSizer->Add( ThreadUnitSizer, 0, wxALIGN_CENTER|wxALL, 5);
 
  //-- isa name
  ISANameSizer = new wxBoxSizer( wxHORIZONTAL );
  ISANameText= new wxStaticText(Wnd,
                                 8,
                                 wxT("ISA Name"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  ISANameText->Wrap(-1);
  ISANameSizer->Add( ISANameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- isa name ctrl
  ISACtrl = new wxTextCtrl(Wnd,
                           2,
                           Core ? wxString(Core->GetISA()->GetName()) : "",
                           wxDefaultPosition,
                           wxSize(320,25),
                           wxTE_PROCESS_ENTER,
                           wxDefaultValidator,
                           wxT("ISA") );
  ISANameSizer->Add( ISACtrl, 0, wxALL, 0 );
  InnerSizer->Add( ISANameSizer, 0, wxALIGN_CENTER|wxALL, 5);
 
  //-- cache name
  CacheNameSizer = new wxBoxSizer( wxHORIZONTAL );
  CacheNameText = new wxStaticText(Wnd,
                                 9,
                                 wxT("Cache Name"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  CacheNameText->Wrap(-1);
  CacheNameSizer->Add( CacheNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- cache name ctrl
  CacheCtrl = new wxTextCtrl(Wnd,
                           3,
                           Core ? wxString(Core->GetCache()->GetName()) : "",
                           wxDefaultPosition,
                           wxSize(320,25),
                           wxTE_PROCESS_ENTER,
                           wxDefaultValidator,
                           wxT("Cache") );
  CacheNameSizer->Add( CacheCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CacheNameSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- regclass name
  RegClassSizer = new wxBoxSizer( wxHORIZONTAL );
  RegClassNameText = new wxStaticText(Wnd,
                                 10,
                                 wxT("Register Classes"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  RegClassNameText->Wrap(-1);
  RegClassSizer->Add( RegClassNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- reg classes
  RegClassCtrl = new wxTextCtrl(Wnd,
                           4,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(320,100),
                           wxTE_MULTILINE,
                           wxDefaultValidator,
                           wxT("Reg Classes") );
  if(Core){
    for( unsigned i=0; i<Core->GetNumRegClass(); i++ ){
      RegClassCtrl->AppendText(wxString(Core->GetRegClass(i)->GetName() + "\n"));
    }
  }
  RegClassSizer->Add( RegClassCtrl, 0, wxALL, 0 );
  InnerSizer->Add( RegClassSizer, 0, wxALIGN_CENTER|wxALL, 5 );
 
  //-- ext name
  ExtNameSizer = new wxBoxSizer( wxHORIZONTAL );
  ExtNameText = new wxStaticText(Wnd,
                             11,
                             wxT("Extensions"),
                             wxDefaultPosition,
                             wxSize(160,-1),
                             0 );
  ExtNameText->Wrap(-1);
  ExtNameSizer->Add( ExtNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  //-- ext's
  ExtCtrl = new wxTextCtrl(Wnd,
                           5,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(320,100),
                           wxTE_MULTILINE,
                           wxDefaultValidator,
                           wxT("Extensions") );
  if(Core){
    for( unsigned i=0; i<Core->GetNumExt(); i++ ){
      ExtCtrl->AppendText(wxString(Core->GetExt(i)->GetName() + "\n"));
    }
  }
  ExtNameSizer->Add( ExtCtrl, 0, wxALL, 0 );
  InnerSizer->Add( ExtNameSizer, 0, wxALIGN_CENTER|wxALL, 5);

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(Core) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreCoreInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreCoreInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->CoreNode, CGCore))
    this->EndModal(wxID_SAVE);
}

void CoreCoreInfoWin::OnPressEnter(wxCommandEvent& enter){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->CoreNode, CGCore);
}

CoreCoreInfoWin::~CoreCoreInfoWin(){
}

// EOF
