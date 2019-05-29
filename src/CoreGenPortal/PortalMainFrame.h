//
// _PORTALMAINFRAME_H_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _PORTALMAINFRAME_H_
#define _PORTALMAINFRAME_H_

//-- WX HEADERS
#include <wx/wxprec.h>
#include <wx/splash.h>
#include <wx/artprov.h>
#include <wx/aui/auibook.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dirctrl.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/process.h>
#include <wx/filefn.h>
#include <wx/txtstrm.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/stc/stc.h>

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalConsts.h"
#include "CoreGenPortal/PortalVerifPrefWin.h"
#include "CoreGenPortal/PortalUserPrefWin.h"
#include "CoreGenPortal/PortalCore/CoreErrorDiag.h"
#include "CoreGenPortal/PortalCore/CoreStoneCutterEditor.h"
#include "CoreGenPortal/PortalCore/CoreUserConfig.h"
#include "CoreGenPortal/PortalCore/CoreVerifConfig.h"
#include "CoreGenPortal/PortalCore/CoreConsts.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

#include <stdlib.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PortalMainFrame: public wxFrame{
public:
  PortalMainFrame( const wxString& title,
                   const wxPoint& pos,
                   const wxSize& size );
  ~PortalMainFrame();

private:
  // data handlers
  CoreUserConfig *UserConfig;
  CoreVerifConfig *VerifConfig;

  // top level manager
  wxAuiManager Mgr;

  // menu bar items
  wxMenuBar *MenuBar;
  wxMenu *FileMenu;
  wxMenu *EditMenu;
  wxMenu *PrefMenu;
  wxMenu *ProjectMenu;
  wxMenu *BuildMenu;
  wxMenu *PluginMenu;
  wxMenu *HelpMenu;

  // tool bar items
  wxToolBar *ToolBar;

  // layout items
  wxTextCtrl *LogPane;
  wxAuiNotebook* ModulesNotebook;
  wxListBox *ModuleBox;
  wxListBox *PluginBox;
  wxGenericDirCtrl *ProjDir;

  wxAuiNotebook* EditorNotebook;
  wxStyledTextCtrl *IRPane;

  // private functions
  void InitAuiMgr();
  void UpdateAuiMgr();
  void CreateMenuBar();
  void CreatePortalToolBar();
  void CreateWindowLayout();

  void CloseProject();

  // menu handlers
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnVerifPref(wxCommandEvent& event);
  void OnUserPref(wxCommandEvent& event);
};

enum
{
  MARGIN_LINE_NUMBERS,
  ID_PROJNEW            = 10,
  ID_BUILD_VERIFY       = 20,
  ID_BUILD_CODEGEN      = 21,
  ID_BUILD_SIGMAP       = 22,
  ID_BUILD_STONECUTTER  = 23,
  ID_BUILD_VERILOG      = 24,
  ID_BUILD_COMPILER     = 25,
  ID_BUILD_SIM          = 26,
  ID_COMPILE_SIM        = 30,
  ID_COMPILE_COMPILER   = 31,
  ID_PREF_USER          = 40,
  ID_PREF_VERIF         = 41,
  ID_PREF_STONECUTTER   = 42
};

#endif

// EOF
