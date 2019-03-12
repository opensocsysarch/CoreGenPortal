# src/CoreGenPortal/PortalCore CMake
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CGPortalCoreSrcs
CoreStoneCutterEditor.cpp
CoreErrorDiag.cpp
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
