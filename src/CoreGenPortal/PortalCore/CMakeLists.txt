# src/CoreGenPortal/PortalCore CMake
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CGPortalCoreSrcs
CoreStoneCutterEditor.cpp
CoreErrorDiag.cpp
CoreUserConfig.cpp
CoreVerifConfig.cpp
CoreInfoWin.cpp
CoreSocInfoWin.cpp
CoreCoreInfoWin.cpp
CoreInstFormatInfoWin.cpp
CoreDrawInstFormat.cpp
CoreInstInfoWin.cpp
CorePInstInfoWin.cpp
CoreRegClassInfoWin.cpp
CoreRegInfoWin.cpp
CoreISAInfoWin.cpp
CoreCacheInfoWin.cpp
CoreEncodingInfoWin.cpp
CoreExtInfoWin.cpp
CoreCommInfoWin.cpp
CoreSpadInfoWin.cpp
CoreMCtrlInfoWin.cpp
CoreVTPInfoWin.cpp
CorePluginInfoWin.cpp
CoreVerifWin.cpp
CoreStatsWin.cpp
CoreSpecDocWin.cpp
CorePluginBrowser.cpp
)

#-- target deps
find_package(wxWidgets COMPONENTS net adv aui core gl html propgrid qa ribbon richtext stc xrc base REQUIRED)
IF(wxWidgets_FOUND)
  INCLUDE("${wxWidgets_USE_FILE}")
  include_directories ( . )
  include_directories (${COREGEN_INSTALL_PATH}/include)
  include_directories (${PORTAL_INCLUDE_PATH})
  add_library(PortalCoreObjs OBJECT ${CGPortalCoreSrcs})
ELSE(wxWidgets_FOUND)
  MESSAGE("wxWidgets not found!")
ENDIF(wxWidgets_FOUND)

#-- EOF
