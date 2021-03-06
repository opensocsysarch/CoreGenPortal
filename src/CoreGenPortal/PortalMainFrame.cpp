//
// _PORTALMAINFRAME_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <sstream>
#include <vector>
#include "PortalMainFrame.h"

// PortalMainFrame::PortalMainFrame
PortalMainFrame::PortalMainFrame(const wxString &title,
                                 const wxPoint &pos,
                                 const wxSize &size)
    : wxFrame(NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE),
      CGProject(nullptr),
      UserConfig(nullptr),
      VerifConfig(nullptr),
      SCConfig(nullptr),
      MenuBar(NULL),
      FileMenu(NULL),
      EditMenu(NULL),
      PrefMenu(NULL),
      ProjectMenu(NULL),
      BuildMenu(NULL),
      PluginMenu(NULL),
      HelpMenu(NULL),
      ToolBar(NULL),
      LogPane(NULL),
      ModulesNotebook(NULL),
      ModuleBox(NULL),
      PluginBox(NULL),
      ProjDir(NULL),
      EditorNotebook(NULL),
      IRPane(NULL){

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
  SetStatusText("Welcome to CoreGenPortal!");

  // update the aui manager
  UpdateAuiMgr();

  // initialize the verification configuration data
  VerifConfig = new CoreVerifConfig();
  if (VerifConfig->isValid())
    LogPane->AppendText("Initialized the verification pass preferences\n");
  else
    LogPane->AppendText("Error initializing the verification pass preferences\n");

  // initialize the stonecutter compiler configuration
  SCConfig = new SCCompConfig();
  if (SCConfig->isValid())
    LogPane->AppendText("Initialized the StoneCutter compiler preferences\n");
  else
    LogPane->AppendText("Error initializaing the StoneCutter compiler preferences\n");

  // set the default path in the project window
  ProjDir->SetPath(UserConfig->wxGetProjectDir());
}

// PortalMainFrame::~PortalMainFrame
PortalMainFrame::~PortalMainFrame(){

  if (CGProject)
    CloseProject();

  if (UserConfig)
    delete UserConfig;

  if (VerifConfig)
    delete VerifConfig;

  if (SCConfig)
    delete SCConfig;

  Mgr.UnInit();
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
  PrefMenu->Append(ID_PREF_USER, wxT("&User Preferences"));
  PrefMenu->Append(ID_PREF_VERIF, wxT("&Verification Preferences"));
  PrefMenu->Append(ID_PREF_STONECUTTER, wxT("&StoneCutter Preferences"));

  //-- Project Menu
  ProjectMenu->Append(ID_PROJNEW, wxT("&New Project"));
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(wxID_OPEN);
  ProjectMenu->Append(wxID_CLOSE);
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(wxID_SAVE);
  ProjectMenu->Append(wxID_SAVEAS);
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(ID_PROJFILESAVE, wxT("&Save File"));
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(ID_PROJSCOPEN, wxT("&Open StoneCutter"));
  ProjectMenu->Append(ID_PROJSUMMARY, wxT("&Project Summary"));
  ProjectMenu->Append(ID_PROJSPECDOC, wxT("&Build Specification Doc"));
  ProjectMenu->AppendSeparator();
  ProjectMenu->Append(ID_PROJVIZIR, wxT("&Visualize IR"));
  ProjectMenu->Append(ID_PROJVIZINSTF, wxT("&Visualize Instruction Format"));
  ProjectMenu->Append(ID_PROJVIZPIPE, wxT("&Visualize Pipeline"));

  //-- Build Menu
  BuildMenu->Append(ID_BUILD_VERIFY, wxT("&Verify Design"));
  BuildMenu->Append(ID_BUILD_CODEGEN, wxT("&Execute Chisel Codegen"));
  BuildMenu->Append(ID_BUILD_LLVM_CODEGEN, wxT("&Execute LLVM Codegen"));
  BuildMenu->Append(ID_BUILD_SIGMAP, wxT("&Generate Signal Map"));
  BuildMenu->Append(ID_BUILD_STONECUTTER, wxT("&Build StoneCutter"));
  BuildMenu->Append(ID_BUILD_VERILOG, wxT("&Build Verilog"));
  BuildMenu->Append(ID_BUILD_COMPILER, wxT("&Build Compiler"));
  BuildMenu->Append(ID_BUILD_SIM, wxT("&Build Simulator"));
  BuildMenu->Append(ID_BUILD_ALL, wxT("&Build All"));
  BuildMenu->AppendSeparator();
  BuildMenu->Append(ID_COMPILE_SIM, wxT("&Compile Simulator"));
  BuildMenu->Append(ID_COMPILE_COMPILER, wxT("&Compiler Compiler"));
  BuildMenu->Append(ID_COMPILE_ALL, wxT("&Compile All"));

  //-- Help Menu
  HelpMenu->Append(wxID_ABOUT);

  // enable all the menus
  MenuBar->Append(FileMenu, wxT("&File"));
  MenuBar->Append(EditMenu, wxT("&Edit"));
  MenuBar->Append(PrefMenu, wxT("&Preferences"));
  MenuBar->Append(ProjectMenu, wxT("&Project"));
  MenuBar->Append(BuildMenu, wxT("&Build"));
  MenuBar->Append(PluginMenu, wxT("&Plugins"));
  MenuBar->Append(HelpMenu, wxT("&Help"));

  SetMenuBar(MenuBar);

  // connect all the handlers
  //-- file menu
  Connect(wxID_PRINT, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnPrint));
  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnQuit));

  //-- edit menu
  Connect(wxID_COPY, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnCopyText));
  Connect(wxID_PASTE, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnPasteText));

  //-- preferences menu
  Connect(ID_PREF_VERIF, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnVerifPref));
  Connect(ID_PREF_STONECUTTER, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnSCPref));
  Connect(ID_PREF_USER, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnUserPref));

  //-- project menu
  Connect(ID_PROJNEW, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjNew));
  Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjOpen));
  Connect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjClose));
  Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSave));
  Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSaveAs));
  Connect(ID_PROJSCOPEN, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSCOpen));
  Connect(ID_PROJSUMMARY, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSummary));
  Connect(ID_PROJSPECDOC, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSpecDoc));
  Connect(ID_PROJFILESAVE, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnProjSaveFile));
  Connect(ID_PROJVIZIR, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnVizIR));
  Connect(ID_PROJVIZINSTF, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnVizInstFormat));
  Connect(ID_PROJVIZPIPE, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnVizPipeline));

  //-- build menu
  Connect(ID_BUILD_VERIFY, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnBuildVerify));
  Connect(ID_BUILD_CODEGEN, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnBuildCodegen));
  Connect(ID_BUILD_LLVM_CODEGEN, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnBuildLLVMCodegen));
  Connect(ID_BUILD_STONECUTTER, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnBuildStoneCutter));
  Connect(ID_BUILD_SIGMAP, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnBuildSigmap));

  //-- help menu
  Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
          wxCommandEventHandler(PortalMainFrame::OnAbout));

  // center the main frame
  Centre();
}

// PortalMainFrame::CreateToolBar
// Initialize the tool bar
void PortalMainFrame::CreatePortalToolBar()
{
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
  ToolBar->AddTool(wxID_EXIT, wxT("Exit CoreGenPortal"), ExitBmp);
  ToolBar->AddTool(wxID_OPEN, wxT("Open Project"), OpenBmp);
  ToolBar->AddTool(wxID_SAVE, wxT("Save Project"), SaveBmp);
  ToolBar->AddTool(wxID_SAVEAS, wxT("Save Project As"), SaveAsBmp);
  ToolBar->AddTool(wxID_CLOSE, wxT("Close Project"), CloseBmp);

  ToolBar->Realize();

  Mgr.AddPane(ToolBar, wxTOP);
}

// PortalMainFrame::CreateWindowLayout
// Initialize the window layout
void PortalMainFrame::CreateWindowLayout(){

  // Log pane
  LogPane = new wxTextCtrl(this, -1, _("CoreGenPortal Log....\n"),
                           wxDefaultPosition, wxSize(200, 100),
                           wxNO_BORDER | wxTE_MULTILINE);

  // read the user configuration data
  UserConfig = new CoreUserConfig();
  if (UserConfig->isValid())
    LogPane->AppendText("Read user configuration data; ConfigFile=" + UserConfig->wxGetConfFile() + "\n");
  else
    LogPane->AppendText("Error reading user configuration data; ConfigFile=" + UserConfig->wxGetConfFile() + "\n");

  // Modules Ribbons
  ModulesNotebook = new wxAuiNotebook(this, wxID_ANY,
                                      wxDefaultPosition,
                                      wxSize(300, 150),
                                      wxAUI_NB_TOP |
                                          wxAUI_NB_TAB_SPLIT |
                                          wxAUI_NB_TAB_MOVE |
                                          wxAUI_NB_SCROLL_BUTTONS);

  ModuleBox = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize,
                             wxTR_HAS_BUTTONS | wxTR_MULTIPLE,
                             wxDefaultValidator, wxEmptyString);
  SetupModuleBox(); // setup the module box

  PluginBox = new wxListBox(this, wxID_ANY, wxDefaultPosition,
                            wxDefaultSize, 0, NULL, wxLB_MULTIPLE);
  SetupPluginBox(); // setup the plugin box

  ProjDir = new wxGenericDirCtrl(this, wxID_ANY, wxEmptyString,
                                 wxDefaultPosition, wxDefaultSize,
                                 wxDIRCTRL_3D_INTERNAL | wxSUNKEN_BORDER,
                                 wxEmptyString, 0);
  ProjDir->ShowHidden(false);

  ModulesNotebook->AddPage(ModuleBox, L"Modules");
  ModulesNotebook->AddPage(PluginBox, L"Plugins");
  ModulesNotebook->AddPage(ProjDir, L"ProjectFiles");

  // Editor Ribbons
  EditorNotebook = new wxAuiNotebook(this, wxID_ANY,
                                     wxDefaultPosition,
                                     wxSize(300, 300),
                                     wxAUI_NB_TOP |
                                         wxAUI_NB_TAB_SPLIT |
                                         wxAUI_NB_SCROLL_BUTTONS);

  //-- setup the IR editor
  IRPane = new wxStyledTextCtrl(this, wxID_ANY);
  IRPane->StyleClearAll();
  IRPane->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
  IRPane->SetTabWidth(3);
  IRPane->SetIndent(3);
  IRPane->SetUseTabs(false);
  IRPane->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
  IRPane->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
  IRPane->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
  IRPane->SetWrapMode(wxSTC_WRAP_WORD);
  IRPane->SetLexer(wxSTC_LEX_YAML);

  // -- set all the colors
  IRPane->StyleSetForeground(wxSTC_YAML_DEFAULT, *wxBLACK);
  IRPane->StyleSetForeground(wxSTC_YAML_COMMENT, *wxLIGHT_GREY);
  IRPane->StyleSetForeground(wxSTC_YAML_IDENTIFIER, *wxBLUE);
  IRPane->StyleSetForeground(wxSTC_YAML_KEYWORD, *wxGREEN);
  IRPane->StyleSetForeground(wxSTC_YAML_NUMBER, *wxGREEN);
  IRPane->StyleSetForeground(wxSTC_YAML_REFERENCE, *wxCYAN);
  IRPane->StyleSetForeground(wxSTC_YAML_DOCUMENT, *wxBLACK);
  IRPane->StyleSetForeground(wxSTC_YAML_TEXT, *wxBLACK);
  IRPane->StyleSetForeground(wxSTC_YAML_ERROR, *wxRED);
  IRPane->StyleSetForeground(wxSTC_YAML_OPERATOR, *wxBLUE);
  IRPane->StyleSetBold(wxSTC_YAML_IDENTIFIER, true);

  // -- set all the keywords
  IRPane->SetKeyWords(0, wxString(L0KeyWords.c_str()));
  IRPane->SetKeyWords(1, wxString(L1KeyWords.c_str()));
  IRPane->SetKeyWords(2, wxString(L2KeyWords.c_str()));
  EditorNotebook->AddPage(IRPane, L"IR Editor");

  // Add the panes to the manager
  Mgr.AddPane(ModulesNotebook, wxLEFT, wxT("Project Notebook"));
  Mgr.GetPane(ModulesNotebook).CloseButton(false);

  Mgr.AddPane(LogPane, wxBOTTOM, wxT("CoreGenPortal Log"));
  Mgr.AddPane(EditorNotebook, wxCENTER);
  Mgr.GetPane(EditorNotebook).CloseButton(false);
}

// PortalMainFrame::SetupPluginBox
// initializes the plugin box
void PortalMainFrame::SetupPluginBox(){
  // walk the plugin directory and derive our installed
  // set of plugins
  wxDir PluginDir(UserConfig->wxGetPluginDir());
  if (!PluginDir.IsOpened()){
    LogPane->AppendText("Could not open plugin directory at " +
                        UserConfig->wxGetPluginDir() + "\n");
  }else{
    LogPane->AppendText("Initializing plugin directories\n");
    wxString filename;
    unsigned pos = 0;
    bool cont = PluginDir.GetFirst(&filename, wxEmptyString, wxDIR_DIRS);
    while (cont){
      wxDir LPluginDir(UserConfig->wxGetPluginDir() + wxT("/") + filename);
      wxString FullPluginPath;
      if (LPluginDir.HasFiles(wxT("*.so"))){
        FullPluginPath = UserConfig->wxGetPluginDir() +
                         wxT("/") + filename +
                         wxT("/") + wxT("lib") + filename + wxT(".so");
        PluginPanes.push_back(std::make_pair(filename,
                                             FullPluginPath));
        LogPane->AppendText("Loaded Plugin Path: " + FullPluginPath + wxT("\n"));
        PluginBox->InsertItems(1, &filename, pos);
        pos = pos + 1;
      }else if (LPluginDir.HasFiles(wxT("*.dylib"))){
        FullPluginPath = UserConfig->wxGetPluginDir() +
                         wxT("/") + filename +
                         wxT("/") + wxT("lib") + filename + wxT(".dylib");
        PluginPanes.push_back(std::make_pair(filename,
                                             FullPluginPath));
        LogPane->AppendText("Loaded Plugin Path: " + FullPluginPath + wxT("\n"));
        PluginBox->InsertItems(1, &filename, pos);
        pos = pos + 1;
      }else{
        LogPane->AppendText("No plugin library found for " + filename);
      }
      cont = PluginDir.GetNext(&filename);
    }
  }
}

