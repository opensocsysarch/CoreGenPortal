//
// _COREINFOWIN_H_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREINFOWIN_H_
#define _COREINFOWIN_H_

//-- WX HEADERS
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/dirctrl.h>
#include <wx/aui/auibook.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h>

//-- PORTAL HEADERS
#include "CoreGenPortal/PortalCore/CoreSocInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreCoreInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreDataPathInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreInstFormatInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreInstInfoWin.h"
#include "CoreGenPortal/PortalCore/CorePInstInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreRegClassInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreRegInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreISAInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreCacheInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreEncodingInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreExtInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreCommInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreSpadInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreMCtrlInfoWin.h"
#include "CoreGenPortal/PortalCore/CoreVTPInfoWin.h"
#include "CoreGenPortal/PortalCore/CorePluginInfoWin.h"

//-- COREGEN HEADERS
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

class CoreInfoWin{
public:
  CoreInfoWin( wxWindow* parent,
               wxWindowID id = wxID_ANY,
               CoreGenNode *Node = nullptr,
               CGNodeType Type = CGTop);
  ~CoreInfoWin();

protected:
private:
};

#endif

// EOF
