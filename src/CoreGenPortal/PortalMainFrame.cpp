//
// _PORTALMAINFRAME_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "PortalMainFrame.h"

// PortalMainFrame::PortalMainFrame
PortalMainFrame::PortalMainFrame( const wxString& title,
                                  const wxPoint& pos,
                                  const wxSize& size )
  : wxFrame( NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE ),
    CGProject(nullptr), UserConfig(nullptr),VerifConfig(nullptr),
    MenuBar(NULL), FileMenu(NULL), EditMenu(NULL), PrefMenu(NULL), ProjectMenu(NULL),
    BuildMenu(NULL), PluginMenu(NULL), HelpMenu(NULL), ToolBar(NULL),
    LogPane(NULL), ModulesNotebook(NULL), ModuleBox(NULL), PluginBox(NULL),
    ProjDir(NULL), EditorNotebook(NULL), IRPane(NULL) {

  // init the aui window manager
  InitAuiMgr();

  // create the menu bar
  CreateMenuBar();

  // create the tool bar
  CreatePortalToolBar();

  // create the window layout
  CreateWindowLayout();

  // create the status bar
  CreateStatusBar();
  SetStatusText( "Welcome to CoreGenPortal!" );

  //Bind(wxEVT_TEXT_ENTER, &PortalMainFrame::OnPressEnter, this);

  // update the aui manager
  UpdateAuiMgr();

  // initialize the verification configuration data
  VerifConfig = new CoreVerifConfig();
  if( VerifConfig->isValid() )
    LogPane->AppendText("Initialized the verification pass preferences\n");
  else
    LogPane->AppendText("Error initializing the verification pass preferences\n");

  // set the default path in the project window
  ProjDir->SetPath(UserConfig->wxGetProjectDir());
}

// PortalMainFrame::~PortalMainFrame
PortalMainFrame::~PortalMainFrame(){
  Mgr.UnInit();
  delete UserConfig;
  delete VerifConfig;
}

// PortalMainFrame::InitAuiMgr
// Initialize the AUI window manager
void PortalMainFrame::InitAuiMgr(){
  // notify the wxAUI which frame to use
  Mgr.SetManagedWindow(this);
}

// PortalMainFrame::UpdateAuiMgr
// Update the AUI window context
void PortalMainFrame::UpdateAuiMgr(){
  // commit all the changes to the window
  Mgr.Update();
}

// PortalMainFrame::CreateMenuBar
// Initialize the menu bar
void PortalMainFrame::CreateMenuBar(){
  MenuBar = new wxMenuBar;
  FileMenu = new wxMenu();
  EditMenu = new wxMenu();
  PrefMenu = new wxMenu();
  ProjectMenu = new wxMenu();
  BuildMenu = new wxMenu();
  PluginMenu = new wxMenu();
  HelpMenu = new wxMenu();

  //-- file menu
  FileMenu->Append(wxID_PRINT);
  FileMenu->Append(wxID_EXIT);

  //-- Edit Menu
  EditMenu->Append(wxID_COPY);
  EditMenu->Append(wxID_PASTE);
  EditMenu->Append(wxID_SELECTALL);

  //-- Preferences Menu
  PrefMenu->Append( ID_PREF_USER,           wxT("&User Preferences"));
  PrefMenu->Append( ID_PREF_VERIF,          wxT("&Verification Preferences"));
  PrefMenu->Append( ID_PREF_STONECUTTER,    wxT("&StoneCutter Preferences"));

  //-- Project Menu
  ProjectMenu->Append( ID_PROJNEW, wxT("&New Project"));
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(wxID_OPEN);
  ProjectMenu->Append(wxID_CLOSE);
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(wxID_SAVE);
  ProjectMenu->Append(wxID_SAVEAS);
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append( ID_PROJSCOPEN, wxT("&Open StoneCutter"));

  //-- Build Menu
  BuildMenu->Append( ID_BUILD_VERIFY,       wxT("&Verify Design"));
  BuildMenu->Append( ID_BUILD_CODEGEN,      wxT("&Execute CoreGen Codegen"));
  BuildMenu->Append( ID_BUILD_SIGMAP,       wxT("&Generate Signal Map"));
  BuildMenu->Append( ID_BUILD_STONECUTTER,  wxT("&Build StoneCutter"));
  BuildMenu->Append( ID_BUILD_VERILOG,      wxT("&Build Verilog"));
  BuildMenu->Append( ID_BUILD_COMPILER,     wxT("&Build Compiler"));
  BuildMenu->Append( ID_BUILD_SIM,          wxT("&Build Simulator"));
  BuildMenu->Append( ID_BUILD_ALL,          wxT("&Build All"));
  BuildMenu->AppendSeparator();
  BuildMenu->Append( ID_COMPILE_SIM,        wxT("&Compile Simulator"));
  BuildMenu->Append( ID_COMPILE_COMPILER,   wxT("&Compiler Compiler"));
  BuildMenu->Append( ID_COMPILE_ALL,        wxT("&Compile All"));

  //-- Help Menu
  HelpMenu->Append(wxID_ABOUT);

  // enable all the menus
  MenuBar->Append( FileMenu,    wxT("&File") );
  MenuBar->Append( EditMenu,    wxT("&Edit") );
  MenuBar->Append( PrefMenu,    wxT("&Preferences") );
  MenuBar->Append( ProjectMenu, wxT("&Project") );
  MenuBar->Append( BuildMenu,   wxT("&Build") );
  MenuBar->Append( PluginMenu,  wxT("&Plugins") );
  MenuBar->Append( HelpMenu,    wxT("&Help") );

  SetMenuBar(MenuBar);

  // connect all the handlers
  //-- file menu
  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnQuit));

  //-- edit menu

  //-- preferences menu
  Connect(ID_PREF_VERIF, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnVerifPref));
  Connect(ID_PREF_USER, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnUserPref));

  //-- project menu
  Connect(ID_PROJNEW, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjNew));
  Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjOpen));
  Connect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjClose));
  Connect(ID_PROJSCOPEN, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSCOpen));

  //-- build menu
  Connect(ID_BUILD_VERIFY, wxEVT_COMMAND_MENU_SELECTED,
         wxCommandEventHandler(PortalMainFrame::OnBuildVerify));

  //-- help menu
  Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnAbout));

  // center the main frame
  Centre();
}

