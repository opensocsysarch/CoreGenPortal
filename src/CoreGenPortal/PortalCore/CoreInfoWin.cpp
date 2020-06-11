//
// _COREINFOWIN_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreInfoWin.h"

CoreInfoWin::CoreInfoWin( wxWindow *parent,
                          wxWindowID id,
                          CoreGenNode *Node,
                          CGNodeType Type){
  if(Node) Type = Node->GetType();

  if( Type == CGSoc ){
    CoreSocInfoWin *Win = new CoreSocInfoWin( parent, id,
                                              Node ? Node->GetName() : "New SoC",
                                              static_cast<CoreGenSoC *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGCore ){
    CoreCoreInfoWin *Win = new CoreCoreInfoWin( parent, id,
                                                Node ? Node->GetName() : "New Core",
                                                static_cast<CoreGenCore *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGDPath ){
    CoreDataPathInfoWin *Win = new CoreDataPathInfoWin( parent, id,
                                                Node ? Node->GetName() : "New Datapath",
                                                static_cast<CoreGenDataPath *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGInstF ){

    // Create a new instruction format image
    PortalViz *Viz = new PortalViz();
    std::string Img;  // This string will contain the path to the image

    Viz->GenerateInstFormatImg(static_cast<CoreGenInstFormat *>(Node),Img);

    CoreInstFormatInfoWin *Win = new CoreInstFormatInfoWin(parent, id,
                                                           Node ? Node->GetName() : "New Instruction Format",
                                                           static_cast<CoreGenInstFormat *>(Node));
    Win->ShowModal();
    delete Win;
    delete Viz; // deletes the image and the dot file
  }else if( Type == CGInst ){
    CoreInstInfoWin *Win = new CoreInstInfoWin( parent, id,
                                                Node ? Node->GetName() : "New Instruction",
                                                static_cast<CoreGenInst *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGPInst ){
    CorePInstInfoWin *Win = new CorePInstInfoWin( parent, id,
                                                  Node ? Node->GetName() : "New Pseudo Instruction",
                                                  static_cast<CoreGenPseudoInst *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGRegC ){
    CoreRegClassInfoWin *Win = new CoreRegClassInfoWin( parent, id,
                                                  Node ? Node->GetName() : "New Register Class",
                                                  static_cast<CoreGenRegClass *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGReg ){
    CoreRegInfoWin *Win = new CoreRegInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Register",
                                              static_cast<CoreGenReg *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGISA ){
    CoreISAInfoWin *Win = new CoreISAInfoWin( parent, id,
                                              Node ? Node->GetName() : "New ISA",
                                              static_cast<CoreGenISA *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGCache ){
    CoreCacheInfoWin *Win = new CoreCacheInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Cache",
                                              static_cast<CoreGenCache *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGEnc ){
    CoreEncodingInfoWin *Win = new CoreEncodingInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Encoding",
                                              static_cast<CoreGenEncoding *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGExt ){
    CoreExtInfoWin *Win = new CoreExtInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Extension",
                                              static_cast<CoreGenExt *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGComm ){
    CoreCommInfoWin *Win = new CoreCommInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Comm",
                                              static_cast<CoreGenComm *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGSpad ){
    CoreSpadInfoWin *Win = new CoreSpadInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Scratchpad",
                                              static_cast<CoreGenSpad *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGMCtrl ){
    CoreMCtrlInfoWin *Win = new CoreMCtrlInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Memory Controller",
                                              static_cast<CoreGenMCtrl *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGVTP ){
    CoreVTPInfoWin *Win = new CoreVTPInfoWin( parent, id,
                                              Node ? Node->GetName() : "New VTP",
                                              static_cast<CoreGenVTP *>(Node) );
    Win->ShowModal();
    delete Win;
  }else if( Type == CGPlugin ){
    CorePluginInfoWin *Win = new CorePluginInfoWin( parent, id,
                                              Node ? Node->GetName() : "New Plugin",
                                              static_cast<CoreGenPlugin *>(Node) );
    Win->ShowModal();
    delete Win;
  }

  // TODO: print an error dialog box
}

CoreInfoWin::~CoreInfoWin(){
}

// EOF
