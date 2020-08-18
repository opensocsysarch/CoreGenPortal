// _INSTFORMATVIZWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "InstFormatVizWin.h"

// ---------------------------------------------------
// IMAGEVIEWER
// ---------------------------------------------------
IMPLEMENT_CLASS( InstFormatImageViewer, wxPanel )

BEGIN_EVENT_TABLE(InstFormatImageViewer, wxPanel)
  EVT_PAINT(InstFormatImageViewer::OnPaint)
END_EVENT_TABLE()

void InstFormatImageViewer::OnPaint(wxPaintEvent &event){
  wxUnusedVar( event );
  wxPaintDC dc(this);  // always construct a paint dc when handling OnPaint
  PrepareDC(dc);

  if ( (m_bitmap != NULL) && m_bitmap->Ok()) // If the bitmap is OK, draw it
    dc.DrawBitmap(*m_bitmap, 0, 0, true);
}

// ---------------------------------------------------
// InstFormatVIZWIN
// ---------------------------------------------------
IMPLEMENT_CLASS( InstFormatVizWin, wxDialog )

InstFormatVizWin::InstFormatVizWin(wxWindow *parent,
                   wxWindowID id,
                   const wxString& title,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   wxString ImgPath,
                   wxString ProjPath,
                   bool SaveViz )
  : wxDialog( parent, id, title, pos, size, style ), ImgPath(ImgPath) {

  // determine if we need to save it
  if( SaveViz ){
    std::string SrcPath = std::string(ImgPath.mb_str());
    std::string DestPath = std::string(ProjPath.mb_str()) + "/InstFormatViz.png";
    std::ifstream source(SrcPath, std::ios::binary);
    std::ofstream dest(DestPath, std::ios::binary);
    dest << source.rdbuf();
    source.close();
    dest.close();
  }

  // generate the bitmap
  bmp = new wxBitmap( ImgPath,wxBITMAP_TYPE_PNG );

  viewer = new InstFormatImageViewer(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL|wxVSCROLL, wxT("scrolledpanel"),
                           bmp);
}

InstFormatVizWin::~InstFormatVizWin(){
  if( bmp )
    delete bmp;
}

// EOF
