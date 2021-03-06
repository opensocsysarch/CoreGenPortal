//
// _COREINSTINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREINSTINFOWIN_H_
#define _COREINSTINFOWIN_H_

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

class CoreInstInfoWin : public wxDialog {
public:
  CoreInstInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Inst Node"),
                 CoreGenInst *Node = nullptr);
  ~CoreInstInfoWin();


protected:
  CoreGenInst *InstNode;          ///< Inst node for this window
  // window handlers
  wxScrolledWindow *Wnd;         ///< scrolling window handler

  // box sizers
  wxBoxSizer *OuterSizer;         ///< outer sizer
  wxBoxSizer *InnerSizer;         ///< inner sizer
  wxBoxSizer *InstNameSizer;      ///< inst name sizer
  wxBoxSizer *InstFNameSizer;     ///< inst format name sizer
  wxBoxSizer *ISANameSizer;       ///< ISA name sizer
  wxBoxSizer *SyntaxSizer;        ///< syntax sizer
  wxBoxSizer *StoneCutterSizer;   ///< StoneCuttter Syntax Sizer
  wxBoxSizer *EncodingSizer;      ///< encoding sizer

  // static lines
  wxStaticLine* FinalStaticLine;  ///< final static line

  wxStaticText *InstNameText;     ///< static text for SoC name
  wxStaticText *InstFNameText;    ///< static text for inst format
  wxStaticText *ISANameText;      ///< static text for isa format
  wxStaticText *SyntaxText;       ///< static text for syntax
  wxStaticText *StoneCText;       ///< static text for stonecutter
  wxStaticText *EncText;          ///< static text for encodings

  wxTextCtrl *InstNameCtrl;       ///< instruction name
  wxTextCtrl *InstFNameCtrl;      ///< instruction format name
  wxTextCtrl *ISANameCtrl;        ///< instruction set name
  wxTextCtrl *SyntaxCtrl;         ///< instruction syntax (asm)
  wxTextCtrl *StoneCCtrl;         ///< StoneCutter syntax
  wxTextCtrl *EncCtrl;            ///< StoneCutter syntax

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
