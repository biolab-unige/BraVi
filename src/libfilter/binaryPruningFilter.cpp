//
// C++ Implementation: binaryPruningFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "binaryPruningFilter.h"

#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryPruningImageFilter.h>

namespace filter
{

binaryPruningFilter::binaryPruningFilter() : abstractFilter()
{
    setName("binaryPruningFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
}


binaryPruningFilter::~binaryPruningFilter()
{
}

bool binaryPruningFilter::Execute( void )
{
    Update();


    typedef itk::BinaryThresholdImageFilter< ImageType, ImageType > ThresholdingFilterType;
    ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
    thresholder->SetLowerThreshold(1);
    thresholder->SetUpperThreshold(255);
    thresholder->SetOutsideValue(0);
    thresholder->SetInsideValue(1);
    thresholder->SetInput( m_InputPointer );


    typedef itk::BinaryPruningImageFilter< ImageType, ImageType > BinaryPruningFilterType;

    BinaryPruningFilterType::Pointer binaryPruning = BinaryPruningFilterType::New();
    binaryPruning->SetInput( thresholder->GetOutput() );
    binaryPruning->DebugOn();

    ThresholdingFilterType::Pointer thresholder2 = ThresholdingFilterType::New();
    thresholder2->SetLowerThreshold(1);
    thresholder2->SetUpperThreshold(255);
    thresholder2->SetOutsideValue(0);
    thresholder2->SetInsideValue(255);
    thresholder2->SetInput( binaryPruning->GetOutput() );



// Esecuzione
    try
    {
        std::cerr << "Thresholder 1" << std::endl;
        thresholder->Update();
        std::cout << "BinaryPruning" << std::endl;
        binaryPruning->Update();
        std::cout << "Thresholder 2" << std::endl;
        thresholder2->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: sigmoidFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = thresholder2->GetOutput();

    return true;
}

} //namespace filter