// PortalMainFrame::CreateToolBar
// Initialize the tool bar
void PortalMainFrame::CreatePortalToolBar(){
  wxBitmap ExitBmp = wxArtProvider::GetBitmap("wxART_QUIT",
                                              wxART_MENU);
  wxBitmap OpenBmp = wxArtProvider::GetBitmap("wxART_FILE_OPEN",
                                              wxART_MENU);
  wxBitmap SaveBmp = wxArtProvider::GetBitmap("wxART_FILE_SAVE",
                                              wxART_MENU);
  wxBitmap SaveAsBmp = wxArtProvider::GetBitmap("wxART_FILE_SAVE_AS",
                                                wxART_MENU);
  wxBitmap CloseBmp = wxArtProvider::GetBitmap("wxART_CLOSE",
                                               wxART_MENU);

  ToolBar = new wxToolBar(this, wxID_ANY);
  ToolBar->AddTool(wxID_EXIT,   wxT("Exit CoreGenPortal"),  ExitBmp );
  ToolBar->AddTool(wxID_OPEN,   wxT("Open Project"),        OpenBmp );
  ToolBar->AddTool(wxID_SAVE,   wxT("Save Project"),        SaveBmp );
  ToolBar->AddTool(wxID_SAVEAS, wxT("Save Project As"),     SaveAsBmp );
  ToolBar->AddTool(wxID_CLOSE,  wxT("Close Project"),       CloseBmp );

  ToolBar->Realize();

  Mgr.AddPane(ToolBar, wxTOP);
}

// PortalMainFrame::CreateWindowLayout
// Initialize the window layout
void PortalMainFrame::CreateWindowLayout(){

  // Log pane
  LogPane = new wxTextCtrl(this, -1, _("CoreGenPortal Log....\n"),
                             wxDefaultPosition, wxSize(200,100),
                             wxNO_BORDER | wxTE_MULTILINE);

  // read the user configuration data
  UserConfig = new CoreUserConfig();
  if( UserConfig->isValid() )
    LogPane->AppendText("Read user configuration data; ConfigFile="
                        + UserConfig->wxGetConfFile() + "\n");
  else
    LogPane->AppendText("Error reading user configuration data; ConfigFile="
                        + UserConfig->wxGetConfFile() + "\n");

  // Modules Ribbons
  ModulesNotebook = new wxAuiNotebook(this, wxID_ANY,
                                      wxDefaultPosition,
                                      wxSize(300,150),
                                      wxAUI_NB_TOP |
                                      wxAUI_NB_TAB_SPLIT |
                                      wxAUI_NB_TAB_MOVE |
                                      wxAUI_NB_SCROLL_BUTTONS);

  ModuleBox = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize,
                             wxTR_HAS_BUTTONS|wxTR_MULTIPLE,
                             wxDefaultValidator, wxEmptyString );
  SetupModuleBox(); // setup the module box

  PluginBox = new wxListBox(this, wxID_ANY, wxDefaultPosition,
                            wxDefaultSize, 0, NULL, wxLB_MULTIPLE);
  SetupPluginBox(); // setup the plugin box

  ProjDir = new wxGenericDirCtrl(this,wxID_ANY, wxEmptyString,
                                 wxDefaultPosition, wxDefaultSize,
                                 wxDIRCTRL_3D_INTERNAL|wxSUNKEN_BORDER,
                                 wxEmptyString, 0 );
  ProjDir->ShowHidden( false );

  ModulesNotebook->AddPage( ModuleBox, L"Modules" );
  ModulesNotebook->AddPage( PluginBox, L"Plugins" );
  ModulesNotebook->AddPage( ProjDir, L"ProjectFiles" );

  // Editor Ribbons
  EditorNotebook = new wxAuiNotebook(this, wxID_ANY,
                                      wxDefaultPosition,
                                      wxSize(300,300),
                                      wxAUI_NB_TOP |
                                      wxAUI_NB_TAB_SPLIT |
                                      wxAUI_NB_SCROLL_BUTTONS );

  //-- setup the IR editor
  IRPane = new wxStyledTextCtrl(this, wxID_ANY);
  IRPane->StyleClearAll();
  IRPane->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
  IRPane->SetTabWidth(3);
  IRPane->SetIndent(3);
  IRPane->SetUseTabs(false);
  IRPane->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
  IRPane->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
  IRPane->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
  IRPane->SetWrapMode(wxSTC_WRAP_WORD);
  IRPane->SetLexer(wxSTC_LEX_YAML);

  // -- set all the colors
  IRPane->StyleSetForeground(wxSTC_YAML_DEFAULT,    *wxBLACK);
  IRPane->StyleSetForeground(wxSTC_YAML_COMMENT,    *wxLIGHT_GREY);
  IRPane->StyleSetForeground(wxSTC_YAML_IDENTIFIER, *wxBLUE);
  IRPane->StyleSetForeground(wxSTC_YAML_KEYWORD,    *wxGREEN);
  IRPane->StyleSetForeground(wxSTC_YAML_NUMBER,     *wxGREEN);
  IRPane->StyleSetForeground(wxSTC_YAML_REFERENCE,  *wxCYAN);
  IRPane->StyleSetForeground(wxSTC_YAML_DOCUMENT,   *wxBLACK);
  IRPane->StyleSetForeground(wxSTC_YAML_TEXT,       *wxBLACK);
  IRPane->StyleSetForeground(wxSTC_YAML_ERROR,      *wxRED);
  IRPane->StyleSetForeground(wxSTC_YAML_OPERATOR,   *wxBLUE);
  IRPane->StyleSetBold(wxSTC_YAML_IDENTIFIER, true);

  // -- set all the keywords
  IRPane->SetKeyWords(0, wxString(L0KeyWords.c_str()) );
  IRPane->SetKeyWords(1, wxString(L1KeyWords.c_str()) );
  IRPane->SetKeyWords(2, wxString(L2KeyWords.c_str()) );
  EditorNotebook->AddPage( IRPane, L"IR Editor" );

  // Add the panes to the manager
  Mgr.AddPane( ModulesNotebook, wxLEFT,   wxT("Project Notebook"));
  Mgr.GetPane( ModulesNotebook ).CloseButton(false);

  Mgr.AddPane( LogPane,         wxBOTTOM, wxT("CoreGenPortal Log"));
  Mgr.AddPane( EditorNotebook,  wxCENTER);
  Mgr.GetPane( EditorNotebook ).CloseButton(false);
}

