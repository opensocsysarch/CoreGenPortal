//
// _PORTALNEWPROJWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PORTALNEWPROJWIN_H_
#define _PORTALNEWPROJWIN_H_

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
#include <wx/stc/stc.h>

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalCore/CoreUserConfig.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#include <stdlib.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PortalNewProjWin : public wxDialog {
private:
  wxGenericDirCtrl *ProjDir;    ///< project directory handler for MainFrame
  wxTextCtrl *LogPane;          ///< log pane for messaging
  wxStyledTextCtrl *IRPane;     ///< ir file pane
  CoreUserConfig *User;         ///< user configuration data
  CoreGenBackend *CG;           ///< coregen backend pointer

  wxStaticText *ProjNameText;   ///< static text for project name
  wxStaticText *ProjTypeText;   ///< static text for project type

  wxTextCtrl *ProjNameCtrl;     ///< project name input box
  wxChoice *ProjType;           ///< project type selection box

  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );

  /// handles the 'cancel' button press
  void OnPressCancel( wxCommandEvent& event );

protected:
  // window handlers
  wxPanel *m_panel1;              ///< main panel

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  // buttons
  wxStdDialogButtonSizer* m_projbuttonsizer;  ///< button sizer
  wxButton *m_userOK;                         ///< ok button
  wxButton *m_userCancel;                     ///< cancel button

public:
  PortalNewProjWin( wxWindow* parent,
                    wxWindowID id = wxID_ANY,
                    const wxString& title = wxT("New Project"),
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxDEFAULT_DIALOG_STYLE,
                    wxGenericDirCtrl *ProjDir = nullptr,
                    wxTextCtrl *LogPane = nullptr,
                    wxStyledTextCtrl *IRPane = nullptr,
                    CoreUserConfig *User = nullptr );
  CoreGenBackend *GetCoreGenPtr() { return CG; }
  ~PortalNewProjWin();
};


#endif

// EOF
