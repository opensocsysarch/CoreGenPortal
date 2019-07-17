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
#include <wx/dir.h>
#include <wx/dirctrl.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/process.h>
#include <wx/filefn.h>
#include <wx/txtstrm.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/stc/stc.h>
#include <wx/filedlg.h>
#include <wx/treelist.h>
#include <wx/treectrl.h>
#include <wx/textctrl.h>

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalConsts.h"
#include "CoreGenPortal/PortalVerifPrefWin.h"
#include "CoreGenPortal/PortalUserPrefWin.h"
#include "CoreGenPortal/PortalNewProjWin.h"
#include "CoreGenPortal/PortalCore/CoreErrorDiag.h"
#include "CoreGenPortal/PortalCore/CoreStoneCutterEditor.h"
#include "CoreGenPortal/PortalCore/CoreUserConfig.h"
#include "CoreGenPortal/PortalCore/CoreVerifConfig.h"
#include "CoreGenPortal/PortalCore/CoreConsts.h"
#include "CoreGenPortal/PortalCore/CoreInfoWin.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

//-- STANDARD HEADERS
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PortalMainFrame: public wxFrame{
public:
  PortalMainFrame( const wxString& title,
                   const wxPoint& pos,
                   const wxSize& size );
  ~PortalMainFrame();

  // node info update handlers
  void OnPressEnter(wxCommandEvent &event, CoreGenNode *node, int InfoWinType);

private:
  // CoreGenHandlers
  CoreGenBackend *CGProject;

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
  wxTreeCtrl *ModuleBox;
  wxListBox *PluginBox;
  wxGenericDirCtrl *ProjDir;

  wxAuiNotebook* EditorNotebook;
  wxStyledTextCtrl *IRPane;
  std::vector<std::pair<wxStyledTextCtrl *,wxString>> SCPanes;
  std::vector<std::pair<wxString,wxString>> PluginPanes;

  wxString IRFileName;

  wxTreeItemId ParentModule;
  std::vector<wxTreeItemId> TreeItems;
  std::vector<std::pair<wxTreeItemId,CoreGenNode *>> NodeItems;
  std::vector<std::pair<wxTreeItemId,CoreGenEncoding *>> EncItems;
  std::vector<std::tuple<wxTreeItemId,CoreGenExt *,CoreGenNode *>> ExtItems;
  std::vector<std::tuple<wxTreeItemId,CoreGenPlugin *,CoreGenNode *>> PluginItems;

  // private functions
  void InitAuiMgr();
  void UpdateAuiMgr();
  void CreateMenuBar();
  void CreatePortalToolBar();
  void CreateWindowLayout();
  void SetupModuleBox();
  void SetupPluginBox();
  void LoadModuleBox();
  void LoadExtNodes(wxTreeItemId, CoreGenExt *Ext);
  void LoadPluginNodes(wxTreeItemId, CoreGenPlugin *Plugin);
  void LoadInstEncodings(wxTreeItemId, CoreGenInst *Inst);
  void LoadPInstEncodings(wxTreeItemId, CoreGenPseudoInst *PInst);
  void CloseProject();
  void DeleteNode(CoreGenNode *N);
  void AddNodeWin();
  void OpenFileFromWin(wxString Path);
  void OpenSCFile(wxString NP, wxFileName NPF);

  wxString FindNodeStr(CoreGenNode *Parent);
  CoreGenNode *GetNodeFromItem(wxTreeItemId Id);

  // menu handlers
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnVerifPref(wxCommandEvent& event);
  void OnUserPref(wxCommandEvent& event);
  void OnProjNew(wxCommandEvent& event);
  void OnProjOpen(wxCommandEvent& event);
  void OnProjSCOpen(wxCommandEvent& event);
  void OnProjClose(wxCommandEvent& event);
  void OnBuildVerify(wxCommandEvent& event);
  void OnSelectPlugin(wxCommandEvent& event);
  void OnSelectNode(wxTreeEvent& event);
  void OnRightClickNode(wxTreeEvent& event);
  void OnMiddleClickNode(wxTreeEvent& event);
  void OnPopupNode(wxCommandEvent &event);
};

enum
{
  MARGIN_LINE_NUMBERS,
  ID_PROJNEW            = 10,
  ID_PROJSCOPEN         = 11,
  ID_BUILD_VERIFY       = 20,
  ID_BUILD_CODEGEN      = 21,
  ID_BUILD_SIGMAP       = 22,
  ID_BUILD_STONECUTTER  = 23,
  ID_BUILD_VERILOG      = 24,
  ID_BUILD_COMPILER     = 25,
  ID_BUILD_SIM          = 26,
  ID_BUILD_ALL          = 27,
  ID_COMPILE_SIM        = 30,
  ID_COMPILE_COMPILER   = 31,
  ID_COMPILE_ALL        = 32,
  ID_PREF_USER          = 40,
  ID_PREF_VERIF         = 41,
  ID_PREF_STONECUTTER   = 42,
  ID_TREE_SELECTNODE    = 50,
  ID_TREE_INFONODE      = 51,
  ID_TREE_ADDNODE       = 52,
  ID_TREE_DELNODE       = 53
};

enum{
  TREE_NODE_CACHE       = 0,
  TREE_NODE_COMM        = 1,
  TREE_NODE_CORE        = 2,
  TREE_NODE_EXT         = 3,
  TREE_NODE_ISA         = 4,
  TREE_NODE_INST        = 5,
  TREE_NODE_INSTFORMAT  = 6,
  TREE_NODE_MCTRL       = 7,
  TREE_NODE_PLUGIN      = 8,
  TREE_NODE_PSEUDOINST  = 9,
  TREE_NODE_REG         = 10,
  TREE_NODE_REGCLASS    = 11,
  TREE_NODE_SOC         = 12,
  TREE_NODE_SPAD        = 13,
  TREE_NODE_VTP         = 14
};

enum{
  TREE_EXT_NODE_CACHE       = 0,
  TREE_EXT_NODE_COMM        = 1,
  TREE_EXT_NODE_CORE        = 2,
  TREE_EXT_NODE_EXT         = 3,
  TREE_EXT_NODE_ISA         = 4,
  TREE_EXT_NODE_INST        = 5,
  TREE_EXT_NODE_PINST       = 6,
  TREE_EXT_NODE_INSTFORMAT  = 7,
  TREE_EXT_NODE_MCTRL       = 8,
  TREE_EXT_NODE_REG         = 9,
  TREE_EXT_NODE_REGCLASS    = 10,
  TREE_EXT_NODE_SPAD        = 11
};

enum{
  TREE_PLUGIN_NODE_CACHE      = 0,
  TREE_PLUGIN_NODE_CORE       = 1,
  TREE_PLUGIN_NODE_INST       = 2,
  TREE_PLUGIN_NODE_PINST      = 3,
  TREE_PLUGIN_NODE_INSTFORMAT = 4,
  TREE_PLUGIN_NODE_REG        = 5,
  TREE_PLUGIN_NODE_REGCLASS   = 6,
  TREE_PLUGIN_NODE_SOC        = 7,
  TREE_PLUGIN_NODE_ISA        = 8,
  TREE_PLUGIN_NODE_EXT        = 9,
  TREE_PLUGIN_NODE_COMM       = 10,
  TREE_PLUGIN_NODE_SPAD       = 11,
  TREE_PLUGIN_NODE_MCTRL      = 12,
  TREE_PLUGIN_NODE_VTP        = 13

};

#endif

// EOF
