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

  StartX = Width/2 - IF_BOX_PIXEL_WIDTH/2;
  InitialX = StartX;
  BitPixelWidth = IF_BOX_PIXEL_WIDTH/FormatWidth;

  dc->SetPen( wxPen( *wxBLACK, 3 ) );
  dc->SetAxisOrientation(false, false);
  dc->SetDeviceOrigin( Width, 0 );
  std::string CurrName;
  std::string NextName;
  std::string FieldRange;
  unsigned DrawWidth;
  unsigned CurrStartBit;
  unsigned CurrEndBit;
  unsigned NextStartBit;
  unsigned FieldWidth;
  unsigned OverlapWidth;
  unsigned NextBitToDraw = 0;
  bool PrevFieldOverlap = false;
  bool BrokeLoop = false;
  wxCoord w, h;
  
  drawKey(dc, Width);

  for(unsigned i = 1; i < IF->GetNumFields(); i++){
    //gather information needed to draw current field
    CurrName = IF->GetFieldName(i-1);
    NextName = IF->GetFieldName(i);
    CurrStartBit = IF->GetStartBit(CurrName);
    CurrEndBit = IF->GetEndBit(CurrName);
    FieldRange = std::to_string(CurrEndBit) + " - " + std::to_string(CurrStartBit);
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

      //draw field name
      dc->GetTextExtent(CurrName, &w, &h);
      wxCoord XOffset = StartX + DrawWidth/2 + w/2;
      wxCoord YOffset = IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT/2 - h/2;
      dc->DrawText(CurrName, XOffset, YOffset);

      //draw field range
      dc->GetTextExtent(FieldRange, &w, &h);
      XOffset = StartX + DrawWidth/2 + w/2;
      YOffset = IF_BOX_TOP - 5 - h;
      dc->DrawText(FieldRange, XOffset, YOffset);

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

      //draw field name
      dc->GetTextExtent(CurrName, &w, &h);
      wxCoord XOffset = StartX + DrawWidth/2 + w/2;
      wxCoord YOffset = IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT/2 - h/2;
      dc->DrawText(CurrName, XOffset, YOffset);

      //draw field range
      dc->GetTextExtent(FieldRange, &w, &h);
      XOffset = StartX + DrawWidth/2 + w/2;
      YOffset = IF_BOX_TOP - 5 - h;
      dc->DrawText(FieldRange, XOffset, YOffset);

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
  
  //draw last field
  dc->SetBrush( *wxTRANSPARENT_BRUSH );
  DrawWidth = IF_BOX_PIXEL_WIDTH + InitialX - StartX;
  dc->DrawRectangle( StartX, IF_BOX_TOP, DrawWidth, IF_BOX_PIXEL_HEIGHT );

  //draw field name
  CurrName = IF->GetFieldName(IF->GetNumFields()-1);
  dc->GetTextExtent(CurrName, &w, &h);
  wxCoord XOffset = StartX + DrawWidth/2 + w/2;
  wxCoord YOffset = IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT/2 - h/2;
  dc->DrawText(CurrName, XOffset, YOffset);

  //draw field range
  CurrStartBit = IF->GetStartBit(CurrName);
  CurrEndBit = IF->GetEndBit(CurrName);
  FieldRange = std::to_string(CurrEndBit) + " - " + std::to_string(CurrStartBit);
  dc->GetTextExtent(FieldRange, &w, &h);
  XOffset = StartX + DrawWidth/2 + w/2;
  YOffset = IF_BOX_TOP - 5 - h;
  dc->DrawText(FieldRange, XOffset, YOffset);

  //draw endcap if field extends beyond format width
  if(CurrEndBit > FormatWidth){
    StartX += DrawWidth;
    dc->SetBrush( *wxBLACK_BRUSH );
    dc->DrawRectangle( StartX, IF_BOX_TOP, 10, IF_BOX_PIXEL_HEIGHT );
  }
}

void CoreDrawInstFormat::drawKey(wxPaintDC *dc, wxCoord Width){
  std::string UnusedLabel = "Unused Bits";
  std::string OverlapLabel = "Overlapping Bits";
  wxCoord uLabelWidth;
  wxCoord uLabelHeight;
  wxCoord oLabelWidth;
  wxCoord oLabelHeight;
  wxCoord KeyWidth;
  wxCoord UnusedBoxX;
  wxCoord UnusedLabelX;
  wxCoord OverlapBoxX;
  wxCoord OverlapLabelX;

  //get x coords for all key elements
  dc->GetTextExtent(UnusedLabel, &uLabelWidth, &uLabelHeight);
  dc->GetTextExtent(OverlapLabel, &oLabelWidth, &oLabelHeight);
  KeyWidth = uLabelWidth + oLabelWidth + 40;
  UnusedBoxX = Width/2 + KeyWidth/2 - 10;
  UnusedLabelX = UnusedBoxX - 5;
  OverlapBoxX = UnusedLabelX - uLabelWidth - 20;
  OverlapLabelX = OverlapBoxX - 5;

  //draw unused key
  dc->SetBrush(* wxYELLOW_BRUSH );
  dc->DrawRectangle(UnusedBoxX, IF_KEY_TOP, 10, 10);
  dc->DrawText(UnusedLabel, UnusedLabelX, IF_KEY_TOP + 5 - uLabelHeight/2);
  
  //draw overlap key
  dc->SetBrush(* wxRED_BRUSH );
  dc->DrawRectangle(OverlapBoxX, IF_KEY_TOP, 10, 10);
  dc->DrawText(OverlapLabel, OverlapLabelX, IF_KEY_TOP + 5 - oLabelHeight/2);
}

CoreDrawInstFormat::~CoreDrawInstFormat(){
}

// EOF
