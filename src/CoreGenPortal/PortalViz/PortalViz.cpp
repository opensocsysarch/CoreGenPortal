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

std::vector<std::string> PortalViz::GetRowNodes(std::string inst,
                                                std::string stage){
  std::vector<std::string> NodeVect;

  for( const auto &i : NodeToSig ){
    if( (std::get<1>(i) == inst) &&
        (std::get<2>(i)->GetPipeName() == stage) ){
      NodeVect.push_back(std::get<0>(i));
    }
  }

  return NodeVect;
}

std::vector<std::string> PortalViz::GetLevelNodes(std::string stage){
  std::vector<std::string> NodeVect;
  for( const auto &i : NodeToSig ){
    if( std::get<2>(i)->GetPipeName() == stage) {
      NodeVect.push_back(std::get<0>(i));
    }
  }
  return NodeVect;
}

bool PortalViz::GeneratePipeline(CoreGenSigMap *SM,
                                 std::string &Img){

  // Stage 1: Ensure that the signal map is valid
  if( !SM )
    return false;

  // Stage 2: Determine how to format our matrix
  std::ofstream dofs;
  dofs.open(DOTFile.c_str(), std::ofstream::out | std::ofstream::app);
  if( !dofs.is_open() )
    return false;

  std::vector<std::string> Pipelines = SM->GetPipelines();

  // -- write out all the top-level node info
  dofs << "digraph matrix {" << std::endl;
  dofs << "node [shape=box]" << std::endl;

  // write out the "Stages" block
  dofs << "Mt[ label = \"Stages\", width = 1.5, style = filled, fillcolor = firebrick1, group = 1 ];" << std::endl;

  // write out the Pipeline blcoks
  for( unsigned i=0; i<Pipelines.size(); i++ ){
    dofs << "P" << std::to_string(i) << " [label = \"" << Pipelines[i]
         << "\" width = 1.5 style = filled, fillcolor = lightskyblue, group = "
         << std::to_string(i+2) << " ];" << std::endl;
  }

  // write out all the instructions
  std::vector<std::string> InstVect = SM->GetInstVect();
  unsigned base = Pipelines.size() + 2;
  for( unsigned i=0; i<InstVect.size(); i++ ){
    dofs << "N" << std::to_string(i)
         << "[label = \"" << InstVect[i]
         << "\" width = 1.5 style = filled, fillcolor = grey, group = "
         << std::to_string(base+i) << " ];"
         << std::endl;
  }

  // write out the node rankings for the column headers
  dofs << "{ rank = same; Mt; ";
  for( unsigned i=0; i<Pipelines.size(); i++ ){
    dofs << "P" << std::to_string(i) << "; ";
  }
  for( unsigned i=0; i<InstVect.size(); i++ ){
    dofs << "N" << std::to_string(i) << "; ";
  }
  dofs << "}" << std::endl;

  // link all the initial nodes
  std::string Last = "Mt";
  for( unsigned i=0; i<Pipelines.size(); i++ ){
    dofs << Last << "->P" << std::to_string(i) << ";" << std::endl;
    Last = "P" + std::to_string(i);
  }
  for( unsigned i=0; i<InstVect.size(); i++ ){
    dofs << Last << "->N" << std::to_string(i) << ";" << std::endl;
    Last = "N" + std::to_string(i);
  }

  // write out all the stage blocks
  base = 2;
  unsigned row = 0;
  for( unsigned i=0; i<Pipelines.size(); i++ ){
    for( unsigned j=0; j<SM->GetNumPipeStages(Pipelines[i]); j++ ){
      std::string Name = "P" + std::to_string(i) + "_" + std::to_string(j);
      dofs << Name
           << "[label = \"" << SM->GetPipelineStage(Pipelines[i],j)
           << "\" width = 1.5 style = filled, fillcolor = green, group = "
           << std::to_string(base) << " ];" << std::endl;

      // save off the row placement
      PipeStageRow.push_back(std::make_tuple(Pipelines[i],
                                             SM->GetPipelineStage(Pipelines[i],j),
                                             Name,
                                             row));
      row++;
    }

    // setup the stage node linkage
    Last = "P" + std::to_string(i);
    for( unsigned j=0; j<SM->GetNumPipeStages(Pipelines[i]); j++ ){
      dofs << Last << "->P" << std::to_string(i)
           << "_" << std::to_string(j) << ";" << std::endl;
      Last = "P" +  std::to_string(i) + "_" + std::to_string(j);
    }
    base++;
  }

  // draw the stage boxes for each instruction
  base = Pipelines.size() + 2;
  std::string Name;
  for( unsigned i=0; i<InstVect.size(); i++ ){
    std::vector<SCSig *> SV = SM->GetSigVect(InstVect[i]);
    for( unsigned j=0; j<SV.size(); j++ ){
      Name = "N" + std::to_string(i) + "_" + std::to_string(j);
      dofs << Name
           << "[label = \"" << SV[j]->GetName()
           << "\" width = 1.5 style = filled, fillcolor = white, group = "
           << std::to_string(base+i) << " ];"
           << std::endl;
      NodeToSig.push_back(std::make_tuple(Name,
                                          InstVect[i],
                                          SV[j]));
    }
  }

  // link the stages together
  std::vector<std::string> Prev;
  std::vector<std::string> Cur;
  for( unsigned i=0; i<InstVect.size(); i++ ){
    Prev.clear();
    Cur.clear();
    Prev.push_back("N" + std::to_string(i));
    for( const auto &j : PipeStageRow ){
      // retrieve all the nodes that belong to this instruction + pipe stage
      Cur = GetRowNodes(InstVect[i],std::get<1>(j));
      bool swap = false;

      // map the Prev nodes to Cur nodes
      for( unsigned a=0; a<Prev.size(); a++ ){
        for( unsigned b=0; b<Cur.size(); b++ ){
          dofs << Prev[a] << "->" << Cur[b] << ";" << std::endl;
          swap = true;
        }
      }
      if( swap ){
        Prev.clear();
        Prev = Cur;
        Cur.clear();
      }
    }
  }

  // organize all the stages into rows
  for( const auto &i : PipeStageRow ){
  //  if( (std::get<0>(i) == pipeline) &&
    std::vector<std::string> RowVect;
    std::vector<std::string> TmpRowVect;
    RowVect.push_back(std::get<2>(i));  // push back the stage name
    TmpRowVect = GetLevelNodes(std::get<2>(i));

    RowVect.insert(RowVect.begin(),TmpRowVect.begin(),TmpRowVect.end());

    dofs << "{ rank = same; ";
    for( auto i : RowVect ){
      dofs << i << ";";
    }
    dofs << "}" << std::endl;
  }

  dofs << "}" << std::endl;

  dofs.close();

  // Stage 3: Run graphviz against the dot input file
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
