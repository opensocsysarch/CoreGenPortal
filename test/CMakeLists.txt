# CoreGenPortal test CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

find_program (PYTHON3 python3)
if (PYTHON3)
  message(STATUS "FOUND PYTHON3 AT ${PYTHON3}")
else ()
  message(FATAL_ERROR "COULD NOT FIND PYTHON3")
endif ()

if(BUILD_ALL_TESTING)
  message(STATUS "ENABLING ALL GUI TESTS")
  set(BUILD_PORTAL_GUI_TESTS "ON")
endif()

add_subdirectory(GUITest)

#-- EOF