// PortalMainFrame::SetupModuleBox
// initializes the modulebox tree hierarchy
void PortalMainFrame::SetupModuleBox(){

  ParentModule = ModuleBox->AddRoot(wxT("Nodes"), -1, -1, NULL);

  TreeItems.push_back(ModuleBox->AppendItem(ModuleBox->GetRootItem(),
                                            wxT("Cache"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Comm"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Core"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Datapath"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Ext"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("ISA"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Inst"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("InstFormat"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("MCtrl"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Plugin"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("PseudoInst"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Reg"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("RegClass"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("SoC"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("Spad"),
                                            -1,
                                            -1,
                                            NULL));
  TreeItems.push_back(ModuleBox->AppendItem(ParentModule,
                                            wxT("VTP"),
                                            -1,
                                            -1,
                                            NULL));

  // connect the module box window handlers
  Bind(wxEVT_LISTBOX_DCLICK, &PortalMainFrame::OnSelectPlugin, this);
  Bind(wxEVT_TREE_ITEM_ACTIVATED, &PortalMainFrame::OnSelectNode, this);
  Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &PortalMainFrame::OnRightClickNode, this);
  Bind(wxEVT_TREE_ITEM_MIDDLE_CLICK, &PortalMainFrame::OnMiddleClickNode, this);
}

// PortalMainFrame::OnPasteText
// pastes the clipboard text to the current window
void PortalMainFrame::OnPasteText(wxCommandEvent &WXUNUSED(event)){
  if (!CGProject)
    return;

  if (wxTheClipboard->Open()){
    if (wxTheClipboard->IsSupported(wxDF_TEXT)){
      wxTextDataObject data;
      wxTheClipboard->GetData(data);
      wxStyledTextCtrl *SW = (wxStyledTextCtrl *)(EditorNotebook->GetPage(
          EditorNotebook->GetSelection()));
      SW->InsertText(SW->GetCurrentPos(), data.GetText());
    }
    wxTheClipboard->Close();
  }
}

// PortalMainFrame::OnCopyText
// copies the text from the current window to the clipboard
void PortalMainFrame::OnCopyText(wxCommandEvent &WXUNUSED(event)){
  if (!CGProject)
    return;

  if (wxTheClipboard->Open()){
    if (wxTheClipboard->IsSupported(wxDF_TEXT)){
      wxStyledTextCtrl *SW = (wxStyledTextCtrl *)(EditorNotebook->GetPage(
          EditorNotebook->GetSelection()));
      wxTheClipboard->AddData(
          new wxTextDataObject(SW->GetStringSelection()));
      wxTheClipboard->Flush();
    }
    wxTheClipboard->Close();
  }
}

// PortalMainFrame::FindNodeStr
// converts a node name into its appropriate Yaml text for searching
wxString PortalMainFrame::FindNodeStr(CoreGenNode *Parent){
  switch (Parent->GetType()){
  case CGSoc:
    return wxT("- Soc: ") + wxString(Parent->GetName());
    break;
  case CGCore:
    return wxT("- Core: ") + wxString(Parent->GetName());
    break;
  case CGDPath:
    return wxT("- Datapath: ") + wxString(Parent->GetName());
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
void PortalMainFrame::LoadModuleBox(bool editing){
  CoreGenNode *Top = CGProject->GetTop();

  if (Top == nullptr){
    LogPane->AppendText("Error loading modules...\n");
    return;
  }

  if (!editing)
    LogPane->AppendText("Loading modules...\n");

  for (unsigned i = 0; i < Top->GetNumChild(); i++){
    switch (Top->GetChild(i)->GetType()){
    case CGSoc:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_SOC],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGCore:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_CORE],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGDPath:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_DPATH],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
    case CGInstF:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_INSTFORMAT],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGInst:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_INST],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      LoadInstEncodings(NodeItems[NodeItems.size() - 1].first,
                        static_cast<CoreGenInst *>(Top->GetChild(i)));
      break;
    case CGPInst:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_PSEUDOINST],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      LoadPInstEncodings(NodeItems[NodeItems.size() - 1].first,
                         static_cast<CoreGenPseudoInst *>(Top->GetChild(i)));
      break;
    case CGRegC:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_REGCLASS],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGReg:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_REG],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGISA:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_ISA],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGCache:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_CACHE],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGComm:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_COMM],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGSpad:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_SPAD],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGMCtrl:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_MCTRL],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGVTP:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_VTP],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               -1,
                                                               -1,
                                                               NULL),
                                         Top->GetChild(i)));
      break;
    case CGPlugin:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_PLUGIN],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               wxTreeListCtrl::NO_IMAGE,
                                                               wxTreeListCtrl::NO_IMAGE,
                                                               NULL),
                                         Top->GetChild(i)));
      LoadPluginNodes(NodeItems[NodeItems.size() - 1].first,
                      static_cast<CoreGenPlugin *>(Top->GetChild(i)));
      break;
    case CGExt:
      NodeItems.push_back(std::make_pair(ModuleBox->AppendItem(TreeItems[TREE_NODE_EXT],
                                                               wxString(Top->GetChild(i)->GetName()),
                                                               wxTreeListCtrl::NO_IMAGE,
                                                               wxTreeListCtrl::NO_IMAGE,
                                                               NULL),
                                         Top->GetChild(i)));
      LoadExtNodes(NodeItems[NodeItems.size() - 1].first,
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
void PortalMainFrame::LoadExtNodes(wxTreeItemId Parent,
                                   CoreGenExt *Ext){
  std::vector<wxTreeItemId> wxExtItems;
  wxTreeItemId TmpItem;

  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Cache"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Comm"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Core"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Datapath"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Ext"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("ISA"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Inst"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("PseudoInst"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("InstFormat"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("MCtrl"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Reg"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("RegClass"),
                                             -1,
                                             -1,
                                             NULL));
  wxExtItems.push_back(ModuleBox->AppendItem(Parent,
                                             wxT("Spad"),
                                             -1,
                                             -1,
                                             NULL));

  // retrieve all the children from the ext and insert them
  // into the appropriate slot
  for (unsigned i = 0; i < Ext->GetNumChild(); i++){
    CoreGenNode *Child = Ext->GetChild(i);
    switch (Child->GetType()){
    case CGCache:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_CACHE],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGExt:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_EXT],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGInst:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_INST],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      TmpItem = std::get<0>(ExtItems[ExtItems.size() - 1]);
      LoadInstEncodings(TmpItem,
                        static_cast<CoreGenInst *>(Child));
    case CGPInst:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_PINST],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      TmpItem = std::get<0>(ExtItems[ExtItems.size() - 1]);
      LoadPInstEncodings(TmpItem,
                         static_cast<CoreGenPseudoInst *>(Child));
      break;
    case CGInstF:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_INSTFORMAT],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGReg:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_REG],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGRegC:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_REGCLASS],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGISA:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_ISA],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGComm:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_COMM],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGSpad:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_SPAD],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGMCtrl:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_MCTRL],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGCore:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_CORE],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    case CGDPath:
      ExtItems.push_back(std::make_tuple(
          ModuleBox->AppendItem(
              wxExtItems[TREE_EXT_NODE_DPATH],
              wxString(Child->GetName()),
              wxTreeListCtrl::NO_IMAGE,
              wxTreeListCtrl::NO_IMAGE,
              NULL),
          Ext, Child));
      break;
    default:
      LogPane->AppendText("Unable to load Ext child node into module tree\n");
      break;
    }
  }
}

