//
// _COREREGINFOWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreRegInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreRegInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CoreRegInfoWin::OnPressOk)
  EVT_BUTTON(wxID_SAVE, CoreRegInfoWin::OnSave)
wxEND_EVENT_TABLE()

CoreRegInfoWin::CoreRegInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenReg *Reg )
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){

  RegNode = (CoreGenReg*)Reg;

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
  RegNameSizer = new wxBoxSizer( wxHORIZONTAL );
  RegNameText = new wxStaticText( Wnd,
                                   4,
                                   wxT("Register Name"),
                                   wxDefaultPosition,
                                   wxSize(160,-1),
                                   0 );
  RegNameText->Wrap(-1);
  RegNameSizer->Add( RegNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  RegNameCtrl = new wxTextCtrl( Wnd,
                                 0,
                                 Reg ? wxString(Reg->GetName()) : "",
                                 wxDefaultPosition,
                                 wxSize(320,25),
                                 0,
                                 wxDefaultValidator,
                                 wxT("Reg Name") );
  RegNameSizer->Add( RegNameCtrl, 0, wxALL, 0 );
  InnerSizer->Add(RegNameSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- reg idx
  RegIdxSizer = new wxBoxSizer( wxHORIZONTAL );
  RegIdxText = new wxStaticText( Wnd,
                              5,
                              wxT("Register Index"),
                              wxDefaultPosition,
                              wxSize(160, -1),
                              0 );
  RegIdxText->Wrap(-1);
  RegIdxSizer->Add( RegIdxText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  RegIdxCtrl = new wxTextCtrl( Wnd,
                            1,
                            Reg ? wxString::Format(wxT("%i"),Reg->GetIndex()) : "",
                            wxDefaultPosition,
                            wxSize(320,25),
                            0,
                            wxDefaultValidator,
                            wxT("Register Index") );
  RegIdxSizer->Add( RegIdxCtrl, 0, wxALL, 0 );
  InnerSizer->Add(RegIdxSizer, 0, wxALIGN_CENTER|wxALL, 5);

  //-- width
  WidthSizer = new wxBoxSizer( wxHORIZONTAL );
  WidthText = new wxStaticText( Wnd,
                              6,
                              wxT("Register Width (in bits)"),
                              wxDefaultPosition,
                              wxSize(160, -1),
                              0 );
  WidthText->Wrap(-1);
  WidthSizer->Add( WidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  WidthCtrl = new wxTextCtrl( Wnd,
                            2,
                            Reg ? wxString::Format(wxT("%i"),Reg->GetWidth()): "",
                            wxDefaultPosition,
                            wxSize(320,25),
                            0,
                            wxDefaultValidator,
                            wxT("Register Width") );
  WidthSizer->Add( WidthCtrl, 0, wxALL, 0 );
  InnerSizer->Add( WidthSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- simd 
  SIMDSizer = new wxBoxSizer( wxHORIZONTAL );
  SIMDText = new wxStaticText( Wnd,
                              8,
                              wxT("SIMD Width (in bits)"),
                              wxDefaultPosition,
                              wxSize(160, -1),
                              0 );
  SIMDText->Wrap(-1);
  SIMDSizer->Add( SIMDText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  SIMDCtrl = new wxTextCtrl( Wnd,
                            16,
                            Reg ? std::to_string(Reg->GetSIMDWidth()) : "",
                            wxDefaultPosition,
                            wxSize(320,25),
                            0,
                            wxDefaultValidator,
                            wxT("Register Width") );
  SIMDSizer->Add( SIMDCtrl, 0, wxALL, 0 );
  InnerSizer->Add( SIMDSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  //-- subregs
  SubRegSizer = new wxBoxSizer( wxHORIZONTAL );
  SubRegText = new wxStaticText( Wnd,
                              7,
                              wxT("Subregister Fields"),
                              wxDefaultPosition,
                              wxSize(160, -1),
                              0 );
  WidthText->Wrap(-1);
  SubRegSizer->Add( SubRegText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

  SubRegCtrl = new wxTextCtrl( Wnd,
                            3,
                            wxEmptyString,
                            wxDefaultPosition,
                            wxSize(320,100),
                            wxTE_MULTILINE|wxHSCROLL,
                            wxDefaultValidator,
                            wxT("Subregisters ") );
  SubRegCtrl->AppendText(wxT("NAME:START_BIT:END_BIT\n"));
  std::string SRName;
  unsigned SRStart;
  unsigned SREnd;
  if(Reg){
    for( unsigned i=0; i<Reg->GetNumSubRegs(); i++ ){
      Reg->GetSubReg(i,SRName,SRStart,SREnd);
      SubRegCtrl->AppendText(wxString(SRName) + wxT(":") +
                            wxString::Format(wxT("%i"),SRStart) + wxT(":") +
                            wxString::Format(wxT("%i"),SREnd) + wxT("\n") );
    }
  }
  SubRegSizer->Add( SubRegCtrl, 0, wxALL, 0 );
  InnerSizer->Add( SubRegSizer, 0, wxALIGN_CENTER|wxALL, 5 );

  HCheckSizer1 = new wxBoxSizer( wxHORIZONTAL );
  
  /*
  if( Reg && Reg->IsSIMD() )
    SIMDCheck->SetValue(true);
  else
    SIMDCheck->SetValue(false);

  HCheckSizer1->Add(SIMDCheck, 0, wxALL, 0);
  */

  //-- rw check box
  RWCheck = new wxCheckBox( Wnd,
                              9,
                              wxT("Read/Write Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("RWREGISTER") );
  if( Reg  && Reg->IsRWAttr() )
    RWCheck->SetValue(true);
  else
    RWCheck->SetValue(false);

  HCheckSizer1->Add(RWCheck, 0, wxALL, 0);

  //-- ro check box
  ROCheck = new wxCheckBox( Wnd,
                              10,
                              wxT("Read-Only Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("ROREGISTER") );
  if( Reg && Reg->IsROAttr() )
    ROCheck->SetValue(true);
  else
    ROCheck->SetValue(false);

  HCheckSizer1->Add(ROCheck, 0, wxALL, 0 );
  InnerSizer->Add(HCheckSizer1, 0, wxALIGN_CENTER|wxALL, 5 );
  
  HCheckSizer2 = new wxBoxSizer( wxHORIZONTAL );
  //-- csr check box
  CSRCheck = new wxCheckBox( Wnd,
                              11,
                              wxT("Config Status Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("CSRREGISTER") );
  if( Reg && Reg->IsCSRAttr() )
    CSRCheck->SetValue(true);
  else
    CSRCheck->SetValue(false);

  HCheckSizer2->Add(CSRCheck, 0, wxALL, 0);

  //-- ams check box
  AMSCheck = new wxCheckBox( Wnd,
                              12,
                              wxT("Arithmetic Machine State Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("AMSREGISTER") );
  if( Reg && Reg->IsAMSAttr() )
    AMSCheck->SetValue(true);
  else
    AMSCheck->SetValue(false);

  HCheckSizer2->Add(AMSCheck, 0, wxALL, 0);
  InnerSizer->Add( HCheckSizer2, 0, wxALIGN_CENTER|wxALL, 5 );
 
  HCheckSizer3 = new wxBoxSizer( wxHORIZONTAL );
  //-- tus check box
  TUSCheck = new wxCheckBox( Wnd,
                              13,
                              wxT("Thread Unit Shared Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("TUSREGISTER") );
  if( Reg && Reg->IsTUSAttr() )
    TUSCheck->SetValue(true);
  else
    TUSCheck->SetValue(false);

  HCheckSizer3->Add(TUSCheck, 0, wxALL, 0);
  

  //-- pc check box
  PCCheck = new wxCheckBox( Wnd,
                              14,
                              wxT("PC Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("PCREGISTER") );
  if( Reg && Reg->IsPCAttr() )
    PCCheck->SetValue(true);
  else
    PCCheck->SetValue(false);

  HCheckSizer3->Add(PCCheck, 0, wxALL, 0);

  //-- shared check box
  SharedCheck = new wxCheckBox( Wnd,
                              15,
                              wxT("Shared Register"),
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxALIGN_RIGHT,
                              wxDefaultValidator,
                              wxT("SHAREDREGISTER") );
  if( Reg && Reg->IsShared() )
    SharedCheck->SetValue(true);
  else
    SharedCheck->SetValue(false);

  HCheckSizer3->Add(SharedCheck, 0, wxALL, 0);
  InnerSizer->Add( HCheckSizer3, 0, wxALIGN_CENTER|wxALL, 5 );

  // add the static line
  FinalStaticLine = new wxStaticLine( Wnd,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxLI_HORIZONTAL );
  InnerSizer->Add( FinalStaticLine, 1, wxEXPAND | wxALL, 5 );

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if(Reg) m_userOK = new wxButton( Wnd, wxID_OK );
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

void CoreRegInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

void CoreRegInfoWin::OnSave(wxCommandEvent& save){
  PortalMainFrame *PMF = (PortalMainFrame*)this->GetParent();
  if(PMF->OnSave(this, this->RegNode, CGReg))
    this->EndModal(wxID_SAVE);
}

CoreRegInfoWin::~CoreRegInfoWin(){
}

// EOF
