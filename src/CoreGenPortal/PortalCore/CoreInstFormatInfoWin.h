//
// _COREINSTFORMATINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREINSTFORMATINFOWIN_H_
#define _COREINSTFORMATINFOWIN_H_

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

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalCore/CoreDrawInstFormat.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class CoreInstFormatInfoWin : public wxDialog {
public:
  CoreInstFormatInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Inst Format Node"),
                 CoreGenInstFormat *Node = nullptr);
  ~CoreInstFormatInfoWin();


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

  // standard sizers
  wxBoxSizer *OuterSizer;     ///< outer box sizer
  wxBoxSizer *InnerSizer;     ///< inner box sizer

  CoreDrawInstFormat *IFDraw; ///< inst format drawing object

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
};

#endif

// EOF
