//
// _IRVIZWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _IRVIZWIN_H_
#define _IRVIZWIN_H_

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
#include <wx/stc/stc.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/scrolwin.h>
#include <stdlib.h>

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalViz/PortalViz.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class ImageViewer : public wxScrolledWindow {
  DECLARE_CLASS(ImageViewer)
  DECLARE_EVENT_TABLE()

  wxBitmap *m_bitmap;

  // Event handlers
  /// Paints the image
  void OnPaint(wxPaintEvent& event);

public:
  /// Set the the bitmap image
  void SetBitmap(wxBitmap *bmp){ m_bitmap = bmp;}

  /// default constructor
  ImageViewer(wxWindow* parent, wxWindowID id = wxID_ANY,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxHSCROLL|wxVSCROLL,
                        const wxString& name = wxT("scrolledpanel"),
                        wxBitmap *bmp = nullptr):
                                wxScrolledWindow(parent,
                                                 id,
                                                 pos,
                                                 size,
                                                 style,
                                                 name),
                                m_bitmap(bmp){
    SetScrollbars(1,1, m_bitmap->GetWidth(), m_bitmap->GetHeight(), 0, 0);
    SetScrollRate( 10, 10 );
  }
};

class IRVizWin : public wxDialog {
private:
  // private data
  CoreGenBackend *CG;   ///< CoreGenBackend object

  PortalViz *Viz;       ///< Design visualization object

  wxBitmap *bmp;        ///< Bitmap

  /// Declares the event table
  //wxDECLARE_EVENT_TABLE();
  DECLARE_CLASS(IRVizWin)

  // button handlers
  /// handles the 'close' button press
  void OnQuit(wxCloseEvent &event);

protected:
  // window handlers
  ImageViewer *viewer;      ///< image viewer object

public:
  IRVizWin(wxWindow* parent,
           wxWindowID id = wxID_ANY,
           const wxString& title = wxT("IR Visualization"),
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_DIALOG_STYLE,
           CoreGenBackend *CG = nullptr);
  ~IRVizWin();
};

#endif

// EOF
