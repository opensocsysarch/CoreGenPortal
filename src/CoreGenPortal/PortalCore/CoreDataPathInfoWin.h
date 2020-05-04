//
// _COREDATAPATHINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREDATAPATHINFOWIN_H_
#define _COREDATAPATHINFOWIN_H_

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

class CoreDataPathInfoWin : public wxDialog {
public:
  CoreDataPathInfoWin( wxWindow* parent,
                       wxWindowID id = wxID_ANY,
                       const wxString& title = wxT("DataPath Node"),
                       CoreGenDataPath *Node = nullptr);
  ~CoreDataPathInfoWin();

protected:
  CoreGenDataPath *CoreGenDPath;  ///< Node

  // window handlers
  wxScrolledWindow *Wnd;          ///< scrolling window handler

  // static lines
  wxStaticLine *FinalStaticLine;  ///< final static line

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *DPathNameSizer;     ///< data path name sizer
  wxBoxSizer *StyleNameSizer;     ///< style name sizer

  wxStaticText *DPathNameText;    ///< static text for data path name
  wxStaticText *StyleNameText;    ///< static text for style name

  wxTextCtrl *DPathNameCtrl;      ///< name of the data path
  wxTextCtrl *StyleNameCtrl;      ///< name of the style

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
