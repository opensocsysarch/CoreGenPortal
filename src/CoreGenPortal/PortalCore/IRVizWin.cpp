// _IRVIZWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "IRVizWin.h"

// ---------------------------------------------------
// IMAGEVIEWER
// ---------------------------------------------------
IMPLEMENT_CLASS( ImageViewer, wxPanel )

BEGIN_EVENT_TABLE(ImageViewer, wxPanel)
  EVT_PAINT(ImageViewer::OnPaint)
END_EVENT_TABLE()

void ImageViewer::OnPaint(wxPaintEvent &event){
  wxUnusedVar( event );
  wxPaintDC dc(this);  // always construct a paint dc when handling OnPaint
  PrepareDC(dc);

  if ( (m_bitmap != NULL) && m_bitmap->Ok()) // If the bitmap is OK, draw it
    dc.DrawBitmap(*m_bitmap, 0, 0, true);
}

// ---------------------------------------------------
// IRVIZWIN
// ---------------------------------------------------
IMPLEMENT_CLASS( IRVizWin, wxDialog )

IRVizWin::IRVizWin(wxWindow *parent,
                   wxWindowID id,
                   const wxString& title,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   CoreGenBackend *CG)
  : wxDialog( parent, id, title, pos, size, style ), CG(CG) {

  Viz = new PortalViz();
  std::string DesignImg;
  Viz->GenerateDesignImg(CG,DesignImg);

  // generate the bitmap
  bmp = new wxBitmap( wxString(DesignImg),wxBITMAP_TYPE_PNG );

  viewer = new ImageViewer(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL|wxVSCROLL, wxT("scrolledpanel"),
                           bmp);
}

IRVizWin::~IRVizWin(){
  if( bmp )
    delete bmp;
  if( Viz )
    delete Viz;
}

// EOF
