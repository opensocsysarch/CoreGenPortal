# src/CoreGenPortal CMake
#
# Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

#-- add subdirectory
add_subdirectory( imgs )

set(CGPortalSrcs
PortalMain.cpp
PortalMainFrame.cpp
PortalVersion.h
PortalMainFrame.h
)

#-- target deps
find_package(wxWidgets COMPONENTS net adv aui core gl html propgrid qa ribbon richtext stc xrc base REQUIRED)
IF(wxWidgets_FOUND)
  INCLUDE("${wxWidgets_USE_FILE}")
  include_directories ( . )
  include_directories (${COREGEN_INSTALL_PATH}/include)
  link_directories (${COREGEN_INSTALL_PATH}/lib)
  add_executable( CoreGenPortal ${CGPortalSrcs} )
#  TARGET_LINK_LIBRARIES( CoreGenPortal CoreGenBackend )
#  TARGET_LINK_LIBRARIES( CoreGenPortal SCComp )
  TARGET_LINK_LIBRARIES( CoreGenPortal ${wxWidgets_LIBRARIES} )
ELSE(wxWidgets_FOUND)
  MESSAGE("wxWidgets not found!")
ENDIF(wxWidgets_FOUND)

#-- EOF
