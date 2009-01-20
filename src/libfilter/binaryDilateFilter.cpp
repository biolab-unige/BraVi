//
// C++ Implementation: binaryDilateFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "binaryDilateFilter.h"

#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>

namespace filter
{

binaryDilateFilter::binaryDilateFilter() : abstractFilter()
{
    setName("binaryDilateFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<ImageType::ValueType>(m_Foreground, "Foreground", 255));
    addParameter(new filterParameter<ImageType::ValueType>(m_Background, "Background", 0));
    addParameter(new filterParameter<int>(m_DilateRadius, "Radius", 1));

}


binaryDilateFilter::~binaryDilateFilter()
{
}

bool binaryDilateFilter::Execute( void )
{
    Update();

// Creazione Kernel
    typedef itk::BinaryBallStructuringElement<ImageType::ValueType, Dimension> StructuringElementType;
    StructuringElementType structuringElement;
    structuringElement.SetRadius( m_DilateRadius );


//Creazione Filtro
    typedef itk::BinaryDilateImageFilter< ImageType, ImageType, StructuringElementType> BinaryDilateFilterType;

    BinaryDilateFilterType::Pointer binaryDilate = BinaryDilateFilterType::New();
    binaryDilate->SetKernel( structuringElement );
    binaryDilate->SetBackgroundValue(m_Background);
    binaryDilate->SetDilateValue(m_Foreground);

    binaryDilate->SetInput( m_InputPointer );



// Esecuzione
    try
    {
        std::cout << "BinaryDilate" << std::endl;
        binaryDilate->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: BinaryDilateFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = binaryDilate->GetOutput();

    return true;
}

} //namespace filter
