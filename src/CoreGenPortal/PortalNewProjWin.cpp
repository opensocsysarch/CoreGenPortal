//
// _PORTALNEWPROJWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "PortalNewProjWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(PortalNewProjWin, wxDialog)
  EVT_BUTTON(wxID_OK, PortalNewProjWin::OnPressOk)
  EVT_BUTTON(wxID_CANCEL, PortalNewProjWin::OnPressCancel)
wxEND_EVENT_TABLE()

PortalNewProjWin::PortalNewProjWin( wxWindow *parent,
                                    wxWindowID id,
                                    const wxString& title,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style,
                                    wxGenericDirCtrl *P,
                                    wxTextCtrl *LP,
                                    wxStyledTextCtrl *IP,
                                    CoreUserConfig *U): wxDialog( parent,
                                                                     id,
                                                                     title,
                                                                     pos,
                                                                     size,
                                                                     style),
                                                        ProjDir(P),
                                                        LogPane(LP),
                                                        IRPane(IP),
                                                        User(U),
                                                        CG(nullptr){
  // init the internals
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  // create the box sizers
  wxBoxSizer *bSizer1 = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer *bSizer2 = new wxBoxSizer( wxVERTICAL );

  m_panel1 = new wxPanel( this,
                          wxID_ANY,
                          wxDefaultPosition,
                          wxDefaultSize,
                          wxTAB_TRAVERSAL );
  bSizer2->Add( m_panel1, 1, wxEXPAND | wxALL,5 );

  // init all the options
  //-- project name
  ProjNameText = new wxStaticText( this,
                                   wxID_ANY,
                                   wxT("Project Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  ProjNameText->Wrap(-1);
  bSizer2->Add( ProjNameText, 0, wxALIGN_CENTER|wxALL, 5 );

  // -- project name input box
  ProjNameCtrl = new wxTextCtrl( this,
                                 wxID_ANY,
                                 wxT("PROJECTNAME"),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("Projectname") );
  bSizer2->Add( ProjNameCtrl, 0, wxALIGN_CENTER|wxALL, 5 );

  // -- project type
  ProjTypeText = new wxStaticText( this,
                                   wxID_ANY,
                                   wxT("Project Type"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  ProjTypeText->Wrap(-1);
  bSizer2->Add( ProjTypeText, 0, wxALIGN_CENTER|wxALL, 5 );

  // -- project type choice box
  wxString ProjTypeChoices[] = { wxT("SoC"), wxT("Module"),
                                 wxT("Extension"), wxT("Unknown") };
  int ProjTypeNChoices = sizeof( ProjTypeChoices ) / sizeof( wxString );
  ProjType = new wxChoice( this,
                           wxID_ANY,
                           wxDefaultPosition,
                           wxDefaultSize,
                           ProjTypeNChoices,
                           ProjTypeChoices,
                           0,
                           wxDefaultValidator,
                           wxT("Project Type") );
  ProjType->SetSelection(0);
  bSizer2->Add( ProjType, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  bSizer2->Add( FinalStaticLine, 1, wxEXPAND|wxALL, 5 );
  bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

  // setup all the buttons
  wxBoxSizer *bSizer3 = new wxBoxSizer( wxVERTICAL );

  m_projbuttonsizer = new wxStdDialogButtonSizer();
  m_userOK = new wxButton( this, wxID_OK );
  m_projbuttonsizer->AddButton( m_userOK );
  m_userCancel = new wxButton( this, wxID_CANCEL );
  m_projbuttonsizer->AddButton( m_userCancel );
  m_projbuttonsizer->Realize();

  bSizer3->Add( m_projbuttonsizer, 1, wxEXPAND, 5 );
  bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );

  // draw the dialog box until we get more info
  this->SetSizer( bSizer1 );
  this->Layout();
  bSizer1->Fit(this);
  this->Centre(wxBOTH);
}

void PortalNewProjWin::OnPressOk( wxCommandEvent& ok ){

  // read all the values
  /// project name
  std::string PName = std::string(ProjNameCtrl->GetValue().mb_str());

  if(!CoreGenBackend::IsValidName(PName)){
    LogPane->AppendText(PName + " is not a valid project name.\n");
    ProjNameText->SetForegroundColour(wxColour(255, 0, 0));
    return;
  }

  /// project type
  CGProjType PType = CGNULLProj;
  int TYPE = ProjType->GetSelection();
  if( TYPE != wxNOT_FOUND ){
    switch( TYPE ){
    case 0:
      PType = CGSocProj;
      break;
    case 1:
      PType = CGModProj;
      break;
    case 2:
      PType = CGExtProj;
      break;
    default:
      PType = CGNULLProj;
      break;
    }
  }

  // attempt to create a new project
  /// build the full project directory path and create it
  wxString FullPath = User->wxGetProjectDir() + wxT("/") + ProjNameCtrl->GetValue();
  if( !wxFileName::Mkdir( FullPath, wxS_DIR_DEFAULT,0) ){
    // throw an error
    LogPane->AppendText("Failed to create project directory at: " + FullPath + wxT("\n") );
    this->EndModal( wxID_CANCEL );
  }

  LogPane->AppendText("Created project directory: " + FullPath + wxT("\n") );

  /// directory created, now construct the coregen object
  CG = new CoreGenBackend( PName,
                           User->GetProjectDir() + "/" + PName,
                           User->GetArchiveDir(),
                           PType );
  if( CG == nullptr ){
    // throw an error
    LogPane->AppendText("Error creating new CoreGen project\n");
    this->EndModal( wxID_CANCEL );
  }

  // write out an IR template
  wxString IRFile = FullPath + wxT("/") + ProjNameCtrl->GetValue() + wxT(".yaml");
  if( !CG->WriteIR( std::string(FullPath.mb_str()) + "/" + PName + ".yaml" ) ){
    // throw an error
    LogPane->AppendText("Failed to write new IR to : " + IRFile + wxT("\n"));
    this->EndModal( wxID_CANCEL );
  }

  LogPane->AppendText("Wrote new IR file to : " + IRFile + wxT("\n") );

  // Everything was successful, update the main window parameters
  LogPane->AppendText("Project creation was successful\n" );
  IRPane->LoadFile(IRFile);
  IRFileName = IRFile;
  ProjDir->SetPath(FullPath);
  this->EndModal( wxID_OK );
}

void PortalNewProjWin::OnPressCancel( wxCommandEvent& ok ){
  // cancel everyting and close the window
  this->EndModal(wxID_CANCEL);
}

PortalNewProjWin::~PortalNewProjWin(){
}

// EOF
