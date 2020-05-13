//
// _SCCompConfig_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _SCCOMPCONFIG_H_
#define _SCCOMPCONFIG_H_

// wxWidgets headers
#include <wx/wxprec.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// portal headers
#include "CoreGenPortal/PortalCore/CoreConsts.h"

// coregen headers
#include "CoreGen/StoneCutter/SCOpts.h"
#include "CoreGen/StoneCutter/SCParser.h"
#include "CoreGen/StoneCutter/SCMsg.h"

// standard C++ headers
#include <string>
#include <algorithm>
#include <tuple>
#include <vector>

// standard C headers
#include <stdlib.h>
#include <stdio.h>

class SCCompConfig{
private:
  // private variables
  bool IsValid;                                         ///< Determines if the configuration is valid
  bool IsPipeline;                                      ///< Determines if the pipeliner is enabled
  bool IsParse;                                         ///< Forces only parsing, no code generation
  bool IsKeep;                                          ///< Keep the intermediate LLVM files
  int OptLevel;                                         ///< Optimization level; -1 is disabled (no opts)
  wxString SigMapFile;                                  ///< User-defined signal map output file
  std::vector<std::pair<std::string,bool>> LLVMPasses;  ///< Vector of LLVM passes
  std::vector<std::pair<std::string,bool>> SCPasses;    ///< Vector of SC passes

  // private functions
  /// Initializes the default compiler options
  void SetupDefaultOptimizations();

public:
  /// Default constructor
  SCCompConfig();

  /// Default destructor
  ~SCCompConfig();

  /// Determine if the configuration is valid
  bool isValid() { return IsValid; }

  /// Set the -O0 optimization level
  void SetO0();

  /// Set the -O1 optimization level
  void SetO1();

  /// Set the -O2 optimization level
  void SetO2();

  /// Set the -O3 optimization level
  void SetO3();

  /// Retrieve the number of LLVM passes
  unsigned GetNumLLVMPasses() { return LLVMPasses.size(); }

  /// Retrieve the number of SC passes
  unsigned GetNumSCPasses() { return SCPasses.size(); }

  /// Retrieve the name of the LLVM Pass
  wxString GetLLVMPassName(unsigned P);

  /// Retrieve whether the target LLVM pass is enabled
  bool IsLLVMPassEnabled(unsigned P);

  /// Retrieve the name of the SC Pass
  wxString GetSCPassName(unsigned P);

  /// Retrieve whether the target sc pass is enabled
  bool IsSCPassEnabled(unsigned P);

  /// Enable the pipeliner
  void EnablePipeliner() { IsPipeline = true; }

  /// Enable the keeping of intermediate files
  void EnableKeep() { IsKeep = true; }

  /// Enable the parsing (but disabling codegen)
  void EnableParse() { IsParse = true; }

  /// Disable the pipeliner
  void DisablePipeliner() { IsPipeline = false; }

  /// Disable the keeping of intermediate files
  void DisableKeep() { IsKeep = false; }

  /// Disable only the parser (full code generation enabled)
  void DisableParse() { IsParse = true; }

  /// Retrieve the optimization level
  int GetOptLevel() { return OptLevel; }

  /// Retrieve whether the pipeliner is enabled
  bool IsPipelineEnabled() { return IsPipeline; }

  /// Retrieve whether we should keep the intermediate files
  bool IsKeepEnabled() { return IsKeep; }

  /// Retrieve whether only parsing is enabled (disabling the codegen)
  bool IsParseEnabled() { return IsParse; }
};

#endif

// EOF
