//
// _COREINSTFORMATINFOWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreInstFormatInfoWin.h"

// Event Table
wxBEGIN_EVENT_TABLE(CoreInstFormatInfoWin, wxDialog)
  EVT_BUTTON(wxID_SAVE, CoreInstFormatInfoWin::OnSave)
  EVT_BUTTON(wxID_CANCEL, CoreInstFormatInfoWin::OnPressCancel)
  EVT_BUTTON(ID_EXPANDFIELDS, CoreInstFormatInfoWin::OnPressExpandFields)
wxEND_EVENT_TABLE()

CoreInstFormatInfoWin::CoreInstFormatInfoWin(wxWindow *parent,
                                             wxWindowID id,
                                             const wxString &title,
                                             CoreGenInstFormat *InstF,
                                             CoreGenBackend *CG)
    : wxDialog(parent, id, title, wxDefaultPosition,
               wxSize(500, 600), wxDEFAULT_DIALOG_STYLE | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE),
      FieldsInformation(nullptr), InstFNode(InstF), CG(CG),
      isFieldsExpanded(false), isCreateNode(false),
      NumberOfFields(0) {

  // Create new node if adding node
  if ( !InstFNode ) {

    // first, determine if there are any ISA nodes.  This isn't the easiest thing
    // to do, but it can be done.  All we need to find is the first one
    CoreGenNode *TNode = CG->GetTop();
    CoreGenISA *ISANode = nullptr;
    bool found = false;
    unsigned i = 0;
    while( (i<TNode->GetNumChild()) && (!found) ){
      if( TNode->GetChild(i)->GetType() == CGISA ){
        ISANode = static_cast<CoreGenISA *>(TNode->GetChild(i));
        found = true;
      }
      i++;
    }

    if( !ISANode  ){
      // we didn't find any ISA nodes, flag an error and return
      PortalMainFrame *PMF = (PortalMainFrame *)this->GetParent();
      PMF->LogError("Could not create new instruction format.  No ISA's exist\n");
      isFieldsExpanded = false;
      this->EndModal(wxID_CANCEL);
    }

    // create a new inst format node using the first ISA as a placeholder
    InstFNode = CG->InsertInstFormat("NewInstFormat",
                                     ISANode);
                                     
    if( !InstFNode ){
      PortalMainFrame *PMF = (PortalMainFrame *)this->GetParent();
      PMF->LogError("Could not create new instruction format. " +
                    wxString(CG->GetErrStr()) + "\n" );
      isFieldsExpanded = false;
      this->EndModal(wxID_CANCEL);
    }
    isCreateNode = true;
  }
  // node exists already, gather existing fields
  else {
    for ( unsigned i = 0; i < InstFNode->GetNumFields(); i++ ) {
      ExistingFields.emplace_back(InstFNode->GetFieldName(i));
    }
  }
  // init the internals
  this->SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  // create the outer sizer
  OuterSizer = new wxBoxSizer(wxVERTICAL);

  // create scrollable window
  Wnd = new wxScrolledWindow(this,
                             wxID_ANY,
                             wxDefaultPosition,
                             wxDefaultSize,
                             0,
                             wxT("Scroll"));

  // create the inner sizer
  InnerSizer = new wxBoxSizer(wxVERTICAL);

  //-- instruction format name static text
  InstFormatNameSizer = new wxBoxSizer(wxHORIZONTAL);
  InstFormatNameText = new wxStaticText(Wnd,
                                        0,
                                        wxT("Instruction Format"),
                                        wxDefaultPosition,
                                        wxSize(160, -1),
                                        0);
  InstFormatNameText->Wrap(-1);
  InstFormatNameSizer->Add(InstFormatNameText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

  //-- instruction format name ctrl
  InstFormatNameCtrl = new wxTextCtrl(Wnd,
                                      1,
                                      InstFNode ? wxString(InstFNode->GetName()) : "",
                                      wxDefaultPosition,
                                      wxSize(320, 25),
                                      0,
                                      wxDefaultValidator,
                                      wxT("Instruction Format Name"));
  InstFormatNameSizer->Add(InstFormatNameCtrl, 0, wxALL, 0);
  InnerSizer->Add(InstFormatNameSizer, 0, wxALIGN_CENTER | wxALL, 5);

  //-- isa name
  ISANameSizer = new wxBoxSizer(wxHORIZONTAL);
  ISANameText = new wxStaticText(Wnd,
                                 2,
                                 wxT("ISA Name"),
                                 wxDefaultPosition,
                                 wxSize(160, -1),
                                 0);
  ISANameText->Wrap(-1);
  ISANameSizer->Add(ISANameText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

  //-- isa name ctrl
  ISACtrl = new wxTextCtrl(Wnd,
                           3,
                           InstFNode && InstFNode->GetISA() ? wxString(InstFNode->GetISA()->GetName()) : "",
                           wxDefaultPosition,
                           wxSize(320, 25),
                           0,
                           wxDefaultValidator,
                           wxT("ISA"));
  ISANameSizer->Add(ISACtrl, 0, wxALL, 0);
  InnerSizer->Add(ISANameSizer, 0, wxALIGN_CENTER | wxALL, 5);

  //-- format width static text
  FormatWidthSizer = new wxBoxSizer(wxHORIZONTAL);
  FormatWidthText = new wxStaticText(Wnd,
                                     4,
                                     wxT("Format Width"),
                                     wxDefaultPosition,
                                     wxSize(160, -1),
                                     0);
  FormatWidthText->Wrap(-1);
  FormatWidthSizer->Add(FormatWidthText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

  //-- format width ctrl
  FormatWidthCtrl = new wxTextCtrl(Wnd,
                                   5,
                                   InstFNode ? wxString::Format(wxT("%i"), InstFNode->GetFormatWidth()) : "0",
                                   wxDefaultPosition,
                                   wxSize(320, 25),
                                   0,
                                   wxDefaultValidator,
                                   wxT("Format Width"));
  FormatWidthSizer->Add(FormatWidthCtrl, 0, wxALL, 0);
  InnerSizer->Add(FormatWidthSizer, 0, wxALIGN_CENTER | wxALL, 5);

  //-- num fields static text
  NumFieldsSizer = new wxBoxSizer(wxHORIZONTAL);
  NumFieldsText = new wxStaticText(Wnd,
                                   6,
                                   wxT("Number of Fields"),
                                   wxDefaultPosition, wxSize(160, -1),
                                   0);
  NumFieldsSizer->Add(NumFieldsText, 0, wxALL, 0);
  InnerSizer->Add(NumFieldsSizer, 0, wxALIGN_CENTER | wxALL, 5);
  
  NumFieldsOptionsArray.Add(wxString::Format(wxT("%i"), 1));
  for (unsigned i = 2; i <= wxAtoi(FormatWidthCtrl->GetValue()); i++){
    NumFieldsOptionsArray.Add(wxString::Format(wxT("%i"), i));
  }
  //-- num fields combo box
  NumFields = new wxComboBox(Wnd,
                             7,
                             InstFNode ? wxString::Format(wxT("%i"),
                                                          InstFNode->GetNumFields())
                                       : wxString::Format(wxT("0")),
                             wxDefaultPosition,
                             wxSize(320, 25),
                             wxArrayString(NumFieldsOptionsArray),
                             0,
                             wxDefaultValidator,
                             wxT("Number of Fields"));
  NumFields->SetStringSelection(InstFNode->GetNumFields() != 0 ? wxString::Format(wxT("%i"), InstFNode->GetNumFields()) : wxString::Format(wxT("%i"), 0));
  NumFieldsText->Wrap(-1);
  NumFieldsSizer->Add(NumFields, 0, wxALL, 0);

  // expands fields button
  ExpandFieldsButton = new wxButton(Wnd,
                                  ID_EXPANDFIELDS,
                                  wxT("Expand Fields"),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0);
  wxStdDialogButtonSizer *ExpandFieldsSizer = new wxStdDialogButtonSizer();
  ExpandFieldsButton->SetBackgroundColour(wxColour(0, 0, 255));
  ExpandFieldsSizer->Add(ExpandFieldsButton);
  InnerSizer->Add(ExpandFieldsSizer, 1, wxALIGN_CENTER | wxALL, 5);

  // add the static line
  FinalStaticLine = new wxStaticLine(Wnd,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxLI_HORIZONTAL);
  InnerSizer->Add(FinalStaticLine, 1, wxEXPAND | wxALL, 5);

  Wnd->SetScrollbars(20, 20, 50, 50);
  Wnd->SetSizer(InnerSizer);
  Wnd->SetAutoLayout(true);
  Wnd->Layout();

  // setup all the buttons
  m_socbuttonsizer = new wxStdDialogButtonSizer();
  if ( !InstFNode )
    m_userCANCEL = new wxButton(Wnd, wxID_CANCEL);
  else {
    m_userCANCEL = new wxButton(Wnd, wxID_CANCEL);
    m_userCANCEL->SetBackgroundColour(wxColour(255, 0, 50));
    m_userSAVE = new wxButton(Wnd, wxID_SAVE);
    m_userSAVE->SetBackgroundColour(wxColour(0, 230, 130));

    m_socbuttonsizer->SetAffirmativeButton(m_userSAVE);
    m_socbuttonsizer->SetCancelButton(m_userCANCEL);
    m_socbuttonsizer->Realize();
    InnerSizer->Add(m_socbuttonsizer, 0, wxALL, 5);

    // draw the dialog box until we get more info
    OuterSizer->Add(Wnd, 1, wxEXPAND | wxALL, 5);
    this->SetSizer(OuterSizer);
    this->SetAutoLayout(true);
    this->Layout();
  }
}

void CoreInstFormatInfoWin::OnPressCancel(wxCommandEvent &cancel){
  // check to see if we created a new node in this window
  // if so, delete it because the user pressed the 'cancel' button
  if( isCreateNode )
    CG->DeleteNode(static_cast<CoreGenNode *>(InstFNode));
  isFieldsExpanded = false;
  this->EndModal(wxID_CANCEL);
}

void CoreInstFormatInfoWin::OnSave(wxCommandEvent &save){
  PortalMainFrame *PMF = (PortalMainFrame *)this->GetParent();
  // if( !isFieldsExpanded ){
  //   wxMessageBox("Please expand fields before saving\n", "Warning", wxOK | wxICON_INFORMATION);
  // }
  if ( PMF->OnSaveInstFormat(this, static_cast<CoreGenInstFormat *>(InstFNode),
                            CGInstF, FieldsInformation,
                            ExistingFields) ) {
    isFieldsExpanded = false;
    this->EndModal(wxID_SAVE);
  }
}

CoreInstFormatInfoWin::~CoreInstFormatInfoWin(){
  if( FieldsInformation )
    delete FieldsInformation;
}

void CoreInstFormatInfoWin::OnPressExpandFields(wxCommandEvent &ok){

  // Check entered value
  NumberOfFields = wxAtoi(NumFields->GetValue());
  if ( NumberOfFields <= 0 ){
    wxMessageBox("Number of fields invalid!", "Error",
                 wxOK | wxICON_INFORMATION);
    return;
  }
  
  FieldsInformation = new std::vector<std::vector<std::any>>;

  //--- Get Instruction Format Name
  FormatName = InstFormatNameCtrl->GetValue();

  //--- Field Type Options Array
  FieldTypeOptionsArray.Add(wxT("CGInstReg"));
  FieldTypeOptionsArray.Add(wxT("CGInstCode"));
  FieldTypeOptionsArray.Add(wxT("CGInstImm"));
  FieldTypeOptionsArray.Add(wxT("CGInstUnk"));
  
  //--- Start/End Bit Options Array
  int FormatWidth = wxAtoi(FormatWidthCtrl->GetValue());
  for ( int j = 0; j <= FormatWidth; j++ ){
    StartBitOptionsArray.Add(wxString::Format(wxT("%i"), j));    
    EndBitOptionsArray.Add(wxString::Format(wxT("%i"), j));
  }

  // Initialize field inputs
  for ( unsigned i = 0; i < NumberOfFields; i++ ) {
    // field vector
    std::vector<std::any> Field;

    // field sizer
    FieldNameSizer = new wxBoxSizer(wxHORIZONTAL);

    // static text for "Field N"
    wxStaticText *FieldN = new wxStaticText(Wnd,
                                            8,
                                            "Field " + std::to_string(i) + " Name",
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            0);
    FieldN->Wrap(-1);
    FieldNameSizer->Add(FieldN, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);

    //-- field name input box
    wxTextCtrl *FieldNameCtrl = new wxTextCtrl(Wnd,
                                               wxID_ANY,
                                               InstFNode->GetNumFields() != 0 ? InstFNode->GetFieldName(i) : "",
                                               wxDefaultPosition,
                                               wxSize(320, 25),
                                               0,
                                               wxDefaultValidator,
                                               wxT("Format Width"));
    FieldNameSizer->Add(FieldNameCtrl, 0, wxALIGN_LEFT | wxALL, 5);
    InnerSizer->Add(FieldNameSizer, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);
    
    //-- store field name
    std::any TmpFieldNameCtrl;
    try{
      TmpFieldNameCtrl = std::any_cast<wxTextCtrl *>(FieldNameCtrl);
    } catch (const std::bad_cast &e){
      std::cout << "FIRST CAST: " << e.what() << std::endl;
    }
    Field.push_back(TmpFieldNameCtrl);

    //-- field type text
    FieldTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    FieldTypeText = new wxStaticText(Wnd,
                                     10,
                                     wxT("Field Type"),
                                     wxDefaultPosition,
                                     wxSize(160, -1),
                                     0);
    FieldTypeText->Wrap(-1);
    FieldTypeSizer->Add(FieldTypeText, 0, wxALIGN_LEFT | wxALL, 5);

    //-- field type combobox
    FieldTypeName = new wxComboBox(Wnd,
                                   11,
                                   InstFNode->GetNumFields() != 0 ? InstFNode->CGInstFieldToStr(InstFNode->GetFieldType(InstFNode->GetFieldName(i))) : "CGInstUnk",
                                   wxDefaultPosition,
                                   wxSize(320, 25),
                                   wxArrayString(FieldTypeOptionsArray),
                                   0,
                                   wxDefaultValidator,
                                   wxT("Field Type"));

    // -- set the field type selection
    if( InstFNode ){
      CoreGenInstFormat::CGInstField LType = InstFNode->GetNumFields() != 0 ? InstFNode->GetFieldType(InstFNode->GetFieldName(i)) : CoreGenInstFormat::CGInstField::CGInstUnk;
      switch( LType ){
      case CoreGenInstFormat::CGInstField::CGInstReg:
        FieldTypeName->SetSelection(0);
        break;
      case CoreGenInstFormat::CGInstField::CGInstCode:
        FieldTypeName->SetSelection(1);
        break;
      case CoreGenInstFormat::CGInstField::CGInstImm:
        FieldTypeName->SetSelection(2);
        break;
      case CoreGenInstFormat::CGInstField::CGInstUnk:
      default:
        FieldTypeName->SetSelection(3);
        break;
      }
    }

    //-- store field type
    FieldTypeSizer->Add(FieldTypeName, 0, wxALIGN_LEFT | wxALL, 0);
    InnerSizer->Add(FieldTypeSizer, 0, wxALIGN_LEFT | wxALL, 5);
    std::any TmpFieldTypeName;
    try{
      TmpFieldTypeName = std::any_cast<wxComboBox *>(FieldTypeName);
    } catch (const std::bad_cast &e) {
      std::cout << "SECOND CAST: " << e.what() << std::endl;
    }
    Field.push_back(TmpFieldTypeName);

    //-- start bit text
    StartBitSizer = new wxBoxSizer(wxHORIZONTAL);
    StartBitText = new wxStaticText(Wnd,
                                    12,
                                    wxT("Start Bit"),
                                    wxDefaultPosition,
                                    wxSize(160, -1),
                                    0);
    StartBitText->Wrap(-1);
    StartBitSizer->Add(StartBitText, 0, wxALIGN_LEFT | wxALL, 5);

    //-- start bit combobox
    StartBit = new wxComboBox(Wnd,
                              13,
                              InstFNode->GetNumFields() != 0 ? wxString::Format(wxT("%i"), InstFNode->GetStartBit(InstFNode->GetFieldName(i))) : wxString::Format(wxT("%i"), 0),
                              wxDefaultPosition,
                              wxSize(320, 25),
                              wxArrayString(StartBitOptionsArray),
                              0,
                              wxDefaultValidator,
                              wxT("Field Type"));
    StartBitSizer->Add(StartBit, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);
    InnerSizer->Add(StartBitSizer, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);
    StartBit->SetStringSelection(InstFNode->GetNumFields() != 0 ? wxString::Format(wxT("%i"), InstFNode->GetStartBit(InstFNode->GetFieldName(i))): wxString::Format(wxT("%i"), 0));
    
    //-- store start bit
    std::any TmpStartBit;
    try {
      TmpStartBit = std::any_cast<wxComboBox *>(StartBit);
    } catch (const std::bad_cast &e) {
      std::cout << "THIRD CAST: " << e.what() << std::endl;
    }
    Field.push_back(TmpStartBit);

    //-- end bit text
    EndBitSizer = new wxBoxSizer(wxHORIZONTAL);
    EndBitText = new wxStaticText(Wnd,
                                  12,
                                  wxT("End Bit"),
                                  wxDefaultPosition,
                                  wxSize(160, -1),
                                  0);
    EndBitText->Wrap(-1);
    EndBitSizer->Add(EndBitText, 0, wxALIGN_LEFT | wxALL, 5);

    //-- end bit combobox
    EndBit = new wxComboBox(Wnd,
                            13,
                            InstFNode->GetNumFields() != 0 ? wxString::Format(wxT("%i"), InstFNode->GetEndBit(InstFNode->GetFieldName(i))) : wxString::Format(wxT("%i"), 1),
                            wxDefaultPosition,
                            wxSize(320, 25),
                            wxArrayString(EndBitOptionsArray),
                            0,
                            wxDefaultValidator,
                            wxT("Field Type"));
    EndBitSizer->Add(EndBit, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);
    InnerSizer->Add(EndBitSizer, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);
    EndBit->SetStringSelection(InstFNode->GetNumFields() != 0 ? wxString::Format(wxT("%i"), InstFNode->GetEndBit(InstFNode->GetFieldName(i))) : wxString::Format(wxT("%i"), 0));

    //-- store end bit
    std::any TmpEndBit;
    try {
      TmpEndBit = std::any_cast<wxComboBox *>(EndBit);
    } catch (const std::bad_cast &e) {
      std::cout << "FOURTH CAST: " << e.what() << std::endl;
    }
    Field.push_back(TmpEndBit);

    //-- mandatory field text
    MandatoryFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    MandatoryFieldText = new wxStaticText(Wnd,
                                          14,
                                          wxT("Mandatory Field"),
                                          wxDefaultPosition,
                                          wxSize(160, -1),
                                          0);
    MandatoryFieldText->Wrap(-1);
    MandatoryFieldSizer->Add(MandatoryFieldText, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);

    //-- Mandatory field flag checkbox
    MandatoryField = new wxCheckBox(Wnd,
                                    15,
                                    wxT(""),
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    0);
    // set default choice if fields already exist
    MandatoryField->SetValue(InstFNode->GetNumFields() != 0 ? InstFNode->GetMandatoryFlag(InstFNode->GetFieldName(i)) : false);
    MandatoryFieldSizer->Add(MandatoryField, 0, wxEXPAND | wxALIGN_LEFT | wxALL, 5);
    InnerSizer->Add(MandatoryFieldSizer, 0, wxEXPAND | wxALL, 5);

    //-- store mandatory field 
    std::any TmpMandatoryField;
    try{
      TmpMandatoryField = std::any_cast<wxCheckBox *>(MandatoryField);
    } catch (const std::bad_cast &e){
      std::cout << "FIRST CAST: " << e.what() << std::endl;
    }
    Field.push_back(TmpMandatoryField);

    //-- register class
    RegClassSizer = new wxBoxSizer(wxHORIZONTAL);
    RegClassText = new wxStaticText(Wnd,
                                    16,
                                    wxT("Register Class"),
                                    wxDefaultPosition,
                                    wxSize(160, -1),
                                    0);
    RegClassText->Wrap(-1);
    RegClassSizer->Add(RegClassText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
    
    std::string RegClass = "";
    
    //-- only check instreg types
    if (InstFNode->GetNumFields() != 0 && InstFNode->GetFieldType(InstFNode->GetFieldName(i)) == CoreGenInstFormat::CGInstField::CGInstReg)
    {
      RegClass = InstFNode->GetFieldRegClass(InstFNode->GetFieldName(i))->GetName();
    }

    //-- regclass text ctrl
    RegClassCtrl = new wxTextCtrl(Wnd,
                                  17,
                                  RegClass,
                                  wxDefaultPosition,
                                  wxSize(320, 25),
                                  0,
                                  wxDefaultValidator,
                                  wxT("RegClass"));
    RegClassSizer->Add(RegClassCtrl, 0, wxALL, 0);
    InnerSizer->Add(RegClassSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    //-- store regclass
    std::any TmpRegClass;
    try {
      TmpRegClass = std::any_cast<wxTextCtrl *>(RegClassCtrl);
    } catch (const std::bad_cast &e) {
      std::cout << "RegClassCast" << e.what() << std::endl;
    }
    Field.push_back(TmpRegClass);
    FieldsInformation->push_back(Field);
  }
  Wnd->Layout();
  Wnd->Refresh();
  this->Layout();
}
      
