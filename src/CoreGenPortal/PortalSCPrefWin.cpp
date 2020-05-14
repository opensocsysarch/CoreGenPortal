//
// _PORTALSCPREFWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "PortalSCPrefWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(PortalSCPrefWin, wxDialog)
  EVT_BUTTON(wxID_OK, PortalSCPrefWin::OnPressOk)
  EVT_BUTTON(wxID_CANCEL, PortalSCPrefWin::OnPressCancel)
wxEND_EVENT_TABLE()

PortalSCPrefWin::PortalSCPrefWin( wxWindow *parent,
                                  wxWindowID id,
                                  const wxString& title,
                                  const wxPoint& pos,
                                  const wxSize& size,
                                  long style,
                                  SCCompConfig *C ): wxDialog( parent,
                                                               id,
                                                               title,
                                                               pos,
                                                               size,
                                                               style ),
                                                      Config(C){
  // init the internals
  this->SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the outer sizer
  OuterSizer = new wxBoxSizer( wxVERTICAL );

  // create the scrolled window
  Wnd = new wxScrolledWindow( this,
                              wxID_ANY,
                              wxDefaultPosition,
                              wxDefaultSize,
                              0,
                              wxT("scroll"));

  // create the inner sizer
  InnerSizer = new wxBoxSizer( wxVERTICAL );

  // standard options

  ParseBox = new wxCheckBox( Wnd,
                             wxID_ANY,
                             wxT("Enable Parsing/Disable Codegen"),
                             wxDefaultPosition,
                             wxDefaultSize,
                             0 );
  if( Config->IsParseEnabled() )
    ParseBox->SetValue(true);
  InnerSizer->Add( ParseBox, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  KeepBox = new wxCheckBox( Wnd,
                            wxID_ANY,
                            wxT("Keep LLVM Intermediate Files"),
                            wxDefaultPosition,
                            wxDefaultSize,
                            0 );
  if( Config->IsKeepEnabled() )
    KeepBox->SetValue(true);
  InnerSizer->Add( KeepBox, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  PipelineBox = new wxCheckBox( Wnd,
                                wxID_ANY,
                                wxT("Enable Pipeliner (default in -O3)"),
                                wxDefaultPosition,
                                wxDefaultSize,
                                0 );
  if( Config->IsPipelineEnabled() )
    PipelineBox->SetValue(true);
  InnerSizer->Add( PipelineBox, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  FirstStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FirstStaticLine, 1, wxEXPAND | wxALL, 5 );

  // optimization options
  // -- -O0
  wxCheckBox *CB = new wxCheckBox( Wnd,
                                   wxID_ANY,
                                   wxT("-O0"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  if( Config->GetOptLevel() == 0 )
    CB->SetValue(true);
  InnerSizer->Add( CB, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
  OptOptions.push_back(CB);

  // -- -O1
  CB = new wxCheckBox( Wnd,
                       wxID_ANY,
                       wxT("-O1"),
                       wxDefaultPosition,
                       wxDefaultSize,
                       0 );
  if( Config->GetOptLevel() == 1 )
    CB->SetValue(true);
  InnerSizer->Add( CB, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
  OptOptions.push_back(CB);

  // -- -O2
  CB = new wxCheckBox( Wnd,
                       wxID_ANY,
                       wxT("-O2"),
                       wxDefaultPosition,
                       wxDefaultSize,
                       0 );
  if( Config->GetOptLevel() == 2 )
    CB->SetValue(true);
  InnerSizer->Add( CB, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
  OptOptions.push_back(CB);

  // -- -O3
  CB = new wxCheckBox( Wnd,
                       wxID_ANY,
                       wxT("-O3"),
                       wxDefaultPosition,
                       wxDefaultSize,
                       0 );
  if( Config->GetOptLevel() == 3 ){
    CB->SetValue(true);
    PipelineBox->SetValue(true);
  }
  InnerSizer->Add( CB, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
  OptOptions.push_back(CB);

  SecondStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( SecondStaticLine, 1, wxEXPAND | wxALL, 5 );

  // llvm options
  for( unsigned i=0; i<Config->GetNumLLVMPasses(); i++ ){
    wxCheckBox *LP = new wxCheckBox( Wnd,
                                     wxID_ANY,
                                     Config->GetLLVMPassName(i),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     0 );
    if( Config->IsLLVMPassEnabled(i) )
      LP->SetValue(true);
    InnerSizer->Add( LP, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
    LLVMOptions.push_back(LP);
  }

  ThirdStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( ThirdStaticLine, 1, wxEXPAND | wxALL, 5 );

  // stonecutter options
  for( unsigned i=0; i<Config->GetNumSCPasses(); i++ ){
    wxCheckBox *LP = new wxCheckBox( Wnd,
                                     wxID_ANY,
                                     Config->GetSCPassName(i),
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     0 );
    if( Config->IsSCPassEnabled(i) )
      LP->SetValue(true);
    InnerSizer->Add( LP, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
    SCOptions.push_back(LP);
  }

  // add the final static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_scbuttonsizer = new wxStdDialogButtonSizer();
  m_scOK = new wxButton( Wnd, wxID_OK );
  m_scbuttonsizer->AddButton( m_scOK );
  m_scCancel = new wxButton( Wnd, wxID_CANCEL );
  m_scbuttonsizer->AddButton( m_scCancel );
  m_scbuttonsizer->Realize();

  InnerSizer->Add( m_scbuttonsizer, 1, wxEXPAND, 5 );

  // draw the diag box until we get more info
  Wnd->SetScrollbars(20,20,50,50);
  Wnd->SetSizer( InnerSizer );
  Wnd->SetAutoLayout(true);
  Wnd->Layout();

  OuterSizer->Add( Wnd, 1, wxEXPAND | wxALL, 5 );
  this->SetSizer( OuterSizer );
  this->SetAutoLayout(true);
  this->Layout();
}

void PortalSCPrefWin::OnPressOk( wxCommandEvent& ok ){
  // user pressed 'ok', walk through all the options and update
  // the configuration

  // parser
  if( ParseBox->GetValue() )
    Config->EnableParse();
  else
    Config->DisableParse();

  // keep files
  if( KeepBox->GetValue() )
    Config->EnableKeep();
  else
    Config->DisableKeep();

  // pipeline
  if( PipelineBox->GetValue() )
    Config->EnablePipeliner();
  else
    Config->DisablePipeliner();

  // optimization options
  if( OptOptions[0]->GetValue() ){
    Config->SetO0();
    for( unsigned i=0; i<LLVMOptions.size(); i++ ){
      Config->DisableLLVMPass(i);
      LLVMOptions[i]->SetValue(false);
    }
    for( unsigned i=0; i<SCOptions.size(); i++ ){
      Config->DisableSCPass(i);
      SCOptions[i]->SetValue(false);
    }
  }
  if( OptOptions[1]->GetValue() ){
    Config->SetO1();
    for( unsigned i=0; i<LLVMOptions.size(); i++ ){
      Config->EnableLLVMPass(i);
      LLVMOptions[i]->SetValue(true);
    }
    for( unsigned i=0; i<SCOptions.size(); i++ ){
      Config->DisableSCPass(i);
      SCOptions[i]->SetValue(false);
    }
  }
  if( OptOptions[2]->GetValue() ){
    Config->SetO2();
    for( unsigned i=0; i<LLVMOptions.size(); i++ ){
      Config->EnableLLVMPass(i);
      LLVMOptions[i]->SetValue(true);
    }
    for( unsigned i=0; i<SCOptions.size(); i++ ){
      Config->EnableSCPass(i);
      SCOptions[i]->SetValue(true);
    }
  }
  if( OptOptions[3]->GetValue() ){
    Config->SetO3();
    Config->EnablePipeliner();
    for( unsigned i=0; i<LLVMOptions.size(); i++ ){
      Config->EnableLLVMPass(i);
      LLVMOptions[i]->SetValue(true);
    }
    for( unsigned i=0; i<SCOptions.size(); i++ ){
      Config->EnableSCPass(i);
      SCOptions[i]->SetValue(true);
    }
  }

  // LLVM options
  for( unsigned i=0; i<LLVMOptions.size(); i++ ){
    if( LLVMOptions[i]->GetValue() )
      Config->EnableLLVMPass(i);
    else
      Config->DisableLLVMPass(i);
  }

  // SC options
  for( unsigned i=0; i<SCOptions.size(); i++ ){
    if( SCOptions[i]->GetValue() )
      Config->EnableSCPass(i);
    else
      Config->DisableSCPass(i);
  }

  this->EndModal( wxID_OK );
}

void PortalSCPrefWin::OnPressCancel( wxCommandEvent& ok ){
  // cancel everything and close the window
  this->EndModal(wxID_CANCEL);
}

PortalSCPrefWin::~PortalSCPrefWin(){
}

// EOF
