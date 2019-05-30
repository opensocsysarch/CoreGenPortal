//
// _CoreVerifConfig_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREVERIFCONFIG_H_
#define _COREVERIFCONFIG_H_

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
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

// standard C++ headers
#include <string>
#include <algorithm>
#include <tuple>
#include <vector>

// standard C headers
#include <stdlib.h>
#include <stdio.h>

class CoreVerifConfig{
private:
  // private variables
  bool IsValid;                                     ///< Determines if the config is valid
  std::vector<std::pair<std::string,bool>> Passes;  ///< Vector of verification passes

  // private functions
  /// Initializes the base pass list
  void SetupPasses();

public:
  /// Default constructor
  CoreVerifConfig();

  /// Overloaded constructor
  CoreVerifConfig(wxString Conf);

  /// Default destructor
  ~CoreVerifConfig();

  /// Retrieve the number of passes
  unsigned GetNumPasses() { return Passes.size(); }

  /// Retrieve the target pass name as a wxString
  wxString GetPassName( unsigned i );

  /// Determine whether the target pass is enabled
  bool IsPassEnabled( unsigned i );

  /// Determine whether the pass is enabled
  bool IsPassEnabled( wxString P );

  /// Determine if the config is valid
  bool isValid() { return IsValid; }

  /// Set the target pass to be enabled
  bool EnablePass( unsigned i );

  /// Set the target pass to be enabled
  bool EnablePass( wxString P );

  /// Set the target pass to be disabled
  bool DisablePass( unsigned i );

  /// Set the target pass to be disabled
  bool DisablePass( wxString P );
};

#endif

// EOF
