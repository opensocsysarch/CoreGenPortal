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
  EVT_LEFT_DOWN(CoreDrawInstFormat::handleLeftClick)
wxEND_EVENT_TABLE()

CoreDrawInstFormat::CoreDrawInstFormat(wxWindow *parent,
                                       wxWindowID id,
                                       const wxString& title,
                                       CoreGenInstFormat *IF ) :
  wxPanel(parent, id, wxDefaultPosition, wxDefaultSize,
          wxTAB_TRAVERSAL, title), IF(IF){
    dc = NULL;
    DisplayBoundaries = false;
}

void CoreDrawInstFormat::paintEvent(wxPaintEvent& evt){
  this->paintNow();
}

void CoreDrawInstFormat::paintNow(){
  if(!dc) dc = new wxPaintDC(this);;
  dc->SetBrush( *wxTRANSPARENT_BRUSH );
  dc->Clear();
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

  dc->SetPen( wxPen( *wxBLACK, 4 ) );
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
  
  drawKey(Width);

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

void CoreDrawInstFormat::drawKey(wxCoord Width){
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

void CoreDrawInstFormat::handleLeftClick(wxMouseEvent& mevt){
  wxPoint Loc = mevt.GetLogicalPosition(*dc);
  wxCoord Width;
  wxCoord Height;
  wxCoord StartX;
  unsigned BitPixelWidth;
  unsigned FormatWidth = IF->GetFormatWidth();

  dc->GetSize(&Width, &Height);
  StartX = Width/2 - IF_BOX_PIXEL_WIDTH/2;
  BitPixelWidth = IF_BOX_PIXEL_WIDTH/FormatWidth;

  //handle when boundaries are being displayed
  if(DisplayBoundaries){
    //toggle and clear bit boundary markers
    int selectedButton = getSelectedButton(Loc, StartX);
    if(boundedFields.size() > 1 && selectedButton){
      drawSelectButtons(StartX, selectedButton);
    }
    else{
      DisplayBoundaries = false;
      int bit = pixelToBit(Loc, StartX, BitPixelWidth);
      if(bit > -1){
        if(boundedFields.size() == 1){
          if(setStartOrEndBit == "start"){
            IF->SetStartBit(boundedFields[0], bit);
          }
          if(setStartOrEndBit == "end"){
            IF->SetEndBit(boundedFields[0], bit);
          }
        }
        else{
          if(currButton == 1){
            IF->SetStartBit(boundedFields[0], bit);
            IF->SetEndBit(boundedFields[1], bit-1);
          }
          if(currButton == 2){
            IF->SetStartBit(boundedFields[0], bit);
          }
          if(currButton == 3){
            IF->SetEndBit(boundedFields[1], bit-1);
          }
        }
      }
      this->boundedFields.clear();
      this->paintNow();
    }
  }
  //handle when boundaries are not being displayed
  else if(clickedBoundary(Loc, StartX, BitPixelWidth)){
    DisplayBoundaries = true;
    
    //draw bit boundaries
    dc->SetPen( wxPen(*wxCYAN, 4) );
    
    for(int i = StartX; i <= StartX + IF_BOX_PIXEL_WIDTH; i += BitPixelWidth){
      if(Loc.x < i+2 && Loc.x > i-2 && Loc.y > IF_BOX_TOP && Loc.y < IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT) 
        dc->SetPen( wxPen(*wxBLUE, 4));
      dc->DrawLine(i, IF_BOX_TOP, i, IF_BOX_PIXEL_HEIGHT + IF_BOX_TOP);
      dc->SetPen( wxPen(*wxCYAN, 4));
    }
  }
}

bool CoreDrawInstFormat::clickedBoundary(wxPoint Loc, wxCoord StartX, unsigned BitPixelWidth){
  unsigned clickedBit = pixelToBit(Loc, StartX, BitPixelWidth);
  boundedFields.clear();
  bool clickedBoundary = false;
  for(unsigned i = 0; i < IF->GetNumFields(); i++){
    std::string fieldName = IF->GetFieldName(i);
    unsigned startBit = IF->GetStartBit(fieldName);
    unsigned endBit = IF->GetEndBit(fieldName);
    if(startBit == clickedBit && (Loc.y > IF_BOX_TOP && Loc.y < IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT)){
      clickedBoundary = true;
      setStartOrEndBit = "start";
      boundedFields.insert(boundedFields.begin(), fieldName);
    }
    if( endBit + 1 == clickedBit && (Loc.y > IF_BOX_TOP && Loc.y < IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT)){
      clickedBoundary = true;
      setStartOrEndBit == "end";
      boundedFields.push_back(fieldName);
    }
  }
  if(boundedFields.size() > 1){
    drawSelectButtons(StartX, 1);
  }
  return clickedBoundary;
}

void CoreDrawInstFormat::drawSelectButtons(wxCoord StartX, int selectedBox){
  currButton = selectedBox;
  drawSelectButton("Both", StartX, selectedBox == 1 ? wxBLUE : wxBLACK);
  drawSelectButton(boundedFields[0], StartX + 110, selectedBox == 2 ? wxBLUE: wxBLACK);
  drawSelectButton(boundedFields[1], StartX + 220, selectedBox == 3 ? wxBLUE : wxBLACK);
}

void CoreDrawInstFormat::drawSelectButton(std::string Label, wxCoord StartX, const wxColour *Colour){
  int buttonTop = IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT + 20;
  dc->SetPen( wxPen( *Colour, 4 ));
  wxCoord w, h;
  dc->GetTextExtent(Label, &w, &h);
  dc->DrawRectangle(StartX, buttonTop, 100, IF_BOX_PIXEL_HEIGHT);
  dc->DrawText(Label, StartX + 50 + w/2, buttonTop + IF_BOX_PIXEL_HEIGHT/2 - h/2 - 2);
}

unsigned CoreDrawInstFormat::pixelToBit(wxPoint Loc, wxCoord StartX, unsigned BitPixelWidth){
  if(Loc.y < IF_BOX_TOP || Loc.y > IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT) return -1;
  for(unsigned i = 0; i <= IF->GetFormatWidth(); i++){
    int lineCenter = i*BitPixelWidth + StartX;
    if(Loc.x < lineCenter+2 && Loc.x > lineCenter-2) return i;
  }
  return -1;
}

int CoreDrawInstFormat::getSelectedButton(wxPoint ClickedPoint, wxCoord StartX){
  int buttonTop = IF_BOX_TOP + IF_BOX_PIXEL_HEIGHT + 20;
  if(ClickedPoint.y > buttonTop && ClickedPoint.y < buttonTop + IF_BOX_PIXEL_HEIGHT){
    if(ClickedPoint.x > StartX && ClickedPoint.x < StartX + 100) return 1;
    if(ClickedPoint.x > StartX + 110 && ClickedPoint.x < StartX + 210) return 2;
    if(ClickedPoint.x > StartX + 220 && ClickedPoint.x < StartX + 320) return 3;
  }
  return 0;
}

CoreDrawInstFormat::~CoreDrawInstFormat(){
  delete dc;
}

// EOF
