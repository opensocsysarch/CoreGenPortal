# CoreGenPortal test/GUITest CMakeLists.txt
# Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.py)

if(BUILD_PORTAL_GUI_TESTS)
  foreach(testSrc ${TEST_SRCS})
    get_filename_component(testName ${testSrc} NAME_WE)

    add_test(NAME ${testName}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${PYTHON3} ${testSrc})
    set_tests_properties( ${testName} PROPERTIES PASS_REGULAR_EXPRESSION "")
  endforeach(testSrc)
endif()

#-- EOF
