//
// C++ Implementation: convertFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "padFilter.h"

#include <fstream>
#include <cmath>

#include <itkImageRegionIteratorWithIndex.h>



namespace filter
{

padFilter::padFilter() : abstractFilter()
{
    setName("padFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<unsigned int>(m_PadSize, "Pad Size", 1));
    addParameter(new filterParameter<unsigned char>(m_PadReplaceValue, "Pad ReplaceValue", 0));
}


padFilter::~padFilter()
{
}

bool padFilter::Execute( void )
{
    Update();

    m_OutputPointer = ImageType::New();
    m_OutputPointer->SetRegions( m_InputPointer->GetRequestedRegion() );
    m_OutputPointer->CopyInformation( m_InputPointer );
    try
    {
        m_OutputPointer->Allocate();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR ---" << std::endl << message << std::endl;
        return false;
    }


    typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;

    IteratorType input( m_InputPointer, m_InputPointer->GetRequestedRegion() );
    IteratorType output( m_OutputPointer, m_OutputPointer->GetRequestedRegion() );

    ImageType::IndexType index;
    ImageType::SizeType size = m_InputPointer->GetLargestPossibleRegion().GetSize();
    std::cout << "Start" << std::endl;
    for ( input.GoToBegin(), output.GoToBegin(); !input.IsAtEnd(); ++input, ++output)
    {
        output.Set(input.Get());

        index = input.GetIndex();
        for (int i = 0; i < Dimension; i++)
        {
            if (index[i] < m_PadSize || index[i] > size[i] - 1 - m_PadSize )
            {
                output.Set( m_PadReplaceValue);
                break;
            }
        }

    }
    std::cout << "End" << std::endl;

    return true;
}











} //namespace filter
