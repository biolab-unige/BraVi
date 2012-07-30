project(BraVi)
cmake_minimum_required(VERSION 2.6.2)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)

set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})


find_package(VTK REQUIRED)
if(VTK_FOUND)
  include(${VTK_USE_FILE})
  message(STATUS "VTK Ok!")
endif(VTK_FOUND)

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
  message(STATUS "ITK Ok!")
endif(ITK_FOUND)

set(QT_MIN_VERSION 4.5.0)
find_package(Qt4 REQUIRED)
if(QT4_FOUND)
  include(${QT_USE_FILE})
  message(STATUS "QT4 Ok!")
  include_directories(${QT_INCLUDE_DIR})
endif(QT4_FOUND)

if(WIN32)
  option(SV_DICEVIEWER "Compile only the basic viewer without filters support" ON)
  option(SV_NEUROLOGICALCONVENTION "Use neurological convention instead of radiological convention" OFF)
  #option(SV_DEBUG "Debug" OFF)
  option(SV_SIMPLEINTERFACE "Compile with fixed interface" ON)
  mark_as_advanced(SV_SIMPLEINTERFACE)
  option(SV_FASTLOAD "Speed up splash screen" OFF)
  mark_as_advanced(SV_FASTLOAD)
  option(SV_3_SURFACES "Opens up to 3 surfaces" OFF)
  mark_as_advanced(SV_3_SURFACES)
else(WIN32)
  option(SV_DICEVIEWER "Compile only the basic viewer without filters support" OFF)
  option(SV_SIMPLEINTERFACE "Compile with fixed interface" ON)
  option(SV_NEUROLOGICALCONVENTION "Use neurological convention instead of radiological convention" OFF)
  #option(SV_DEBUG "Debug" ON)
  option(SV_3_SURFACES "Opens up to 3 surfaces" ON)
  mark_as_advanced(SV_3_SURFACES)
  option(SV_FASTLOAD "Speed up splash screen" ON)
  mark_as_advanced(SV_FASTLOAD)
endif(WIN32)

option(SV_SLICECONTROLS_2COL "Slice controls on 2 columns" ON)
mark_as_advanced(SV_SLICECONTROLS_2COL)
option(SV_ORTHOCONTROLS_3COL "Ortho slice controls on 3 columns" ON)
mark_as_advanced(SV_ORTHOCONTROLS_3COL)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)



# --- Package generation

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BraVi, biomedical images and signals viewer." )
set(CPACK_PACKAGE_VENDOR "BioLab")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "BraVi${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})")


set(CPACK_PACKAGE_NAME BraVi)
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
   set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS ${BraVi_dependencies})
set(CPACK_PACKAGE_CONTACT "Daniele E. Domenichelli")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_GENERATOR DEB)
include(CPack)






add_subdirectory(src)








