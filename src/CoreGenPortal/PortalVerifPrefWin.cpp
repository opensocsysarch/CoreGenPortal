//
// _PORTALVERIFPREFWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "PortalVerifPrefWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(PortalVerifPrefWin, wxDialog)
  EVT_BUTTON(wxID_OK, PortalVerifPrefWin::OnPressOk)
  EVT_BUTTON(wxID_CANCEL, PortalVerifPrefWin::OnPressCancel)
wxEND_EVENT_TABLE()

PortalVerifPrefWin::PortalVerifPrefWin( wxWindow *parent,
                                        wxWindowID id,
                                        const wxString& title,
                                        const wxPoint& pos,
                                        const wxSize& size,
                                        long style,
                                        CoreVerifConfig *V): wxDialog( parent,
                                                                       id,
                                                                       title,
                                                                       pos,
                                                                       size,
                                                                       style ),
                                                              Verif(V) {
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

  // init all the options
  for( unsigned i=0; i<Verif->GetNumPasses(); i++ ){
    wxCheckBox *CB = new wxCheckBox(Wnd,
                                    wxID_ANY,
                                    Verif->GetPassName(i),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    0 );
    if( Verif->IsPassEnabled( i ) )
      CB->SetValue(true);

    InnerSizer->Add( CB, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );
    Options.push_back(CB);
  }

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_verifbuttonsizer = new wxStdDialogButtonSizer();
  m_verifOK = new wxButton( Wnd, wxID_OK );
  m_verifbuttonsizer->AddButton( m_verifOK );
  m_verifCancel = new wxButton( Wnd, wxID_CANCEL );
  m_verifbuttonsizer->AddButton( m_verifCancel );
  m_verifbuttonsizer->Realize();

  InnerSizer->Add( m_verifbuttonsizer, 1, wxEXPAND, 5 );

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

void PortalVerifPrefWin::OnPressOk( wxCommandEvent& ok ){
  // user pressed 'ok', walk through all the options and update
  // the configuration
  for( unsigned i=0; i<Options.size(); i++ ){
    if( Options[i]->GetValue() )
      Verif->EnablePass(i);
    else
      Verif->DisablePass(i);
  }
  this->EndModal( wxID_OK );
}

void PortalVerifPrefWin::OnPressCancel( wxCommandEvent& ok ){
  // cancel everything and close the window
  this->EndModal(wxID_CANCEL);
}

PortalVerifPrefWin::~PortalVerifPrefWin(){
}

// EOF
