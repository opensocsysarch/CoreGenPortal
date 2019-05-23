# CoreGenPortal Top-Level CMake
#
# Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
#
# See LICENSE in the top level directory for licensing details
#

cmake_minimum_required(VERSION 3.4.3)
project(CoreGenPortal)
set(VERSION "0.2.0")

#------------------------------------------------------------------------
#-- EXTERNAL CMAKE SCRIPTS
#------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH}
)
include(UseLATEX OPTIONAL)


#------------------------------------------------------------------------
#-- COMPILER OPTIONS
#------------------------------------------------------------------------
# Require C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall")

# Check for memory sanity checks
if(COREGEN_MEMORY_SANITY)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Memory sanitizier is suitable for use with Clang")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    message(STATUS "Memory sanitizier is suitable for use with AppleClang")
  else()
    message(FATAL_ERROR "Clang is required to build using memory sanitizer.")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

# Set the system type
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOREGEN_PORTAL_PLATFORM=${CMAKE_SYSTEM_NAME}")
message(STATUS "Building for ${CMAKE_SYSTEM_NAME}")

#------------------------------------------------------------------------
#-- COREGEN PATHS
#------------------------------------------------------------------------
# Add package paths
if(NOT COREGEN_INSTALL_PATH)
  set(COREGEN_INSTALL_PATH "/opt/coregen")
endif()

if(NOT PORTAL_INCLUDE_PATH)
  set(PORTAL_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
  message(STATUS "PORTAL_INCLUDE_PATH set to ${PORTAL_INCLUDE_PATH}")
endif()

# Print the path info
message(STATUS "COREGEN_INSTALL_PATH set to ${COREGEN_INSTALL_PATH}")

#------------------------------------------------------------------------
#-- Target Link Libraries
#------------------------------------------------------------------------
if( ${CMAKE_SYSTEM_NAME} EQUAL "Linux" )
  find_file (DEBIAN_FOUND debian_version debconf.conf
      PATHS /etc
  )
  if( DEBIAN_FOUND )
    #-- ubuntu or debian system
    set(COREGEN_LINK_LIBS "-lSCComp -lCoreGenBackend -lCoreGenCodegen -lCoreGenPluginImpl -lyaml-cpp -lclingo -lgringo -lclasp -lpyclingo -lluaclingo -lpotassco -lreify -llua5.3" )
  else()
    #-- centos system
    set(COREGEN_LINK_LIBS "-lSCComp -lCoreGenBackend -lCoreGenCodegen -lCoreGenPluginImpl -lyaml-cpp -lclingo -lgringo -lclasp -lpyclingo -lluaclingo -lpotassco -lreify -llua" )
  endif()
else()
  set(COREGEN_LINK_LIBS "-lSCComp -lCoreGenBackend -lCoreGenCodegen -lCoreGenPluginImpl -lyaml-cpp -lclingo -lgringo -lclasp -lpyclingo -lluaclingo -lpotassco -lreify -llua" )
endif()
message(STATUS "COREGEN_LINK_LIBS set to ${COREGEN_LINK_LIBS}")

#------------------------------------------------------------------------
#-- TESTING
#------------------------------------------------------------------------
# Enable testing
enable_testing()

#------------------------------------------------------------------------
#-- DOCUMENTATION
#------------------------------------------------------------------------
# Doxygen config
if(BUILD_DOCUMENTATION)
  if( NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is required to build the documentation.")
  endif()

  set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/CoreGenPortal.conf)

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/html DESTINATION share/doc)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/man DESTINATION share/man)

  message(STATUS "Enabling developer documentation")
#  add_subdirectory(docs)
endif()

#------------------------------------------------------------------------
#-- PACKAGING
#------------------------------------------------------------------------
#-- builds a standard redhat/centos rpm
if(COREGEN_BUILD_RPM)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "RPM")
  set(CPACK_PACKAGE_NAME "CoreGenPortal")
  set(CPACK_PACKAGE_RELEASE 2)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_RPM_PACKAGE_REQUIRES "CoreGen wxGTK3 wxGTK3-devel")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#-- builds a standard debian/ubuntu deb
if(COREGEN_BUILD_DEB)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGE_NAME "CoreGenPortal")
  set(CPACK_PACKAGE_RELEASE 2)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#-- builds a tgz
if(COREGEN_BUILD_TGZ)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_PACKAGE_NAME "CoreGenPortal")
  set(CPACK_PACKAGE_RELEASE 2)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

if(COREGEN_BUILD_DMG)
  set(CPACK_PACKAGE_VERSION ${VERSION})
  set(CPACK_GENERATOR "DMG")
  set(CPACK_PACKAGE_NAME "CoreGenPortal")
  set(CPACK_PACKAGE_RELEASE 1)
  set(CPACK_PACKAGE_CONTACT "John Leidel")
  set(CPACK_PACKAGE_VENDOR "Tactical Computing Laboratories, LLC")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
  include(CPack)
endif()

#------------------------------------------------------------------------
#-- SUBDIRECTORIES
#------------------------------------------------------------------------
# Add subdirectories
add_subdirectory(src)
#add_subdirectory(test)

#-- EOF