// PortalMainFrame::SetupPluginBox
// initializes the plugin box
void PortalMainFrame::SetupPluginBox(){
  // walk the plugin directory and derive our installed
  // set of plugins
  wxDir PluginDir(UserConfig->wxGetPluginDir());
  if( !PluginDir.IsOpened() ){
    LogPane->AppendText("Could not open plugin directory at " +
                        UserConfig->wxGetPluginDir() + "\n" );
  }else{
    LogPane->AppendText("Initializing plugin directories\n");
    wxString filename;
    unsigned pos = 0;
    bool cont = PluginDir.GetFirst(&filename,wxEmptyString,wxDIR_DIRS);
    while( cont ){
      wxDir LPluginDir(UserConfig->wxGetPluginDir() + wxT("/") + filename );
      wxString FullPluginPath;
      if( LPluginDir.HasFiles(wxT("*.so")) ){
        FullPluginPath = UserConfig->wxGetPluginDir() +
                         wxT("/") + filename +
                         wxT("/") + wxT("lib") + filename + wxT(".so");
        PluginPanes.push_back(std::make_pair(filename,
                                             FullPluginPath));
        LogPane->AppendText( "Loaded Plugin Path: " + FullPluginPath + wxT("\n"));
        PluginBox->InsertItems(1,&filename,pos);
        pos = pos+1;
      }else if( LPluginDir.HasFiles(wxT("*.dylib")) ){
        FullPluginPath = UserConfig->wxGetPluginDir() +
                         wxT("/") + filename +
                         wxT("/") + wxT("lib") + filename + wxT(".dylib");
        PluginPanes.push_back(std::make_pair(filename,
                                             FullPluginPath));
        LogPane->AppendText( "Loaded Plugin Path: " + FullPluginPath + wxT("\n"));
        PluginBox->InsertItems(1,&filename,pos);
        pos = pos+1;
      }else{
        LogPane->AppendText("No plugin library found for " + filename );
      }
      cont = PluginDir.GetNext(&filename);
    }
  }
}

// PortalMainFrame::SetupModuleBox
// initializes the modulebox tree hierarchy
void PortalMainFrame::SetupModuleBox(){

  ParentModule = ModuleBox->AddRoot(wxT("Nodes"), -1, -1, NULL);

  TreeItems.push_back( ModuleBox->AppendItem( ModuleBox->GetRootItem(),
                                              wxT("Cache"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Comm"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Core"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Ext"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("ISA"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Inst"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("InstFormat"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("MCtrl"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Plugin"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("PseudoInst"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Reg"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("RegClass"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("SoC"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("Spad"),
                                              -1,
                                              -1,
                                              NULL ) );
  TreeItems.push_back( ModuleBox->AppendItem( ParentModule,
                                              wxT("VTP"),
                                              -1,
                                              -1,
                                              NULL ) );

  // connect the module box window handlers
  Bind(wxEVT_LISTBOX_DCLICK, &PortalMainFrame::OnSelectPlugin, this);
  Bind(wxEVT_TREE_ITEM_ACTIVATED, &PortalMainFrame::OnSelectNode, this);
  Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &PortalMainFrame::OnRightClickNode, this);
  Bind(wxEVT_TREE_ITEM_MIDDLE_CLICK, &PortalMainFrame::OnMiddleClickNode, this);
}

// PortalMainFrame::FindNodeStr
// converts a node name into its appropriate Yaml text for searching
wxString PortalMainFrame::FindNodeStr( CoreGenNode *Parent ){
  switch( Parent->GetType() ){
  case CGSoc:
    return wxT("- Soc: ") + wxString(Parent->GetName());
    break;
  case CGCore:
    return wxT("- Core: ") + wxString(Parent->GetName());
    break;
  case CGInstF:
    return wxT("- InstFormatName: ") + wxString(Parent->GetName());
    break;
  case CGInst:
    return wxT("- Inst: ") + wxString(Parent->GetName());
    break;
  case CGPInst:
    return wxT("- PseudoInst: ") + wxString(Parent->GetName());
    break;
  case CGRegC:
    return wxT("- RegisterClassname: ") + wxString(Parent->GetName());
    break;
  case CGReg:
    return wxT("- RegName: ") + wxString(Parent->GetName());
    break;
  case CGISA:
    return wxT("- ISAName: ") + wxString(Parent->GetName());
    break;
  case CGCache:
    return wxT("- Cache: ") + wxString(Parent->GetName());
    break;
  case CGExt:
    return wxT("- Extension: ") + wxString(Parent->GetName());
    break;
  case CGComm:
    return wxT("- Comm: ") + wxString(Parent->GetName());
    break;
  case CGSpad:
    return wxT("- Scratchpad: ") + wxString(Parent->GetName());
    break;
  case CGMCtrl:
    return wxT("- MemoryController: ") + wxString(Parent->GetName());
    break;
  case CGVTP:
    return wxT("- VTP: ") + wxString(Parent->GetName());
    break;
  case CGPlugin:
    return wxT("- Plugin: ") + wxString(Parent->GetName());
    break;
  default:
    return wxString(Parent->GetName());
    break;
  }
  return wxString(Parent->GetName());
}

// PortalMainFrame::LoadModuleBox
// loads all the module box items
void PortalMainFrame::LoadModuleBox(){
  CoreGenNode *Top = CGProject->GetTop();

  if( Top == nullptr ){
    LogPane->AppendText("Error loading modules...\n");
    return ;
  }

  LogPane->AppendText("Loading modules...\n" );

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    switch( Top->GetChild(i)->GetType() ){
    case CGSoc:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_SOC],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGCore:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_CORE],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGInstF:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_INSTFORMAT],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGInst:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_INST],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      LoadInstEncodings( NodeItems[NodeItems.size()-1].first,
                         static_cast<CoreGenInst *>(Top->GetChild(i)));
      break;
    case CGPInst:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_PSEUDOINST],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      LoadPInstEncodings( NodeItems[NodeItems.size()-1].first,
                         static_cast<CoreGenPseudoInst *>(Top->GetChild(i)));
      break;
    case CGRegC:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_REGCLASS],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGReg:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_REG],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGISA:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_ISA],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGCache:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_CACHE],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGComm:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_COMM],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGSpad:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_SPAD],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGMCtrl:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_MCTRL],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGVTP:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_VTP],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ),Top->GetChild(i)) );
      break;
    case CGPlugin:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_PLUGIN],
                                                wxString(Top->GetChild(i)->GetName()),
                                                wxTreeListCtrl::NO_IMAGE,
                                                wxTreeListCtrl::NO_IMAGE,
                                                NULL ),Top->GetChild(i)) );
      LoadPluginNodes( NodeItems[NodeItems.size()-1].first,
                         static_cast<CoreGenPlugin *>(Top->GetChild(i)));
      break;
    case CGExt:
      NodeItems.push_back( std::make_pair(ModuleBox->AppendItem( TreeItems[TREE_NODE_EXT],
                                                wxString(Top->GetChild(i)->GetName()),
                                                wxTreeListCtrl::NO_IMAGE,
                                                wxTreeListCtrl::NO_IMAGE,
                                                NULL ),Top->GetChild(i)) );
      LoadExtNodes( NodeItems[NodeItems.size()-1].first,
                    static_cast<CoreGenExt *>(Top->GetChild(i)));
      break;
    default:
      LogPane->AppendText("Error loading node: " +
                          wxString(Top->GetChild(i)->GetName()) + "\n");
      break;
    }
  }

  // expand the parent module
  ModuleBox->Expand(ParentModule);
}