// PortalMainFrame::LoadPluginNodes
// loads the wxTreeCtrl Plugin node with child nodes
void PortalMainFrame::LoadPluginNodes(wxTreeItemId Parent,
                                      CoreGenPlugin *Plugin){
  std::vector<wxTreeItemId> wxPluginItems;
  wxTreeItemId TmpItem;

  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Cache"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Core"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Datapath"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Inst"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("PseudoInst"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Reg"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("RegClass"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("SoC"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("ISA"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Ext"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Comm"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("Spad"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("MCtrl"),
                                                -1,
                                                -1,
                                                NULL));
  wxPluginItems.push_back(ModuleBox->AppendItem(Parent,
                                                wxT("VTP"),
                                                -1,
                                                -1,
                                                NULL));

  //-- cache
  std::vector<CoreGenCache *> CacheVect = Plugin->GetCacheVect();
  for (unsigned i = 0; i < CacheVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(CacheVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_CACHE],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- core
  std::vector<CoreGenCore *> CoreVect = Plugin->GetCoreVect();
  for (unsigned i = 0; i < CoreVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(CoreVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_CORE],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- data paths
  std::vector<CoreGenDataPath *> DPathVect = Plugin->GetDataPathVect();
  for (unsigned i = 0; i < DPathVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(DPathVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_DPATH],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- inst
  std::vector<CoreGenInst *> InstVect = Plugin->GetInstVect();
  for (unsigned i = 0; i < InstVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(InstVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_INST],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
    TmpItem = std::get<0>(PluginItems[PluginItems.size() - 1]);
    LoadInstEncodings(TmpItem,
                      static_cast<CoreGenInst *>(Child));
  }

  //-- pinst
  std::vector<CoreGenPseudoInst *> PInstVect = Plugin->GetPseudoInstVect();
  for (unsigned i = 0; i < PInstVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(PInstVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_PINST],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
    TmpItem = std::get<0>(PluginItems[PluginItems.size() - 1]);
    LoadPInstEncodings(TmpItem,
                       static_cast<CoreGenPseudoInst *>(Child));
  }

  //-- instformat
  std::vector<CoreGenInstFormat *> InstFVect = Plugin->GetInstFormatVect();
  for (unsigned i = 0; i < InstFVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(InstFVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_INSTFORMAT],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- reg
  std::vector<CoreGenReg *> RegVect = Plugin->GetRegVect();
  for (unsigned i = 0; i < RegVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(RegVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_REG],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- regclass
  std::vector<CoreGenRegClass *> RegCVect = Plugin->GetRegClassVect();
  for (unsigned i = 0; i < RegCVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(RegCVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_REGCLASS],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- soc
  std::vector<CoreGenSoC *> SocVect = Plugin->GetSocVect();
  for (unsigned i = 0; i < SocVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(SocVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_SOC],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- isa
  std::vector<CoreGenISA *> ISAVect = Plugin->GetISAVect();
  for (unsigned i = 0; i < ISAVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(ISAVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_ISA],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- ext
  std::vector<CoreGenExt *> ExtVect = Plugin->GetExtVect();
  for (unsigned i = 0; i < ExtVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(ExtVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_EXT],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
    TmpItem = std::get<0>(PluginItems[PluginItems.size() - 1]);
    LoadExtNodes(TmpItem,
                 static_cast<CoreGenExt *>(Child));
  }

  //-- comm
  std::vector<CoreGenComm *> CommVect = Plugin->GetCommVect();
  for (unsigned i = 0; i < CommVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(CommVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_COMM],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- spad
  std::vector<CoreGenSpad *> SpadVect = Plugin->GetSpadVect();
  for (unsigned i = 0; i < SpadVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(SpadVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_SPAD],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- mctrl
  std::vector<CoreGenMCtrl *> MCtrlVect = Plugin->GetMCtrlVect();
  for (unsigned i = 0; i < MCtrlVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(MCtrlVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_MCTRL],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }

  //-- vtp
  std::vector<CoreGenVTP *> VTPVect = Plugin->GetVTPVect();
  for (unsigned i = 0; i < VTPVect.size(); i++){
    CoreGenNode *Child = static_cast<CoreGenNode *>(VTPVect[i]);
    PluginItems.push_back(std::make_tuple(
        ModuleBox->AppendItem(
            wxPluginItems[TREE_PLUGIN_NODE_VTP],
            wxString(Child->GetName()),
            wxTreeListCtrl::NO_IMAGE,
            wxTreeListCtrl::NO_IMAGE,
            NULL),
        Plugin, Child));
  }
}

// PortalMainFrame::LoadInstEncodings
// loads the wxTreeCtrl Inst node with its child encodings
void PortalMainFrame::LoadInstEncodings(wxTreeItemId Parent,
                                        CoreGenInst *Inst){
  for (unsigned j = 0; j < Inst->GetNumEncodings(); j++){
    CoreGenEncoding *E = Inst->GetEncoding(j);
    EncItems.push_back(std::make_pair(ModuleBox->AppendItem(Parent,
                                                            wxString(E->GetName()),
                                                            -1,
                                                            -1,
                                                            NULL),
                                      E));
  }
}

// PortalMainFrame::LoadPInstEncodings
// loads the wxTreeCtrl PInst node with its child encodings
void PortalMainFrame::LoadPInstEncodings(wxTreeItemId Parent,
                                         CoreGenPseudoInst *PInst){
  for (unsigned j = 0; j < PInst->GetNumEncodings(); j++){
    CoreGenEncoding *E = PInst->GetEncoding(j);
    EncItems.push_back(std::make_pair(ModuleBox->AppendItem(Parent,
                                                            wxString(E->GetName()),
                                                            -1,
                                                            -1,
                                                            NULL),
                                      E));
  }
}

// PortalMainFrame::CloseProject
// closes any open project files
void PortalMainFrame::CloseProject(bool editing){
  if (!CGProject){
    return;
  }

  if (!editing)
    LogPane->AppendText("Closing open project...\n");

  // save the IR file
  IRPane->SaveFile(IRFileName);

  // clear the IR pane
  IRPane->ClearAll();

  // close out all the StoneCutter windows
  if (!editing)
    LogPane->AppendText("Removing Pages\n");
  for (size_t i = 1; i < EditorNotebook->GetPageCount(); i++) {
    if (!EditorNotebook->RemovePage(i))
      LogPane->AppendText("Error removing page\n");
    if (!EditorNotebook->DeletePage(i))
      LogPane->AppendText("Error deleting page\n");
  }

  SCPanes.clear();

  // close out all the modules
  NodeItems.clear();
  EncItems.clear();
  for (unsigned i = 0; i < TreeItems.size(); i++){
    ModuleBox->CollapseAndReset(TreeItems[i]);
  }
  ModuleBox->Collapse(ParentModule);

  // reset the file browser window
  ProjDir->SetPath(UserConfig->wxGetProjectDir());

  // delete all the stonecutter contexts
  for (unsigned i = 0; i < SCObjects.size(); i++) {
    if (std::get<1>(SCObjects[i]))
      delete std::get<1>(SCObjects[i]);
    if (std::get<2>(SCObjects[i]))
      delete std::get<2>(SCObjects[i]);
  }
  if (Msgs)
    delete Msgs;

  // delete the final bits
  delete CGProject;
  CGProject = nullptr;

  // log the close project
  if (!editing)
    LogPane->AppendText("Closed project\n");
}

// PortalMainFrame::AddNewlines
wxString PortalMainFrame::AddNewlines(wxStyledTextCtrl *CW){
  wxString Str;

  // we insert <pre> tags in the header and footer of text
  // this is a fun little trick to get the raw text to print
  // with newline characters attached using the wxHtmlEasyPrinting
  // class

  Str += "<pre>";
  for (unsigned i = 0; i < CW->GetNumberOfLines(); i++){
    Str += (CW->GetLineText(i) + "\n");
  }
  Str += "</pre>";
  return Str;
}

// PortalMainFrame::OnPrint
// handles the print request
void PortalMainFrame::OnPrint(wxCommandEvent &event){
  if (EditorNotebook->GetPageCount() == 0){
    LogPane->AppendText("No editor panes to print\n");
    return;
  }
  wxStyledTextCtrl *CW = (wxStyledTextCtrl *)(EditorNotebook->GetCurrentPage());
  if (!CW){
    LogPane->AppendText("No editor pane selected\n");
    return;
  }

  wxString PFileName = wxT("");

  // determine what the file name is
  if (CW == IRPane){
    // we're using the IR File
    PFileName = IRFileName;
  }else{
    // its an SCPane
    for (unsigned i = 0; i < SCPanes.size(); i++){
      if (SCPanes[i].first == CW){
        PFileName = SCPanes[i].second;
      }
    }
  }

  wxHtmlEasyPrinting *HP = new wxHtmlEasyPrinting(_("Source Code Printer"), this);
  HP->SetHeader(PFileName + wxT("(@PAGENUM@/@PAGESCNT@)<hr>"),
                wxPAGE_ALL);
  HP->PrintText(AddNewlines(CW), wxEmptyString);
  delete HP;

  return;
}

// PortalMainFrame::OnQuit
// handles quit signals to end the application
void PortalMainFrame::OnQuit(wxCommandEvent &event){
  int answer = wxMessageBox("Close CoreGenPortal?", "Confirm",
                            wxYES_NO | wxCANCEL, this);
  if (answer == wxYES){
    // close the project
    CloseProject();
    Close(true);
  }
}

// PortalMainFrame::OnAbout
// handles the Help->About command event
void PortalMainFrame::OnAbout(wxCommandEvent &event){
  CoreGenBackend *CGA = new CoreGenBackend("", "", "");
  int major = -1;
  int minor = -1;
  wxString MaStr;
  wxString MiStr;
  CGA->CoreGenVersion(&major, &minor);
  MaStr << major;
  MiStr << minor;

  wxMessageDialog *dial = new wxMessageDialog(NULL,
                                              wxT("CoreGenPortal Version ") +
                                                  PORTAL_VERSION_WX +
                                                  wxT(" \n") +
                                                  PORTAL_COPYRIGHT,
                                              wxT("About CoreGenPortal"),
                                              wxOK | wxICON_INFORMATION);
  dial->SetExtendedMessage(wxT("Built with CoreGen Version ") + MaStr + "." + MiStr);
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
  int pos = IRPane->FindText(0, IRPane->GetLastPosition(),
                             FindNodeStr(Node), 0);
  IRPane->GotoPos(pos);
  wxString line = IRPane->GetCurLine();
  unsigned int i;
  for (i = 0; i < line.size() && line[i] != '-'; i++)
    ;
  do{
    IRPane->LineDelete();
    line = IRPane->GetCurLine();
  } while (line.size() > 0 && line[i] != '-' && isspace(line[i]));
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
  CoreDepChildWin *ChildWin = nullptr;
  switch (event.GetId()){
  case ID_TREE_INFONODE:
    // open tree info node
    InfoWin = new CoreInfoWin(this, wxID_ANY,
                              GetNodeFromItem(ModuleBox->GetFocusedItem()),
                              GetNodeFromItem(ModuleBox->GetFocusedItem())->GetType(),
                              CGProject);
    delete InfoWin;
    break;
  case ID_TREE_DELNODE:
    DeleteNode(GetNodeFromItem(ModuleBox->GetFocusedItem()));
    break;
  case ID_TREE_CHILDNODE:
    ChildWin = new CoreDepChildWin(this,
                                   wxID_ANY,
                                   GetNodeFromItem(ModuleBox->GetFocusedItem()));
    ChildWin->ShowModal();
    delete ChildWin;
    break;
  case ID_TREE_ADDNODE:

    if (CGProject){
      wxTreeItemId ID = ModuleBox->GetFocusedItem();
      InfoWin = new CoreInfoWin(this, wxID_ANY, NULL, TreeIdToCGType(ID),CGProject);
      delete InfoWin;
    }else{
      LogPane->AppendText("No project open. You must open a project before you can add a node.\n");
    }
    break;
  }
}

CGNodeType PortalMainFrame::TreeIdToCGType(wxTreeItemId ID){
  unsigned i;
  for (i = 0; i < TreeItems.size(); i++){
    if (TreeItems[i] == ID)
      break;
  }

  switch (i){
  case 0:
    return CGCache;
  case 1:
    return CGComm;
  case 2:
    return CGCore;
  case 3:
    return CGDPath;
  case 4:
    return CGExt;
  case 5:
    return CGISA;
  case 6:
    return CGInst;
  case 7:
    return CGInstF;
  case 8:
    return CGMCtrl;
  case 9:
    return CGPlugin;
  case 10:
    return CGPInst;
  case 11:
    return CGReg;
  case 12:
    return CGRegC;
  case 13:
    return CGSoc;
  case 14:
    return CGSpad;
  case 15:
    return CGVTP;
  default:
    return CGTop;
  }
}

// PortalMainFrame::OnRightClickNode
void PortalMainFrame::OnRightClickNode(wxTreeEvent &event){
  wxMenu mnu;
  if (ModuleBox->GetItemParent(ModuleBox->GetFocusedItem()) == ParentModule){
    // this is a node descriptor type
    mnu.Append(ID_TREE_ADDNODE, "Add Node");
  }else{
    // this is an actual node
    mnu.Append(ID_TREE_INFONODE,  "Node Info");
    mnu.Append(ID_TREE_DELNODE,   "Delete Node");
    mnu.Append(ID_TREE_CHILDNODE, "Children");
  }
  mnu.Connect(wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(PortalMainFrame::OnPopupNode),
              NULL,
              this);
  PopupMenu(&mnu);
}

// PortalMainFrame:GetItemFromNode
wxTreeItemId PortalMainFrame::GetItemFromNode(CoreGenNode *Node){
  wxTreeItemId Id;

  // walk all the main nodes
  for (unsigned i = 0; i < NodeItems.size(); i++){
    if (NodeItems[i].second == Node)
      return NodeItems[i].first;
  }

  // walk the encoding nodes
  for (unsigned i = 0; i < EncItems.size(); i++){
    if (EncItems[i].second == Node)
      return EncItems[i].first;
  }

  // walk the ext nodes
  for (unsigned i = 0; i < ExtItems.size(); i++){
    if (std::get<2>(ExtItems[i]) == Node)
      return std::get<0>(ExtItems[i]);
  }

  // walk the plugin nodes
  for (unsigned i = 0; i < PluginItems.size(); i++){
    if (std::get<2>(PluginItems[i]))
      return std::get<0>(PluginItems[i]);
  }

  return Id;
}

// PortalMainFrame::GetNodeFromItem
CoreGenNode *PortalMainFrame::GetNodeFromItem(wxTreeItemId SelId){
  if (!SelId.IsOk()){
    LogPane->AppendText("Error: could not derive node from selection\n");
    return nullptr;
  }

  // walk the main nodes
  for (unsigned i = 0; i < NodeItems.size(); i++){
    if (NodeItems[i].first == SelId)
      return NodeItems[i].second;
  }

  // walk the encoding nodes
  for (unsigned i = 0; i < EncItems.size(); i++){
    if (EncItems[i].first == SelId){
      wxTreeItemId ParentId = ModuleBox->GetItemParent(EncItems[i].first);
      for (unsigned j = 0; j < NodeItems.size(); j++){
        if (NodeItems[j].first == ParentId)
          return NodeItems[j].second;
      }
    }
  }

  // walk the Ext nodes
  for (unsigned i = 0; i < ExtItems.size(); i++){
    wxTreeItemId TmpItem = std::get<0>(ExtItems[i]);
    if (TmpItem == SelId){
      return std::get<2>(ExtItems[i]);
    }
  }

  // walk the plugin nodes
  for (unsigned i = 0; i < PluginItems.size(); i++){
    wxTreeItemId TmpItem = std::get<0>(PluginItems[i]);
    if (TmpItem == SelId){
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

  if (!SelId.IsOk()){
    LogPane->AppendText("Error: Could not select node\n");
    return;
  }

  // we have a valid node, search for its corresponding object
  CoreGenNode *Node = GetNodeFromItem(SelId);
  if (Node == nullptr){
    LogPane->AppendText("Error : node object is null\n");
    return;
  }
  int pos = IRPane->FindText(0, IRPane->GetLastPosition(),
                             FindNodeStr(Node), 0);
  IRPane->GotoPos(pos);
}

// PortalMainFrame::OpenFile
void PortalMainFrame::OpenFileFromWin(wxString Path){
  wxFileName NPF(Path);
  wxString Ext = NPF.GetExt();

  if (Ext.IsSameAs(wxT("sc"), false)){
    // open a new stonecutter window
    OpenSCFile(Path, NPF);
  }else if (Ext.IsSameAs(wxT("yaml"), false)){
    // open a new yaml window (not a new project)
    OpenYamlFile(Path, NPF);
  }else{
    LogPane->AppendText("Could not open unknown file type at " +
                        Path + wxT("\n"));
  }
}

// PortalMainFrame::OpenYamlFile
void PortalMainFrame::OpenYamlFile(wxString NP, wxFileName NPF){
  LogPane->AppendText("Opening Yaml file at " +
                      NPF.GetFullName() + wxT("\n"));

  // check to see if the window is already open
  wxString TmpName = NPF.GetFullName();
  size_t TmpPage = -1;
  for (unsigned i = 0; i < SCPanes.size(); i++){
    if (std::get<1>(SCPanes[i]) == TmpName)
      TmpPage = i + 1;
  }

  if (TmpPage != -1){
    // page already exists, refocus to the target tab
    LogPane->AppendText("File is already open... refocusing to the appropriate tab\n");
    EditorNotebook->SetSelection(TmpPage);
  }else{
    // create a new window
    wxStyledTextCtrl *SCPane = new wxStyledTextCtrl(this, wxID_ANY);

    SCPane->StyleClearAll();
    SCPane->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
    SCPane->SetTabWidth(3);
    SCPane->SetIndent(3);
    SCPane->SetUseTabs(false);
    SCPane->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
    SCPane->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
    SCPane->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
    SCPane->SetWrapMode(wxSTC_WRAP_WORD);
    SCPane->SetLexer(wxSTC_LEX_YAML);

    // -- set all the colors
    SCPane->StyleSetForeground(wxSTC_YAML_DEFAULT, *wxBLACK);
    SCPane->StyleSetForeground(wxSTC_YAML_COMMENT, *wxLIGHT_GREY);
    SCPane->StyleSetForeground(wxSTC_YAML_IDENTIFIER, *wxBLUE);
    SCPane->StyleSetForeground(wxSTC_YAML_KEYWORD, *wxGREEN);
    SCPane->StyleSetForeground(wxSTC_YAML_NUMBER, *wxGREEN);
    SCPane->StyleSetForeground(wxSTC_YAML_REFERENCE, *wxCYAN);
    SCPane->StyleSetForeground(wxSTC_YAML_DOCUMENT, *wxBLACK);
    SCPane->StyleSetForeground(wxSTC_YAML_TEXT, *wxBLACK);
    SCPane->StyleSetForeground(wxSTC_YAML_ERROR, *wxRED);
    SCPane->StyleSetForeground(wxSTC_YAML_OPERATOR, *wxBLUE);
    SCPane->StyleSetBold(wxSTC_YAML_IDENTIFIER, true);

    // -- set all the keywords
    SCPane->SetKeyWords(0, wxString(L0KeyWords.c_str()));
    SCPane->SetKeyWords(1, wxString(L1KeyWords.c_str()));
    SCPane->SetKeyWords(2, wxString(L2KeyWords.c_str()));

    // the load file
    SCPane->LoadFile(NP);

    // add it to our vector
    SCPanes.push_back(std::make_pair(SCPane, NPF.GetFullName()));

    // add the file to the editor network
    EditorNotebook->AddPage(SCPane, NP, true, wxBookCtrlBase::NO_IMAGE);

    // reset the tab title
    EditorNotebook->SetPageText(EditorNotebook->GetPageCount() - 1,
                                NPF.GetName());
  }
}

// PortalMainFrame::OpenSCFile
void PortalMainFrame::OpenSCFile(wxString NP, wxFileName NPF){
  LogPane->AppendText("Opening StoneCutter file at " +
                      NPF.GetFullName() + wxT("\n"));

  // check to see if the window is already open
  wxString TmpName = NPF.GetFullName();
  size_t TmpPage = -1;
  for (unsigned i = 0; i < SCPanes.size(); i++){
    if (std::get<1>(SCPanes[i]) == TmpName)
      TmpPage = i + 1;
  }

  if (TmpPage != -1){
    // page already exists, refocus to the target tab
    LogPane->AppendText("File is already open... refocusing to the appropriate tab\n");
    EditorNotebook->SetSelection(TmpPage);
  }else{
    // create a new window
    wxStyledTextCtrl *SCPane = new wxStyledTextCtrl(this, wxID_ANY);
    SCPane->StyleClearAll();
    SCPane->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
    SCPane->SetTabWidth(3);
    SCPane->SetIndent(3);
    SCPane->SetUseTabs(false);
    SCPane->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
    SCPane->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
    SCPane->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
    SCPane->SetWrapMode(wxSTC_WRAP_WORD);
    SCPane->SetLexer(wxSTC_LEX_CPP);

    SCPane->StyleSetForeground(wxSTC_C_STRING, wxColour(150, 0, 0));
    SCPane->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(165, 105, 0));
    SCPane->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(40, 0, 60));
    SCPane->StyleSetForeground(wxSTC_C_NUMBER, wxColour(0, 150, 0));
    SCPane->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(150, 0, 0));
    SCPane->StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 150));
    SCPane->StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 150, 0));
    SCPane->StyleSetForeground(wxSTC_C_COMMENT, wxColour(150, 150, 150));
    SCPane->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(150, 150, 150));
    SCPane->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(150, 150, 150));
    SCPane->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 0, 200));
    SCPane->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 0, 200));
    SCPane->StyleSetBold(wxSTC_C_WORD, true);
    SCPane->StyleSetBold(wxSTC_C_WORD2, true);
    SCPane->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

    // the load file
    SCPane->LoadFile(NP);

    // add it to our vector
    SCPanes.push_back(std::make_pair(SCPane, NPF.GetFullName()));

    // add the file to the editor network
    EditorNotebook->AddPage(SCPane, NP, true, wxBookCtrlBase::NO_IMAGE);

    // reset the tab title
    EditorNotebook->SetPageText(EditorNotebook->GetPageCount() - 1,
                                NPF.GetName());
  }
}

