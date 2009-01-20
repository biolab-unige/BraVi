//
// C++ Implementation: maximumFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "maximumFilter.h"
#include <itkMaximumImageFilter.h>

namespace filter
{

maximumFilter::maximumFilter() : abstractFilter()
{
    setName("maximumFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer1, "Input File #1"));
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer2, "Input File #2"));


// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
}


maximumFilter::~maximumFilter()
{
}

bool maximumFilter::Execute( void )
{
    Update();


// Creazione Filtro

// Creazione Filtro
    typedef itk::MaximumImageFilter< ImageType, ImageType, ImageType > MaximumImageFilterType;
    MaximumImageFilterType::Pointer maximum = MaximumImageFilterType::New();

    maximum->SetInput1( m_InputPointer1 );
    maximum->SetInput2( m_InputPointer2 );

// Esecuzione
    try
    {
        std::cout << "---   Maximum Image:                                              " << std::flush;
        maximum->Update();
        std::cout << "OK   ---" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: maximumFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = maximum->GetOutput();

    return true;
}











} //namespace filter
