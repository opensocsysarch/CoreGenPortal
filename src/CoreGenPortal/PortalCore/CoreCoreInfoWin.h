//
// _CORECOREINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _CORECOREINFOWIN_H_
#define _CORECOREINFOWIN_H_

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

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#include "CoreGenPortal/PortalMainFrame.h"

class CoreCoreInfoWin : public wxDialog {
public:
  CoreCoreInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Core Node"),
                 CoreGenCore *Node = nullptr);
  ~CoreCoreInfoWin();

protected:
  CoreGenCore *CoreNode;
  // window handlers
  wxScrolledWindow *Wnd;          ///< scrolling window handler

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *CoreNameSizer;      ///< core name sizer
  wxBoxSizer *SMTMethodSizer;     ///< smt name sizer
  wxBoxSizer *ThreadUnitSizer;    ///< thread unit sizer
  wxBoxSizer *ISANameSizer;       ///< isa name sizer
  wxBoxSizer *CacheNameSizer;     ///< cache name sizer
  wxBoxSizer *RegClassSizer;      ///< register class sizer
  wxBoxSizer *ExtNameSizer;       ///< extension name sizer

  wxStaticText *CoreNameText;     ///< static text for SoC name
  wxStaticText *SMTMethodText;    ///< static text for SMTNameText
  wxStaticText *ThreadUnitText;   ///< static text for thread units name
  wxStaticText *ISANameText;      ///< static text for isa name
  wxStaticText *CacheNameText;    ///< static text for cache name
  wxStaticText *RegClassNameText; ///< static text for register class name
  wxStaticText *ExtNameText;      ///< static text for extension name

  wxTextCtrl *CoreNameCtrl;       ///< name of the SoC
  wxTextCtrl *SMTMethodCtrl;      ///< name of SMT method
  wxTextCtrl *ThreadUnitCtrl;     ///< number of thread units
  wxTextCtrl *ISACtrl;            ///< name of the ISA
  wxTextCtrl *CacheCtrl;          ///< name of the primary cache layer
  wxTextCtrl *RegClassCtrl;       ///< name of the register classes
  wxTextCtrl *ExtCtrl;            ///< name of the extensions

  wxArrayString SMTOptionsArray;
  wxComboBox *SMTMethodName;    ///< name of SMT Method
  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button
  wxButton *m_userSAVE;                       ///< save button

private:
  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );

  /// handles 'save' click
  void OnSave( wxCommandEvent& save );
};

#endif

// EOF
