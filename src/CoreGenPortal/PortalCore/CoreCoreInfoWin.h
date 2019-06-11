//
// _CORECOREINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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
#include <wx/textctrl.h>

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class CoreCoreInfoWin : public wxDialog {
public:
  CoreCoreInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Core Node"),
                 CoreGenCore *Node = nullptr);
  ~CoreCoreInfoWin();

  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );

protected:
  // window handlers
  wxPanel *m_panel1;              ///< main panel

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *CoreNameText;     ///< static text for SoC name
  wxStaticText *ThreadUnitText;   ///< static text for thread units name
  wxStaticText *ISANameText;      ///< static text for isa name
  wxStaticText *CacheNameText;    ///< static text for cache name
  wxStaticText *RegClassNameText; ///< static text for register class name
  wxStaticText *ExtNameText;      ///< static text for extension name

  wxTextCtrl *CoreNameCtrl;       ///< name of the SoC
  wxTextCtrl *ThreadUnitCtrl;     ///< number of thread units
  wxTextCtrl *ISACtrl;            ///< name of the ISA
  wxTextCtrl *CacheCtrl;          ///< name of the primary cache layer
  wxTextCtrl *RegClassCtrl;       ///< name of the register classes
  wxTextCtrl *ExtCtrl;            ///< name of the extensions

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
};

#endif

// EOF