// PortalMainFrame::LoadExtNodes
// loads the wxTreeCtrl Ext node with child nodes
void PortalMainFrame::LoadExtNodes( wxTreeItemId Parent,
                                    CoreGenExt *Ext ){
  std::vector<wxTreeItemId> wxExtItems;
  wxTreeItemId TmpItem;

  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Cache"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Comm"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Core"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Ext"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("ISA"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Inst"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("PseudoInst"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("InstFormat"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("MCtrl"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Reg"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("RegClass"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxExtItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Spad"),
                                              -1,
                                              -1,
                                              NULL ) );

  // retrieve all the children from the ext and insert them
  // into the appropriate slot
  for( unsigned i=0; i<Ext->GetNumChild(); i++ ){
    CoreGenNode *Child = Ext->GetChild(i);
    switch( Child->GetType() ){
    case CGCache:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_CACHE],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGExt:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_EXT],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGInst:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_INST],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      TmpItem = std::get<0>(ExtItems[ExtItems.size()-1]);
      LoadInstEncodings(TmpItem,
                        static_cast<CoreGenInst *>(Child));
    case CGPInst:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_PINST],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      TmpItem = std::get<0>(ExtItems[ExtItems.size()-1]);
      LoadPInstEncodings(TmpItem,
                        static_cast<CoreGenPseudoInst *>(Child));
      break;
    case CGInstF:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_INSTFORMAT],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGReg:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_REG],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGRegC:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_REGCLASS],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGISA:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_ISA],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGComm:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_COMM],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGSpad:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_SPAD],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGMCtrl:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_MCTRL],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    case CGCore:
      ExtItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxExtItems[TREE_EXT_NODE_CORE],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Ext, Child ) );
      break;
    default:
      LogPane->AppendText("Unable to load Ext child node into module tree\n");
      break;
    }
  }
}

