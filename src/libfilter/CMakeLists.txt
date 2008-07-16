SET (FILTERS_QT_HEADERS filterDialog.h
                        filterIOWidget.h
                        fileFilterIOWidget.h
                        vtkFilterIOWidget.h )

SET (FILTERS_QT_SOURCES filterDialog.cpp
                        fileFilterIOWidget.cpp
                        vtkFilterIOWidget.cpp )

SET (FILTERS_HEADERS    reader.h
                        filterParameter.h
                        filterInput.h
                        filterOutput.h
                        abstractFilter.h
                        applyRigidTransformFilter.h
                        betFilter.h
                        binaryDilateFilter.h
                        binaryErodeFilter.h
                        binaryPruningFilter.h
                        binaryThinningFilter.h
                        binaryThresholdFilter.h
                        connectedThresholdFilter.h
                        convertFilter.h
                        convertSeriesFilter.h
                        DanielssonDistanceMapFilter.h
                        derivativeFilter.h
                        electrodeAxisExtractionFilter.h
                        electrodeAxisExtractionFilter2.h
                        electrodeAxisExtractionFilter3.h
                        fastMarchingFilter.h
                        geodesicActiveContourFilter.h
                        geodesicActiveContourFullFilter.h
                        gradientMagnitudeFilter.h
                        hessianFilter.h
                        maximumFilter.h
                        minimumFilter.h
                        orientFilter.h
                        padFilter.h
                        rescaleFilter.h
                        rescaleInt2UCharFilter.h
                        rescaleUShort2UCharFilter.h
                        testFilter.h
                        sigmoidFilter.h
                        skelInsertFilter.h
                        ${FILTERS_QT_HEADERS} )

SET (FILTERS_SOURCES    reader.cpp
                        applyRigidTransformFilter.cpp
                        betFilter.cpp
                        binaryDilateFilter.cpp
                        binaryErodeFilter.cpp
                        binaryPruningFilter.cpp
                        binaryThinningFilter.cpp
                        binaryThresholdFilter.cpp
                        connectedThresholdFilter.cpp
                        convertFilter.cpp
                        convertSeriesFilter.cpp
                        DanielssonDistanceMapFilter.cpp
                        derivativeFilter.cpp
                        electrodeAxisExtractionFilter.cpp
                        electrodeAxisExtractionFilter2.cpp
                        electrodeAxisExtractionFilter3.cpp
                        fastMarchingFilter.cpp
                        geodesicActiveContourFilter.cpp
                        geodesicActiveContourFullFilter.cpp
                        gradientMagnitudeFilter.cpp
                        hessianFilter.cpp
                        maximumFilter.cpp
                        minimumFilter.cpp
                        orientFilter.cpp
                        padFilter.cpp
                        rescaleFilter.cpp
                        rescaleInt2UCharFilter.cpp
                        rescaleUShort2UCharFilter.cpp
                        testFilter.cpp
                        sigmoidFilter.cpp
                        skelInsertFilter.cpp
                        ${FILTERS_QT_SOURCES} )



QT4_WRAP_CPP( FILTERS_MOC_FILES ${FILTERS_QT_HEADERS} )

SET (LIBFILTER_SOURCES  ${FILTERS_HEADERS}
                        ${FILTERS_SOURCES}
                        ${FILTERS_MOC_FILES} )

ADD_LIBRARY(filter ${LIBFILTER_SOURCES} )
TARGET_LINK_LIBRARIES(filter ${QT_LIBRARIES} ${ITK_LIBRARIES})