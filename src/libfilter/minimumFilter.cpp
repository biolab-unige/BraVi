//
// C++ Implementation: minimumFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "minimumFilter.h"
#include <itkMinimumImageFilter.h>

namespace filter
{

minimumFilter::minimumFilter() : abstractFilter()
{
    setName("minimumFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer1, "Input File #1"));
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer2, "Input File #2"));


// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
}


minimumFilter::~minimumFilter()
{
}

bool minimumFilter::Execute( void )
{
    Update();


// Creazione Filtro

// Creazione Filtro
    typedef itk::MinimumImageFilter< ImageType, ImageType, ImageType > MinimumImageFilterType;
    MinimumImageFilterType::Pointer minimum = MinimumImageFilterType::New();

    minimum->SetInput1( m_InputPointer1 );
    minimum->SetInput2( m_InputPointer2 );

// Esecuzione
    try
    {
        std::cout << "---   Minimum Image:                                              " << std::flush;
        minimum->Update();
        std::cout << "OK   ---" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: minimumFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = minimum->GetOutput();

    return true;
}











} //namespace filter
