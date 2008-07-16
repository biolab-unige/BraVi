//
// C++ Implementation: binaryThresholdFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "binaryThresholdFilter.h"

#include <itkBinaryThresholdImageFilter.h>

namespace filter
{

binaryThresholdFilter::binaryThresholdFilter() : abstractFilter()
{
    setName("binaryThresholdFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    addParameter(new filterParameter<ImageType::ValueType>(m_LowerThreshold, "Lower Threshold", 1));
    addParameter(new filterParameter<ImageType::ValueType>(m_UpperThreshold, "Upper Threshold", 255));
    addParameter(new filterParameter<ImageType::ValueType>(m_OutsideValue, "Outside Value", 255));
    addParameter(new filterParameter<ImageType::ValueType>(m_InsideValue, "Inside Value", 0));
}


binaryThresholdFilter::~binaryThresholdFilter()
{
}

bool binaryThresholdFilter::Execute( void )
{
    Update();

// Thresholder
    typedef itk::BinaryThresholdImageFilter<ImageType, ImageType > BinaryThresholdFilterType;
    BinaryThresholdFilterType::Pointer binaryThreshold = BinaryThresholdFilterType::New();
    binaryThreshold->SetLowerThreshold(m_LowerThreshold);
    binaryThreshold->SetUpperThreshold(m_UpperThreshold);
    binaryThreshold->SetOutsideValue(m_OutsideValue);
    binaryThreshold->SetInsideValue(m_InsideValue);
    binaryThreshold->SetInput( m_InputPointer );

// Esecuzione
    try
    {
        std::cout << "---   Binary Threshold:                                                 " << std::flush;
        binaryThreshold->Update();
        std::cout << "OK   ---" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: binaryThreshold ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = binaryThreshold->GetOutput();

    return true;
}


} //namespace filter
