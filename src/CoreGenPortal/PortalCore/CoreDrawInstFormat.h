//
// _COREDRAWINSTFORMAT_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREDRAWINSTFORMAT_H_
#define _COREDRAWINSTFORMAT_H_
#define IF_BOX_PIXEL_WIDTH 320
#define IF_BOX_PIXEL_HEIGHT 30
#define IF_BOX_TOP 50
#define IF_KEY_TOP 10

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
#include <wx/dcclient.h>

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class CoreDrawInstFormat : public wxPanel {
public:
  CoreDrawInstFormat( wxWindow *parent,
                      wxWindowID id = wxID_ANY,
                      const wxString& title = wxT("Inst Format Figure"),
                      CoreGenInstFormat *Node = nullptr);
  ~CoreDrawInstFormat();

  void paintEvent(wxPaintEvent& evt);
  void paintNow();
  void drawKey(wxCoord Width);
  void handleLeftClick(wxMouseEvent& mevt);
  bool clickedBoundary(wxPoint clickPoint, wxCoord StartX, unsigned BitPixelWidth);
  unsigned pixelToBit(wxCoord x, wxCoord StartX, unsigned BitPixelWidth);
  void drawSelectButtons(wxCoord StartX, int selectedBox);
  void drawSelectButton(std::string Label, wxCoord StartX, const wxColour *Colour);
  int getSelectedButton(wxPoint ClickPoint, wxCoord StartX);

  DECLARE_EVENT_TABLE();

protected:
private:
  bool DisplayBoundaries;
  std::vector<std::string> boundedFields;
  wxPaintDC* dc;
  CoreGenInstFormat *IF;    //< handler for the instruction format
};

#endif

// EOF
