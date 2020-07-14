// _PIPEVIZWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "PipeVizWin.h"

// ---------------------------------------------------
// IMAGEVIEWER
// ---------------------------------------------------
IMPLEMENT_CLASS( PipeImageViewer, wxPanel )

BEGIN_EVENT_TABLE(PipeImageViewer, wxPanel)
  EVT_PAINT(PipeImageViewer::OnPaint)
END_EVENT_TABLE()

void PipeImageViewer::OnPaint(wxPaintEvent &event){
  wxUnusedVar( event );
  wxPaintDC dc(this);  // always construct a paint dc when handling OnPaint
  PrepareDC(dc);

  if ( (m_bitmap != NULL) && m_bitmap->Ok()) // If the bitmap is OK, draw it
    dc.DrawBitmap(*m_bitmap, 0, 0, true);
}

// ---------------------------------------------------
// PipeVIZWIN
// ---------------------------------------------------
IMPLEMENT_CLASS( PipeVizWin, wxDialog )

PipeVizWin::PipeVizWin(wxWindow *parent,
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
    std::string DestPath = std::string(ProjPath.mb_str()) + "/PipeViz.png";
    std::ifstream source(SrcPath, std::ios::binary);
    std::ofstream dest(DestPath, std::ios::binary);
    dest << source.rdbuf();
    source.close();
    dest.close();
  }

  // generate the bitmap
  bmp = new wxBitmap( ImgPath,wxBITMAP_TYPE_PNG );

  viewer = new PipeImageViewer(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL|wxVSCROLL, wxT("scrolledpanel"),
                           bmp);
}

PipeVizWin::~PipeVizWin(){
  if( bmp )
    delete bmp;
}

// EOF
