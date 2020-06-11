# src/CoreGenPortal/PortalViz CMake
#
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

set(CGPortalVizSrcs
PortalViz.cpp
PortalViz.h
)

#-- target deps
include_directories ( . )
include_directories (${COREGEN_INSTALL_PATH}/include)
include_directories (${PORTAL_INCLUDE_PATH})
include_directories (${GRAPHVIZ_INCLUDE_PATH})
add_library(PortalVizObjs OBJECT ${CGPortalVizSrcs})

#-- EOF
