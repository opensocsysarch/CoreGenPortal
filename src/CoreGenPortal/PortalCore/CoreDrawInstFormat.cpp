//
// _COREDRAWINSTFORMAT_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreDrawInstFormat.h"

wxBEGIN_EVENT_TABLE(CoreDrawInstFormat, wxPanel)
  EVT_PAINT(CoreDrawInstFormat::paintEvent)
wxEND_EVENT_TABLE()

CoreDrawInstFormat::CoreDrawInstFormat(wxWindow *parent,
                                       wxWindowID id,
                                       const wxString& title,
                                       CoreGenInstFormat *IF ) :
  wxPanel(parent, id, wxDefaultPosition, wxDefaultSize,
          wxTAB_TRAVERSAL, title), IF(IF){
}

void CoreDrawInstFormat::paintEvent(wxPaintEvent& evt){
  this->paintNow();
}

void CoreDrawInstFormat::paintNow(){
  wxPaintDC *dc = new wxPaintDC(this);
  wxCoord X;
  wxCoord Y;
  dc->GetLogicalOrigin(&X,&Y);
  dc->DrawText(wxString(IF->GetName()),X,Y);
#if 0
  dc->SetBrush(*wxBLUE_BRUSH); // blue filling
  dc->SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
  dc->DrawRectangle( 300, 100, 400, 200 );
#endif
}

CoreDrawInstFormat::~CoreDrawInstFormat(){
}

// EOF
