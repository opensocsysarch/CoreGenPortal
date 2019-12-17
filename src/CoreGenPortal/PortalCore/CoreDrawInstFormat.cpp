//
// _COREDRAWINSTFORMAT_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
  unsigned FormatWidth = IF->GetFormatWidth();

  dc->GetSize(&Width, &Height);
  //IF->SortFieldsByStartBit();

  StartX = Width/2 - IF_BOX_PIXEL_WIDTH/2;
  InitialX = StartX;
  BitPixelWidth = IF_BOX_PIXEL_WIDTH/FormatWidth;

  // TODO: draw all the rectangles
  dc->SetPen( wxPen( *wxBLACK, 3 ) );
  std::string CurrName;
  std::string NextName;
  unsigned DrawWidth;
  unsigned CurrStartBit;
  unsigned CurrEndBit;
  unsigned NextStartBit;
  unsigned FieldWidth;
  unsigned OverlapWidth;
  unsigned NextBitToDraw = 0;
  bool PrevFieldOverlap = false;
  bool BrokeLoop = false;

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
    CurrStartBit = IF->GetStartBit(CurrName);
    CurrEndBit = IF->GetEndBit(CurrName);
    NextStartBit = IF->GetStartBit(NextName);
    FieldWidth = IF->GetFieldWidth(CurrName);

    dc->SetBrush( *wxTRANSPARENT_BRUSH );

    if(PrevFieldOverlap){
      FieldWidth -= OverlapWidth;
    }

    //No overlap case
    if( CurrEndBit < NextStartBit ){
      //break if field ends beyond format width
      if(CurrEndBit > FormatWidth){
        BrokeLoop = true;
        break;
      }
      //draw any unused space
      if(CurrStartBit != NextBitToDraw && !PrevFieldOverlap){
        dc->SetBrush(* wxYELLOW_BRUSH );
        DrawWidth = (CurrStartBit - NextBitToDraw)*BitPixelWidth;
        dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

        //prepare to draw field
        StartX += DrawWidth;
        NextBitToDraw = CurrStartBit;
        dc->SetBrush( *wxTRANSPARENT_BRUSH );
      }
      //draw current field
      DrawWidth = FieldWidth*BitPixelWidth;
      dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

      //prepare to draw next field
      PrevFieldOverlap = false;
      StartX += DrawWidth;
    }
    //Overlapping case
    else{
      //break if field ends beyond format width
      if(CurrEndBit > FormatWidth){
        BrokeLoop = true;
        break;
      }
      //draw any unused space
      if(CurrStartBit != NextBitToDraw && !PrevFieldOverlap){
        dc->SetBrush(* wxYELLOW_BRUSH );
        DrawWidth = (CurrStartBit - NextBitToDraw)*BitPixelWidth;
        dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

        //prepare to draw field
        StartX += DrawWidth;
        NextBitToDraw = CurrStartBit;
        dc->SetBrush( *wxTRANSPARENT_BRUSH );
      }
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

    NextBitToDraw += FieldWidth;
  }
  
  if(!BrokeLoop){
    CurrStartBit = IF->GetStartBit(NextName);
    CurrEndBit = IF->GetEndBit(NextName);
  }
    

  //draw any unused space
  if(CurrStartBit != NextBitToDraw && !PrevFieldOverlap){
    dc->SetBrush(* wxYELLOW_BRUSH );
    DrawWidth = (CurrStartBit - NextBitToDraw)*BitPixelWidth;
    dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

    //prepare to draw field
    StartX += DrawWidth;
  }
  
  dc->SetBrush( *wxTRANSPARENT_BRUSH );
  DrawWidth = IF_BOX_PIXEL_WIDTH + InitialX - StartX;
  dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

  //draw endcap if field extends beyond format width
  if(CurrEndBit > FormatWidth){
    StartX += DrawWidth;
    dc->SetBrush( *wxBLACK_BRUSH );
    dc->DrawRectangle( StartX, IF_BOX_TOP, 10, IF_BOX_PIXEL_HEIGHT );
  }
  
}

CoreDrawInstFormat::~CoreDrawInstFormat(){
}

// EOF
