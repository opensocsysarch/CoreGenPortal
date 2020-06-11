//
// _PORTALVIZ_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalViz/PortalViz.h"

PortalViz::PortalViz(){
  CreateTmpFile();
}

PortalViz::~PortalViz(){
  if( PNGFile.length() > 0 )
    DeleteTmpFile(PNGFile);
  if( DOTFile.length() > 0 )
    DeleteTmpFile(DOTFile);
}

void PortalViz::CreateTmpFile(){
  //FileName = std::tmpnam(nullptr);
  FileName = "/tmp/CoreGenPortal" + std::to_string(rand());
  PNGFile = FileName + ".png";
  DOTFile = FileName + ".dot";
}

bool PortalViz::DeleteTmpFile(std::string &name){
  if( remove(name.c_str()) != 0 ){
    return false;
  }else{
    return true;
  }
}

bool PortalViz::GenerateInstFormatImg(CoreGenInstFormat *IF,
                                      std::string &Img){
  // Stage 1: ensure the data is good
  if( !IF )
    return false;

  // Stage 2: open the file
  std::ofstream dofs;
  dofs.open(DOTFile.c_str(), std::ofstream::out | std::ofstream::app);
  if( !dofs.is_open() )
    return false;

  // Stage 2: Walk the instruction format fields in little-endian order
  dofs << "digraph struct {" << std::endl;
  dofs << "\tnode[shape=record];" << std::endl;
  unsigned NumFields = IF->GetNumFields();
  for( signed j=((signed)(NumFields)-1); j>=0; j-- ){
    if( j==((signed)(NumFields)-1) ){
      dofs << "struct1 [label=\"";
    }else{
      dofs << "|";
    }
    dofs << IF->GetFieldName(j) << "\\n["
        << std::to_string(IF->GetEndBit(IF->GetFieldName(j))) << ":"
        << std::to_string(IF->GetStartBit(IF->GetFieldName(j))) << "]";
  }
  dofs << "\"];" << std::endl;
  dofs << "}" << std::endl;

  // Stage 3: Close the file
  dofs.close();

  // Stage 4: Run graphviz against the dot input file
  GVC_t *gvc;
  Agraph_t *g;
  FILE *fp;
  gvc = gvContext();
  fp = fopen(DOTFile.c_str(), "r");
  g = agread(fp, 0);
  gvLayout(gvc, g, "dot");
  gvRender(gvc, g, "png", fopen(PNGFile.c_str(), "w"));
  gvFreeLayout(gvc, g);
  agclose(g);
  gvFreeContext(gvc);

  // Stage 5: Return the image
  Img = PNGFile;
  return true;
}

bool PortalViz::GenerateDesignImg(CoreGenBackend *CG,
                                  std::string &Img){

  // Stage 1: Ensure that CG is valid
  if( !CG )
    return false;

  // Stage 2: Use the existing infrastructure to build the DAG and generate
  //          the dot file using our tmp file as the target
  if( !CG->BuildDAG() )
    return false;
  if( !CG->ExportDOT(DOTFile) )
    return false;

  // Starge 3: Run graphviz against the dot input file
  GVC_t *gvc;
  Agraph_t *g;
  FILE *fp;
  gvc = gvContext();
  fp = fopen(DOTFile.c_str(), "r");
  g = agread(fp, 0);
  gvLayout(gvc, g, "dot");
  gvRender(gvc, g, "png", fopen(PNGFile.c_str(), "w"));
  gvFreeLayout(gvc, g);
  agclose(g);
  gvFreeContext(gvc);

  // Stage 4: Return the image
  Img = PNGFile;

  return true;
}

// EOF
