//
// _PORTALVIZ_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PORTALVIZ_H_
#define _PORTALVIZ_H_

//-- C++ Headers
#include <sys/stat.h>
#include <sys/time.h>
#include <cerrno>
#include <cstdio>
#include <string>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <sstream>
#include <algorithm>

//-- GRAPHVIZ HEADERS
#include <gvc.h>

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

class PortalViz {
private:
  std::string FileName;   ///< Temporary file name
  std::string PNGFile;    ///< Temporary PNG output file name
  std::string DOTFile;    ///< Temporary DOT output file name

  std::vector<std::tuple<std::string,
                         std::string,
                         std::string,
                         unsigned>> PipeStageRow; ///< Pipeline:Stage:Name:Row mapping

  std::vector<std::tuple<std::string,
                         std::string,
                         SCSig *>> NodeToSig; ///< NodeName:Instruction:Signal mapping

  /// creates a temporary file name
  void CreateTmpFile();

  /// deletes the target temporary file
  bool DeleteTmpFile(std::string& name);

  /// retrieve the nodes from the instruction:stage combination
  std::vector<std::string> GetRowNodes(std::string inst, std::string stage);

  /// retrieve all the signals from all instructions that fall into the target stage
  std::vector<std::string> GetLevelNodes(std::string stage);

public:
  /// Default constructor
  PortalViz();

  /// Default destructor
  ~PortalViz();

  /// Generate an image based upon the instruction format input (little endian)
  bool GenerateInstFormatImg(CoreGenInstFormat *InstFormat,
                             std::string &Img);

  /// Generate an image based upon the design dependence graph
  bool GenerateDesignImg(CoreGenBackend *CG,
                         std::string &Img);

  /// Generate an image based upon the target pipeline
  bool GeneratePipeline(CoreGenSigMap *SM,
                        std::string &Img);
};

#endif

// EOF
