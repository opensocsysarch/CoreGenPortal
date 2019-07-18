//
// _COREPLUGINBROWSER_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CorePluginBrowser.h"
#include <iostream>

// Event Table
wxBEGIN_EVENT_TABLE(CorePluginBrowser, wxDialog)
  EVT_BUTTON(wxID_OK, CorePluginBrowser::OnPressOk)
wxEND_EVENT_TABLE()

CorePluginBrowser::CorePluginBrowser( wxWindow *parent,
                                      wxWindowID id,
                                      const wxString& title,
                                      wxString PluginName,
                                      wxString PluginPath,
                                      CoreGenPlugin *PluginPtr,
                                      CoreGenBackend *CGProject,
                                      wxTextCtrl *LogPane )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(600,600), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ),
    PluginName(PluginName), PluginPath(PluginPath), PLUGIN(PluginPtr),
    CGProject(CGProject), LogPane(LogPane) {

  // init the internals
  this->SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  unsigned Major = 0;
  unsigned Minor = 0;
  unsigned Patch = 0;
  PLUGIN->GetVersion(&Major,&Minor,&Patch);

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

  // create all the window elements
  //-- plugin name
  wxString NAME = PluginName + wxT(" Version " ) +
                  wxString::Format(wxT("%i"),Major) +
                  wxT(".") +
                  wxString::Format(wxT("%i"),Minor) +
                  wxT(".") +
                  wxString::Format(wxT("%i"),Patch);
  PluginNameSizer = new wxBoxSizer( wxHORIZONTAL );
  PluginNameText = new wxStaticText( Wnd,
                                 wxID_ANY,
                                 NAME,
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  PluginNameText->Wrap(-1);
  PluginNameSizer->Add( PluginNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
  InnerSizer->Add( PluginNameSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- hdl codegen
  HDLSizer = new wxBoxSizer( wxHORIZONTAL );
  if( PLUGIN->HasHDLCodegen() ){
    HDLCodegen = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("HDL Codegen: yes"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  }else{
    HDLCodegen = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("HDL Codegen: no"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  }
  HDLCodegen->Wrap(-1);
  HDLSizer->Add( HDLCodegen, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
  InnerSizer->Add( HDLSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- llvm codegen
  LLVMSizer = new wxBoxSizer( wxHORIZONTAL );
  if( PLUGIN->HasLLVMCodegen() ){
    LLVMCodegen = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("LLVM Codegen: yes"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  }else{
    LLVMCodegen = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("LLVM Codegen: yes"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  }
  LLVMCodegen->Wrap(-1);
  LLVMSizer->Add( LLVMCodegen, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
  InnerSizer->Add( LLVMSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- features
  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 0, wxEXPAND | wxALL, 5 );

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

void CorePluginBrowser::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CorePluginBrowser::~CorePluginBrowser(){
}

// EOF
