//
// _PORTALMAIN_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <wx/wxprec.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/splash.h>

#include "PortalMainFrame.h"
#include "PortalConsts.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class CoreGenPortal: public wxApp{
public:
  virtual bool OnInit();
};

IMPLEMENT_APP(CoreGenPortal);

// PortalMain::OnInit
// Initialize the main frame
bool CoreGenPortal::OnInit(){

  // Initial setup
  SetAppName(PORTAL_APP_NAME);
  SetAppDisplayName(PORTAL_APP_NAME);
  SetVendorName(PORTAL_VENDOR_NAME);
  SetVendorDisplayName(PORTAL_VENDOR_NAME);

  // Setup the splash screen
  wxFileName f(wxStandardPaths::Get().GetExecutablePath());
  wxString bitmapPath = f.GetPath() + wxT("/imgs/logo.png");
  wxImage::AddHandler(new wxPNGHandler);
  wxBitmap bitmap;
  if( bitmap.LoadFile(bitmapPath, wxBITMAP_TYPE_PNG)){
    wxSplashScreen* splash = new wxSplashScreen(bitmap,
                    wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
                    3000, NULL, -1, wxDefaultPosition, wxDefaultSize,
                    wxBORDER_SIMPLE|wxSTAY_ON_TOP);
  }
  wxYield();

  // Setup the main frame
  PortalMainFrame *MainFrame = new PortalMainFrame( "CoreGenPortal",
                                                    wxPoint(50,50),
                                                    wxSize(1024,768));
  MainFrame->Show( true );
  return true;
}

// EOF
