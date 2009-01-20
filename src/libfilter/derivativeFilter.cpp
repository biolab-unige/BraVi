//
// C++ Implementation: derivativeFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "derivativeFilter.h"
#include <itkDerivativeImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>


namespace filter
{

derivativeFilter::derivativeFilter() : abstractFilter()
{
    setName("derivativeFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    addParameter(new filterParameter<unsigned int>(m_Order, "Order", 2));
    addParameter(new filterParameter<unsigned int>(m_Direction, "Direction", 0));
    addParameter(new filterParameter<InternalPixelType>(m_RescaleInternalOutputMinimum, "Internal Minimum", 0.0));
    addParameter(new filterParameter<InternalPixelType>(m_RescaleInternalOutputMaximum, "Internal Maximum", 1.0));
    addParameter(new filterParameter<PixelType>(m_RescaleOutputMinimum, "Output Minimum", 0));
    addParameter(new filterParameter<PixelType>(m_RescaleOutputMaximum, "Output Maximum", 255));

}


derivativeFilter::~derivativeFilter()
{
}

bool derivativeFilter::Execute( void )
{
    Update();



    typedef itk::RescaleIntensityImageFilter<ImageType, InternalImageType> RescaleInternalIntensityFilterType;

    RescaleInternalIntensityFilterType::Pointer rescaleInternal = RescaleInternalIntensityFilterType::New();
    rescaleInternal->SetOutputMinimum(m_RescaleInternalOutputMinimum);
    rescaleInternal->SetOutputMaximum(m_RescaleInternalOutputMaximum);
    rescaleInternal->SetInput( m_InputPointer );

    typedef itk::DerivativeImageFilter< InternalImageType, InternalImageType >  DerivativeFilterType;
    DerivativeFilterType::Pointer derivative = DerivativeFilterType::New();
    derivative->SetOrder( m_Order );
    derivative->SetDirection( m_Direction );
    derivative->SetInput( rescaleInternal->GetOutput() );

    typedef itk::RescaleIntensityImageFilter<InternalImageType, ImageType> RescaleIntensityFilterType;
    RescaleIntensityFilterType::Pointer rescale = RescaleIntensityFilterType::New();
    rescale->SetOutputMinimum(m_RescaleOutputMinimum);
    rescale->SetOutputMaximum(m_RescaleOutputMaximum);
    rescale->SetInput( derivative->GetOutput() );


// Esecuzione
    try
    {
        rescaleInternal->Update();
        derivative->Update();
        rescale->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: derivativeFilter ---" << std::endl << message << std::endl;
        return false;
    }


    m_OutputPointer = rescale->GetOutput();

    return true;
}











} //namespace filter
