//
// _COREPLUGININFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREPLUGININFOWIN_H_
#define _COREPLUGININFOWIN_H_

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
#include <wx/scrolwin.h>

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class CorePluginInfoWin : public wxDialog {
public:
  CorePluginInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Scratchpad Node"),
                 CoreGenPlugin *Node = nullptr);
  ~CorePluginInfoWin();

  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );

protected:
  // window handlers
  wxScrolledWindow *Wnd;         ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *PluginNameText;   ///< static text for plugin name
  wxStaticText *LibNameText;      ///< static text for lib name
  wxStaticText *VersionText;      ///< static text for version name
  wxStaticText *FeatureText;      ///< static text for feature name

  wxTextCtrl *PluginNameCtrl;     ///< plugin name
  wxTextCtrl *LibNameCtrl;        ///< library name
  wxTextCtrl *VersionCtrl;        ///< version number
  wxTextCtrl *FeatureCtrl;        ///< features

  wxCheckBox *HDLCodegen;         ///< does this have an HDL codegen?
  wxCheckBox *LLVMCodegen;        ///< does this have an LLVM codegen?

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
  wxString GetFeatureStr(CoreGenPlugin *Plugin, unsigned i);
};

#endif

// EOF
