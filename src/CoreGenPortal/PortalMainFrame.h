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
#include "CoreGenPortal/PortalCore/CoreErrorDiag.h"
#include "CoreGenPortal/PortalCore/CoreStoneCutterEditor.h"

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
  // CoreGenPortal constants
  const std::string L0KeyWords = "Registers RegClasses ISAs InstFormats Insts PseudoInsts Caches Scratchpads MemoryControllers Extensions Cores Socs";
  const std::string L1KeyWords = "RegName RegisterClassName ISAName InstFormatName Inst PseudoInst Cache Scratchpad MemoryController MajorVersion MinorVersion Type Extension Core Soc";
  const std::string L2KeyWords = "Width Index PseudoName IsFixedValue IsSIMD RWReg ROReg CSRReg AMSReg Shared Registers ISA FormatWidth Fields FieldName FieldType FieldWidth StartBit EndBit MandatoryField InstFormat Encodings EncodingField EncodingWidth EncodingValue Sets Ways SubLevel MemSize RqstPorts RspPorts StartAddr Ports RTLFile RegClass";

  // top level manager
  wxAuiManager Mgr;

  // menu bar items
  wxMenuBar *MenuBar;
  wxMenu *FileMenu;
  wxMenu *EditMenu;
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
};

enum
{
  MARGIN_LINE_NUMBERS,
  ID_PROJNEW            = 10
};

#endif

// EOF
