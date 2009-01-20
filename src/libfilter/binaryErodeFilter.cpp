//
// C++ Implementation: binaryErodeFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "binaryErodeFilter.h"

#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryBallStructuringElement.h>

namespace filter
{

binaryErodeFilter::binaryErodeFilter() : abstractFilter()
{
    setName("binaryErodeFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<ImageType::ValueType>(m_Foreground, "Foreground", 255));
    addParameter(new filterParameter<ImageType::ValueType>(m_Background, "Background", 0));
    addParameter(new filterParameter<int>(m_ErodeRadius, "Radius", 1));

}


binaryErodeFilter::~binaryErodeFilter()
{
}

bool binaryErodeFilter::Execute( void )
{
    Update();

// Creazione Kernel
    typedef itk::BinaryBallStructuringElement<ImageType::ValueType, Dimension> StructuringElementType;
    StructuringElementType structuringElement;
    structuringElement.SetRadius( m_ErodeRadius );


//Creazione Filtro
    typedef itk::BinaryErodeImageFilter< ImageType, ImageType, StructuringElementType> BinaryErodeFilterType;

    BinaryErodeFilterType::Pointer binaryErode = BinaryErodeFilterType::New();
    binaryErode->SetKernel( structuringElement );
    binaryErode->SetBackgroundValue(m_Background);
    binaryErode->SetErodeValue(m_Foreground);

    binaryErode->SetInput( m_InputPointer );



// Esecuzione
    try
    {
        std::cout << "BinaryErode" << std::endl;
        binaryErode->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: BinaryErodeFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = binaryErode->GetOutput();

    return true;
}

} //namespace filter
