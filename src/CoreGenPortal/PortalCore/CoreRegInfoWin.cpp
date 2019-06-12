//
// _COREREGINFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreRegInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreRegInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreRegInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CoreRegInfoWin::CoreRegInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenReg *Reg )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Reg == nullptr ){
    this->EndModal(wxID_OK);
  }

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
  // -- reg name
  RegNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Register Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  RegNameText->Wrap(-1);
  InnerSizer->Add( RegNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  RegNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Reg->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("Reg Name") );
  InnerSizer->Add( RegNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- reg idx
  RegIdxText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Register Index"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  RegIdxText->Wrap(-1);
  InnerSizer->Add( RegIdxText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  RegIdxCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxString::Format(wxT("%i"),Reg->GetIndex()),
                            wxDefaultPosition,
                            wxSize(400,25),
                            wxTE_READONLY,
                            wxDefaultValidator,
                            wxT("Register Index") );
  InnerSizer->Add( RegIdxCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- width
  WidthText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Register Width (in bits)"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  WidthText->Wrap(-1);
  InnerSizer->Add( WidthText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  WidthCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxString::Format(wxT("%i"),Reg->GetWidth()),
                            wxDefaultPosition,
                            wxSize(400,25),
                            wxTE_READONLY,
                            wxDefaultValidator,
                            wxT("Register Width") );
  InnerSizer->Add( WidthCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- subregs
  SubRegText = new wxStaticText( Wnd,
                              wxID_ANY,
                              wxT("Subregister Fields"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              0 );
  WidthText->Wrap(-1);
  InnerSizer->Add( SubRegText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  SubRegCtrl = new wxTextCtrl( Wnd,
                            wxID_ANY,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(400,100),
                            wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("Subregisters ") );
  SubRegCtrl->AppendText(wxT("NAME:START_BIT:END_BIT\n"));
  std::string SRName;
  unsigned SRStart;
  unsigned SREnd;
  for( unsigned i=0; i<Reg->GetNumSubRegs(); i++ ){
    Reg->GetSubReg(i,SRName,SRStart,SREnd);
    SubRegCtrl->AppendText(wxString(SRName) + wxT(":") +
                           wxString::Format(wxT("%i"),SRStart) + wxT(":") +
                           wxString::Format(wxT("%i"),SREnd) + wxT("\n") );
  }
  InnerSizer->Add( SubRegCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- simd check box
  SIMDCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("SIMD Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("SIMDREGISTER") );
  if( Reg->IsSIMD() )
    SIMDCheck->SetValue(true);
  else
    SIMDCheck->SetValue(false);

  InnerSizer->Add(SIMDCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- rw check box
  RWCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("Read/Write Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("RWREGISTER") );
  if( Reg->IsRWAttr() )
    RWCheck->SetValue(true);
  else
    RWCheck->SetValue(false);

  InnerSizer->Add(RWCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- ro check box
  ROCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("Read-Only Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("ROREGISTER") );
  if( Reg->IsROAttr() )
    ROCheck->SetValue(true);
  else
    ROCheck->SetValue(false);

  InnerSizer->Add(ROCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- csr check box
  CSRCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("Configuration Status Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("CSRREGISTER") );
  if( Reg->IsCSRAttr() )
    CSRCheck->SetValue(true);
  else
    CSRCheck->SetValue(false);

  InnerSizer->Add(CSRCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- ams check box
  AMSCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("Arithmetic Machine State Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("AMSREGISTER") );
  if( Reg->IsAMSAttr() )
    AMSCheck->SetValue(true);
  else
    AMSCheck->SetValue(false);

  InnerSizer->Add(AMSCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- tus check box
  TUSCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("Thread Unit Shared Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("TUSREGISTER") );
  if( Reg->IsTUSAttr() )
    TUSCheck->SetValue(true);
  else
    TUSCheck->SetValue(false);

  InnerSizer->Add(TUSCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- pc check box
  PCCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("PC Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("PCREGISTER") );
  if( Reg->IsPCAttr() )
    PCCheck->SetValue(true);
  else
    PCCheck->SetValue(false);

  InnerSizer->Add(PCCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);

  //-- shared check box
  SharedCheck = new wxCheckBox( Wnd,
                              wxID_ANY,
                              wxT("Shared Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("SHAREDREGISTER") );
  if( Reg->IsShared() )
    SharedCheck->SetValue(true);
  else
    SharedCheck->SetValue(false);

  InnerSizer->Add(SharedCheck, 1, wxEXPAND|wxALIGN_LEFT|wxALL, 5);


  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

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

void CoreRegInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

CoreRegInfoWin::~CoreRegInfoWin(){
}

// EOF
