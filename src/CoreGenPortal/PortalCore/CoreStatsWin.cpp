//
// _CORESTATSWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreStatsWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreStatsWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreStatsWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreStatsWin::CoreStatsWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              std::ostringstream *StatsBuf )
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

  StatsWinSizer = new wxBoxSizer( wxHORIZONTAL );


  StatsResults = new wxRichTextCtrl(Wnd,
                           4,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(500,500),
                           wxTE_MULTILINE|wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("Project Summary") );
  std::stringstream ss((*StatsBuf).str());
  std::string out;
  while(std::getline(ss,out,'\n')){
    std::size_t foundarrow = out.find("==>");
    std::size_t foundtotal = out.find("TOTAL NODES:");

    if( (foundarrow != std::string::npos) &&
        (foundtotal != std::string::npos ) ){
      StatsResults->BeginTextColour(wxColour("RED"));
      StatsResults->BeginBold();
      StatsResults->WriteText(wxString(out)+wxT("\n"));
      StatsResults->EndBold();
      StatsResults->EndTextColour();
    }else if( foundarrow != std::string::npos ){
      StatsResults->BeginBold();
      StatsResults->WriteText(wxString(out)+wxT("\n"));
      StatsResults->EndBold();
    }else{
      StatsResults->AppendText(wxString(out)+wxT("\n"));
    }
  }

  // write the pass/fail status
  StatsWinSizer->Add( StatsResults, 0, wxALL, 0 );
  InnerSizer->Add( StatsWinSizer, 0, wxALIGN_CENTER|wxALL, 5);

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

void CoreStatsWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreStatsWin::~CoreStatsWin(){
}

// EOF
