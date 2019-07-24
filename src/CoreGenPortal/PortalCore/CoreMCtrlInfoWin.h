//
// _COREMCTRLINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREMCTRLINFOWIN_H_
#define _COREMCTRLINFOWIN_H_

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

class CoreMCtrlInfoWin : public wxDialog {
public:
  CoreMCtrlInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Memory Controller Node"),
                 CoreGenMCtrl *Node = nullptr);
  ~CoreMCtrlInfoWin();

protected: 
  CoreGenMCtrl *MCtrlNode;
  // window handlers
  wxScrolledWindow *Wnd;         ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *MCtrlNameSizer;     ///< mctrl name sizer
  wxBoxSizer *InputPortSizer;     ///< input port sizer

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *MCtrlNameText;    ///< static text for memory controller name
  wxStaticText *InputPortText;    ///< static text for input ports

  wxTextCtrl *MCtrlCtrl;          ///< memory controller name
  wxTextCtrl *InputPortCtrl;      ///< input ports for memory controller

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
  // Event handler functions
  /// Declares the event table
  wxDECLARE_EVENT_TABLE();

  /// handles the 'ok' button press
  void OnPressOk( wxCommandEvent& event );

  /// handles 'enter' press in textctrl box
  void OnPressEnter( wxCommandEvent& event);
};

#endif

// EOF
