//
// C++ Implementation: gradientMagnitudeFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "gradientMagnitudeFilter.h"
#include <itkGradientMagnitudeImageFilter.h>

namespace filter
{

gradientMagnitudeFilter::gradientMagnitudeFilter() : abstractFilter()
{
    setName("gradientMagnitudeFilter");

// Parametri

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
}


gradientMagnitudeFilter::~gradientMagnitudeFilter()
{
}

bool gradientMagnitudeFilter::Execute( void )
{
    Update();

// Creazione Filtro
//    typedef itk::GradientMagnitudeRecursiveGaussianImageFilter< InputImageType, OutputImageType > GradientFilterType;
//    gradientMagnitude->SetSigma(gradientMagnitudeSigma);


    typedef itk::GradientMagnitudeImageFilter< ImageType, ImageType > GradientMagnitudeFilterType;
    GradientMagnitudeFilterType::Pointer gradientMagnitude = GradientMagnitudeFilterType::New();

    gradientMagnitude->SetInput( m_InputPointer );

// Esecuzione
    try
    {
        gradientMagnitude->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: gradientMagnitudeFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = gradientMagnitude->GetOutput();

    return true;
}











} //namespace filter
