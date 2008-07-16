//
// C++ Implementation: rescaleUShort2UCharFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rescaleUShort2UCharFilter.h"

#include <itkRescaleIntensityImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkNiftiImageIO.h>

namespace filter
{

rescaleUShort2UCharFilter::rescaleUShort2UCharFilter() : abstractFilter()
{
    setName("rescaleUShort2UCharFilter");

// Input
    addParameter(new filterParameter<std::string>(m_InputFileName, "Input File", "~"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri

    addParameter(new filterParameter<unsigned char>(m_OutputMinimum, "Output Minimum", 0));
    addParameter(new filterParameter<unsigned char>(m_OutputMaximum, "Output Maximum", 255));

}


rescaleUShort2UCharFilter::~rescaleUShort2UCharFilter()
{
}

bool rescaleUShort2UCharFilter::Execute( void )
{
    Update();

    typedef itk::Image<unsigned short,3> UShortImageType;



    typedef itk::ImageFileReader<UShortImageType> ImageReaderType;
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


    typedef itk::RescaleIntensityImageFilter< UShortImageType, ImageType > RescaleFilterType;

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
        std::cerr << "--- ERROR: rescaleUShort2UCharFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = rescale->GetOutput();

    return true;
}











} //namespace filter
