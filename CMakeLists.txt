PROJECT(seegViewer)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

SET(VERSION 0.1.0)


IF(UNIX)
  IF(APPLE)
    SET(GUI "Cocoa")
  ELSE(APPLE)
    SET(GUI "X11")
  ENDIF(APPLE)
ELSE(UNIX)
  IF(WIN32)
    SET(GUI "Win32")
  ELSE(WIN32)
    SET(GUI "Unknown")
  ENDIF(WIN32)
ENDIF(UNIX)
MESSAGE(STATUS "GUI system is ${GUI}")


FIND_PACKAGE(VTK REQUIRED)
IF (VTK_FOUND)
    INCLUDE(${VTK_USE_FILE})
    MESSAGE(STATUS "VTK Ok!")
ENDIF(VTK_FOUND)

FIND_PACKAGE(ITK REQUIRED)
IF (ITK_FOUND)
    INCLUDE(${ITK_USE_FILE})
    MESSAGE(STATUS "ITK Ok!")
ENDIF(ITK_FOUND)

SET(QT_MIN_VERSION 4.3.0) #necessario per QMdiArea
FIND_PACKAGE(Qt4 REQUIRED)
IF (QT4_FOUND)
    INCLUDE(${QT_USE_FILE})
    MESSAGE(STATUS "QT4 Ok!")
    INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
ENDIF(QT4_FOUND)

#niftilib è gia inclusa nelle itk, quindi non è più necessaria
#INCLUDE (CMakeModules/FindNifti.cmake)
#IF (NIFTI_FOUND)
#    MESSAGE(STATUS "NIFTI Ok!")
#    INCLUDE_DIRECTORIES(${NIFTI_INCLUDE_DIR})
#ENDIF (NIFTI_FOUND)



IF(WIN32)
  OPTION(SV_DICEVIEWER "Compile only the basic viewer without filters support" ON)
  OPTION(SV_NEUROLOGICALCONVENTION "Use neurological convention instead of radiological convention" OFF)
  #OPTION(SV_DEBUG "Debug" OFF)
  OPTION(SV_SIMPLEINTERFACE "Compile with fixed interface" ON)
  MARK_AS_ADVANCED(SV_SIMPLEINTERFACE)
  OPTION(SV_DANZ "Adds 'Danz' Label" OFF)
  MARK_AS_ADVANCED(SV_DANZ)
  OPTION(SV_FASTLOAD "Speed up splash screen" OFF)
  MARK_AS_ADVANCED(SV_FASTLOAD)
  OPTION(SV_3_SURFACES "Opens up to 3 surfaces" OFF)
  MARK_AS_ADVANCED(SV_3_SURFACES)
ELSE(WIN32)
  OPTION(SV_DICEVIEWER "Compile only the basic viewer without filters support" OFF)
  OPTION(SV_SIMPLEINTERFACE "Compile with fixed interface" ON)
  OPTION(SV_NEUROLOGICALCONVENTION "Use neurological convention instead of radiological convention" OFF)
  #OPTION(SV_DEBUG "Debug" ON)
  OPTION(SV_DANZ "Adds 'Danz' Label" ON)
  MARK_AS_ADVANCED(SV_DANZ)
  OPTION(SV_3_SURFACES "Opens up to 3 surfaces" ON)
  MARK_AS_ADVANCED(SV_3_SURFACES)
  OPTION(SV_FASTLOAD "Speed up splash screen" ON)
  MARK_AS_ADVANCED(SV_FASTLOAD)
ENDIF(WIN32)

OPTION(SV_SLICECONTROLS_2COL "Slice controls on 2 columns" ON)
MARK_AS_ADVANCED(SV_SLICECONTROLS_2COL)
OPTION(SV_ORTHOCONTROLS_3COL "Ortho slice controls on 3 columns" ON)
MARK_AS_ADVANCED(SV_ORTHOCONTROLS_3COL)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

ADD_SUBDIRECTORY(src)