// PortalMainFrame::OnSelectPlugin
void PortalMainFrame::OnSelectPlugin(wxCommandEvent &event){
  if (!CGProject){
    LogPane->AppendText("Cannot load plugin; no project open\n");
    return;
  }
  int Plugin = PluginBox->GetSelection();
  if ((unsigned)(Plugin) > (PluginPanes.size() - 1)){
    LogPane->AppendText("Invalid plugin item\n");
  }else{
    wxString PName = std::get<0>(PluginPanes[(unsigned)(Plugin)]);
    wxString PPath = std::get<1>(PluginPanes[(unsigned)(Plugin)]);
    if (!CGProject->LoadPlugin(std::string(PPath.mb_str()))){
      LogPane->AppendText(wxT("Failed to load plugin at ") + PPath + wxT("\n"));
      LogPane->AppendText(wxString(CGProject->GetErrStr()));
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
      if (PB->ShowModal() == wxID_OK){
        PB->Destroy();
        if (!CGProject->ReleasePlugin(PID)){
          LogPane->AppendText(wxT("Failed to release plugin at ") + PPath + wxT("\n"));
          LogPane->AppendText(wxString(CGProject->GetErrStr()));
        }
      }
    }
  }
}

// PortalMainFrame::OnSelectNode
void PortalMainFrame::OnSelectNode(wxTreeEvent &event){
  switch (ModulesNotebook->GetSelection())
  {
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
                                                  wxSize(500, 500),
                                                  wxDEFAULT_DIALOG_STYLE | wxVSCROLL,
                                                  VerifConfig);
  if (VP->ShowModal() == wxID_OK){
    LogPane->AppendText("Committed verification preferences\n");
  }
  VP->Destroy();
}

// PortalMainFram::OnSCPref
void PortalMainFrame::OnSCPref(wxCommandEvent &event){
  LogPane->AppendText("Loading StoneCutter compiler preferences...\n");
  PortalSCPrefWin *SP = new PortalSCPrefWin(this,
                                            wxID_ANY,
                                            wxT("StoneCutter Preferences"),
                                            wxDefaultPosition,
                                            wxSize(500, 500),
                                            wxDEFAULT_DIALOG_STYLE | wxVSCROLL,
                                            SCConfig);
  if (SP->ShowModal() == wxID_OK){
    LogPane->AppendText("Committed StoneCutter compiler preferences\n");
  }
  SP->Destroy();
}

// PortalMainFrame::OnUserPref
void PortalMainFrame::OnUserPref(wxCommandEvent &event){
  LogPane->AppendText("Loading user preferences...\n");
  PortalUserPrefWin *UP = new PortalUserPrefWin(this,
                                                wxID_ANY,
                                                wxT("User Preferences"),
                                                wxDefaultPosition,
                                                wxSize(500, 500),
                                                wxDEFAULT_DIALOG_STYLE | wxVSCROLL,
                                                UserConfig);
  if (UP->ShowModal() == wxID_OK){
    LogPane->AppendText("Committed user preferences\n");
  }
  UP->Destroy();
}

// PortalMainFrame::OnProjSave
void PortalMainFrame::OnProjSave(wxCommandEvent& WXUNUSED(event)){
  if (!CGProject){
    LogPane->AppendText("No project is open!\n");
    return;
  }

  CGProject->WriteIR(std::string(IRFileName.mb_str()));
  IRPane->LoadFile(IRFileName);
}


