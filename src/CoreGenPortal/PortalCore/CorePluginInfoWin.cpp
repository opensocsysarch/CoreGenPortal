//
// _COREPLUGININFOWIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CorePluginInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CorePluginInfoWin, wxDialog)
  EVT_BUTTON(wxID_OK, CorePluginInfoWin::OnPressOk)
wxEND_EVENT_TABLE()

CorePluginInfoWin::CorePluginInfoWin( wxWindow* parent,
                              wxWindowID id,
                              const wxString& title,
                              CoreGenPlugin *Plugin)
  : wxDialog( parent, id, title, wxDefaultPosition,
              wxSize(500,500), wxDEFAULT_DIALOG_STYLE|wxVSCROLL ){
  if( Plugin == nullptr ){
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
  //-- plugin name
  PluginNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Plugin Node Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  PluginNameText->Wrap(-1);
  InnerSizer->Add( PluginNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  PluginNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Plugin->GetName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("PluginName") );
  InnerSizer->Add( PluginNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- library name
  LibNameText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Library Name"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  LibNameText->Wrap(-1);
  InnerSizer->Add( LibNameText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  LibNameCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxString(Plugin->GetPluginName()),
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("LibName") );
  InnerSizer->Add( LibNameCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- version info
  VersionText = new wxStaticText( Wnd,
                                  wxID_ANY,
                                  wxT("Version"),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0 );
  VersionText->Wrap(-1);
  InnerSizer->Add( VersionText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  unsigned Major = 0;
  unsigned Minor = 0;
  unsigned Patch = 0;
  Plugin->GetVersion(&Major,&Minor,&Patch);
  wxString VersionInfo = wxString::Format(wxT("%i"),Major) + wxT(".") +
                         wxString::Format(wxT("%i"),Minor) + wxT(".") +
                         wxString::Format(wxT("%i"),Patch);
  VersionCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 VersionInfo,
                                 wxDefaultPosition,
                                 wxSize(400,25),
                                 wxTE_READONLY,
                                 wxDefaultValidator,
                                 wxT("LibName") );
  InnerSizer->Add( VersionCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- feature box
  FeatureText = new wxStaticText( Wnd,
                                   wxID_ANY,
                                   wxT("Plugin Features"),
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   0 );
  FeatureText->Wrap(-1);
  InnerSizer->Add( FeatureText, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  FeatureCtrl = new wxTextCtrl( Wnd,
                                 wxID_ANY,
                                 wxEmptyString,
                                 wxDefaultPosition,
                                 wxSize(400,100),
                                 wxTE_READONLY|wxTE_MULTILINE|wxHSCROLL,
                                 wxDefaultValidator,
                                 wxT("Features") );
  FeatureCtrl->AppendText(wxT("FEATURE_NAME:FEATURE_TYPE:FEATURE_VALUE\n"));
  for( unsigned i=0; i<Plugin->GetNumFeatures(); i++ ){
    FeatureCtrl->AppendText(GetFeatureStr(Plugin,i));
  }

  InnerSizer->Add( FeatureCtrl, 1, wxEXPAND|wxALIGN_CENTER|wxALL, 5 );

  //-- HDL codegen
  HDLCodegen = new wxCheckBox( Wnd,
                               wxID_ANY,
                               wxT("HDL Code Generation"),
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxALIGN_RIGHT,
                               wxDefaultValidator,
                               wxT("HDLCodegen") );
  if( Plugin->HasHDLCodegen() )
    HDLCodegen->SetValue(true);
  else
    HDLCodegen->SetValue(false);

  //-- LLVM codegen
  LLVMCodegen = new wxCheckBox( Wnd,
                               wxID_ANY,
                               wxT("LLVM Code Generation"),
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxALIGN_RIGHT,
                               wxDefaultValidator,
                               wxT("LLVMCodegen") );
  if( Plugin->HasLLVMCodegen() )
    LLVMCodegen->SetValue(true);
  else
    LLVMCodegen->SetValue(false);

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

void CorePluginInfoWin::OnPressOk(wxCommandEvent& ok){
  this->EndModal(wxID_OK);
}

wxString CorePluginInfoWin::GetFeatureStr( CoreGenPlugin *Plugin,
                                           unsigned Idx ){
  // Name:Type:Value
  // name
  wxString FeatureName(Plugin->GetFeatureName(Idx));

  // type & value
  wxString FeatureType;
  wxString FeatureValue;
  CGFeatureVal Value;

  switch( Plugin->GetFeatureType(Idx) ){
  case CGFUnsigned:
    FeatureType = wxT("unsigned");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format(wxT("%i"),Value.UnsignedData);
    break;
  case CGFUin32t:
    FeatureType = wxT("uint32_t");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format(wxT("%i"),Value.Uint32tData);
    break;
  case CGFint32t:
    FeatureType = wxT("int32_t");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format(wxT("%i"),Value.Int32tData);
    break;
  case CGFUint64t:
    FeatureType = wxT("uint64_t");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format("%" wxLongLongFmtSpec "u", Value.Uint64tData);
    break;
  case CGFInt64t:
    FeatureType = wxT("int64_t");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format("%" wxLongLongFmtSpec "u", Value.Int64tData);
    break;
  case CGFFloat:
    FeatureType = wxT("float");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format(wxT("%f"),Value.FloatData);
    break;
  case CGFDouble:
    FeatureType = wxT("double");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString::Format(wxT("%f"),Value.DoubleData);
    break;
  case CGFString:
    FeatureType = wxT("string");
    Value = Plugin->GetFeatureValue(Idx);
    FeatureValue = wxString(Value.StringData);
    break;
  case CGFBool:
    FeatureType = wxT("bool");
    Value = Plugin->GetFeatureValue(Idx);
    if( Value.BoolData )
      FeatureValue = wxT("true");
    else
      FeatureValue = wxT("false");
    break;
  case CGFUnknown:
  default:
    FeatureType = wxT("unknown");
    FeatureValue = wxT("unknown");
    break;
  }

  wxString Final = FeatureName + wxT(":") +
                   FeatureType + wxT(":") +
                   FeatureValue + wxT("\n");
  return Final;
}

CorePluginInfoWin::~CorePluginInfoWin(){
}

// EOF
