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

class PortalViz {
private:
  std::string FileName;   ///< Temporary file name
  std::string PNGFile;    ///< Temporary PNG output file name
  std::string DOTFile;    ///< Temporary DOT output file name

  /// creates a temporary file name
  void CreateTmpFile();

  /// deletes the target temporary file
  bool DeleteTmpFile(std::string& name);

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
};

#endif

// EOF
