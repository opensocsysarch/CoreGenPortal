//
// _CoreUserConfig_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREUSERCONFIG_H_
#define _COREUSERCONFIG_H_

// wxWidgets headers
#include <wx/wxprec.h>
#include <wx/file.h>
#include <wx/utils.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// portal headers
#include "CoreGenPortal/PortalCore/CoreConsts.h"

// standard C++ headers
#include <string>

// standard C headers
#include <stdlib.h>
#include <stdio.h>

class CoreUserConfig{
private:
  bool isValidConfig;           ///< Is this configuration valid?
  wxString HomeDir;             ///< User's home directory
  wxString ConfFile;            ///< Configuration file path
  wxString ProjectDir;          ///< Top-level project directory
  wxString ArchiveDir;          ///< Archive directory
  wxString PluginDir;           ///< Plugin directory

  // private functions

  /// Reads the configuration data from ConfFile
  bool ReadConfigFile();

  /// Builds a default user configuration file
  bool BuildDefaultConfigFile();

public:
  /// Default constructor
  CoreUserConfig();

  /// Overloaded constructor
  CoreUserConfig(wxString Conf);

  /// Default destructor
  ~CoreUserConfig();

  /// Determine if the configuration data was successfully validated
  bool isValid() { return isValidConfig; }

  /// Retrieve the user's home directory as a wxString
  wxString wxGetHomeDir() { return HomeDir; }

  /// Retrieve the user's home directory as a std::string
  std::string GetHomeDir() { return std::string(HomeDir.mb_str()); }

  /// Retrieve the plugin directory as a wxString
  wxString wxGetPluginDir() { return PluginDir; }

  /// Retrieve the plugin directory as a std::string
  std::string GetPluginDir() { return std::string(PluginDir.mb_str()); }

  /// Retrieve the user's configuration file as a wxString
  wxString wxGetConfFile() { return ConfFile; }

  /// Retrieve the user's configuration file as a std::string
  std::string GetConfFile() { return std::string(ConfFile.mb_str()); }

  /// Retrieve the user's top-level project directory as a wxString
  wxString wxGetProjectDir() { return ProjectDir; }

  /// Retrieve the user's top-level project directory as a std::string
  std::string GetProjectDir() { return std::string(ProjectDir.mb_str()); }

  /// Retrieve the user's archive directory as a wxString
  wxString wxGetArchiveDir() { return ArchiveDir; }

  /// Retrieve the user's archive directory as a std::string
  std::string GetArchiveDir() { return std::string(ArchiveDir.mb_str()); }

  /// Sets the user's home directory
  bool SetHomeDir( wxString S );

  /// Sets the user's configuration file
  bool SetConfFile( wxString S );

  /// Sets the user's project directory
  bool SetProjectDir( wxString S );

  /// Sets the user's archive directory
  bool SetArchiveDir( wxString S );

  /// Writes the configuration out to the file
  bool WriteConfig();
};

#endif

// EOF
