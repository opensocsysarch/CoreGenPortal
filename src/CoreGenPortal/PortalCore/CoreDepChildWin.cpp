//
// _COREDEPCHILDWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreDepChildWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreDepChildWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreDepChildWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreDepChildWin::CoreDepChildWin( wxWindow *parent,
                                  wxWindowID id,
                                  CoreGenNode *Node )
  : wxDialog( parent, id, wxT("Dependent Children"), wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  // Record the node
  DepNode = Node;

  // init the internals
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the box sizers
  wxBoxSizer *bSizer2 = new wxBoxSizer( wxVERTICAL );

  m_panel1 = new wxPanel( this,
                          wxID_ANY,
                          wxDefaultPosition,
                          wxDefaultSize,
                          wxTAB_TRAVERSAL );
  bSizer2->Add( m_panel1, 1, wxEXPAND|wxALL, 5);

  // init all the options
  ChildNameSizer = new wxBoxSizer( wxHORIZONTAL );
  ChildNameText = new wxStaticText(this,
                                 3,
                                 wxT("Child Nodes"),
                                 wxDefaultPosition,
                                 wxSize(160,-1),
                                 0 );
  ChildNameText->Wrap(-1);
  ChildNameSizer->Add( ChildNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
  ChildNameCtrl = new wxTextCtrl(this,
                               1,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxSize(320,100),
                               wxTE_MULTILINE|wxHSCROLL,
                               wxDefaultValidator,
                               wxT("Child Nodes") );
  if(DepNode){
    // add all the child names
    for( unsigned i=0; i<DepNode->GetNumChild(); i++ ){
      ChildNameCtrl->AppendText( wxString(DepNode->GetChild(i)->GetName()) + wxT("\n") );
    }
  }
  ChildNameSizer->Add( ChildNameCtrl, 0, wxALL, 0 );
  bSizer2->Add( ChildNameSizer, 0, wxALIGN_CENTER|wxALL, 0 );

  // add the static line
  FinalStaticLine = new wxStaticLine( this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  bSizer2->Add( FinalStaticLine, 1, wxEXPAND|wxALL, 5 );

  // setup all the buttons
  wxBoxSizer *bSizer3 = new wxBoxSizer( wxVERTICAL );

  m_depbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( this, wxID_CANCEL );
  m_depbuttonsizer->SetAffirmativeButton( m_userOK );
  m_depbuttonsizer->Realize();

  bSizer2->Add( m_depbuttonsizer, 1, wxEXPAND, 5 );
  bSizer3->Add( bSizer2, 1, wxEXPAND|wxALL, 5 );

  // draw the dialog box until we get more info
  this->SetSizer( bSizer3 );
  this->Layout();
  bSizer3->Fit(this);
}

void CoreDepChildWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreDepChildWin::~CoreDepChildWin(){
}

// EOF
