//
// _PORTALUSERPREFWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PORTALUSERPREFWIN_H_
#define _PORTALUSERPREFWIN_H_

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
#include "CoreGenPortal/PortalCore/CoreUserConfig.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#include <stdlib.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PortalUserPrefWin : public wxDialog{
private:
  // internal data
  CoreUserConfig *User;   ///< User configuration data

  wxStaticText *ProjectDirText;   ///< static text for project directory
  wxStaticText *ArchiveDirText;   ///< static text for archive directory

  wxTextCtrl *ProjectDirCtrl;     ///< project directory input box
  wxTextCtrl *ArchiveDirCtrl;     ///< archive directory input box

  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  // button handlers
  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& ok );

  /// handles the 'cancel' button press
  void OnPressCancel( wxCommandEvent& ok );

protected:
  // window handlers
  wxPanel *m_panel1;                      ///< main panel

  wxStaticLine* FinalStaticLine;  ///< final static line

  // buttons
  wxStdDialogButtonSizer* m_userbuttonsizer;  ///< button sizer
  wxButton* m_userOK;                         ///< ok button
  wxButton* m_userCancel;                     ///< cancel button

public:
  PortalUserPrefWin( wxWindow *parent,
                     wxWindowID id = wxID_ANY,
                     const wxString& title = wxT("Userication Preferences"),
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxDEFAULT_DIALOG_STYLE,
                     CoreUserConfig *User = nullptr);
  ~PortalUserPrefWin();
};

#endif

// EOF
