//
// C++ Implementation: DanielssonDistanceMapFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "DanielssonDistanceMapFilter.h"

#include <itkDanielssonDistanceMapImageFilter.h>
//#include <itkRescaleIntensityImageFilter.h>

namespace filter
{

DanielssonDistanceMapFilter::DanielssonDistanceMapFilter() : abstractFilter()
{
    setName("DanielssonDistanceMapFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parameters
    addParameter(new filterParameter<PixelType>(m_RescaleOutputMinimum, "Minimum", 0));
    addParameter(new filterParameter<PixelType>(m_RescaleOutputMaximum, "Maximum", 255));
}


DanielssonDistanceMapFilter::~DanielssonDistanceMapFilter()
{
}

bool DanielssonDistanceMapFilter::Execute( void )
{
    Update();



//Creazione Filtro
//    typedef itk::DanielssonDistanceMapImageFilter< ImageType, InternalImageType> DanielssonDistanceMapFilterType;
    typedef itk::DanielssonDistanceMapImageFilter< ImageType, ImageType> DanielssonDistanceMapFilterType;

    DanielssonDistanceMapFilterType::Pointer DanielssonDistanceMap = DanielssonDistanceMapFilterType::New();
    DanielssonDistanceMap->InputIsBinaryOn();

    DanielssonDistanceMap->SetInput( m_InputPointer );

/*
    typedef itk::RescaleIntensityImageFilter<InternalImageType, ImageType> RescaleIntensityFilterType;
    RescaleIntensityFilterType::Pointer rescale = RescaleIntensityFilterType::New();
    rescale->SetOutputMinimum(m_RescaleOutputMinimum);
    rescale->SetOutputMaximum(m_RescaleOutputMaximum);
    rescale->SetInput( DanielssonDistanceMap->GetOutput() );
*/

// Esecuzione
    try
    {
        std::cout << "DanielssonDistanceMap" << std::endl;
        DanielssonDistanceMap->Update();
//        std::cout << "Rescale" << std::endl;
//        rescale->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: DanielssonDistanceMapFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = DanielssonDistanceMap->GetOutput();

    return true;
}

} //namespace filter
