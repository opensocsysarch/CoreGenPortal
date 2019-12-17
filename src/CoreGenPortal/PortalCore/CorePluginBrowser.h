//
// _COREPLUGINBROWSER_H_
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREPLUGINBROWSER_H_
#define _COREPLUGINBROWSER_H_

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

class CorePluginBrowser : public wxDialog{
public:
  CorePluginBrowser( wxWindow *parent,
                      wxWindowID id = wxID_ANY,
                      const wxString& title = wxT("Plugin Browser"),
                      wxString PluginName = wxT("NONAME"),
                      wxString PluginPath = wxT("NOPATH"),
                      CoreGenPlugin *PluginPtr = nullptr,
                      CoreGenBackend* CGProject = nullptr,
                      wxTextCtrl *LogPane = nullptr );
  ~CorePluginBrowser();

protected:
  // window handlers
  wxScrolledWindow *Wnd;          ///< scrolling window handler

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  // box sizers
  wxBoxSizer *OuterSizer;         ///< Outer sizer
  wxBoxSizer *InnerSizer;         ///< Inner sizer
  wxBoxSizer *PluginNameSizer;    ///< Plugin name sizer
  wxBoxSizer *HDLSizer;           ///< HDL sizer
  wxBoxSizer *LLVMSizer;          ///< LLVM sizer
  wxBoxSizer *FeatureSizer;       ///< Feature sizer

  // window elements
  wxStaticText* PluginNameText;   ///< Plugin name text
  wxStaticText* HDLCodegen;       ///< HDL codegen text
  wxStaticText* LLVMCodegen;      ///< LLVM codegen text
  wxStaticText* FeaturesText;     ///< Features text
  std::vector<wxStaticText *> Features; ///< Features vector

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  // Private variables
  wxString PluginName;
  wxString PluginPath;
  CoreGenPlugin *PLUGIN;
  CoreGenBackend *CGProject;
  wxTextCtrl *LogPane;

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );

  /// converts a feature type to a wxString
  wxString FeatureTypeToString(CGFeatureType Type );
};

#endif

// EOF
