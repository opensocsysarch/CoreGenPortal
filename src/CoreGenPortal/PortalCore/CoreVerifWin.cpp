//
// _COREVERIFWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreVerifWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreVerifWin, wxDialog)
  EVT_BUTTON(wxID_OK,         CoreVerifWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreVerifWin::CoreVerifWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              std::ostringstream *VerifBuf )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(600,600), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  // connect the command handlers
  Bind(wxEVT_TEXT_COPY, &CoreVerifWin::OnCopyText, this);

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

  VerifWinSizer = new wxBoxSizer( wxHORIZONTAL );


  VerifResults = new wxRichTextCtrl(Wnd,
                           4,
                           wxEmptyString,
                           wxDefaultPosition,
                           wxSize(500,500),
                           wxTE_MULTILINE|wxTE_READONLY,
                           wxDefaultValidator,
                           wxT("Verification Results") );
  //VerifResults->AppendText( wxString((*VerifBuf).str())+wxT("\n") );
  std::stringstream ss((*VerifBuf).str());
  std::string out;
  unsigned FailedTests = 0;
  unsigned PassedTests = 0;
  while(std::getline(ss,out,'\n')){
    std::size_t foundp = out.find("PASSED");
    std::size_t foundf = out.find("FAILED");

    if( foundp != std::string::npos ){
      VerifResults->BeginBold();
      VerifResults->WriteText(wxString(out)+wxT("\n"));
      VerifResults->EndBold();
      PassedTests = PassedTests+1;
    }else if( foundf != std::string::npos ){
      VerifResults->BeginTextColour(wxColour("RED"));
      VerifResults->BeginBold();
      VerifResults->WriteText(wxString(out)+wxT("\n"));
      VerifResults->EndBold();
      VerifResults->EndTextColour();
      FailedTests = FailedTests+1;
    }else{
      VerifResults->AppendText(wxString(out)+wxT("\n"));
    }
  }

  // write the pass/fail status
  VerifResults->WriteText(wxT("\n\n\nSUMMARY OF VERIFICATION PASSES\n"));
  VerifResults->WriteText(wxT("==============================\n"));
  VerifResults->BeginBold();
  VerifResults->WriteText(wxT("Number of passing tests: ") +
                          wxString::Format(wxT("%i"),PassedTests)+
                          wxT("\n"));
  VerifResults->EndBold();
  VerifResults->BeginTextColour(wxColour("RED"));
  VerifResults->BeginBold();
  VerifResults->WriteText(wxT("Number of failed tests: ") +
                          wxString::Format(wxT("%i"),FailedTests)+
                          wxT("\n"));
  VerifResults->EndBold();
  VerifResults->EndTextColour();

  VerifWinSizer->Add( VerifResults, 0, wxALL, 0 );
  InnerSizer->Add( VerifWinSizer, 0, wxALIGN_CENTER|wxALL, 5);

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

void CoreVerifWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreVerifWin::OnCopyText( wxClipboardTextEvent& event ){
  if(wxTheClipboard->Open()){
    if (wxTheClipboard->IsSupported( wxDF_TEXT )){
      wxTheClipboard->AddData(
        new wxTextDataObject(VerifResults->GetStringSelection()));
      wxTheClipboard->Flush();
    }
    wxTheClipboard->Close();
  }
}

CoreVerifWin::~CoreVerifWin(){
}

// EOF
