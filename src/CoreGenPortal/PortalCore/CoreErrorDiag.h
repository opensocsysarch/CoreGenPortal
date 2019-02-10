//
// _COREERRORDIAG_H_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREERRORDIAG_H_
#define _COREERRORDIAG_H_

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

class CoreErrorDiag : public wxDialog {
public:
  CoreErrorDiag( wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& title = wxT("CoreGenPortal Error"),
                 const wxString Msg = wxT("Encountered a unknown error"));
  ~CoreErrorDiag();

protected:
private:
  wxString Msg;     ///< Error message for dialog box
};

#endif

// EOF
