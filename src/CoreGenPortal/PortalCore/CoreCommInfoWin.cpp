//
// _CORECOMMINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreCommInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreCommInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreCommInfoWin::OnPressOk)
  EVT_BUTTON(wxID_SAVE, CoreCommInfoWin::OnSave)
  EVT_TEXT_ENTER(wxID_ANY, CoreCommInfoWin::OnPressEnter)
wxEND_EVENT_TABLE()

CoreCommInfoWin::CoreCommInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenComm *Comm )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,350), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  this->CommNode = Comm;

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
  //-- comm
  CommNameSizer = new wxBoxSizer( wxHORIZONTAL );
  CommNameText = new wxStaticText( Wnd,
                                   4,
                                   wxT("Comm Node Name"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  CommNameText->Wrap(-1);
  CommNameSizer->Add( CommNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  CommNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 Comm ? wxString(Comm->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("CommName") );
  CommNameSizer->Add( CommNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CommNameSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- comm type
  CommTypeSizer = new wxBoxSizer( wxHORIZONTAL );
  CommTypeText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Comm Node Type"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  CommTypeText->Wrap(-1);
  CommTypeSizer->Add( CommTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  CommTypeCtrl = new wxTextCtrl( Wnd,
                                 1,
                                 wxEmptyString,
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("CommType") );
  if(Comm){
    switch( Comm->GetCommType() ){
    case CGCommP2P:
      CommTypeCtrl->AppendText(wxT("Point-to-Point"));
      break;
    case CGCommBus:
      CommTypeCtrl->AppendText(wxT("Bus"));
      break;
    case CGCommNoc:
      CommTypeCtrl->AppendText(wxT("Network on Chip"));
      break;
    case CGCommUnk:
    default:
      CommTypeCtrl->AppendText(wxT("Unknown"));
    break;
    }
  }
  
  CommTypeSizer->Add( CommTypeCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CommTypeSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- width
  CommWidthSizer = new wxBoxSizer( wxHORIZONTAL );
  WidthText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Comm Channel Width"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  WidthText->Wrap(-1);
  CommWidthSizer->Add( WidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  WidthCtrl = new wxTextCtrl( Wnd,
                              2,
                              Comm ? wxString::Format(wxT("%i"),Comm->GetWidth()) : "",
                              wxDefaultPosition,
                              wxSize(320,25),
                              0,
                              wxDefaultValidator,
                              wxT("CommWidth") );
  CommWidthSizer->Add( WidthCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CommWidthSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- endpoints
  CommEndpointSizer = new wxBoxSizer( wxHORIZONTAL );
  EndpointText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Endpoints"),
                              wxDefaultPosition,
                              wxSize(160,-1),
                              0 );
  EndpointText->Wrap(-1);
  CommEndpointSizer->Add( EndpointText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  EndpointCtrl = new wxTextCtrl( Wnd,
                            3,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(320,100),
                            wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("endpoints") );
  if(Comm){
    for( unsigned i=0; i<Comm->GetNumEndpoints(); i++ ){
      EndpointCtrl->AppendText(wxString(Comm->GetEndpoint(i)->GetName())+wxT("\n") );
    }
  }
  CommEndpointSizer->Add( EndpointCtrl, 0, wxALL, 0 );
  InnerSizer->Add( CommEndpointSizer, 0, wxALIGN_CENTER|wxALL, 5 );

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
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(Comm) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreCommInfoWin::OnPressOk( wxCommandEvent& ok ){
  this->EndModal(wxID_OK);
}

void CoreCommInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->CommNode, CGComm))
    this->EndModal(wxID_SAVE);
}

void CoreCommInfoWin::OnPressEnter( wxCommandEvent& enter ){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  PMF->OnPressEnter(enter, this->CommNode, CGComm);
}

CoreCommInfoWin::~CoreCommInfoWin(){
}

// EOF
