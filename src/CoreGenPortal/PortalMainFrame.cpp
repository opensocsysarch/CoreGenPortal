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

  // update the aui manager
  UpdateAuiMgr();

  // read the user configuration data
  UserConfig = new CoreUserConfig();
  if( UserConfig->isValid() )
    LogPane->AppendText("Read user configuration data; ConfigFile="
                        + UserConfig->wxGetConfFile() + "\n");
  else
    LogPane->AppendText("Error reading user configuration data; ConfigFile="
                        + UserConfig->wxGetConfFile() + "\n");

  // initialize the verification configuration data
  VerifConfig = new CoreVerifConfig();
  if( VerifConfig->isValid() )
    LogPane->AppendText("Initialized the verification pass preferences\n");
  else
    LogPane->AppendText("Error initializing the verification pass preferences\n");
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

  //-- Build Menu
  BuildMenu->Append( ID_BUILD_VERIFY,       wxT("&Verify Design"));
  BuildMenu->Append( ID_BUILD_CODEGEN,      wxT("&Execute CoreGen Codegen"));
  BuildMenu->Append( ID_BUILD_SIGMAP,       wxT("&Generate Signal Map"));
  BuildMenu->Append( ID_BUILD_STONECUTTER,  wxT("&Build StoneCutter"));
  BuildMenu->Append( ID_BUILD_VERILOG,      wxT("&Build Verilog"));
  BuildMenu->Append( ID_BUILD_COMPILER,     wxT("&Build Compiler"));
  BuildMenu->Append( ID_BUILD_SIM,          wxT("&Build Simulator"));
  BuildMenu->AppendSeparator();
  BuildMenu->Append( ID_COMPILE_SIM,        wxT("&Compile Simulator"));
  BuildMenu->Append( ID_COMPILE_COMPILER,   wxT("&Compiler Compiler"));

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

  //-- build menu

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
  SetupModuleBox();

  PluginBox = new wxListBox(this, wxID_ANY, wxDefaultPosition,
                            wxDefaultSize, 0, NULL, wxLB_MULTIPLE);
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
                                      wxAUI_NB_TAB_MOVE |
                                      wxAUI_NB_SCROLL_BUTTONS);

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
  Bind(wxEVT_TREE_ITEM_ACTIVATED, &PortalMainFrame::OnSelectNode, this);
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
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_SOC],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGCore:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_CORE],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGInstF:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_INSTFORMAT],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGInst:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_INST],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      LoadInstEncodings( NodeItems[NodeItems.size()-1],
                         static_cast<CoreGenInst *>(Top->GetChild(i)));
      break;
    case CGPInst:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_PSEUDOINST],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      LoadPInstEncodings( NodeItems[NodeItems.size()-1],
                         static_cast<CoreGenPseudoInst *>(Top->GetChild(i)));
      break;
    case CGRegC:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_REGCLASS],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGReg:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_REG],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGISA:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_ISA],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGCache:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_CACHE],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGComm:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_COMM],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGSpad:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_SPAD],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGMCtrl:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_MCTRL],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGVTP:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_VTP],
                                                wxString(Top->GetChild(i)->GetName()),
                                                -1,
                                                -1,
                                                NULL ) );
      break;
    case CGPlugin:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_PLUGIN],
                                                wxString(Top->GetChild(i)->GetName()),
                                                wxTreeListCtrl::NO_IMAGE,
                                                wxTreeListCtrl::NO_IMAGE,
                                                NULL ) );
      break;
    case CGExt:
      NodeItems.push_back( ModuleBox->AppendItem( TreeItems[TREE_NODE_EXT],
                                                wxString(Top->GetChild(i)->GetName()),
                                                wxTreeListCtrl::NO_IMAGE,
                                                wxTreeListCtrl::NO_IMAGE,
                                                NULL ) );
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

// PortalMainFrame::LoadInstEncodings
// loads the wxTreeCtrl Inst node with its child encodings
void PortalMainFrame::LoadInstEncodings( wxTreeItemId Parent,
                                         CoreGenInst *Inst ){
  for( unsigned j=0; j<Inst->GetNumEncodings(); j++ ){
    CoreGenEncoding *E = Inst->GetEncoding(j);
    EncItems.push_back( ModuleBox->AppendItem( Parent,
                                               wxString(E->GetName()),
                                               -1,
                                               -1,
                                               NULL ) );
  }
}

// PortalMainFrame::LoadPInstEncodings
// loads the wxTreeCtrl PInst node with its child encodings
void PortalMainFrame::LoadPInstEncodings( wxTreeItemId Parent,
                                         CoreGenPseudoInst *PInst ){
  for( unsigned j=0; j<PInst->GetNumEncodings(); j++ ){
    CoreGenEncoding *E = PInst->GetEncoding(j);
    EncItems.push_back( ModuleBox->AppendItem( Parent,
                                               wxString(E->GetName()),
                                               -1,
                                               -1,
                                               NULL ) );
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

// PortalMainFrame::OnSelectNode
void PortalMainFrame::OnSelectNode(wxTreeEvent &event){
  LogPane->AppendText("selected a node");
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

// PortalMainFrame::OnProjOpen
void PortalMainFrame::OnProjOpen(wxCommandEvent& WXUNUSED(event)){
  // stage 1, decide whether we need to close the current project
  if( CGProject ){
    CloseProject();
  }

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

    // load all the modules into the modulebox
    LoadModuleBox();

    LogPane->AppendText( "Successfully opened project from IR at " + NP + wxT("\n" ));
  }

  // clean up the dialog box
  OpenDialog->Destroy();
}

// EOF
