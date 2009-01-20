//
// C++ Implementation: fastMarchingFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fastMarchingFilter.h"

#include <itkFastMarchingImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

namespace filter
{

fastMarchingFilter::fastMarchingFilter() : abstractFilter()
{
    setName("fastMarchingFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    addParameter(new filterParameter<double>(m_StoppingTime, "Stopping Value (Stopping Time)", 0.0));
    addParameter(new filterParameter<double>(m_SpeedConstant, "Speed Constant", 1));
    addParameter(new filterParameter<double>(m_SeedValue, "Seed Value", 0.0));

    addParameter(new filterParameter<ImageType::IndexValueType>(m_Seed[0], "Seed [0]", (ImageType::IndexValueType)165));
    addParameter(new filterParameter<ImageType::IndexValueType>(m_Seed[1], "Seed [1]", (ImageType::IndexValueType)197));
    addParameter(new filterParameter<ImageType::IndexValueType>(m_Seed[2], "Seed [2]", (ImageType::IndexValueType)161));

// Seeds

    addParameter(new filterParameter<InternalPixelType>(m_RescaleInternalOutputMinimum, "Internal Minimum", 0.0));
    addParameter(new filterParameter<InternalPixelType>(m_RescaleInternalOutputMaximum, "Internal Maximum", 1.0));

    addParameter(new filterParameter<PixelType>(m_RescaleOutputMinimum, "Minimum", 0));
    addParameter(new filterParameter<PixelType>(m_RescaleOutputMaximum, "Maximum", 255));
}


fastMarchingFilter::~fastMarchingFilter()
{
}

bool fastMarchingFilter::Execute( void )
{
    Update();


    typedef itk::RescaleIntensityImageFilter<ImageType, InternalImageType> RescaleInternalIntensityFilterType;

    RescaleInternalIntensityFilterType::Pointer rescaleInternal = RescaleInternalIntensityFilterType::New();
    rescaleInternal->SetOutputMinimum(m_RescaleInternalOutputMinimum);
    rescaleInternal->SetOutputMaximum(m_RescaleInternalOutputMaximum);
    rescaleInternal->SetInput( m_InputPointer );




    typedef itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
    FastMarchingFilterType::Pointer fastMarching = FastMarchingFilterType::New();
    fastMarching->SetInput( rescaleInternal->GetOutput() );


    typedef FastMarchingFilterType::NodeContainer NodeContainer;
    typedef FastMarchingFilterType::NodeType NodeType;
    NodeContainer::Pointer seeds = NodeContainer::New();

    NodeType node;
    node.SetValue( m_SeedValue );
    node.SetIndex( m_Seed );
    seeds->Initialize();
    seeds->InsertElement(0,node);

    fastMarching->SetTrialPoints(seeds);
    fastMarching->SetOutputSize( rescaleInternal->GetOutput()->GetBufferedRegion().GetSize() );

    if (m_StoppingTime)
    {
        std::cout << "Stopping Time = " << m_StoppingTime << std::endl;
        fastMarching->SetStoppingValue( m_StoppingTime );
    }
    if (m_SpeedConstant)
    {
        std::cout << "SpeedConstant = " << m_SpeedConstant << std::endl;
        fastMarching->SetSpeedConstant( m_SpeedConstant );
    }


    typedef itk::RescaleIntensityImageFilter<InternalImageType, ImageType> RescaleIntensityFilterType;
    RescaleIntensityFilterType::Pointer rescale = RescaleIntensityFilterType::New();
    rescale->SetOutputMinimum(m_RescaleOutputMinimum);
    rescale->SetOutputMaximum(m_RescaleOutputMaximum);
    rescale->SetInput( fastMarching->GetOutput() );

// Esecuzione
    try
    {
        std::cout << "Fast Marching:" << std::endl;
        std::cout << "- Rescale internal" << std::endl;
        rescaleInternal->Update();
        std::cout << "- Fast Marching" << std::endl;
        fastMarching->Update();
        std::cout << "- Rescale" << std::endl;
        rescale->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: fastMarchingFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = rescale->GetOutput();

    return true;
}











} //namespace filter
