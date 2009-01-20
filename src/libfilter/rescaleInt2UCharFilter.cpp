//
// C++ Implementation: rescaleInt2UCharFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rescaleInt2UCharFilter.h"

#include <itkRescaleIntensityImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkNiftiImageIO.h>

namespace filter
{

rescaleInt2UCharFilter::rescaleInt2UCharFilter() : abstractFilter()
{
    setName("rescaleInt2UCharFilter");

// Input
    addParameter(new filterParameter<std::string>(m_InputFileName, "Input File", "~"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri

    addParameter(new filterParameter<unsigned char>(m_OutputMinimum, "Output Minimum", 0));
    addParameter(new filterParameter<unsigned char>(m_OutputMaximum, "Output Maximum", 255));

}


rescaleInt2UCharFilter::~rescaleInt2UCharFilter()
{
}

bool rescaleInt2UCharFilter::Execute( void )
{
    Update();

    typedef itk::Image<int,3> IntImageType;



    typedef itk::ImageFileReader<IntImageType> ImageReaderType;
    ImageReaderType::Pointer imageReaderPointer = ImageReaderType::New();

    imageReaderPointer->SetFileName(m_InputFileName.c_str());

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


    typedef itk::RescaleIntensityImageFilter< IntImageType, ImageType > RescaleFilterType;

    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetOutputMinimum(m_OutputMinimum);
    rescale->SetOutputMaximum(m_OutputMaximum);

    rescale->SetInput( imageReaderPointer->GetOutput() );

// Esecuzione
    try
    {
        rescale->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: rescaleInt2UCharFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = rescale->GetOutput();

    return true;
}











} //namespace filter
