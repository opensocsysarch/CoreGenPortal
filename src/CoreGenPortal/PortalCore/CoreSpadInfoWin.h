//
// _CORESPADINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _CORESPADINFOWIN_H_
#define _CORESPADINFOWIN_H_

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

class CoreSpadInfoWin : public wxDialog {
public:
  CoreSpadInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Scratchpad Node"),
                 CoreGenSpad *Node = nullptr);
  ~CoreSpadInfoWin();

protected:
  CoreGenSpad *SpadNode;
  // window handlers
  wxScrolledWindow *Wnd;         ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *SpadNameSizer;      ///< spad name sizer
  wxBoxSizer *SizeNameSizer;      ///< spad size sizer
  wxBoxSizer *RqstNameSizer;      ///< request name sizer
  wxBoxSizer *RspNameSizer;       ///< response name sizer
  wxBoxSizer *StartNameSizer;     ///< Starting mem position sizer

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *SpadNameText;     ///< static text for spad name
  wxStaticText *SizeNameText;     ///< static text for size name
  wxStaticText *RqstNameText;     ///< static text for request ports
  wxStaticText *RspNameText;      ///< static text for response ports
  wxStaticText *StartNameText;    ///< static text for start address

  wxTextCtrl *SpadNameCtrl;       ///< scratchpad name
  wxTextCtrl *SizeCtrl;           ///< memsize
  wxTextCtrl *RqstCtrl;           ///< rqst ports
  wxTextCtrl *RspCtrl;            ///< rsp ports
  wxTextCtrl *StartCtrl;          ///< starting address

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

  /// handles 'enter' press in textctrl box
  void OnPressEnter( wxCommandEvent& event);

  /// handles 'save' click
  void OnSave( wxCommandEvent& save );
};

#endif

// EOF
