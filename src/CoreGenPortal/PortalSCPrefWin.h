//
// _PORTALSCPREFWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PORTALSCPREFWIN_H_
#define _PORTALSCPREFWIN_H_

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

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalCore/SCCompConfig.h"

#include <stdlib.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PortalSCPrefWin : public wxDialog {
private:
  // internal data
  SCCompConfig *Config;                 ///< StoneCutter compiler configuration

  wxCheckBox *PipelineBox;              ///< pipeline check box
  wxCheckBox *KeepBox;                  ///< keep check box
  wxCheckBox *ParseBox;                 ///< parse box
  std::vector<wxCheckBox*> OptOptions;  ///< Optimization Options
  std::vector<wxCheckBox*> LLVMOptions; ///< LLVM Pass Options
  std::vector<wxCheckBox*> SCOptions;   ///< SC Pass Options

  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  // button handlers
  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& ok );

  /// handles the 'cancel' button press
  void OnPressCancel( wxCommandEvent& ok );

protected:
  // window handlers
  wxBoxSizer *OuterSizer;   ///< outer window sizer
  wxBoxSizer *InnerSizer;   ///< inner window sizer
  wxScrolledWindow *Wnd;    ///< main scrolled window

  wxStaticLine *FirstStaticLine;  ///< first static line
  wxStaticLine *SecondStaticLine; ///< final static line
  wxStaticLine *ThirdStaticLine;  ///< final static line
  wxStaticLine *FinalStaticLine;  ///< final static line

  // buttons
  wxStdDialogButtonSizer* m_scbuttonsizer;  ///< button sizer
  wxButton* m_scOK;                         ///< OK button
  wxButton* m_scCancel;                     ///< Cancel button
public:
  PortalSCPrefWin( wxWindow *parent,
                   wxWindowID id = wxID_ANY,
                   const wxString& title = wxT("StoneCutter Compiler Preferences"),
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxDEFAULT_DIALOG_STYLE|wxVSCROLL,
                   SCCompConfig *Config = nullptr);
  ~PortalSCPrefWin();
};

#endif

// EOF
