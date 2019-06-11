//
// _CORECACHEINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _CORECACHEINFOWIN_H_
#define _CORECACHEINFOWIN_H_

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

class CoreCacheInfoWin : public wxDialog {
public:
  CoreCacheInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Inst Node"),
                 CoreGenCache *Node = nullptr);
  ~CoreCacheInfoWin();

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

  wxStaticText *CacheNameText;    ///< static text for cache name
  wxStaticText *SetsText;         ///< static text for cache sets
  wxStaticText *WaysText;         ///< static text for cache ways
  wxStaticText *ParentCacheText;  ///< static text for parent cache
  wxStaticText *ChildCacheText;   ///< static text for child cache

  wxTextCtrl *CacheNameCtrl;      ///< cache name
  wxTextCtrl *SetsCtrl;           ///< cache sets
  wxTextCtrl *WaysCtrl;           ///< cache ways
  wxTextCtrl *ParentCacheCtrl;    ///< parent cache
  wxTextCtrl *ChildCacheCtrl;     ///< child cache

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
};

#endif

// EOF
