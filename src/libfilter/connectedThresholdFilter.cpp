//
// C++ Implementation: connectedThresholdFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "connectedThresholdFilter.h"
#include <itkConnectedThresholdImageFilter.h>

namespace filter
{

connectedThresholdFilter::connectedThresholdFilter() : abstractFilter()
{
    setName("connectedThresholdFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    addParameter(new filterParameter<long int>(m_Seed[0], "Seed [0]", (long int)165));
    addParameter(new filterParameter<long int>(m_Seed[1], "Seed [1]", (long int)197));
    addParameter(new filterParameter<long int>(m_Seed[2], "Seed [2]", (long int)161));

    addParameter(new filterParameter<ImageType::ValueType>(m_Lower, "Lower Value", 0));
    addParameter(new filterParameter<ImageType::ValueType>(m_Upper, "Upper Value", 128));
    addParameter(new filterParameter<ImageType::ValueType>(m_ReplaceValue, "Replace Value", 255));
}


connectedThresholdFilter::~connectedThresholdFilter()
{
}

bool connectedThresholdFilter::Execute( void )
{
    Update();


// Creazione Filtro
    typedef itk::ConnectedThresholdImageFilter< ImageType, ImageType > ConnectedThresholdFilterType;
    ConnectedThresholdFilterType::Pointer connectedThreshold = ConnectedThresholdFilterType::New();

    connectedThreshold->SetLower(m_Lower);
    connectedThreshold->SetUpper(m_Upper);
    connectedThreshold->SetReplaceValue(m_ReplaceValue);

    connectedThreshold->SetSeed(m_Seed);

    std::cout << "Seed: " << m_Seed << " -> Value: " << (int)m_InputPointer->GetPixel(m_Seed) << std::endl;


    connectedThreshold->SetInput( m_InputPointer );

// Esecuzione
    try
    {
        std::cout << "---   Connected Threshold:                                              " << std::flush;
        connectedThreshold->Update();
        std::cout << "OK   ---" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: connectedThresholdFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = connectedThreshold->GetOutput();

    return true;
}











} //namespace filter
