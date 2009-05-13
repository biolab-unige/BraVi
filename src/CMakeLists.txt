add_subdirectory(libfilter)

include_directories(BEFORE ${CMAKE_BINARY_DIR})

# QT4 options
set(QT_USE_OPENGL TRUE)
include(${QT_USE_FILE})


# Files da processare per le QT4
set(QT_SOURCES MainWidget.cpp ReadImageDialog.cpp SurfaceWidget.cpp QVTKSquareWidget.cpp PACSQueryDialog.cpp)
set(QT_HEADERS MainWidget.h ReadImageDialog.h SurfaceWidget.h QVTKSquareWidget.h PACSQueryDialog.h)
set(QT_RESOURCES resources.qrc)


# Processa i files usati dalle QT
qt4_add_resources(QT4_RES ${QT_RESOURCES})
qt4_wrap_cpp(MOC_FILES ${QT_HEADERS})


set(SOURCES main.cpp;
            Volume3DActor.cpp;
            SliceActor.cpp;
            Surface3DActor.cpp;
            vtkGenericMedicalImageReader.cpp;
            vtkNiftiImageReader.cpp;
            ${QT_SOURCES})


set(HEADERS ${CMAKE_BINARY_DIR}/config.h;
            Volume3DActor.h;
            SliceActor.h;
            Surface3DActor.h;
            vtkGenericMedicalImageReader.h;
            vtkNiftiImageReader.h;
            ${QT_HEADERS};)



# Librerie all'interno del progetto
set(VTK_LIBRARIES vtkIO vtkVolumeRendering vtkWidgets vtkHybrid QVTK) #VTK
set(ITK_LIBRARIES ITKCommon ITKIO ITKniftiio ITKBasicFilters) #ITK
#QT_LIBRARIES è generata automaticamente
set(PROJECT_LIBRARIES filter) # Aggiungere qua eventuali altre librerie create nel progetto

set(XTENSVIEWER_LIBRARIES ${VTK_LIBRARIES} ${ITK_LIBRARIES} ${QT_LIBRARIES} ${PROJECT_LIBRARIES} )


if(WIN32)
  set(WIN32_RES ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/xtensViewer.dir/resfile.obj)
  add_custom_command(OUTPUT WIN32_RES COMMAND windres.exe resfile.rc -o ${WIN32_RES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res )
  add_executable(xtensViewer WIN32 ${SOURCES} ${HEADERS} ${MOC_FILES} ${QT4_RES} ${WIN32_RES}) #WIN32 fa in modo che l'applicazione non venga lanciata assieme ad una shell
  set(WINDOWS_LIBRARIES wininet dsound ole32 winmm gdi32 uuid shell32 vfw32 rpcrt4 oleaut32 comdlg32 winspool)
  set(XTENSVIEWER_LIBRARIES ${XTENSVIEWER_LIBRARIES} ${WINDOWS_LIBRARIES} )
  add_custom_command(TARGET xtensViewer POST_BUILD COMMAND strip -sxv ${CMAKE_CURRENT_BINARY_DIR}/xtensviewer.exe)
else(WIN32)
  add_executable(xtensViewer ${SOURCES} ${HEADERS} ${MOC_FILES} ${QT4_RES})
  set(XTENSVIEWER_LIBRARIES ${XTENSVIEWER_LIBRARIES})
endif(WIN32)

target_link_libraries(xtensViewer ${XTENSVIEWER_LIBRARIES})

install(TARGETS xtensViewer RUNTIME DESTINATION bin)

if(UNIX)
  install(FILES res/xtensViewer.png  DESTINATION share/icons)
  install(FILES res/xtensViewer.desktop res/xtensViewer2.desktop DESTINATION share/applications)
endif(UNIX)
