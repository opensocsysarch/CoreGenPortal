//
// _PORTALVERIFPREFWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PORTALVERIFPREFWIN_H_
#define _PORTALVERIFPREFWIN_H_

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
#include "CoreGenPortal/PortalCore/CoreVerifConfig.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#include <stdlib.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PortalVerifPrefWin : public wxDialog{
private:
  // internal data
  CoreVerifConfig *Verif;   ///< Verification pass handler

  std::vector<wxCheckBox*> Options;

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
  wxScrolledWindow* Wnd;    ///< main scrolled window

  wxStaticLine* FinalStaticLine;  ///< final static line

  // buttons
  wxStdDialogButtonSizer* m_verifbuttonsizer;
  wxButton* m_verifOK;
  wxButton* m_verifCancel;

public:
  PortalVerifPrefWin( wxWindow *parent,
                      wxWindowID id = wxID_ANY,
                      const wxString& title = wxT("Verification Preferences"),
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE|wxVSCROLL,
                      CoreVerifConfig *Verif = nullptr);
  ~PortalVerifPrefWin();
};

#endif

// EOF
