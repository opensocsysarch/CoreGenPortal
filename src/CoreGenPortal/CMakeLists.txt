# src/CoreGenPortal CMake
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
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
PortalSCPrefWin.cpp
PortalSCPrefWin.h
)

set(llvm_codegens
  x86codegen
  AArch64codegen
  ARMcodegen
  Mipscodegen
  PowerPCcodegen
)
set(llvm_parsers
  x86asmparser
  AArch64asmparser
  ARMasmparser
  Mipsasmparser
  PowerPCasmparser
)

llvm_map_components_to_libnames(llvm_libs
  support
  core
  irreader
  mcjit
  ipo
  executionengine
  instrumentation
  transformutils
  vectorize
  scalaropts
  instcombine
  bitreader
  bitwriter
  ${llvm_codegens}
  ${llvm_parsers}
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
  add_executable( CoreGenPortal ${CGPortalSrcs}
                                $<TARGET_OBJECTS:PortalCoreObjs> )
  TARGET_LINK_LIBRARIES( CoreGenPortal SCComp ${llvm_libs} ${COREGEN_LINK_LIBS} ${wxWidgets_LIBRARIES} )
  install(TARGETS CoreGenPortal DESTINATION bin)
ELSE(wxWidgets_FOUND)
  MESSAGE("wxWidgets not found!")
ENDIF(wxWidgets_FOUND)

#-- EOF
