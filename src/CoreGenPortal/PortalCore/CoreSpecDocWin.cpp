//
// _CORESPECDOCWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreSpecDocWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreSpecDocWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreSpecDocWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreSpecDocWin::CoreSpecDocWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              std::ostringstream *SpecDocBuf )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(600,600), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

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

  SpecDocWinSizer = new wxBoxSizer( wxHORIZONTAL );


  SpecDocResults = new wxRichTextCtrl(Wnd,
                           4,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(500,500),
                           wxTE_MULTILINE|wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("Specification Document Build") );
  std::stringstream ss((*SpecDocBuf).str());
  std::string out;
  bool failed = false;
  while(std::getline(ss,out,'\n')){
    std::size_t foundp = out.find("PASSED");
    std::size_t foundf = out.find("FAILED");

    if( foundf != std::string::npos ){
      SpecDocResults->BeginTextColour(wxColour("RED"));
      SpecDocResults->BeginBold();
      SpecDocResults->WriteText(wxString(out)+wxT("\n"));
      SpecDocResults->EndBold();
      SpecDocResults->EndTextColour();
      failed = true;
    }else if( foundp != std::string::npos ){
      SpecDocResults->BeginBold();
      SpecDocResults->WriteText(wxString(out)+wxT("\n"));
      SpecDocResults->EndBold();
    }else{
      SpecDocResults->AppendText(wxString(out)+wxT("\n"));
    }
  }

  // write the pass/fail status
  if( failed ){
      SpecDocResults->BeginTextColour(wxColour("RED"));
      SpecDocResults->BeginBold();
      SpecDocResults->AppendText(wxT("Failed to create architecture specification document\n"));
      SpecDocResults->EndBold();
      SpecDocResults->EndTextColour();
  }else{
      SpecDocResults->BeginBold();
      SpecDocResults->AppendText(wxT("Successfully created architecture specification document\n"));
      SpecDocResults->EndBold();
  }
  SpecDocWinSizer->Add( SpecDocResults, 0, wxALL, 0 );
  InnerSizer->Add( SpecDocWinSizer, 0, wxALIGN_CENTER|wxALL, 5);

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

void CoreSpecDocWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreSpecDocWin::~CoreSpecDocWin(){
}

// EOF
