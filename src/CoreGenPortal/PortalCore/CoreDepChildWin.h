//
// _COREDEPCHILDWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREDEPCHILDWIN_H_
#define _COREDEPCHILDWIN_H_

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

#include "CoreGenPortal/PortalMainFrame.h"

class CoreDepChildWin : public wxDialog {
public:
  CoreDepChildWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 CoreGenNode *Node = nullptr);
  ~CoreDepChildWin();


protected:
  CoreGenNode *DepNode;           ///< top level node to query for childen

  wxBoxSizer *ChildNameSizer;     ///< Node name sizer

  wxStaticText *ChildNameText;    ///< static text for core name

  wxTextCtrl *ChildNameCtrl;      ///< name of the children

  // window handlers
  wxPanel *m_panel1;              ///< main panel

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  // buttons
  wxStdDialogButtonSizer* m_depbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );
};

#endif

// EOF
