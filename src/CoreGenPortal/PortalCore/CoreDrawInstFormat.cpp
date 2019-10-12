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
  wxCoord Width;
  wxCoord Height;
  wxCoord StartX;
  wxCoord InitialX;
  unsigned BitPixelWidth;

  dc->GetSize(&Width, &Height);
  //IF->SortFieldsByStartBit();

  StartX = Width/2 - IF_BOX_PIXEL_WIDTH/2;
  InitialX = StartX;
  BitPixelWidth = IF_BOX_PIXEL_WIDTH/IF->GetFormatWidth();

  // TODO: draw all the rectangles
  dc->SetPen( wxPen( *wxBLACK, 3 ) );
  std::string CurrName;
  std::string NextName;
  unsigned DrawWidth;
  unsigned CurrEndBit;
  unsigned NextStartBit;
  unsigned FieldWidth;
  unsigned OverlapWidth;
  bool PrevFieldOverlap = false;

  /*
  CurrName = IF->GetFieldName(0);
  DrawWidth = IF->GetFieldWidth(CurrName)*BitPixelWidth;
  dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );
  
  NextName = IF->GetFieldName(0);
  DrawWidth = IF->GetFieldWidth(NextName)*BitPixelWidth;
  dc->DrawRectangle( StartX + 80, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );
  */


  
  for(unsigned i = 1; i < IF->GetNumFields(); i++){
    //gather information needed to draw current field
    CurrName = IF->GetFieldName(i-1);
    NextName = IF->GetFieldName(i);
    CurrEndBit = IF->GetEndBit(CurrName);
    NextStartBit = IF->GetStartBit(NextName);
    FieldWidth = IF->GetFieldWidth(CurrName);
    dc->SetBrush( *wxTRANSPARENT_BRUSH );

    if(PrevFieldOverlap){
      FieldWidth -= OverlapWidth;
    }

    if( NextStartBit > CurrEndBit){
      //draw current field
      DrawWidth = FieldWidth*BitPixelWidth;
      dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

      //prepare to draw next field
      PrevFieldOverlap = false;
      StartX += DrawWidth;
    }
    else{
      //draw non-overlapping part of current field
      OverlapWidth = CurrEndBit - NextStartBit + 1;
      DrawWidth = (FieldWidth - OverlapWidth)*BitPixelWidth;
      dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

      //draw overlapping part of current field
      StartX += DrawWidth;
      dc->SetBrush( *wxRED_BRUSH );
      DrawWidth = OverlapWidth*BitPixelWidth;
      dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );
      
      //prepare to draw next field
      PrevFieldOverlap = true;
      StartX += DrawWidth;
    }
  }
  
  DrawWidth = IF_BOX_PIXEL_WIDTH + InitialX - StartX;
  dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );
  
}

CoreDrawInstFormat::~CoreDrawInstFormat(){
}

// EOF
