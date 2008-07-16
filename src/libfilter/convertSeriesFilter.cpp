//
// C++ Implementation: convertSeriesFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "convertSeriesFilter.h"

#include <itkDICOMImageIO2Factory.h>
#include <itkDICOMImageIO2.h>
#include <itkImageSeriesReader.h>
#include <itkDICOMSeriesFileNames.h>
//#include <BasicFilters/itkFilterWatcher.h>

#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkNiftiImageIO.h>

namespace filter
{

convertSeriesFilter::convertSeriesFilter() : abstractFilter()
{
    setName("convertSeriesFilter");

// Parameters
    addParameter(new filterParameter<std::string>(m_DirectoryName, "Directory", "~"));
    addParameter(new filterParameter<unsigned char>(m_ReverseOrder, "Reverse Order", 0, 0, 1));
    addParameter(new filterParameter<std::string>(m_OutputFileName, "Output File", "~"));
}


convertSeriesFilter::~convertSeriesFilter()
{
}

bool convertSeriesFilter::Execute( void )
{
    Update();


    typedef itk::Image<unsigned short,3> DICOMImageType;
    typedef itk::ImageSeriesReader<DICOMImageType> ReaderType;

    itk::DICOMImageIO2::Pointer io = itk::DICOMImageIO2::New();

    // Get the DICOM filenames from the directory
    itk::DICOMSeriesFileNames::Pointer names = itk::DICOMSeriesFileNames::New();
    names->SetDirectory(m_DirectoryName.c_str());

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileNames(names->GetFileNames());
    reader->SetImageIO(io);
    std::cout << names;

//    FilterWatcher watcher(reader);

    try
    {
        if (m_ReverseOrder)
        {
            reader->ReverseOrderOn();
        }
        reader->Update();
        reader->GetOutput()->Print(std::cout);
    }

    catch (itk::ExceptionObject &ex)
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: reader ---" << std::endl << message << std::endl;
        return false;

    }

    typedef itk::ImageFileWriter<DICOMImageType> ImageWriterType;
    ImageWriterType::Pointer imageWriterPointer = ImageWriterType::New();

    imageWriterPointer->SetFileName(m_OutputFileName.c_str());

    imageWriterPointer->SetInput( reader->GetOutput() );

    try
    {
        imageWriterPointer->Update();
        imageWriterPointer->Write();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: writer ---" << std::endl << message << std::endl;
        return false;
    }
    return true;

}











} //namespace filter
