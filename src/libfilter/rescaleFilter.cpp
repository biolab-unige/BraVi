//
// C++ Implementation: rescaleFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rescaleFilter.h"

#include <itkRescaleIntensityImageFilter.h>

namespace filter
{

rescaleFilter::rescaleFilter() : abstractFilter()
{
    setName("rescaleFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    addParameter(new filterParameter<unsigned char>(m_OutputMinimum, "Output Minimum", 0));
    addParameter(new filterParameter<unsigned char>(m_OutputMaximum, "Output Maximum", 255));

}


rescaleFilter::~rescaleFilter()
{
}

bool rescaleFilter::Execute( void )
{
    Update();

    typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;

    RescaleFilterType::Pointer rescale = RescaleFilterType::New();
    rescale->SetOutputMinimum(m_OutputMinimum);
    rescale->SetOutputMaximum(m_OutputMaximum);

    rescale->SetInput( m_InputPointer );

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
        std::cerr << "--- ERROR: rescaleFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = rescale->GetOutput();

    return true;
}











} //namespace filter
