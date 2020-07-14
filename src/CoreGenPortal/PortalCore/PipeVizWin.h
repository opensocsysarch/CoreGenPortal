//
// _PIPEVIZWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PIPEVIZWIN_H_
#define _PIPEVIZWIN_H_

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

//-- C++ HEADERS
#include <stdlib.h>
#include <iostream>
#include <fstream>


//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PipeImageViewer : public wxScrolledWindow {
private:
  DECLARE_CLASS(PipeImageViewer)
  DECLARE_EVENT_TABLE()

  wxBitmap *m_bitmap;

  // Event handlers
  /// Paints the image
  void OnPaint(wxPaintEvent& event);

public:
  /// Set the the bitmap image
  void SetBitmap(wxBitmap *bmp){ m_bitmap = bmp;}

  /// default constructor
  PipeImageViewer(wxWindow* parent, wxWindowID id = wxID_ANY,
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

class PipeVizWin : public wxDialog {
private:
  // private data
  wxString ImgPath;     ///< CoreGenBackend object

  wxBitmap *bmp;        ///< Bitmap

  /// Declares the event table
  DECLARE_CLASS(PipeVizWin)

  // button handlers
  /// handles the 'close' button press
  void OnQuit(wxCloseEvent &event);

protected:
  // window handlers
  PipeImageViewer *viewer;      ///< image viewer object

public:
  /// Default Constructor
  PipeVizWin(wxWindow* parent,
           wxWindowID id = wxID_ANY,
           const wxString& title = wxT("Pipeline Visualization"),
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_DIALOG_STYLE,
           wxString ImgPath = wxT(""),
           wxString ProjPath = wxT(""),
           bool SaveViz = false);

  /// Default destructor
  ~PipeVizWin();
};

#endif

// EOF
