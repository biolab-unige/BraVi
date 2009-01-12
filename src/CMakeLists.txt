ADD_SUBDIRECTORY(libfilter)

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_BINARY_DIR})

# QT4 options
SET(QT_USE_OPENGL TRUE)
INCLUDE(${QT_USE_FILE})


# Files da processare per le QT4
SET (QT_SOURCES MainWidget.cpp ReadImageDialog.cpp SurfaceWidget.cpp QVTKSquareWidget.cpp PACSQueryDialog.cpp)
SET (QT_HEADERS MainWidget.h ReadImageDialog.h SurfaceWidget.h QVTKSquareWidget.h PACSQueryDialog.h)
SET (QT_RESOURCES resources.qrc)


# Processa i files usati dalle QT
QT4_ADD_RESOURCES(QT4_RES ${QT_RESOURCES})
QT4_WRAP_CPP(MOC_FILES ${QT_HEADERS})


SET(SOURCES main.cpp;
            Volume3DActor.cpp;
            SliceActor.cpp;
            Surface3DActor.cpp;
            vtkGenericMedicalImageReader.cpp;
            vtkNiftiImageReader.cpp;
            ${QT_SOURCES})


SET(HEADERS ${CMAKE_BINARY_DIR}/config.h;
            Volume3DActor.h;
            SliceActor.h;
            Surface3DActor.h;
            vtkGenericMedicalImageReader.h;
            vtkNiftiImageReader.h;
            ${QT_HEADERS};)



# Librerie all'interno del progetto
SET(VTK_LIBRARIES vtkIO vtkVolumeRendering vtkWidgets vtkHybrid QVTK) #VTK
SET(ITK_LIBRARIES ITKCommon ITKIO ITKniftiio ITKBasicFilters) #ITK
#QT_LIBRARIES è generata automaticamente
SET(PROJECT_LIBRARIES filter) # Aggiungere qua eventuali altre librerie create nel progetto

SET(SEEGVIEWER_LIBRARIES ${VTK_LIBRARIES} ${ITK_LIBRARIES} ${QT_LIBRARIES} ${PROJECT_LIBRARIES} )


IF (WIN32)
  SET (WIN32_RES ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/seegViewer.dir/resfile.obj)
  ADD_CUSTOM_COMMAND(OUTPUT WIN32_RES COMMAND windres.exe resfile.rc -o ${WIN32_RES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res )
  ADD_EXECUTABLE(seegViewer WIN32 ${SOURCES} ${HEADERS} ${MOC_FILES} ${QT4_RES} ${WIN32_RES}) #WIN32 fa in modo che l'applicazione non venga lanciata assieme ad una shell
  SET(WINDOWS_LIBRARIES wininet dsound ole32 winmm gdi32 uuid shell32 vfw32 rpcrt4 oleaut32 comdlg32 winspool)
  SET(SEEGVIEWER_LIBRARIES ${SEEGVIEWER_LIBRARIES} ${WINDOWS_LIBRARIES} )
  ADD_CUSTOM_COMMAND(TARGET seegViewer POST_BUILD COMMAND strip -sxv ${CMAKE_CURRENT_BINARY_DIR}/seegviewer.exe)

ELSE (WIN32)

  ADD_EXECUTABLE(seegViewer ${SOURCES} ${HEADERS} ${MOC_FILES} ${QT4_RES})
  SET(SEEGVIEWER_LIBRARIES ${SEEGVIEWER_LIBRARIES})

ENDIF (WIN32)


TARGET_LINK_LIBRARIES(seegViewer ${SEEGVIEWER_LIBRARIES})


INSTALL(TARGETS seegViewer RUNTIME DESTINATION bin)

IF(UNIX)
  INSTALL(FILES res/seegViewer.png  DESTINATION share/icons)
  INSTALL(FILES res/seegViewer.desktop res/seegViewer2.desktop DESTINATION share/applications)
ENDIF(UNIX)