// PortalMainFrame::OnProjSaveAs
void PortalMainFrame::OnProjSaveAs(wxCommandEvent& WXUNUSED(event)){
  if (!CGProject){
    LogPane->AppendText("No project is open!\n");
    return;
  }

  wxFileDialog saveFileDialog(this,
                              _("Save Project As"),
                              UserConfig->wxGetProjectDir(),
                              wxEmptyString,
                              "Yaml Design Files (*.yaml)|*.yaml",
                              wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

  if( saveFileDialog.ShowModal() == wxID_CANCEL )
    return ;

  // create the new directory
  LogPane->AppendText("Saving project to " + saveFileDialog.GetPath() + wxT("\n") );

  // close the current project
  IRPane->SaveFile(IRFileName);
  CloseProject(true);
  ModuleBox->DeleteAllItems();
  TreeItems.clear();
  NodeItems.clear();

  // copy the top-level design file over
  wxCopyFile(IRFileName,saveFileDialog.GetPath());
  IRFileName = saveFileDialog.GetPath();
  wxFileName TName;
  TName.Assign(IRFileName);
  TName.MakeAbsolute();

  // open the new project
  SetupModuleBox();
  OpenProject(IRFileName, true);

  // reset the project's path in the yaml file
  if( CGProject ){
    if( !CGProject->GetProj()->SetProjRoot(std::string(TName.GetPath().mb_str())) ){
      LogPane->AppendText("Error resetting project root to " + TName.GetPath() + wxT("\n"));
    }
    CGProject->WriteIR(std::string(IRFileName.mb_str()));
    IRPane->LoadFile(IRFileName);
  }
}

// PortalMainFrame::OnProjSaveFile
void PortalMainFrame::OnProjSaveFile(wxCommandEvent &event){
  if (!CGProject){
    LogPane->AppendText("No project is open!\n");
    return;
  }

  wxStyledTextCtrl *SW = (wxStyledTextCtrl *)(EditorNotebook->GetPage(
      EditorNotebook->GetSelection()));
  if (SW == IRPane){
    // write out the new IR file
    IRPane->SaveFile(IRFileName);
    CloseProject(true);
    ModuleBox->DeleteAllItems();
    TreeItems.clear();
    NodeItems.clear();
    SetupModuleBox();
    OpenProject(IRFileName, true);
    LogPane->AppendText("Updated modules\n");
  }else{
    // just save the file
    for (unsigned i = 0; i < SCPanes.size(); i++){
      if (std::get<0>(SCPanes[i]) == SW){
        SW->SaveFile(std::get<1>(SCPanes[i]));
      }
    }
  }
}

// PortalMainFrame::OnProjSpecDoc
void PortalMainFrame::OnProjSpecDoc(wxCommandEvent &event){
  if (!CGProject){
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // create the directory
  wxString FullPath = ProjDir->GetPath() + wxT("/spec/");
  if (!wxDirExists(FullPath)){
    LogPane->AppendText("Creating spec directory at " + FullPath + wxT("\n"));
    if (!wxFileName::Mkdir(FullPath, wxS_DIR_DEFAULT, 0)){
      LogPane->AppendText("Error creating spec directory at " + FullPath + wxT("\n"));
      return;
    }
  }

  // refresh the project window
  ProjDir->ReCreateTree();

  // build the dag
  if (!CGProject->BuildDAG()){
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // init the pass manager
  if (!CGProject->InitPassMgr()){
    LogPane->AppendText("Error initializing the CoreGen pass manager\n");
    return;
  }

  LogPane->AppendText("Building specification document...\n");

  // Set the pass output path
  if (!CGProject->SetPassOutputPath("SpecDoc",
                                    std::string(FullPath.mb_str()))){
    LogPane->AppendText("Error initializing SpecDoc pass output\n");
    LogPane->AppendText(wxString(CGProject->GetErrStr()) + wxT("\n"));
    return;
  }

  // setup the text redirector
  std::streambuf *oldBuf = std::cout.rdbuf();
  std::ostringstream newBuf;
  std::cout.rdbuf(newBuf.rdbuf());

  bool failed = false;

  // Execute the pass
  if (!CGProject->ExecuteSysPass("SpecDoc")){
    LogPane->AppendText("Error executing SpecDoc\n");
    LogPane->AppendText(wxString(CGProject->GetErrStr()) + wxT("\n"));
    failed = true;
  }

  // restore the old cout buffer
  std::cout.rdbuf(oldBuf);
  if (failed){
    return;
  }

  // else, display the results
  CoreSpecDocWin *SW = new CoreSpecDocWin(this,
                                          wxID_ANY,
                                          wxT("Specification Document"),
                                          &newBuf);
  if (SW->ShowModal() == wxID_OK){
    SW->Destroy();
  }
}

// PortalMainFrame::OnProjSummary
void PortalMainFrame::OnProjSummary(wxCommandEvent &event){
  if (!CGProject){
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // build the dag
  if (!CGProject->BuildDAG()){
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // init the pass manager
  if (!CGProject->InitPassMgr()){
    LogPane->AppendText("Error initializing the CoreGen pass manager\n");
    return;
  }

  LogPane->AppendText("Building project summary...\n");

  // setup the text redirector
  std::streambuf *oldBuf = std::cout.rdbuf();
  std::ostringstream newBuf;
  std::cout.rdbuf(newBuf.rdbuf());

  // Execute the pass
  if (!CGProject->ExecutePass("StatsPass")){
    LogPane->AppendText("Error executing StatsPass\n");
  }

  // restore the old cout buffer
  std::cout.rdbuf(oldBuf);
  CoreStatsWin *SW = new CoreStatsWin(this,
                                      wxID_ANY,
                                      wxT("Project Summary"),
                                      &newBuf);
  if (SW->ShowModal() == wxID_OK){
    SW->Destroy();
  }
}

// PortalMainFrame::OnBuildSigmap
void PortalMainFrame::OnBuildSigmap(wxCommandEvent &event){
  if (!CGProject){
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // build the dag
  if (!CGProject->BuildDAG()){
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // determine if the target stonecutter directory exists
  wxString FullPath = ProjDir->GetPath() + wxT("/RTL/stonecutter/");
  if (!wxDirExists(FullPath)){
    LogPane->AppendText("StoneCutter path does not exist: " + FullPath + "\n");
    return;
  }

  // walk the ~/Project/RTL/stonecutter directory and discover all the
  // stonecutter source files
  wxString SCFile = wxFindFirstFile(FullPath + "*.sc");
  while (!SCFile.empty()){
    bool isFound = false;
    unsigned Idx = 0;
    for (unsigned i = 0; i < SCObjects.size(); i++){
      if (std::get<0>(SCObjects[i]) == SCFile){
        isFound = true;
        Idx = i;
      }
    }

    // split the file name into its constituent parts
    wxString RawPath;
    wxString RawName;
    wxString RawExt;

    wxFileName::SplitPath(SCFile, &RawPath, &RawName, &RawExt);

    if (!isFound){

      // create a new SCOpts context
      SCOpts *SCO = new SCOpts(Msgs);
      if( !InitSCOpts(SCO) ){
        LogPane->AppendText( "Failed to initialize StoneCutter options for signal map\n" );
        delete SCO;
        return ;
      }

      // set all the options
      std::string OutFile = std::string(ProjDir->GetPath().mb_str()) +
                            "/RTL/stonecutter/" +
                            std::string(RawName.mb_str()) + ".yaml";
      SCO->PurgeInputFiles();
      SCO->SetInputFile(std::string(SCFile.mb_str()));
      SCO->SetSignalMap(OutFile);
      SCO->UnsetChisel();

      // create a new SCExec context
      SCExec *SCE = new SCExec(SCO, Msgs);

      // add it to the vector
      SCObjects.push_back(std::make_tuple(SCFile, SCO, SCE));
    }else{
      SCOpts *SCO = std::get<1>(SCObjects[Idx]);
      if( !InitSCOpts(SCO) ){
        LogPane->AppendText( "Failed to initialize StoneCutter options for signal map\n" );
        delete SCO;
        return ;
      }

      std::string OutFile = std::string(ProjDir->GetPath().mb_str()) +
                            "/RTL/stonecutter/" +
                            std::string(RawName.mb_str()) + ".yaml";
      SCO->PurgeInputFiles();
      SCO->SetInputFile(std::string(SCFile.mb_str()));
      SCO->SetSignalMap(OutFile);
      SCO->UnsetChisel();
    }

    // find the next file
    SCFile = wxFindNextFile();
  }

  // setup the text redirector
  std::streambuf *oldBuf = std::cout.rdbuf();
  std::ostringstream newBuf;
  std::cout.rdbuf(newBuf.rdbuf());
  // execute each of the signal map generators
  for (unsigned i = 0; i < SCObjects.size(); i++) {
    SCExec *SCE = std::get<2>(SCObjects[i]);
    bool Success = SCE->Exec();
    LogPane->AppendText(wxString(newBuf.str()) + wxT("\n"));
    if (!Success){
      LogPane->AppendText("Failed to build signal map from " +
                          std::get<0>(SCObjects[i]) + wxT("\n"));
    }else{
      LogPane->AppendText("Successfully built signal map from " +
                          std::get<0>(SCObjects[i]) + wxT("\n"));
    }
  }
  // restore the old cout buffer
  std::cout.rdbuf(oldBuf);
  ProjDir->ReCreateTree();
}

// PortalMainFrame::InitSCOpts
bool PortalMainFrame::InitSCOpts(SCOpts *Opts){
  // set all the standard options
  if (SCConfig->IsParseEnabled()){
    Opts->UnsetCG();
  }else{
    Opts->SetCG();
  }
  if (SCConfig->IsKeepEnabled()){
    Opts->SetKeep();
  }else{
    Opts->UnsetKeep();
  }

  // set the optimization options
  if (SCConfig->GetOptLevel() == 0){
    // optimize = false
    Opts->UnsetOptimize();
    // pipeline = false
    Opts->UnsetPipeline();
  }

  if (SCConfig->GetOptLevel() == 1){
    // optimize = true
    Opts->SetOptimize();
    // scenable = true, but disable all passes
    Opts->UnsetSCPasses();
    // pipeline = false
    Opts->UnsetPipeline();
  }

  if (SCConfig->GetOptLevel() == 2){
    // optimize = true
    Opts->SetOptimize();
    // scenable = true
    Opts->DisableManualSCPasses();
    // pipeline = false
    Opts->UnsetPipeline();
  }

  if (SCConfig->GetOptLevel() == 3){
    // optimize = true
    Opts->SetOptimize();
    // scenable = true
    Opts->DisableManualSCPasses();
    // pipeline = true
    Opts->SetPipeline();
  }

  // TODO: set the individual pass options

  // TODO: Add pipeliner options

  return true;
}

// PortalMainFrame::OnBuildStoneCutter
void PortalMainFrame::OnBuildStoneCutter(wxCommandEvent &event)
{
  if (!CGProject)
  {
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // build the dag
  if (!CGProject->BuildDAG())
  {
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // determine if the target stonecutter directory exists
  wxString FullPath = ProjDir->GetPath() + wxT("/RTL/stonecutter/");
  if (!wxDirExists(FullPath))
  {
    LogPane->AppendText("StoneCutter path does not exist: " + FullPath + "\n");
    return;
  }

  // walk the ~/Project/RTL/stonecutter directory and discover all the
  // stonecutter source files
  wxString SCFile = wxFindFirstFile(FullPath + "*.sc");
  while (!SCFile.empty())
  {
    bool isFound = false;
    unsigned Idx = 0;
    for (unsigned i = 0; i < SCObjects.size(); i++)
    {
      if (std::get<0>(SCObjects[i]) == SCFile)
      {
        isFound = true;
        Idx = i;
      }
    }

    // split the file name into its constituent parts
    wxString RawPath;
    wxString RawName;
    wxString RawExt;

    wxFileName::SplitPath(SCFile, &RawPath, &RawName, &RawExt);

    if (!isFound)
    {

      // create a new SCOpts context
      SCOpts *SCO = new SCOpts(Msgs);
      if (!InitSCOpts(SCO))
      {
        LogPane->AppendText("Failed to initialize StoneCutter options\n");
        delete SCO;
        return;
      }

      // set all the options
      std::string OutFile = std::string(ProjDir->GetPath().mb_str()) +
                            "/RTL/chisel/src/main/scala/" +
                            std::string(RawName.mb_str()) + ".chisel";
      SCO->PurgeInputFiles();
      SCO->SetInputFile(std::string(SCFile.mb_str()));
      SCO->SetOutputFile(OutFile);
      SCO->SetChisel();

      // create a new SCExec context
      SCExec *SCE = new SCExec(SCO, Msgs);

      // add it to the vector
      SCObjects.push_back(std::make_tuple(SCFile, SCO, SCE));
    }
    else
    {
      SCOpts *SCO = std::get<1>(SCObjects[Idx]);
      if (!InitSCOpts(SCO))
      {
        LogPane->AppendText("Failed to initialize StoneCutter options\n");
        return;
      }
      std::string OutFile = std::string(ProjDir->GetPath().mb_str()) +
                            "/RTL/chisel/src/main/scala/" +
                            std::string(RawName.mb_str()) + ".chisel";
      SCO->PurgeInputFiles();
      SCO->SetInputFile(std::string(SCFile.mb_str()));
      SCO->SetOutputFile(OutFile);
      SCO->SetChisel();
      SCO->UnsetSignalMap();
    }

    // find the next file
    SCFile = wxFindNextFile();
  }

  // now that we've build the entire list of SCExec objects,
  // execute each one individually
  for (unsigned i = 0; i < SCObjects.size(); i++)
  {
    // setup the text redirector
    std::streambuf *oldBuf = std::cout.rdbuf();
    std::ostringstream newBuf;
    std::cout.rdbuf(newBuf.rdbuf());

    SCExec *SCE = std::get<2>(SCObjects[i]);
    bool Success = SCE->Exec();
    LogPane->AppendText(wxString(newBuf.str()) + wxT("\n"));
    if (!Success)
    {
      LogPane->AppendText("Failed to compile StoneCutter from " +
                          std::get<0>(SCObjects[i]) + wxT("\n"));
    }
    else
    {
      LogPane->AppendText("Successfully compiled StoneCutter from " +
                          std::get<0>(SCObjects[i]) + wxT("\n"));
    }

    // restore the old cout buffer
    std::cout.rdbuf(oldBuf);
  }
  ProjDir->ReCreateTree();
}

// PortalMainFrame::OnBuildLLVMCodegen
void PortalMainFrame::OnBuildLLVMCodegen(wxCommandEvent &event)
{
  if (!CGProject)
  {
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // build the dag
  if (!CGProject->BuildDAG())
  {
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }
  // execute the codegen
  if (!CGProject->ExecuteLLVMCodegen(LLVMComp))
  {
    LogPane->AppendText(wxString(CGProject->GetErrStr()) + wxT("\n"));
    LogPane->AppendText("Failed to execute LLVM codegen!\n");
  }
  else
  {
    LogPane->AppendText("Successfully executed LLVM codegen\n");
  }

  ProjDir->ReCreateTree();
}

// PortalMainFrame::OnBuildCodegen
void PortalMainFrame::OnBuildCodegen(wxCommandEvent &event)
{
  if (!CGProject)
  {
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // build the dag
  if (!CGProject->BuildDAG())
  {
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // execute the codegen
  if (!CGProject->ExecuteChiselCodegen())
  {
    LogPane->AppendText(wxString(CGProject->GetErrStr()) + wxT("\n"));
    LogPane->AppendText("Failed to execute Chisel codegen!\n");
  }
  else
  {
    LogPane->AppendText("Successfully executed Chisel codegen\n");
  }
  ProjDir->ReCreateTree();
}

// PortalMainFrame::OnBuildVerify
void PortalMainFrame::OnBuildVerify(wxCommandEvent &event)
{
  if (!CGProject)
  {
    LogPane->AppendText("No project is open!\n");
    return;
  }

  // check the verification config options
  if (!VerifConfig)
  {
    LogPane->AppendText("Error: verification preferences are not initialized\n");
    return;
  }

  if (!VerifConfig->isValid())
  {
    LogPane->AppendText("Error: verification preferences are not valid\n");
    return;
  }

  // build the dag
  if (!CGProject->BuildDAG())
  {
    LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // init the pass manager
  if (!CGProject->InitPassMgr())
  {
    LogPane->AppendText("Error initializing the CoreGen pass manager\n");
    return;
  }

  LogPane->AppendText("Executing verification passes...\n");

  // setup the text redirector
  std::streambuf *oldBuf = std::cout.rdbuf();
  std::ostringstream newBuf;
  std::cout.rdbuf(newBuf.rdbuf());

  // execute all the enabled passes
  for (unsigned i = 0; i < VerifConfig->GetNumPasses(); i++)
  {
    if (VerifConfig->IsPassEnabled(i))
    {
      if (!CGProject->ExecutePass(
              std::string(VerifConfig->GetPassName(i).mb_str())))
      {
        LogPane->AppendText("Error executing pass: " +
                            VerifConfig->GetPassName(i) +
                            wxT("\n"));
      }
    }
  }

  // restore the old cout buffer
  std::cout.rdbuf(oldBuf);
  CoreVerifWin *VW = new CoreVerifWin(this,
                                      wxID_ANY,
                                      wxT("Verification Results"),
                                      &newBuf);
  if (VW->ShowModal() == wxID_OK)
  {
    VW->Destroy();
  }
}

// PortalMainFrame::OnProjNew
void PortalMainFrame::OnProjNew(wxCommandEvent &event)
{
  PortalNewProjWin *NP = new PortalNewProjWin(this,
                                              wxID_ANY,
                                              wxT("New Project"),
                                              wxDefaultPosition,
                                              wxSize(500, 500),
                                              wxDEFAULT_DIALOG_STYLE | wxVSCROLL,
                                              ProjDir,
                                              LogPane,
                                              IRPane,
                                              UserConfig);

  if (NP->ShowModal() == wxID_OK)
  {
    CGProject = NP->GetCoreGenPtr();
    IRFileName = NP->GetIRFileName();
    NP->Destroy();
  }
}

// PortalMainFrame::OnVizInstFormat
void PortalMainFrame::OnVizInstFormat(wxCommandEvent& WXUNUSED(event)){
  if( !CGProject ){
    LogPane->AppendText( "No project open\n" );
    return ;
  }

  // retrieve the selected node
  wxTreeItemId SelId = ModuleBox->GetFocusedItem();

  if (!SelId.IsOk()){
    LogPane->AppendText("Error: Could not select node\n");
    return;
  }

  // we have a valid node, search for its corresponding object
  CoreGenNode *Node = GetNodeFromItem(SelId);
  if (Node == nullptr){
    LogPane->AppendText("Error : node object is null\n");
    return;
  }
  if( Node->GetType() != CGInstF ){
    LogPane->AppendText("Error : select a valid instruction format node\n");
    return;
  }

  CoreGenInstFormat *IF = static_cast<CoreGenInstFormat *>(Node);

  // generate a new image for the instruction format
  std::string ImgPath;
  PortalViz *Viz = new PortalViz();
  if( !Viz->GenerateInstFormatImg(IF,ImgPath) ){
    LogPane->AppendText( "Could not visualize instruction format\n" );
    delete Viz;
    return ;
  }

  // visualize it
  InstFormatVizWin *PV = new InstFormatVizWin( this,
                                               wxID_ANY,
                                               wxT("Instruction Format Visualization"),
                                               wxDefaultPosition,
                                               wxSize(500,100),
                                               wxDEFAULT_DIALOG_STYLE|wxVSCROLL|wxHSCROLL,
                                               wxString(ImgPath) );
  PV->ShowModal();
  PV->Destroy();

  delete Viz;
}

// PortalMainFrame::OnVizPipeline
void PortalMainFrame::OnVizPipeline(wxCommandEvent& WXUNUSED(event)){
  if( !CGProject ){
    LogPane->AppendText( "No project open\n" );
    return ;
  }

  // retrieve the selected signal map
  std::string SM = std::string(ProjDir->GetFilePath().mb_str());
  if( SM.length() == 0 ){
    LogPane->AppendText( "No signal map selected\n" );
  }

  // attempt to read it
  CoreGenSigMap *SigMap = new CoreGenSigMap();
  if( !SigMap->ReadSigMap( SM ) ){
    LogPane->AppendText( "Could not read signal map " + wxString(SM) + "\n");
    LogPane->AppendText( wxString(SigMap->GetErrStr()) + "\n" );
    delete SigMap;
    return ;
  }

  // generate a new image from the pipeline
  std::string ImgPath;
  PortalViz *Viz = new PortalViz();
  if( !Viz->GeneratePipeline(SigMap,ImgPath) ){
    LogPane->AppendText( "Could not generate pipeline visualization\n" );
    delete Viz;
    delete SigMap;
    return ;
  }

  // visualize the pipeline
  PipeVizWin *PV = new PipeVizWin( this,
                                   wxID_ANY,
                                   wxT("Pipeline Visualization"),
                                   wxDefaultPosition,
                                   wxSize(1024,768),
                                   wxDEFAULT_DIALOG_STYLE|wxVSCROLL|wxHSCROLL,
                                   wxString(ImgPath) );
  PV->ShowModal();
  PV->Destroy();
  delete Viz;
  delete SigMap;
}

// PortalMainFrame::OnVizIR
void PortalMainFrame::OnVizIR(wxCommandEvent &WXUNUSED(event))
{
  if( !CGProject ){
    LogPane->AppendText( "No project open\n" );
    return ;
  }

  // visualize the design IR
  IRVizWin *VW = new IRVizWin( this,
                               wxID_ANY,
                               wxT("IR Visualization"),
                               wxDefaultPosition,
                               wxSize(1024,768),
                               wxDEFAULT_DIALOG_STYLE|wxVSCROLL|wxHSCROLL,
                               CGProject);
  VW->ShowModal();
  VW->Destroy();
}

// PortalMainFrame::OnProjClose
void PortalMainFrame::OnProjClose(wxCommandEvent &WXUNUSED(event)){
  CloseProject();
}

// PortalMainFrame::OnProjSCOpen
void PortalMainFrame::OnProjSCOpen(wxCommandEvent &WXUNUSED(event)){

  if (!CGProject){
    LogPane->AppendText("No project open\n");
    return;
  }

  wxFileName CurProj(IRFileName);
  wxFileDialog *OpenDialog = new wxFileDialog(this,
                                              _("Choose a StoneCutter file to open"),
                                              CurProj.GetPath(),
                                              wxEmptyString,
                                              _("SC Files (*.sc)|*.sc"),
                                              wxFD_OPEN, wxDefaultPosition);

  wxString NP;
  if (OpenDialog->ShowModal() == wxID_OK){
    NP = OpenDialog->GetPath();

    // derive the file name
    wxFileName NPF(NP);

    LogPane->AppendText("Opening StoneCutter file at " + NP + wxT("\n"));

    OpenSCFile(NP, NPF);
  }

  // clean up the dialog box
  OpenDialog->Destroy();
}

// PortalMainFrame::OpenProject
void PortalMainFrame::OpenProject(wxString NP, bool editing){
  if (!editing)
    LogPane->AppendText("Opening project from IR at " + NP + wxT("\n"));
  wxFileName NPF(NP);

  // create a new coregen object
  CGProject = new CoreGenBackend(std::string(NPF.GetName().mb_str()),
                                 std::string(NPF.GetPath().mb_str()),
                                 UserConfig->GetArchiveDir());
  if (CGProject == nullptr){
    if (!editing)
      LogPane->AppendText("Error opening project from IR at " + NP + wxT("\n"));
    return;
  }

  // read the ir
  if (!CGProject->ReadIR(std::string(NP.mb_str()))){
    if (!editing)
      LogPane->AppendText("Error reading IR into CoreGen from " + NP + wxT("\n"));
    return;
  }

  // Force the DAG to build
  if (!CGProject->BuildDAG()){
    if (!editing)
      LogPane->AppendText("Error constructing DAG of hardware nodes\n");
    return;
  }

  // load the ir into the ir pane
  IRPane->LoadFile(NP);
  IRFileName = NP;

  // switch the directory tree to the project directory
  ProjDir->SetPath(NPF.GetPath());

  // load all the modules into the modulebox
  LoadModuleBox(editing);

  // initialize the stonecutter message context
  Msgs = new SCMsg();
  //wxStreamToTextRedirector(LogPane, &SCBuf);

  if (!editing)
    LogPane->AppendText("Successfully opened project from IR at " + NP + wxT("\n"));
}

// PortalMainFrame::OnProjOpen
void PortalMainFrame::OnProjOpen(wxCommandEvent &WXUNUSED(event)){
  // stage 1, decide whether we need to close the current project
  if (CGProject)
    CloseProject();

  // stage 2, prompt the user to select the new yaml input file
  wxFileDialog *OpenDialog = new wxFileDialog(this,
                                              _("Choose a YAML IR file to open"),
                                              UserConfig->wxGetProjectDir(),
                                              wxEmptyString,
                                              _("IR Files (*.yaml)|*.yaml"),
                                              wxFD_OPEN, wxDefaultPosition);

  if (OpenDialog->ShowModal() == wxID_OK){
    OpenProject(OpenDialog->GetPath());
  }

  // clean up the dialog box
  OpenDialog->Destroy();
}

bool PortalMainFrame::OnSaveInstFormat(wxDialog *InfoWin,
                                       CoreGenNode *node,
                                       CGNodeType InfoWinType,
                                       std::vector<std::vector<std::any>> *FieldsInformation,
                                       std::vector<std::string> ExistingFields){
  bool savedAll = false;
  bool createNewNode = false;

  // Retrieve the ISA node
  wxTextCtrl *InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  std::string BoxContents = InfoBox->GetValue().ToStdString();
  CoreGenISA *ISANode = CGProject->GetISANodeByName(BoxContents);
  if( !ISANode ){
    LogPane->AppendText("Failed to retrieve ISA node = " + wxString(BoxContents) + "\n");
    return false;
  }

  // Set all the instruction format node values
  savedAll = SaveInstFormat(InfoWin, static_cast<CoreGenInstFormat *>(node),
                            FieldsInformation, ExistingFields);
  if (savedAll){
    // write out the new IR file
    std::string tempName = std::string(IRFileName.mb_str()) + "tmp";
    CGProject->WriteIR(tempName);
    std::string FName = static_cast<const char *>(IRFileName.c_str());
    std::string NodeName = node->GetName();
    CloseProject(true);
    std::remove(FName.c_str());
    std::rename(tempName.c_str(), FName.c_str());
    std::remove(tempName.c_str());

    ModuleBox->DeleteAllItems();
    TreeItems.clear();
    NodeItems.clear();
    SetupModuleBox();

    OpenProject(IRFileName, true);
    CoreGenNode *TmpNode = CGProject->GetNodeByName(NodeName);
    wxTreeItemId SavedId = GetItemFromNode(TmpNode);

    if (SavedId.IsOk())
      ModuleBox->SetFocusedItem(SavedId);

    LogPane->AppendText("Updated " + NodeName + ".\n");
  }else{
    if (createNewNode)
      CGProject->DeleteNode(node);
    LogPane->AppendText("Errors detected. Changes will not be saved to file until all fields are correct.\n");
  }
  return savedAll;
}

bool PortalMainFrame::OnSave(wxDialog *InfoWin,
                             CoreGenNode *node,
                             CGNodeType InfoWinType){
  bool savedAll = false;
  bool createNewNode = false;
  if (!node)
    createNewNode = true;
  switch (InfoWinType){
  case CGCache:
    if (createNewNode)
      node = CGProject->InsertCache("NewCache", 0, 0);
    savedAll = SaveCache(InfoWin, (CoreGenCache *)node);
    break;
  case CGComm:
    if (createNewNode)
      node = CGProject->InsertComm("NewComm");
    savedAll = SaveComm(InfoWin, (CoreGenComm *)node);
    break;
  case CGCore:
    if (createNewNode)
      node = CGProject->InsertCore("NewCore", nullptr);
    savedAll = SaveCore(InfoWin, (CoreGenCore *)node);
    break;
  case CGDPath:
    if (createNewNode)
      node = CGProject->InsertDPath("NewDataPath", "");
    savedAll = SaveDataPath(InfoWin, (CoreGenDataPath *)node);
    break;
  case CGExt:
    if (createNewNode)
      node = CGProject->InsertExt("NewExt");
    savedAll = SaveExt(InfoWin, (CoreGenExt *)node);
    break;
  case CGISA:
    if (createNewNode)
      node = CGProject->InsertISA("NewISA");
    savedAll = SaveISA(InfoWin, (CoreGenISA *)node);
    break;
  case CGInst:
    if (createNewNode)
      node = CGProject->InsertInst("NewInst", nullptr, nullptr);
    savedAll = SaveInst(InfoWin, (CoreGenInst *)node);
    break;
  case CGMCtrl:
    if (createNewNode)
      node = CGProject->InsertMCtrl("NewMCtrl", 0);
    savedAll = SaveMCtrl(InfoWin, (CoreGenMCtrl *)node);
    break;
  case CGPInst:
    if (createNewNode)
      node = CGProject->InsertPseudoInst("NewPInst", nullptr);
    savedAll = SavePInst(InfoWin, (CoreGenPseudoInst *)node);
    break;
  case CGReg:
    if (createNewNode)
      node = CGProject->InsertReg("NewReg", 0, 0);
    savedAll = SaveReg(InfoWin, (CoreGenReg *)node);
    break;
  case CGRegC:
    if (createNewNode)
      node = CGProject->InsertRegClass("NewRegClass");
    savedAll = SaveRegClass(InfoWin, (CoreGenRegClass *)node);
    break;
  case CGSoc:
    if (createNewNode)
      node = CGProject->InsertSoC("NewSoC");
    savedAll = SaveSoC(InfoWin, (CoreGenSoC *)node);
    break;
  case CGSpad:
    if (createNewNode)
      node = CGProject->InsertSpad("NewSpad", 0, 0, 0);
    savedAll = SaveSpad(InfoWin, (CoreGenSpad *)node);
    break;
  case CGVTP:
    if (createNewNode)
      node = CGProject->InsertVTP("NewVTP");
    savedAll = SaveVTP(InfoWin, (CoreGenVTP *)node);
    break;
  case CGInstF:
  case CGEnc:
  case CGPlugin:
  default:
    LogPane->AppendText("Node not supported\n");
    savedAll = false;
    break;
  }

  if (savedAll){
    // write out the new IR file
    std::string tempName = std::string(IRFileName.mb_str()) + "tmp";
    CGProject->WriteIR(tempName);
    std::string FName = static_cast<const char *>(IRFileName.c_str());
    std::string NodeName = node->GetName();
    CloseProject(true);
    std::remove(FName.c_str());
    std::rename(tempName.c_str(), FName.c_str());
    std::remove(tempName.c_str());

    ModuleBox->DeleteAllItems();
    TreeItems.clear();
    NodeItems.clear();
    SetupModuleBox();

    OpenProject(IRFileName, true);
    CoreGenNode *TmpNode = CGProject->GetNodeByName(NodeName);
    wxTreeItemId SavedId = GetItemFromNode(TmpNode);

    if (SavedId.IsOk())
      ModuleBox->SetFocusedItem(SavedId);

    LogPane->AppendText("Updated " + NodeName + ".\n");
  }
  else{
    if (createNewNode)
      CGProject->DeleteNode(node);
    LogPane->AppendText("Errors detected. Changes will not be saved to file until all fields are correct.\n");
  }
  return savedAll;
}

bool PortalMainFrame::SaveCache(wxDialog *InfoWin, CoreGenCache *CacheNode){
  CoreGenCache *NewChild;
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    CacheNode->SetName(BoxContents);
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid cache name. Keeping old cache name\n");
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set sets
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    CacheNode->SetSets(std::stoi(BoxContents));
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Cache sets will not be changed\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set ways
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(2);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    CacheNode->SetWays(std::stoi(BoxContents));
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Cache ways will not be changed\n");
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set ways
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(10);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    CacheNode->SetLineSize(std::stoi(BoxContents));
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Line Size will not be changed\n");
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set Child Cache
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  BoxContents = InfoBox->GetValue().ToStdString();
  NewChild = CGProject->GetCacheNodeByName(BoxContents);
  if (NewChild){
    if (HasCacheCycle(CacheNode, NewChild)){
      LogPane->AppendText("Inserting " + BoxContents + " would create a cache cycle. Old subcache will be kept.\n");
      InfoWin->FindWindow(8)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
    }else{
      InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
      CoreGenCache *OldChild = CacheNode->GetSubCache();
      if (OldChild){
        OldChild->DeleteParentCache(CacheNode);
        CacheNode->SetNullChildCache();
      }
      CacheNode->SetChildCache(NewChild);
      NewChild->SetParentCache(CacheNode);
    }
  }else if (BoxContents == ""){
    CoreGenCache *OldChild = CacheNode->GetSubCache();
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
    if (OldChild){
      OldChild->DeleteParentCache(CacheNode);
      CacheNode->SetNullChildCache();
    }else{
      LogPane->AppendText("Warning: no subcache was entered and there was no subcache to delete.\n");
    }
  }else{
    LogPane->AppendText("Could not find specified cache. No change made to child cache\n");
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  return savedAll;
}

bool PortalMainFrame::SaveComm(wxDialog *InfoWin, CoreGenComm *CommNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    CommNode->SetName(BoxContents);
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid cache name. Keeping old cache name\n");
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set comm node type
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents.compare("Point-to-Point") == 0){
    CommNode->SetCommType(CGCommP2P);
  }else if (BoxContents.compare("Bus") == 0){
    CommNode->SetCommType(CGCommBus);
  }else if (BoxContents.compare("Network on Chip") == 0){
    CommNode->SetCommType(CGCommNoc);
  }else{
    CommNode->SetCommType(CGCommUnk);
  }

  //set width
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(2);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    CommNode->SetWidth(std::stoi(BoxContents));
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Comm width will not be changed\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set endpoints
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents[BoxContents.size() - 1] != '\n')
    BoxContents += "\n";
  std::istringstream iss(BoxContents);
  std::string nextNodeName;
  while (CommNode->GetNumEndpoints() > 0)
    CommNode->DeleteEndpoint((unsigned)0);
  std::getline(iss, nextNodeName);
  bool allValid = true;
  while (!iss.eof()){
    CoreGenNode *N = CGProject->GetNodeByName(nextNodeName);
    if (N){
      if (!CommNode->InsertEndpoint(N)){
        LogPane->AppendText(nextNodeName + " is not a valid node. It will not be added to the endpoints list.\n");
        InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
        savedAll = false;
        allValid = false;
      }
    }else if (nextNodeName != ""){
      LogPane->AppendText(nextNodeName + " is not a valid node. It will not be added to the endpoints list.\n");
      InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
    }
    getline(iss, nextNodeName);
  }

  if (allValid)
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));

  return savedAll;
}

bool PortalMainFrame::SaveDataPath(wxDialog *InfoWin,
                                   CoreGenDataPath *DPathNode){
  std::istringstream iss;
  std::string nextNodeName;
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    DPathNode->SetName(BoxContents);
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid DataPath name.  Keeping the old DataPath name.\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    DPathNode->SetStyle(BoxContents);
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid DataPath style.  Keeping the old DataPath style.\n");
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  return savedAll;
}

bool PortalMainFrame::SaveCore(wxDialog *InfoWin, CoreGenCore *CoreNode){
  CoreGenNode *newNode;
  std::istringstream iss;
  std::string nextNodeName;
  wxTextCtrl *InfoBox;
  wxComboBox *ComboBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    CoreNode->SetName(BoxContents);
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid core name. Keeping old core name\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set number of thread units
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    CoreNode->SetNumThreadUnits(std::stoi(BoxContents));
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Thread Units will not be changed\n");
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set ISA
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(2);
  BoxContents = InfoBox->GetValue().ToStdString();
  newNode = CGProject->GetISANodeByName(BoxContents);
  if (BoxContents == "" || newNode){
    CoreNode->SetISA((CoreGenISA *)newNode);
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText("Could not find specified ISA. Keeping old ISA.\n");
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set Cache
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  BoxContents = InfoBox->GetValue().ToStdString();
  newNode = CGProject->GetCacheNodeByName(BoxContents);
  if (BoxContents == "" || newNode){
    CoreNode->InsertCache((CoreGenCache *)newNode);
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText("Could not find specified Cache. Keeping old Cache.\n");
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set SMT Method
  ComboBox = (wxComboBox *)InfoWin->FindWindow(10);
  BoxContents = ComboBox->GetStringSelection().ToStdString();

  if (BoxContents != "Unknown" && CoreNode->GetNumThreadUnits() == 1){
    LogPane->AppendText("Invalid SMT Choice for Thread Number.\n");
    InfoWin->FindWindow(89)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }else{
    CGSched Sched = CoreNode->StrToCGSched(BoxContents);
    CoreNode->SetSched(Sched);
    InfoWin->FindWindow(10)->SetForegroundColour(wxColour(0, 0, 0));
  }

  //set Register Classes
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(4);
  BoxContents = InfoBox->GetValue().ToStdString();
  bool allValid = true;
  if (BoxContents[BoxContents.size() - 1] != '\n')
    BoxContents += "\n";
  iss.str(BoxContents);
  while (CoreNode->GetNumRegClass() > 0)
    CoreNode->DeleteRegClass((unsigned)0);
  std::getline(iss, nextNodeName);
  while (!iss.eof()){
    CoreGenRegClass *N = CGProject->GetRegClassNodeByName(nextNodeName);
    if (N){
      CoreNode->InsertRegClass(N);
    }else if (nextNodeName != ""){
      LogPane->AppendText(nextNodeName + " is not a valid Register Class. It will not be added to the Register Class list.\n");
      InfoWin->FindWindow(10)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
    }
    getline(iss, nextNodeName);
  }

  if (allValid)
    InfoWin->FindWindow(10)->SetForegroundColour(wxColour(0, 0, 0));

  //set Extensions
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(5);
  BoxContents = InfoBox->GetValue().ToStdString() + "\n";
  allValid = true;
  if (BoxContents[BoxContents.size() - 1] != '\n')
    BoxContents += "\n";
  iss.clear();
  iss.str(BoxContents);
  while (CoreNode->GetNumExt() > 0)
    CoreNode->DeleteExt((unsigned)0);
  std::getline(iss, nextNodeName);
  while (!iss.eof()){
    CoreGenExt *N = CGProject->GetExtNodeByName(nextNodeName);
    if (N){
      CoreNode->InsertExt(N);
    }else if (nextNodeName != ""){
      LogPane->AppendText(nextNodeName + " is not a valid Extension. It will not be added to the Extension list.\n");
      InfoWin->FindWindow(11)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
    }
    getline(iss, nextNodeName);
  }

  if (allValid)
    InfoWin->FindWindow(11)->SetForegroundColour(wxColour(0, 0, 0));

  return savedAll;
}

bool PortalMainFrame::SaveExt(wxDialog *InfoWin, CoreGenExt *ExtNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    ExtNode->SetName(BoxContents);
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(0, 0, 0));
  }
  else{
    LogPane->AppendText(BoxContents + " is not a valid extension name. Keeping old extension name\n");
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set Extension type
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents == "Template extension"){
    ExtNode->SetType(CGExtTemplate);
  }else if (BoxContents == "Module extension"){
    ExtNode->SetType(CGExtModule);
  }else if (BoxContents == "Communications extension"){
    ExtNode->SetType(CGExtComm);
  }else{
    ExtNode->SetType(CGExtUnk);
  }

  return savedAll;
}

bool PortalMainFrame::SaveISA(wxDialog *InfoWin, CoreGenISA *ISANode){
  wxTextCtrl *InfoBox;

  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    ISANode->SetName(BoxContents);
    InfoWin->FindWindow(1)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid extension name. Keeping old extension name\n");
    InfoWin->FindWindow(1)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }
  return savedAll;
}

bool PortalMainFrame::SaveInstFormat(wxDialog *InfoWin,
                                     CoreGenInstFormat *InstFNode,
                                     std::vector<std::vector<std::any>> *FieldsInformation,
                                     std::vector<std::string> ExistingFields){
  wxTextCtrl *InfoBox;
  wxComboBox *ComboBox;
  std::string BoxContents;
  bool savedAll = true;
  CoreGenNode *newNode;
  std::string nextNodeName;

  // set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    InstFNode->SetName(BoxContents);
    InfoWin->FindWindow(1)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid instruction format name\n");
    InfoWin->FindWindow(0)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  // set isa
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  BoxContents = InfoBox->GetValue().ToStdString();
  newNode = static_cast<CoreGenNode *>(CGProject->GetISANodeByName(BoxContents));
  if( !newNode ){
    LogPane->AppendText("Could not find specified Instruction Set.\n");
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }
  if( !InstFNode->SetISA(static_cast<CoreGenISA *>(newNode)) ){
    LogPane->AppendText("Could not set the ISA.\n");
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  // query format width
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(5);
  if( wxAtoi(InfoBox->GetValue()) != 0 ){
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText("Invalid Format Width.\n");
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  // set num fields
  ComboBox = (wxComboBox *)InfoWin->FindWindow(7);
  BoxContents = ComboBox->GetStringSelection();
  if( wxAtoi(BoxContents) != 0 ){
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else {
    LogPane->AppendText("Invalid Number of Fields");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  // get reg classes
  std::vector<CoreGenRegClass *> RegClasses;
  for ( unsigned i = 0; i < NodeItems.size(); i++ )
  {
    CoreGenNode *TmpNode = NodeItems[i].second;
    if ( TmpNode->GetType() == CGRegC )
    {
      RegClasses.push_back(static_cast<CoreGenRegClass *>(TmpNode));
    }
  }
  // set fields
  for ( unsigned i = 0; i < FieldsInformation->size(); i++ ) {

    std::vector<std::any> Field = FieldsInformation->at(i);

    // store field name
    std::string FieldName = "";
    try {
      FieldName = std::any_cast<wxTextCtrl *>(Field.at(0))->GetValue().ToStdString();
    } catch (std::bad_any_cast &e){
      std::cout << "\n"
                << e.what() << std::endl;
    }

    // store field type
    CoreGenInstFormat::CGInstField FieldTypeObject = CoreGenInstFormat::CGInstField::CGInstUnk;
    std::string TmpFieldType = std::string(std::any_cast<wxComboBox *>(Field.at(1))->GetStringSelection().mb_str());
    if( TmpFieldType == "CGInstReg" ){
      FieldTypeObject = CoreGenInstFormat::CGInstField::CGInstReg;
    }else if( TmpFieldType == "CGInstCode" ){
      FieldTypeObject = CoreGenInstFormat::CGInstField::CGInstCode;
    }else if( TmpFieldType == "CGInstImm" ){
      FieldTypeObject = CoreGenInstFormat::CGInstField::CGInstImm;
    }else{
      FieldTypeObject = CoreGenInstFormat::CGInstField::CGInstUnk;
    }

    // store start bit
    unsigned StartBit = 0;
    try{
      StartBit = (unsigned)(wxAtoi(std::any_cast<wxComboBox *>(Field.at(2))->GetStringSelection()));
    }catch (std::bad_any_cast &e){
      std::cout << "\n"
                << e.what() << std::endl;
    }

    // store end bit
    unsigned EndBit = 0;
    try{
      EndBit = (unsigned)(wxAtoi(std::any_cast<wxComboBox *>(Field.at(3))->GetStringSelection()));
    }catch (std::bad_any_cast &e){
      std::cout << "\n"
                << e.what() << std::endl;
    }

    //-- store mandatory flag
    bool MandatoryFlag = false;
    try {
        MandatoryFlag = (std::any_cast<wxCheckBox *>(Field.at(4)))->GetValue();
    } catch (std::bad_any_cast &e){
      std::cout << "\n"
                << e.what() << std::endl;
    }

    // store register class
    std::string RegisterClassName = "";
    try {
      RegisterClassName = std::any_cast<wxTextCtrl *>(Field.at(5))->GetValue().ToStdString();
    } catch (std::bad_any_cast &e) {
      std::cout << "\n"
                << e.what() << std::endl;
    }

    // retrieve the existing name of the field in case it was changed
    std::string RealName;
    if( i < ExistingFields.size() ){
      RealName = ExistingFields[i];
    } else {
      RealName = FieldName;
    }

    if(RealName != FieldName){
      if(!InstFNode->RemoveField(RealName)){
        LogPane->AppendText("Could not remove " + RealName + "\n");
        savedAll = false;
      }
    }
    // check to see if the field exists.  if it doesn't, insert it
    if( !InstFNode->IsValidField(FieldName) ){
      if ( !InstFNode->InsertField(FieldName,
                                    StartBit,
                                    EndBit,
                                    FieldTypeObject,
                                    MandatoryFlag) ) {
        LogPane->AppendText("Could not insert field =" + RealName +
                              " into the instruction format\n");
        savedAll = false;
      }
    }

    // set field type
    if ( !InstFNode->SetFieldType(FieldName, FieldTypeObject) ) {
      LogPane->AppendText("Could not set field type " + InstFNode->CGInstFieldToStr(FieldTypeObject) + " for " + FieldName + ".\n");
      savedAll = false;
    }

    // check if register class exists
    if ( FieldTypeObject == CoreGenInstFormat::CGInstField::CGInstReg ){
      CoreGenRegClass *RegClass = nullptr;
      // search for the appropriate register class
      for( unsigned r=0; r<RegClasses.size(); r++ ){
        if( RegClasses[r]->GetName() == RegisterClassName ){
          RegClass = RegClasses[r];
        }
      }

      if( RegClass != nullptr ){
        // found a valid register class name
        if ( !InstFNode->InsertRegFieldMap(FieldName,
                                          RegClass) ){
          LogPane->AppendText("Could not associate " + FieldName +
                              " with RegClass " + RegisterClassName);
          savedAll = false;
        }
      }else{
        // error occurred, not found
        LogPane->AppendText("Invalid Register Class Name: " + RegisterClassName);
        savedAll = false;
      }
    } // if FieldTypeObject
  } // for FieldsInformation->size()
  return savedAll;
}

bool PortalMainFrame::SaveInst(wxDialog *InfoWin, CoreGenInst *InstNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;
  CoreGenNode *newNode;
  std::istringstream iss;
  std::string nextNodeName;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    InstNode->SetName(BoxContents);
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid extension name. Keeping old extension name\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set instruction format
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();

  newNode = CGProject->GetInstFormatNodeByName(BoxContents);
  if (BoxContents == "" || newNode){
    InstNode->SetNullFormat();
    InstNode->SetFormat((CoreGenInstFormat *)newNode);
    CoreGenPseudoInst *PInst = CGProject->GetPInstNodeByInstName(InstNode->GetName());
    //clear syntax if IF is deleted
    if (BoxContents == ""){
      InstNode->SetSyntax("");
      if (PInst)
        PInst->SetSyntax("");
    }
    if (PInst){
      PInst->ClearEncodings();
    }
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText("Could not find specified Instruction Format.\n");
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set ISA
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(2);
  BoxContents = InfoBox->GetValue().ToStdString();
  newNode = CGProject->GetISANodeByName(BoxContents);
  if (BoxContents == "" || newNode){
    InstNode->SetISA(static_cast<CoreGenISA *>(newNode));
    CoreGenPseudoInst *PInst = CGProject->GetPInstNodeByInstName(InstNode->GetName());
    if (PInst)
      PInst->SetISA((CoreGenISA *)newNode);
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText("Could not find specified Instruction Set.\n");
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set syntax
  if (InstNode->GetFormat()){
    InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
    BoxContents = InfoBox->GetValue().ToStdString();
    if (InstNode->ValidateSyntax(BoxContents)){
      InstNode->SetSyntax(BoxContents);
      InfoWin->FindWindow(9)->SetForegroundColour(wxColour(0, 0, 0));
    }else{
      LogPane->AppendText("Invalid Syntax. No Changes will be made.\n");
      InfoWin->FindWindow(9)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
    }
  }else{
    LogPane->AppendText("Syntax can not be set when there is no instruction format. Syntax will be set to empty.\n");
    InstNode->SetSyntax("");
  }

  //set implementation
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(4);
  BoxContents = InfoBox->GetValue().ToStdString();
  InstNode->SetImpl(BoxContents);

  //set encodings
  InstNode->ClearEncodings();
  bool allValid = true;
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(5);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (InstNode->GetFormat()){
    if (BoxContents[BoxContents.size() - 1] != '\n')
      BoxContents += "\n";
    std::string Field;
    std::string op;
    int Value;
    iss.str(BoxContents);
    getline(iss, nextNodeName);
    while (!iss.eof()){
      std::stringstream encodingStream(nextNodeName);
      encodingStream >> Field;
      encodingStream >> op;
      encodingStream >> Value;
      if (nextNodeName != "" && !InstNode->SetEncoding(Field, Value)){
        LogPane->AppendText("Invalid field: " + Field + ". Field will not be added to encodings.\n");
        InfoWin->FindWindow(11)->SetForegroundColour(wxColour(255, 0, 0));
        savedAll = false;
        allValid = false;
      }
      /*
      else if(PInst){
        PInst->SetEncoding(Field, Value);
      }
      */
      getline(iss, nextNodeName);
    }
  }else if (BoxContents != ""){
    LogPane->AppendText("Cannot add encodings without an Instruction format.\n");
    InfoWin->FindWindow(11)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
    allValid = false;
  }

  if (allValid)
    InfoWin->FindWindow(11)->SetForegroundColour(wxColour(0, 0, 0));

  return savedAll;
}

bool PortalMainFrame::SaveMCtrl(wxDialog *InfoWin, CoreGenMCtrl *MCtrlNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    MCtrlNode->SetName(BoxContents);
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid cache name. Keeping old cache name\n");
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set input ports
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    MCtrlNode->SetNumInputPorts(std::stoi(BoxContents));
    InfoWin->FindWindow(3)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Input ports will not be changed\n");
    InfoWin->FindWindow(3)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set order
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(5);
  BoxContents = InfoBox->GetValue().ToStdString();
  CGMemOrder Order;
  if (BoxContents == "Weak")
    Order = CGMemOrder::CGWeak;
  if (BoxContents == "TSO")
    Order = CGMemOrder::CGTSO;
  if (BoxContents == "Strong")
    Order = CGMemOrder::CGStrong;
  if ((int)Order < 3){
    MCtrlNode->SetMemOrder(Order);
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " must be Weak, TSO, or Strong. Ordering will not be changed\n");
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  return savedAll;
}

bool PortalMainFrame::SavePInst(wxDialog *InfoWin, CoreGenPseudoInst *PInstNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;
  std::istringstream iss;
  std::string nextNodeName;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    PInstNode->SetName(BoxContents);
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid pseudo-instruction name. Keeping old pseudo-instruction name\n");
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set target instruction
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  CoreGenInst *newNode = CGProject->GetInstNodeByName(BoxContents);
  if (BoxContents == "" || newNode){
    PInstNode->SetNullInst();
    PInstNode->SetTargetInst(newNode);
    if (newNode)
      PInstNode->SetISA(newNode->GetISA());
    else
      PInstNode->SetNullISA();
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText("Could not find specified Instruction. Keeping old Instruction.\n");
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set encodings
  PInstNode->ClearEncodings();
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  bool allValid = true;
  BoxContents = InfoBox->GetValue().ToStdString();
  if (PInstNode->GetInst()){
    if (BoxContents[BoxContents.size() - 1] != '\n')
      BoxContents += "\n";
    std::string Field;
    std::string op;
    int Value;
    iss.str(BoxContents);
    getline(iss, nextNodeName);
    while (!iss.eof()){
      std::stringstream encodingStream(nextNodeName);
      encodingStream >> Field;
      encodingStream >> op;
      encodingStream >> Value;
      if (nextNodeName != "" && !PInstNode->SetEncoding(Field, Value)){
        LogPane->AppendText("Invalid field: " + Field + ". Field will not be added to encodings.\n");
        InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
        savedAll = false;
        allValid = false;
      }
      getline(iss, nextNodeName);
    }
  }else if (BoxContents != ""){
    LogPane->AppendText("Cannot add encodings without an Instruction.\n");
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
    allValid = false;
  }

  if (allValid)
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));

  return savedAll;
}

bool PortalMainFrame::SaveReg(wxDialog *InfoWin, CoreGenReg *RegNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  std::istringstream iss;
  std::string nextNodeName;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    RegNode->SetName(BoxContents);
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid register name. Keeping old register name\n");
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set index
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    RegNode->SetIndex(std::stoi(BoxContents));
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Register Index will not be changed\n");
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set register width
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(2);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    RegNode->SetWidth(std::stoi(BoxContents));
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Register Width will not be changed\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set simd width
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(16);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents == "" || (IsInteger(BoxContents) && std::stoi(BoxContents) < 2)){
    RegNode->UnsetSIMD();
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    if (IsInteger(BoxContents)){
      RegNode->SetSIMD(std::stoi(BoxContents));
      InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
    }else{
      LogPane->AppendText(BoxContents + " is not an integer. SIMD data will not be changed\n");
      InfoWin->FindWindow(8)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
    }
  }

  //set subregisters
  //TODO: make sure works when subregs are added/deleted
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents[BoxContents.size() - 1] != '\n')
    BoxContents += "\n";
  iss.str(BoxContents);
  getline(iss, nextNodeName);
  getline(iss, nextNodeName);
  std::string Name;
  std::string startbit;
  std::string endbit;
  unsigned i;
  bool allValid = true;
  while (RegNode->GetNumSubRegs() > 0)
    RegNode->DeleteSubRegByIndex(0);
  while (!iss.eof()){
    //clear strings
    Name = "";
    startbit = "";
    endbit = "";

    //extract name and start/end bit
    for (i = 0; i < nextNodeName.size() && nextNodeName[i] != ':'; i++){
      Name += nextNodeName[i];
    }
    i++;
    for (; i < nextNodeName.size() && nextNodeName[i] != ':'; i++){
      startbit += nextNodeName[i];
    }
    i++;
    for (; i < nextNodeName.size() && nextNodeName[i] != ':'; i++){
      endbit += nextNodeName[i];
    }

    if (Name == ""){
      LogPane->AppendText("Name can not be empty. Subregister will not be added to the subregisters list.\n");
      InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
      getline(iss, nextNodeName);
      continue;
    }

    //make sure start and end bit are integers
    if (!IsInteger(startbit) || !IsInteger(endbit)){
      LogPane->AppendText("Start and end bit must be integers. " + Name + " will not be added to the subregisters list.\n");
      InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
      getline(iss, nextNodeName);
      continue;
    }
    RegNode->InsertSubReg(Name, std::stoi(startbit), std::stoi(endbit));
    getline(iss, nextNodeName);
  }

  if (allValid)
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));

  //build attribute int
  wxCheckBox *CheckBox;
  uint32_t Attrs = 0x00;
  CheckBox = (wxCheckBox *)InfoWin->FindWindow(9);
  if (CheckBox->GetValue()){
    Attrs |= CoreGenReg::CGRegRW;
  }
  CheckBox = (wxCheckBox *)InfoWin->FindWindow(10);
  if (CheckBox->GetValue()){
    Attrs |= CoreGenReg::CGRegRO;
  }
  CheckBox = (wxCheckBox *)InfoWin->FindWindow(11);
  if (CheckBox->GetValue()){
    Attrs |= CoreGenReg::CGRegCSR;
  }
  CheckBox = (wxCheckBox *)InfoWin->FindWindow(12);
  if (CheckBox->GetValue()){
    Attrs |= CoreGenReg::CGRegAMS;
  }
  CheckBox = (wxCheckBox *)InfoWin->FindWindow(13);
  if (CheckBox->GetValue()){
    Attrs |= CoreGenReg::CGRegTUS;
  }
  CheckBox = (wxCheckBox *)InfoWin->FindWindow(14);
  if (CheckBox->GetValue()){
    Attrs |= CoreGenReg::CGRegPC;
  }

  CheckBox = (wxCheckBox *)InfoWin->FindWindow(15);
  bool isShared = CheckBox->GetValue();

  //check for conflicting attributes
  bool noConflicts = true;
  if ((Attrs & CoreGenReg::CGRegRW) && (Attrs & CoreGenReg::CGRegRO)){
    LogPane->AppendText("A register cannot be RW and RO. Attributes will not be changed.\n");
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(255, 0, 0));
    InfoWin->FindWindow(10)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
    noConflicts = false;
  }else{
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(0, 0, 0));
    InfoWin->FindWindow(10)->SetForegroundColour(wxColour(0, 0, 0));
  }

  if ((Attrs & CoreGenReg::CGRegCSR) && (Attrs & CoreGenReg::CGRegAMS)){
    LogPane->AppendText("A register cannot be CSR and AMS. Attributes will not be changed.\n");
    InfoWin->FindWindow(11)->SetForegroundColour(wxColour(255, 0, 0));
    InfoWin->FindWindow(12)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
    noConflicts = false;
  }else{
    InfoWin->FindWindow(11)->SetForegroundColour(wxColour(0, 0, 0));
    InfoWin->FindWindow(12)->SetForegroundColour(wxColour(0, 0, 0));
  }
  if ((Attrs & CoreGenReg::CGRegTUS) && isShared){
    LogPane->AppendText("A register cannot be TUS and Shared. RW/RO status will not be changed.\n");
    InfoWin->FindWindow(13)->SetForegroundColour(wxColour(255, 0, 0));
    InfoWin->FindWindow(15)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
    noConflicts = false;
  }else{
    InfoWin->FindWindow(13)->SetForegroundColour(wxColour(0, 0, 0));
    InfoWin->FindWindow(15)->SetForegroundColour(wxColour(0, 0, 0));
  }

  if (noConflicts){
    RegNode->UnsetAttrs(Attrs);
    RegNode->SetAttrs(Attrs);
    RegNode->UnsetAttrs(Attrs);
    RegNode->SetAttrs(Attrs);
    RegNode->SetShared(isShared);
  }

  return savedAll;
}

bool PortalMainFrame::SaveRegClass(wxDialog *InfoWin, CoreGenRegClass *RegClassNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  std::istringstream iss;
  std::string nextNodeName;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    RegClassNode->SetName(BoxContents);
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid Register Class name. Keeping old Register Class name\n");
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set read ports
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(5);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    RegClassNode->SetReadPorts(std::stoi(BoxContents));
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Read ports will not be changed\n");
    InfoWin->FindWindow(4)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set write ports
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(7);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    RegClassNode->SetWritePorts(std::stoi(BoxContents));
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Write ports will not be changed\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents[BoxContents.size() - 1] != '\n')
    BoxContents += "\n";
  iss.str(BoxContents);
  bool allValid = true;
  //clear current registers
  while (RegClassNode->GetNumReg() > 0)
    RegClassNode->DeleteChild(RegClassNode->GetChild(0));
  std::getline(iss, nextNodeName);
  while (!iss.eof()){
    CoreGenReg *N = CGProject->GetRegNodeByName(nextNodeName);
    if (N){
      RegClassNode->InsertReg(N);
    }else if (nextNodeName != ""){
      LogPane->AppendText(nextNodeName + " is not a valid register. It will not be added to the registers list.\n");
      InfoWin->FindWindow(3)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
    }
    getline(iss, nextNodeName);
  }

  if (allValid)
    InfoWin->FindWindow(3)->SetForegroundColour(wxColour(0, 0, 0));

  return savedAll;
}

bool PortalMainFrame::SaveSoC(wxDialog *InfoWin, CoreGenSoC *SoCNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  std::istringstream iss;
  std::string nextNodeName;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    SoCNode->SetName(BoxContents);
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid SoC name. Keeping old SoC name\n");
    InfoWin->FindWindow(2)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set cores
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (BoxContents[BoxContents.size() - 1] != '\n')
    BoxContents += "\n";

  //clean current cores
  while (SoCNode->GetNumCores() > 0)
    SoCNode->DeleteCore(SoCNode->GetCore(0));

  //insert all valid cores in the user's list
  iss.str(BoxContents);
  std::getline(iss, nextNodeName);
  bool allValid = true;
  while (!iss.eof()){
    CoreGenCore *N = CGProject->GetCoreNodeByName(nextNodeName);
    if (N){
      SoCNode->InsertCore(N);
    }else if (nextNodeName != ""){
      LogPane->AppendText(nextNodeName + " is not a valid core. It will not be added to the cores list.\n");
      InfoWin->FindWindow(3)->SetForegroundColour(wxColour(255, 0, 0));
      savedAll = false;
      allValid = false;
    }
    getline(iss, nextNodeName);
  }

  if (allValid)
    InfoWin->FindWindow(3)->SetForegroundColour(wxColour(0, 0, 0));

  return savedAll;
}

bool PortalMainFrame::SaveSpad(wxDialog *InfoWin, CoreGenSpad *SpadNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    SpadNode->SetName(BoxContents);
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid SoC name. Keeping old SoC name\n");
    InfoWin->FindWindow(5)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set mem size
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(1);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    SpadNode->SetMemSize(std::stoi(BoxContents));
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Mem size will not be changed\n");
    InfoWin->FindWindow(6)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set request ports
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(2);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    SpadNode->SetRqstPorts(std::stoi(BoxContents));
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Request ports will not be changed\n");
    InfoWin->FindWindow(7)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set response ports
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(3);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    SpadNode->SetRspPorts(std::stoi(BoxContents));
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Response ports will not be changed\n");
    InfoWin->FindWindow(8)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  //set start addr
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(4);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (IsInteger(BoxContents)){
    SpadNode->SetStartAddr(std::stoull(BoxContents));
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not an integer. Response ports will not be changed\n");
    InfoWin->FindWindow(9)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  return savedAll;
}

void PortalMainFrame::LogError(wxString Str){
  LogPane->AppendText(Str);
}

bool PortalMainFrame::SaveVTP(wxDialog *InfoWin, CoreGenVTP *VTPNode){
  wxTextCtrl *InfoBox;
  std::string BoxContents;
  bool savedAll = true;

  //set name
  InfoBox = (wxTextCtrl *)InfoWin->FindWindow(0);
  BoxContents = InfoBox->GetValue().ToStdString();
  if (CGProject->IsValidName(BoxContents)){
    VTPNode->SetName(BoxContents);
    InfoWin->FindWindow(1)->SetForegroundColour(wxColour(0, 0, 0));
  }else{
    LogPane->AppendText(BoxContents + " is not a valid VTP name. Keeping old VTP name\n");
    InfoWin->FindWindow(1)->SetForegroundColour(wxColour(255, 0, 0));
    savedAll = false;
  }

  return savedAll;
}

bool PortalMainFrame::IsInteger(std::string TestString){
  if (TestString == "")
    return false;

  for (unsigned i = 0; i < TestString.size(); i++)
    if (!std::isdigit(TestString[i]))
      return false;

  return true;
}

bool PortalMainFrame::HasCacheCycle(CoreGenCache *SourceCache, CoreGenCache *Cache){
  if (Cache == nullptr)
    return false;
  if (SourceCache->GetName() == Cache->GetName())
    return true;
  return HasCacheCycle(SourceCache, Cache->GetSubCache());
}

// EOF
