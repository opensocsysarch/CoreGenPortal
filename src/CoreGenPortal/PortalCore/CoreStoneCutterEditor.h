//
// _CORESTONECUTTEREDITOR_H_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _CORESTONECUTTEREDITOR_H_
#define _CORESTONECUTTEREDITOR_H_

//-- WX HEADERS
#include <wx/wxprec.h>
#include <wx/splash.h>
#include <wx/artprov.h>
#include <wx/aui/auibook.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dirctrl.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/process.h>
#include <wx/filefn.h>
#include <wx/txtstrm.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/stc/stc.h>

//-- PORTAL HEADERS
#include <stdlib.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CoreStoneCutterEditor: public wxStyledTextCtrl{
public:
  CoreStoneCutterEditor(wxWindow *parent,wxWindowID id);
  ~CoreStoneCutterEditor();

private:
};

enum
{
  SC_MARGIN_LINE_NUMBERS
};

#endif

// EOF
