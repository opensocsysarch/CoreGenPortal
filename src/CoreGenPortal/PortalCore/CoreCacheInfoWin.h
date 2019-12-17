//
// _CORECACHEINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
#include <wx/panel.h>

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#include "CoreGenPortal/PortalMainFrame.h"

class CoreCacheInfoWin : public wxDialog {
public:
  CoreCacheInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Cache Node"),
                 CoreGenCache *Node = nullptr);
  ~CoreCacheInfoWin();

protected:
  CoreGenCache *CacheNode;        ///< the cache node associated with this info window
  // window handlers
  wxScrolledWindow *Wnd;          ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *CacheNameSizer;     ///< cache name sizer
  wxBoxSizer *CacheSetsSizer;     ///< cache sets sizer
  wxBoxSizer *CacheWaysSizer;     ///< cache ways sizer
  wxBoxSizer *ParentCacheSizer;   ///< parent cache sizer
  wxBoxSizer *ChildCacheSizer;    ///< child cache sizer
  wxBoxSizer *SpacerPanelSizer;   ///< spacer panel for spacing at bottom

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
