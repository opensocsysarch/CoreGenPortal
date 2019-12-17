//
// _CORECOMMINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _CORECOMMINFOWIN_H_
#define _CORECOMMINFOWIN_H_

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

#include "CoreGenPortal/PortalMainFrame.h"

class CoreCommInfoWin : public wxDialog {
public:
  CoreCommInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Comm Node"),
                 CoreGenComm *Node = nullptr);
  ~CoreCommInfoWin();

  protected:
  CoreGenComm *CommNode;          ///< the comm node associated with this info window
  // window handlers
  wxScrolledWindow *Wnd;          ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *CommNameSizer;      ///< comm name sizer
  wxBoxSizer *CommTypeSizer;      ///< comm type sizer
  wxBoxSizer *CommWidthSizer;     ///< comm width sizer
  wxBoxSizer *CommEndpointSizer;  ///< comm endpoint sizer
  wxBoxSizer *SpacerPanelSizer;   ///< sizer for spance panel

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *CommNameText;     ///< static text for SoC name
  wxStaticText *CommTypeText;     ///< static text for inst format
  wxStaticText *WidthText;        ///< static text for width
  wxStaticText *EndpointText;     ///< static text for endpoints

  wxTextCtrl *CommNameCtrl;       ///< comm name
  wxTextCtrl *CommTypeCtrl;       ///< comm type
  wxTextCtrl *WidthCtrl;          ///< width
  wxTextCtrl *EndpointCtrl;       ///< endpoints

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
