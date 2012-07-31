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
set(BRAVI_LIBRARIES ${VTK_LIBRARIES} ${QT_LIBRARIES} ${NIFTI_LIBRARIES})


if(WIN32)
  set(WIN32_RES ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/BraVi.dir/resfile.obj)
  add_custom_command(OUTPUT WIN32_RES COMMAND windres.exe resfile.rc -o ${WIN32_RES} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res )
  add_executable(BraVi WIN32 ${SOURCES} ${HEADERS} ${MOC_FILES} ${QT4_RES} ${WIN32_RES}) #WIN32 fa in modo che l'applicazione non venga lanciata assieme ad una shell
  set(WINDOWS_LIBRARIES wininet dsound ole32 winmm gdi32 uuid shell32 vfw32 rpcrt4 oleaut32 comdlg32 winspool)
  set(BRAVI_LIBRARIES ${BRAVI_LIBRARIES} ${WINDOWS_LIBRARIES} )
  add_custom_command(TARGET BraVi POST_BUILD COMMAND strip -sxv ${CMAKE_CURRENT_BINARY_DIR}/xtensviewer.exe)
else(WIN32)
  add_executable(BraVi ${SOURCES} ${HEADERS} ${MOC_FILES} ${QT4_RES})
  set(BRAVI_LIBRARIES ${BRAVI_LIBRARIES})
endif(WIN32)

target_link_libraries(BraVi ${BRAVI_LIBRARIES})

install(TARGETS BraVi RUNTIME DESTINATION bin)

if(UNIX)
  install(FILES res/BraVi.png DESTINATION share/icons)
  install(FILES res/BraVi.desktop DESTINATION share/applications)
endif(UNIX)
