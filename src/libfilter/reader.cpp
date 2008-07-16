//
// C++ Implementation: reader
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "reader.h"


#include <iostream>


namespace filter
{

bool reader::readVTK(vtkAlgorithmOutput* input, ImageType::Pointer outputPointer)
{
/*
    //EXPORTER VTK
    vtkImageExport* vtkExporter = vtkImageExport::New();

    //IMPORTER ITK
    typedef itk::VTKImageImport<ImageType> ImageImportType;
    ImageImportType::Pointer itkImporter = ImageImportType::New();

    vtkExporter->SetInputConnection(input);
    ConnectPipelines(vtkExporter, itkImporter);
    vtkExporter->Export();


    outputPointer = itkImporter->GetOutput();
    //TODO Forse bisogna incrementargli il reference count
*/
    return true;
}





bool reader::readNifti(std::string inputFilename, ImageType::Pointer outputPointer)
{

    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    ImageReaderType::Pointer imageReaderPointer = ImageReaderType::New();
//    imageReaderPointer->ReleaseDataFlagOn();
    imageReaderPointer->SetFileName(inputFilename.c_str());

    try
    {
        imageReaderPointer->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: reader ---" << std::endl << message << std::endl;
        return false;
    }

    outputPointer = imageReaderPointer->GetOutput();

    return true;
}



} //namespace filter
