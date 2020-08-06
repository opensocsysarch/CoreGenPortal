//
// _COREINSTFORMATINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREINSTFORMATINFOWIN_H_
#define _COREINSTFORMATINFOWIN_H_
#define TIMER_ID
#include <string>
#include <any>
#include <iostream>
//-- WX HEADERS
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dirctrl.h>
#include <wx/aui/auibook.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include <wx/combobox.h>
#include <wx/arrstr.h>
#include <wx/timer.h>
#include <wx/checkbox.h>

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalMainFrame.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class CoreInstFormatInfoWin : public wxDialog
{
public:
  CoreInstFormatInfoWin(wxWindow *parent,
                        wxWindowID id = wxID_ANY,
                        const wxString &title = wxT("Inst Format Node"),
                        CoreGenInstFormat *Node = nullptr,
                        CoreGenBackend *CG = nullptr);
  // Reg Class Text
  ~CoreInstFormatInfoWin();

  bool IsFieldsExpanded() { return isFieldsExpanded; }

  wxString GetInstFormatNodeByName();

  bool GetFieldData(int field,
                    wxString *Name,
                    bool *IsMandatory);

  std::vector<std::vector<std::any>> *FieldsInformation;

protected:
  // window handlers
  wxPanel *m_panel1;     ///< main panel
  wxScrolledWindow *Wnd; ///< scrolling window handler

  // static lines
  wxStaticLine *FinalStaticLine; ///< final static line

  // box sizers
  wxBoxSizer *OuterSizer;          ///< outer box sizer
  wxBoxSizer *InnerSizer;          ///< inner box sizer
  wxBoxSizer *InstFormatNameSizer; ///< instruction format sizer
  wxBoxSizer *ISANameSizer;        ///< isa name sizer
  wxBoxSizer *FormatWidthSizer;    ///< format width
  wxBoxSizer *FieldsSizer;         ///< number of fields
  wxBoxSizer *FieldNameSizer;
  wxBoxSizer *FieldTypeSizer;      ///< type of field
  wxBoxSizer *StartBitSizer;       ///< starting bit
  wxBoxSizer *EndBitSizer;         ///< starting bit
  wxBoxSizer *MandatoryFieldSizer; ///< mandatory field
  wxBoxSizer *NumFieldsSizer;      ///<  number of fields
  wxBoxSizer *RegClassSizer;       ///< register class

  wxStaticText *InstFormatNameText; ///< static text for instruction format name
  wxStaticText *ISANameText;        ///< static text for ISA name
  wxStaticText *FormatWidthText;    ///< static text for format width
  wxStaticText *FieldNameText;      ///< static text for field name
  wxStaticText *FieldTypeText;      ///< static text for field type
  wxStaticText *StartBitText;       ///< static text for starting bit
  wxStaticText *EndBitText;         ///< static text for ending bit
  wxStaticText *MandatoryFieldText; ///< static text for mandatory field
  wxStaticText *NumFieldsText;      ///< static text for number of fields
  wxStaticText *RegClassText;       ///< static text for register class

  wxTextCtrl *InstFormatNameCtrl; ///< name of instruction format
  wxTextCtrl *ISACtrl;            ///< name of ISA
  wxTextCtrl *FieldNameCtrl;      ///< name of field
  wxTextCtrl *FormatWidthCtrl;    ///< integer specifying bit width of instruction format
  wxTextCtrl *FieldsCtrl;         ///< integer specifying number of fields
  wxTextCtrl *MandatoryFieldCtrl; ///< mandatory field
  wxTextCtrl *RegClassCtrl;       ///< name of register class

  wxArrayString FieldTypeOptionsArray; ///> options for field type
  wxArrayString StartBitOptionsArray;  ///> options for start bit
  wxArrayString EndBitOptionsArray;    ///> options for end bit
  wxArrayString NumFieldsOptionsArray; ///> options for number of fields

  wxComboBox *FieldTypeName; ///< name of field type
  wxComboBox *StartBit;      ///< starting bit
  wxComboBox *EndBit;        ///< ending bit
  wxComboBox *NumFields;     ///< number of fields

  wxCheckBox *MandatoryField; ///< checkbox for mandatory field flag

  wxString FormatName; ///< already existing format name

  // buttons
  wxStdDialogButtonSizer *m_socbuttonsizer; ///< button sizer
  wxButton *m_userCANCEL;                   ///< ok button
  wxButton *m_userSAVE;                     ///< save button
  wxButton *ExpandFieldsButton;             ///< init fields button

private:
  CoreGenInstFormat *InstFNode;             ///< InstFormat handler
  CoreGenBackend *CG;                       ///< CoreGenBackend handler
  bool isFieldsExpanded;                    ///< have the fields been expanded
  bool isCreateNode;                        ///< did we create a new node
  int NumberOfFields;                       ///< number of fields

  std::vector<std::string> ExistingFields;  ///< records the existing field names

  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressCancel(wxCommandEvent &event);

  /// handles 'save' click
  void OnSave(wxCommandEvent &save);

  /// Init fields based on numFields input
  void OnPressExpandFields(wxCommandEvent &ok);
};

enum
{
  ID_EXPANDFIELDS = 1,
};

enum
{
  FIELD_NAME = 0,
  FIELD_TYPE = 1,
  FIELD_WIDTH = 2,
  START_BIT = 3,
  END_BIT = 4,
  MANDATORY = 5,
  REG_CLASS = 6
} FieldAttributes;

#endif
// EOF
