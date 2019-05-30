# src/CoreGenPortal CMake
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

#-- add subdirectory
add_subdirectory( imgs )
add_subdirectory( PortalCore )

set(CGPortalSrcs
PortalMain.cpp
PortalMainFrame.cpp
PortalVersion.h
PortalMainFrame.h
PortalVerifPrefWin.cpp
PortalVerifPrefWin.h
PortalUserPrefWin.cpp
PortalUserPrefWin.h
PortalNewProjWin.cpp
PortalNewProjWin.h
)

#-- target deps
find_package(wxWidgets COMPONENTS net adv aui core gl html propgrid qa ribbon richtext stc xrc base REQUIRED)
IF(wxWidgets_FOUND)
  INCLUDE("${wxWidgets_USE_FILE}")
  include_directories ( . )
  include_directories (${COREGEN_INSTALL_PATH}/include)
  include_directories (${PORTAL_INCLUDE_PATH})
  link_directories (${COREGEN_INSTALL_PATH}/lib)
  link_directories (${COREGEN_INSTALL_PATH}/lib64)
  link_directories (${COREGEN_INSTALL_PATH}/lib/clasp-3.3.4)
  link_directories (${COREGEN_INSTALL_PATH}/lib/potassco-1.0)
  link_directories (${COREGEN_INSTALL_PATH}/lib64/clasp-3.3.4)
  link_directories (${COREGEN_INSTALL_PATH}/lib64/potassco-1.0)
  add_executable( CoreGenPortal ${CGPortalSrcs}
                                $<TARGET_OBJECTS:PortalCoreObjs> )
  TARGET_LINK_LIBRARIES( CoreGenPortal ${COREGEN_LINK_LIBS} ${wxWidgets_LIBRARIES} )
  install(TARGETS CoreGenPortal DESTINATION bin)
ELSE(wxWidgets_FOUND)
  MESSAGE("wxWidgets not found!")
ENDIF(wxWidgets_FOUND)

#-- EOF