// PortalMainFrame::LoadPluginNodes
// loads the wxTreeCtrl Plugin node with child nodes
void PortalMainFrame::LoadPluginNodes( wxTreeItemId Parent,
                                       CoreGenPlugin *Plugin ){
  std::vector<wxTreeItemId> wxPluginItems;
  wxTreeItemId TmpItem;

  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Cache"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Core"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Inst"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("PseudoInst"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Reg"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("RegClass"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("SoC"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("ISA"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Ext"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Comm"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("Spad"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("MCtrl"),
                                              -1,
                                              -1,
                                              NULL ) );
  wxPluginItems.push_back( ModuleBox->AppendItem( Parent,
                                              wxT("VTP"),
                                              -1,
                                              -1,
                                              NULL ) );

  //-- cache
  std::vector<CoreGenCache *> CacheVect = Plugin->GetCacheVect();
  for( unsigned i=0; i<CacheVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(CacheVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_CACHE],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- core
  std::vector<CoreGenCore *> CoreVect = Plugin->GetCoreVect();
  for( unsigned i=0; i<CoreVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(CoreVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_CORE],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- inst
  std::vector<CoreGenInst *> InstVect = Plugin->GetInstVect();
  for( unsigned i=0; i<InstVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(InstVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_INST],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
    TmpItem = std::get<0>(PluginItems[PluginItems.size()-1]);
    LoadInstEncodings(TmpItem,
                      static_cast<CoreGenInst *>(Child));
  }

  //-- pinst
  std::vector<CoreGenPseudoInst *> PInstVect = Plugin->GetPseudoInstVect();
  for( unsigned i=0; i<PInstVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(PInstVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_PINST],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
    TmpItem = std::get<0>(PluginItems[PluginItems.size()-1]);
    LoadPInstEncodings(TmpItem,
                      static_cast<CoreGenPseudoInst *>(Child));
  }

  //-- instformat
  std::vector<CoreGenInstFormat *> InstFVect = Plugin->GetInstFormatVect();
  for( unsigned i=0; i<InstFVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(InstFVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_INSTFORMAT],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- reg
  std::vector<CoreGenReg *> RegVect = Plugin->GetRegVect();
  for( unsigned i=0; i<RegVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(RegVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_REG],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- regclass
  std::vector<CoreGenRegClass *> RegCVect = Plugin->GetRegClassVect();
  for( unsigned i=0; i<RegCVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(RegCVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_REGCLASS],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- soc
  std::vector<CoreGenSoC *> SocVect = Plugin->GetSocVect();
  for( unsigned i=0; i<SocVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(SocVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_SOC],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- isa
  std::vector<CoreGenISA *> ISAVect = Plugin->GetISAVect();
  for( unsigned i=0; i<ISAVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(ISAVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_ISA],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- ext
  std::vector<CoreGenExt *> ExtVect = Plugin->GetExtVect();
  for( unsigned i=0; i<ExtVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(ExtVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_EXT],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
    TmpItem = std::get<0>(PluginItems[PluginItems.size()-1]);
    LoadExtNodes( TmpItem,
                  static_cast<CoreGenExt *>(Child) );
  }

  //-- comm
  std::vector<CoreGenComm *> CommVect = Plugin->GetCommVect();
  for( unsigned i=0; i<CommVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(CommVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_COMM],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- spad
  std::vector<CoreGenSpad *> SpadVect = Plugin->GetSpadVect();
  for( unsigned i=0; i<SpadVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(SpadVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_SPAD],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- mctrl
  std::vector<CoreGenMCtrl *> MCtrlVect = Plugin->GetMCtrlVect();
  for( unsigned i=0; i<MCtrlVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(MCtrlVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_MCTRL],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }

  //-- vtp
  std::vector<CoreGenVTP *> VTPVect = Plugin->GetVTPVect();
  for( unsigned i=0; i<VTPVect.size(); i++ ){
    CoreGenNode *Child = static_cast<CoreGenNode *>(VTPVect[i]);
    PluginItems.push_back( std::make_tuple(
                                ModuleBox->AppendItem(
                                  wxPluginItems[TREE_PLUGIN_NODE_VTP],
                                  wxString(Child->GetName()),
                                  wxTreeListCtrl::NO_IMAGE,
                                  wxTreeListCtrl::NO_IMAGE,
                                  NULL), Plugin, Child ) );
  }
}

// PortalMainFrame::LoadInstEncodings
// loads the wxTreeCtrl Inst node with its child encodings
void PortalMainFrame::LoadInstEncodings( wxTreeItemId Parent,
                                         CoreGenInst *Inst ){
  for( unsigned j=0; j<Inst->GetNumEncodings(); j++ ){
    CoreGenEncoding *E = Inst->GetEncoding(j);
    EncItems.push_back( std::make_pair(ModuleBox->AppendItem( Parent,
                                               wxString(E->GetName()),
                                               -1,
                                               -1,
                                               NULL ),E) );
  }
}

// PortalMainFrame::LoadPInstEncodings
// loads the wxTreeCtrl PInst node with its child encodings
void PortalMainFrame::LoadPInstEncodings( wxTreeItemId Parent,
                                         CoreGenPseudoInst *PInst ){
  for( unsigned j=0; j<PInst->GetNumEncodings(); j++ ){
    CoreGenEncoding *E = PInst->GetEncoding(j);
    EncItems.push_back( std::make_pair(ModuleBox->AppendItem( Parent,
                                               wxString(E->GetName()),
                                               -1,
                                               -1,
                                               NULL ),E) );
  }
}

// PortalMainFrame::CloseProject
// closes any open project files
void PortalMainFrame::CloseProject(){
  if( !CGProject ){
    return ;
  }

  LogPane->AppendText("Closing open project...\n");

  // save the IR file
  IRPane->SaveFile(IRFileName);

  // clear the IR pane
  IRPane->ClearAll();

  // close out all the StoneCutter windows
  for( size_t i=1; i<EditorNotebook->GetPageCount(); i++ ){
    LogPane->AppendText("Removing Pages\n" );
    if( !EditorNotebook->RemovePage(i) )
      LogPane->AppendText("Error removing page\n" );
  }

  for( unsigned i=0; i<SCPanes.size(); i++ ){
    wxStyledTextCtrl *TmpCtrl = std::get<0>(SCPanes[i]);
    delete TmpCtrl;
  }
  SCPanes.clear();

  // close out all the modules
  NodeItems.clear();
  EncItems.clear();
  for( unsigned i=0; i<TreeItems.size(); i++ ){
    ModuleBox->CollapseAndReset( TreeItems[i] );
  }
  ModuleBox->Collapse(ParentModule);

  // reset the file browser window
  ProjDir->SetPath(UserConfig->wxGetProjectDir());

  // delete the final bits
  delete CGProject;
  CGProject = nullptr;
}

// PortalMainFrame::OnQuit
// handles quit signals to end the application
void PortalMainFrame::OnQuit(wxCommandEvent& event){
  int answer = wxMessageBox("Close CoreGenPortal?", "Confirm",
                            wxYES_NO | wxCANCEL, this);
  if( answer == wxYES ){
    // close the project
    CloseProject();
    Close(true);
  }
}

// PortalMainFrame::OnAbout
// handles the Help->About command event
void PortalMainFrame::OnAbout(wxCommandEvent &event){
  CoreGenBackend *CGA = new CoreGenBackend("","","");
  int major = -1;
  int minor = -1;
  wxString MaStr;
  wxString MiStr;
  CGA->CoreGenVersion( &major, &minor );
  MaStr << major;
  MiStr << minor;

  wxMessageDialog *dial = new wxMessageDialog(NULL,
                                              wxT("CoreGenPortal Version ") +
                                                PORTAL_VERSION_WX +
                                                wxT(" \n") +
                                                PORTAL_COPYRIGHT,
                                              wxT("About CoreGenPortal"),
                                              wxOK | wxICON_INFORMATION);
  dial->SetExtendedMessage( wxT("Built with CoreGen Version " ) + MaStr + "." + MiStr );
  dial->ShowModal();
  delete dial;
  delete CGA;
}

// PortalMainFrame::DeleteNode
void PortalMainFrame::DeleteNode(CoreGenNode *Node){
  //delete from UI
  //ModuleBox->DeleteChildren(ModuleBox->GetFocusedItem());
  //ModuleBox->Delete(ModuleBox->GetFocusedItem());

  //delete from IR text
  int pos = IRPane->FindText(0,IRPane->GetLastPosition(),
                             FindNodeStr(Node), 0 );
  IRPane->GotoPos(pos);
  wxString line = IRPane->GetCurLine();
  unsigned int i;
  for(i = 0; i < line.size() && line[i] != '-'; i++);
  do{
    IRPane->LineDelete();
    line = IRPane->GetCurLine();
  } while(line.size() > 0 && line[i] != '-' && isspace(line[i]));
  IRPane->PageDown();
  IRPane->LineUp();
  IRPane->EnsureCaretVisible();

  //delete from backend
  wxString Dnode(Node->GetName());
  CGProject->DeleteNode(Node);
  CGProject->BuildDAG();

  //delete from UI
  ModuleBox->DeleteAllItems();
  TreeItems.clear();
  NodeItems.clear();
  SetupModuleBox();
  LoadModuleBox();

  LogPane->AppendText(Dnode + wxT(" was deleted.\n"));
}

// PortalMainFrame::AddNodeWin
void PortalMainFrame::AddNodeWin(){
  LogPane->AppendText("AddNodeWin\n");
}

// PortalMainFrame::OnPopupNode
void PortalMainFrame::OnPopupNode(wxCommandEvent &event){
  CoreInfoWin *InfoWin = nullptr;
  switch(event.GetId()){
  case ID_TREE_INFONODE:
    // open tree info node
    InfoWin = new CoreInfoWin(this,wxID_ANY,
                              GetNodeFromItem(ModuleBox->GetFocusedItem()));
    delete InfoWin;
    break;
  case ID_TREE_DELNODE:
    DeleteNode(GetNodeFromItem(ModuleBox->GetFocusedItem()));
    break;
  case ID_TREE_ADDNODE:
    AddNodeWin();
    break;
  }
}

// PortalMainFrame::OnRightClickNode
void PortalMainFrame::OnRightClickNode(wxTreeEvent &event){
  wxMenu mnu;
  if( ModuleBox->GetItemParent(ModuleBox->GetFocusedItem()) == ParentModule ){
    // this is a node descriptor type
    mnu.Append( ID_TREE_ADDNODE, "Add Node" );
  }else{
    // this is an actual node
    mnu.Append( ID_TREE_INFONODE, "Node Info" );
    mnu.Append( ID_TREE_DELNODE,  "Delete Node" );
  }
  mnu.Connect( wxEVT_COMMAND_MENU_SELECTED,
               wxCommandEventHandler(PortalMainFrame::OnPopupNode),
               NULL,
               this );
  PopupMenu(&mnu);
}

// PortalMainFrame::GetNodeFromItem
CoreGenNode *PortalMainFrame::GetNodeFromItem( wxTreeItemId SelId ){
  if( !SelId.IsOk() ){
    LogPane->AppendText("Error: could not derive node from selection\n");
    return nullptr;
  }

  // walk the main nodes
  for( unsigned i=0; i<NodeItems.size(); i++ ){
    if( NodeItems[i].first == SelId )
      return NodeItems[i].second;
  }

  // walk the encoding nodes
  for( unsigned i=0; i<EncItems.size(); i++ ){
    if( EncItems[i].first == SelId ){
      wxTreeItemId ParentId = ModuleBox->GetItemParent(EncItems[i].first);
      for( unsigned j=0; j<NodeItems.size(); j++ ){
        if( NodeItems[j].first == ParentId )
          return NodeItems[j].second;
      }
    }
  }

  // walk the Ext nodes
  for( unsigned i=0; i<ExtItems.size(); i++ ){
    wxTreeItemId TmpItem = std::get<0>(ExtItems[i]);
    if( TmpItem == SelId ){
      return std::get<2>(ExtItems[i]);
    }
  }

  // walk the plugin nodes
  for( unsigned i=0; i<PluginItems.size(); i++ ){
    wxTreeItemId TmpItem = std::get<0>(PluginItems[i]);
    if( TmpItem == SelId ){
      return std::get<2>(PluginItems[i]);
    }
  }

  return nullptr;
}

// PortalMainFrame::OnMiddleClickNode
void PortalMainFrame::OnMiddleClickNode(wxTreeEvent &event){
  // retrieve the name of the selection and search for it
  // in the IRPane, then refocus the IRPane on the target node
  wxTreeItemId SelId = ModuleBox->GetFocusedItem();

  if( !SelId.IsOk() ){
    LogPane->AppendText("Error: Could not select node\n");
    return ;
  }

  // we have a valid node, search for its corresponding object
  CoreGenNode *Node = GetNodeFromItem(SelId);
  if( Node == nullptr ){
    LogPane->AppendText("Error : node object is null\n" );
    return ;
  }
  int pos = IRPane->FindText(0,IRPane->GetLastPosition(),
                             FindNodeStr(Node), 0 );
  IRPane->GotoPos(pos);
}

// PortalMainFrame::OpenFile
void PortalMainFrame::OpenFileFromWin(wxString Path){
  wxFileName NPF(Path);
  wxString Ext = NPF.GetExt();

  if( Ext.IsSameAs(wxT("sc"),false) ){
    OpenSCFile(Path,NPF);
  }else if( Ext.IsSameAs(wxT("yaml"),false) ){
    LogPane->AppendText("Opening Yaml file at " + Path + wxT("\n") );
  }else{
    LogPane->AppendText("Could not open file at " + Path + wxT("\n") );
  }
}

// PortalMainFrame::OpenSCFile
void PortalMainFrame::OpenSCFile(wxString NP, wxFileName NPF){
  LogPane->AppendText( "Opening StoneCutter file at " +
                       NPF.GetFullName() + wxT("\n") );

  // check to see if the window is already open
  wxString TmpName = NPF.GetFullName();
  size_t TmpPage = -1;
  for( unsigned i = 0; i<SCPanes.size(); i++ ){
    if( std::get<1>(SCPanes[i]) == TmpName )
      TmpPage = i+1;
  }

  if( TmpPage != -1 ){
    // page already exists, refocus to the target tab
    LogPane->AppendText("File is already open... refocusing to the appropriate tab\n" );
    EditorNotebook->SetSelection(TmpPage);
  }else{
    // create a new window
    wxStyledTextCtrl *SCPane = new wxStyledTextCtrl(this, wxID_ANY);
    SCPane->StyleClearAll();
    SCPane->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
    SCPane->SetTabWidth(3);
    SCPane->SetIndent(3);
    SCPane->SetUseTabs(false);
    SCPane->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
    SCPane->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
    SCPane->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
    SCPane->SetWrapMode(wxSTC_WRAP_WORD);
    SCPane->SetLexer(wxSTC_LEX_CPP);

    SCPane->StyleSetForeground (wxSTC_C_STRING,            wxColour(150,0,0));
    SCPane->StyleSetForeground (wxSTC_C_PREPROCESSOR,      wxColour(165,105,0));
    SCPane->StyleSetForeground (wxSTC_C_IDENTIFIER,        wxColour(40,0,60));
    SCPane->StyleSetForeground (wxSTC_C_NUMBER,            wxColour(0,150,0));
    SCPane->StyleSetForeground (wxSTC_C_CHARACTER,         wxColour(150,0,0));
    SCPane->StyleSetForeground (wxSTC_C_WORD,              wxColour(0,0,150));
    SCPane->StyleSetForeground (wxSTC_C_WORD2,             wxColour(0,150,0));
    SCPane->StyleSetForeground (wxSTC_C_COMMENT,           wxColour(150,150,150));
    SCPane->StyleSetForeground (wxSTC_C_COMMENTLINE,       wxColour(150,150,150));
    SCPane->StyleSetForeground (wxSTC_C_COMMENTDOC,        wxColour(150,150,150));
    SCPane->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORD, wxColour(0,0,200));
    SCPane->StyleSetForeground (wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0,0,200));
    SCPane->StyleSetBold(wxSTC_C_WORD, true);
    SCPane->StyleSetBold(wxSTC_C_WORD2, true);
    SCPane->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

    // the load file
    SCPane->LoadFile(NP);

    // add it to our vector
    SCPanes.push_back(std::make_pair(SCPane,NPF.GetFullName()));

    // add the file to the editor network
    EditorNotebook->AddPage( SCPane, NP, true, wxBookCtrlBase::NO_IMAGE );

    // reset the tab title
    EditorNotebook->SetPageText( EditorNotebook->GetPageCount()-1,
                                 NPF.GetName() );
  }
}

// PortalMainFrame::OnSelectPlugin
void PortalMainFrame::OnSelectPlugin(wxCommandEvent& event){
  if( !CGProject ){
    LogPane->AppendText("Cannot load plugin; no project open\n" );
    return ;
  }
  int Plugin = PluginBox->GetSelection();
  if( (unsigned)(Plugin) > (PluginPanes.size()-1) ){
    LogPane->AppendText("Invalid plugin item\n");
  }else{
    wxString PName = std::get<0>(PluginPanes[(unsigned)(Plugin)]);
    wxString PPath = std::get<1>(PluginPanes[(unsigned)(Plugin)]);
    if( !CGProject->LoadPlugin( std::string(PPath.mb_str()) ) ){
      LogPane->AppendText(  wxT("Failed to load plugin at ") + PPath + wxT("\n") );
      LogPane->AppendText( wxString(CGProject->GetErrStr()) );
    }else{
      // open a new plugin information window
      unsigned PID = CGProject->GetNumPlugins() - 1;
      CoreGenPlugin *PLUGIN = CGProject->GetPlugin(PID);
      CorePluginBrowser *PB = new CorePluginBrowser(this,
                                                  wxID_ANY,
                                                  wxT("Plugin Browser"),
                                                  PName,
                                                  PPath,
                                                  PLUGIN,
                                                  CGProject,
                                                  LogPane);
      if( PB->ShowModal() == wxID_OK ){
        PB->Destroy();
        if( !CGProject->ReleasePlugin(PID) ){
          LogPane->AppendText( wxT("Failed to release plugin at ") + PPath + wxT("\n") );
          LogPane->AppendText( wxString(CGProject->GetErrStr()) );
        }
      }
    }
  }
}

// PortalMainFrame::OnSelectNode
void PortalMainFrame::OnSelectNode(wxTreeEvent &event){
  switch( ModulesNotebook->GetSelection() ){
  case 0:
    LogPane->AppendText("Module node selected\n");
    // TODO: open the node window
    break;
  case 2:
    OpenFileFromWin(ProjDir->GetFilePath());
    break;
  default:
    LogPane->AppendText("erroneous window selections\n");
    break;
  }
}

// PortalMainFrame::OnVerifPref
void PortalMainFrame::OnVerifPref(wxCommandEvent &event){
  LogPane->AppendText("Loading verification preferences...\n");
  PortalVerifPrefWin *VP = new PortalVerifPrefWin(this,
                                                  wxID_ANY,
                                                  wxT("Verification Preferences"),
                                                  wxDefaultPosition,
                                                  wxSize(500,500),
                                                  wxDEFAULT_DIALOG_STYLE|wxVSCROLL,
                                                  VerifConfig);
  if( VP->ShowModal() == wxID_OK ){
    LogPane->AppendText("Committed verification preferences\n");
  }
  VP->Destroy();
}

// PortalMainFrame::OnUserPref
void PortalMainFrame::OnUserPref(wxCommandEvent &event){
  LogPane->AppendText("Loading user preferences...\n" );
  PortalUserPrefWin *UP = new PortalUserPrefWin(this,
                                               wxID_ANY,
                                               wxT("User Preferences"),
                                               wxDefaultPosition,
                                               wxSize(500,500),
                                               wxDEFAULT_DIALOG_STYLE|wxVSCROLL,
                                               UserConfig);
  if( UP->ShowModal() == wxID_OK ){
    LogPane->AppendText("Committed user preferences\n");
  }
  UP->Destroy();
}

// PortalMainFrame:: OnBuildVerify
void PortalMainFrame::OnBuildVerify(wxCommandEvent &event){
  if( !CGProject ){
    LogPane->AppendText( "No project is open!\n" );
    return ;
  }

  // check the verification config options
  if( !VerifConfig ){
    LogPane->AppendText( "Error: verification preferences are not initialized\n");
    return ;
  }

  if( !VerifConfig->isValid() ){
    LogPane->AppendText( "Error: verification preferences are not valid\n" );
    return ;
  }

  // build the dag
  if( !CGProject->BuildDAG() ){
    LogPane->AppendText( "Error constructing DAG of hardware nodes\n" );
    return ;
  }

  // init the pass manager
  if( !CGProject->InitPassMgr() ){
    LogPane->AppendText( "Error initializing the CoreGen pass manager\n" );
    return ;
  }

  LogPane->AppendText( "Executing verification passes...\n" );

  // setup the text redirector
  std::streambuf *oldBuf = std::cout.rdbuf();
  std::ostringstream newBuf;
  std::cout.rdbuf( newBuf.rdbuf() );

  // execute all the enabled passes
  for( unsigned i=0; i<VerifConfig->GetNumPasses(); i++ ){
    if( VerifConfig->IsPassEnabled(i) ){
      if( !CGProject->ExecutePass(
          std::string(VerifConfig->GetPassName(i).mb_str()) ) ){
        LogPane->AppendText( "Error executing pass: " +
                             VerifConfig->GetPassName(i) +
                             wxT("\n") );
      }
    }
  }

  // restore the old cout buffer
  std::cout.rdbuf( oldBuf );
  //LogPane->AppendText(wxString(newBuf.str())+wxT("\n"));
  CoreVerifWin *VW = new CoreVerifWin(this,
                                      wxID_ANY,
                                      wxT("Verification Results"),
                                      &newBuf );
  if( VW->ShowModal() == wxID_OK ){
    VW->Destroy();
  }
}

// PortalMainFrame::OnProjNew
void PortalMainFrame::OnProjNew(wxCommandEvent &event){
  PortalNewProjWin *NP = new PortalNewProjWin(this,
                                              wxID_ANY,
                                              wxT("New Project"),
                                              wxDefaultPosition,
                                              wxSize(500,500),
                                              wxDEFAULT_DIALOG_STYLE|wxVSCROLL,
                                              ProjDir,
                                              LogPane,
                                              IRPane,
                                              UserConfig);

  if( NP->ShowModal() == wxID_OK ){
    CGProject = NP->GetCoreGenPtr();
    IRFileName = NP->GetIRFileName();
    NP->Destroy();
  }
}

// PortalMainFrame::OnProjClose
void PortalMainFrame::OnProjClose(wxCommandEvent& WXUNUSED(event)){
  CloseProject();
}

// PortalMainFrame::OnProjSCOpen
void PortalMainFrame::OnProjSCOpen(wxCommandEvent& WXUNUSED(event)){
  if( !CGProject ){
    LogPane->AppendText( "No project open\n" );
    return ;
  }

  wxFileName CurProj(IRFileName);
  wxFileDialog* OpenDialog = new wxFileDialog( this,
                                               _("Choose a StoneCutter file to open"),
                                               CurProj.GetPath(),
                                               wxEmptyString,
                                               _("SC Files (*.sc)|*.sc"),
                                               wxFD_OPEN, wxDefaultPosition );

  wxString NP;
  if( OpenDialog->ShowModal() == wxID_OK ){
    NP = OpenDialog->GetPath();

    // derive the file name
    wxFileName NPF(NP);

    LogPane->AppendText( "Opening StoneCutter file at " + NP + wxT("\n") );

    OpenSCFile( NP, NPF );
  }

  // clean up the dialog box
  OpenDialog->Destroy();
}

// PortalMainFrame::OnProjOpen
void PortalMainFrame::OnProjOpen(wxCommandEvent& WXUNUSED(event)){
  // stage 1, decide whether we need to close the current project
  if( CGProject )
    CloseProject();

  // stage 2, prompt the user to select the new yaml input file
  wxFileDialog* OpenDialog = new wxFileDialog( this,
                                               _("Choose a YAML IR file to open"),
                                               UserConfig->wxGetProjectDir(),
                                               wxEmptyString,
                                               _("IR Files (*.yaml)|*.yaml"),
                                               wxFD_OPEN, wxDefaultPosition );

  wxString NP;
  if( OpenDialog->ShowModal() == wxID_OK ){
    NP = OpenDialog->GetPath();
    LogPane->AppendText( "Opening project from IR at " + NP + wxT("\n") );
    wxFileName NPF(NP);

    // create a new coregen object
    CGProject = new CoreGenBackend( std::string(NPF.GetName().mb_str()),
                                    std::string(NPF.GetPath().mb_str()),
                                    UserConfig->GetArchiveDir() );
    if( CGProject == nullptr ){
      LogPane->AppendText( "Error opening project from IR at " + NP + wxT("\n") );
      OpenDialog->Destroy();
    }

    // read the ir
    if( !CGProject->ReadIR( std::string(NP.mb_str()) ) ){
      LogPane->AppendText( "Error reading IR into CoreGen from " + NP + wxT("\n") );
      OpenDialog->Destroy();
    }

    // Force the DAG to build
    if( !CGProject->BuildDAG() ){
      LogPane->AppendText( "Error constructing DAG of hardware nodes\n" );
      OpenDialog->Destroy();
    }

    // load the ir into the ir pane
    IRPane->LoadFile(NP);
    IRFileName = NP;

    // switch the directory tree to the project directory
    ProjDir->SetPath(NPF.GetPath());

    // load all the modules into the modulebox
    LoadModuleBox();

    LogPane->AppendText( "Successfully opened project from IR at " + NP + wxT("\n" ));
  }

  // clean up the dialog box
  OpenDialog->Destroy();
}

void PortalMainFrame::OnPressEnter(wxCommandEvent& enter,
                                   CoreGenNode *node,
                                   int InfoWinType){
  // get the box contents
  wxTextCtrl *ClickedBox = (wxTextCtrl*)enter.GetEventObject();
  std::string BoxContents = ClickedBox->GetValue().ToStdString();
  // get the box id
  int InfoBoxIndex = ClickedBox->GetId();

  // TODO: handle invalid inputs
  // update yaml
  switch(InfoWinType){
    case CGCache:{
      // TODO: handle adding parent and child caches
      CoreGenCache *CacheNode = (CoreGenCache*)node;
      switch(InfoBoxIndex){
        case 0:
          CacheNode->SetName(BoxContents);
          break;
        case 1:
          CacheNode->SetSets(std::stoi(BoxContents));
          break;
        case 2:
          CacheNode->SetWays(std::stoi(BoxContents));
          break;
      }
    }
    break;
    case CGComm:{
      // TODO: handle endpoints
      CoreGenComm *CommNode = (CoreGenComm*)node;
      switch(InfoBoxIndex){
        case 0:
          CommNode->SetName(BoxContents);
          break;
        case 1:
          if( BoxContents.compare("Point-to-Point") == 0 ){
            CommNode->SetCommType(CGCommP2P);
          }
          else if( BoxContents.compare("Bus") == 0 ){
            CommNode->SetCommType(CGCommBus);
          }
          else if( BoxContents.compare("Network on Chip") == 0 ){
            CommNode->SetCommType(CGCommNoc);
          }
          else{
            CommNode->SetCommType(CGCommUnk);
          }
          break;
        case 2:
          CommNode->SetWidth(std::stoi(BoxContents));
          break;
      }
    }
    break;
    case CGCore:{
      // TODO: Handle isa, caches, regclasses, and extensions
      CoreGenCore *CoreNode = (CoreGenCore*)node;
      switch(InfoBoxIndex){
        case 0:
          CoreNode->SetName(BoxContents);
          break;
        case 1:
          CoreNode->SetNumThreadUnits(std::stoi(BoxContents));
          break;
      }
    }
    break;
    case CGExt:{
      CoreGenExt *ExtNode = (CoreGenExt*)node;
      switch(InfoBoxIndex){
        case 0:
          ExtNode->SetName(BoxContents);
          break;
        case 1:
          if( BoxContents.compare("Template extension") == 0 ){
            ExtNode->SetType(CGExtTemplate);
          }
          else if( BoxContents.compare("Module extension") == 0 ){
            ExtNode->SetType(CGExtModule);
          }
          else if( BoxContents.compare("Communications extension") == 0 ){
            ExtNode->SetType(CGExtComm);
          }
          else{
            ExtNode->SetType(CGExtUnk);
          }
          break;
      }
    }
    break;
    case CGISA:
      CoreGenISA *ISANode = (CoreGenISA*)node;
      ISANode->SetName(BoxContents);
    break;
  }

  // write out the new IR file
  LogPane->AppendText(std::string(this->IRFileName.mb_str()) + "\n");
  CGProject->WriteIR(std::string(IRFileName.mb_str()));
  ModuleBox->DeleteAllItems();
  TreeItems.clear();
  NodeItems.clear();
  SetupModuleBox();
  LoadModuleBox();
  //LoadModuleBox();
  LogPane->AppendText("Updated " + wxString(node->GetName()) +
                      " Box " + wxString(std::to_string(InfoBoxIndex)) +
                      ".\n");
}

// EOF
