//
// _COREREGINFOWIN_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREREGINFOWIN_H_
#define _COREREGINFOWIN_H_

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

class CoreRegInfoWin : public wxDialog {
public:
  CoreRegInfoWin( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("Reg Node"),
                 CoreGenReg *Node = nullptr);
  ~CoreRegInfoWin();

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

  wxStaticText *RegNameText;      ///< static text for reg name
  wxStaticText *RegIdxText;       ///< static text for reg idx
  wxStaticText *WidthText;        ///< static text for width
  wxStaticText *SubRegText;       ///< static text for sub registers

  wxTextCtrl *RegNameCtrl;        ///< reg name ctrl
  wxTextCtrl *RegIdxCtrl;         ///< reg idx ctrl
  wxTextCtrl *WidthCtrl;          ///< reg width ctrl
  wxTextCtrl *SubRegCtrl;         ///< sub register ctrl

  wxCheckBox *SIMDCheck;          ///< simd reg check box
  wxCheckBox *RWCheck;            ///< rw reg check box
  wxCheckBox *ROCheck;            ///< ro reg check box
  wxCheckBox *CSRCheck;           ///< csr reg check box
  wxCheckBox *AMSCheck;           ///< ams reg check box
  wxCheckBox *TUSCheck;           ///< tus reg check box
  wxCheckBox *PCCheck;            ///< pc reg check box
  wxCheckBox *SharedCheck;        ///< shared reg check box

  // buttons
  wxStdDialogButtonSizer* m_socbuttonsizer;   ///< button sizer
  wxButton *m_userOK;                         ///< ok button

private:
};

#endif

// EOF
