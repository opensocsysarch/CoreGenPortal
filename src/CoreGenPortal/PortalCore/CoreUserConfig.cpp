//
// _CoreUserConfig_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGenPortal/PortalCore/CoreUserConfig.h"

CoreUserConfig::CoreUserConfig()
  : isValidConfig(false) {
  wxString OverrideConfig = getenv(PORTAL_ENV_USER_CONFIG.c_str());
  if( OverrideConfig.length() > 0 ){
    ConfFile = OverrideConfig;
  }else{
    wxString LH = getenv("HOME");
    ConfFile = LH + wxT("/") + PORTAL_USER_CONFIG;
  }

  wxString OverridePlugin = getenv(PORTAL_ENV_PLUGIN_CONFIG.c_str());
  if( OverridePlugin.length() > 0 ){
    PluginDir = OverridePlugin;
  }else{
    // derive the plugin directory
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    PluginDir = f.GetPathWithSep() + "../plugins/";
  }
  ReadConfigFile();
}

CoreUserConfig::CoreUserConfig(wxString S)
  : isValidConfig(false), ConfFile(S){
  ReadConfigFile();
}

CoreUserConfig::~CoreUserConfig(){
}

bool CoreUserConfig::BuildDefaultConfigFile(){
  wxTextFile file(ConfFile);
  file.Create();
  file.Open();

  // write all the defaults
  file.AddLine( wxT("#-- OpenSoC System Architect User Configuration File" ) );
  file.AddLine( wxT("PROJECT_DIR = ") + ProjectDir );
  file.AddLine( wxT("ARCHIVE_DIR = /opt/coregen/archive") );

  file.Write();
  file.Close();

  return true;
}

bool CoreUserConfig::WriteConfig(){
  wxRemoveFile(ConfFile);
  wxTextFile file(ConfFile);
  file.Create();
  file.Open();

  // write all the defaults
  file.AddLine( wxT("#-- OpenSoC System Architect User Configuration File" ) );
  file.AddLine( wxT("PROJECT_DIR = ") + ProjectDir );
  file.AddLine( wxT("ARCHIVE_DIR = ") + ArchiveDir );

  file.Write();
  file.Close();

  return true;
}

bool CoreUserConfig::ReadConfigFile(){
  if( ConfFile.length() == 0 ){
    isValidConfig = false;
    return false;
  }

  wxString Home( getenv("HOME") );

  // stage 1: set the defaults
  HomeDir = Home;
  ProjectDir = HomeDir + wxT("/OpenSysArch");

  // stage 2: check to see if the config file exists
  if( !wxFileExists(ConfFile) ){
    // file does not exist, create it
    wxFileName UC(Home+wxT("/")+PORTAL_USER_DIR,"");
    if( !UC.Mkdir( wxS_DIR_DEFAULT, 0 ) )
      return false;
    // directory created, build a default config file
    if( !BuildDefaultConfigFile() )
      return false;
  }

  // stage 3: read all the config defaults
  wxTextFile tfile;
  tfile.Open( ConfFile );
  wxString str = tfile.GetFirstLine();
  int pos = -1;

  do{
    if( (pos = str.Find( wxT("PROJECT_DIR = ") )) != wxNOT_FOUND ){
      ProjectDir = str.Mid( pos+14, wxString::npos );
    }else if( (pos = str.Find( wxT("ARCHIVE_DIR = ") )) != wxNOT_FOUND ){
      ArchiveDir = str.Mid( pos+14, wxString::npos );
    }
    str = tfile.GetNextLine();
  }while( !tfile.Eof() );

  tfile.Close();

  // stage 4: check to see if the project directory exists
  if( !wxDirExists(ProjectDir) ){
    // project directory doesn't exist, create it
    wxFileName PD(ProjectDir,"");
    if( !PD.Mkdir( wxS_DIR_DEFAULT,0 ) )
      return false;
  }

  isValidConfig = true;
  return true;
}

bool CoreUserConfig::SetHomeDir( wxString S ){
  if( S.length() == 0 )
    return false;

  HomeDir = S;
  return true;
}

bool CoreUserConfig::SetConfFile( wxString S ){
  if( S.length() == 0 )
    return false;

  ConfFile = S;
  return ReadConfigFile();
}

bool CoreUserConfig::SetProjectDir( wxString S ){
  if( S.length() == 0 )
    return false;

  ProjectDir = S;
  return true;
}

bool CoreUserConfig::SetArchiveDir( wxString S ){
  if( S.length() == 0 )
    return false;

  ArchiveDir = S;
  return true;
}

// EOF
