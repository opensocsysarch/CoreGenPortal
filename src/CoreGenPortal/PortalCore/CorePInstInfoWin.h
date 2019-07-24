//
// _COREPINSTINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREPINSTINFOWIN_H_
#define _COREPINSTINFOWIN_H_

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

class CorePInstInfoWin : public wxDialog {
public:
  CorePInstInfoWin( wxWindow* parent,
                    wxWindowID id = wxID_ANY,
                    const wxString& title = wxT("Pseudo Inst Node"),
                    CoreGenPseudoInst *Node = nullptr);
  ~CorePInstInfoWin();

protected:
  // window handlers
  wxScrolledWindow *Wnd;         ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *PInstNameSizer;     ///< pinst name sizer
  wxBoxSizer *InstNameSizer;      ///< inst name sizer
  wxBoxSizer *ISANameSizer;       ///< ISA name sizer
  wxBoxSizer *EncodingSizer;      ///< encoding sizer


  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *PInstNameText;    ///< static text for SoC name
  wxStaticText *InstNameText;     ///< static text for SoC name
  wxStaticText *ISANameText;      ///< static text for isa format
  wxStaticText *EncText;          ///< static text for encodings

  wxTextCtrl *PInstNameCtrl;      ///< instruction name
  wxTextCtrl *InstNameCtrl;       ///< instruction name
  wxTextCtrl *ISANameCtrl;        ///< instruction set name
  wxTextCtrl *EncCtrl;            ///< StoneCutter syntax

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
  CoreGenPseudoInst *PInstNode;
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
