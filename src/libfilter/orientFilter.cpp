//
// C++ Implementation: orientFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "orientFilter.h"
#include <itkOrientImageFilter.h>

namespace filter
{

orientFilter::orientFilter() : abstractFilter()
{
    setName("orientFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
}


orientFilter::~orientFilter()
{
}

bool orientFilter::Execute( void )
{
    Update();

    typedef itk::OrientImageFilter< ImageType, ImageType > OrientFilterType;

    OrientFilterType::Pointer orient = OrientFilterType::New();

    orient->UseImageDirectionOn();
    //orient->SetGivenCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI);
    orient->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_LPI);

    orient->SetInput( m_InputPointer );

// Esecuzione
    try
    {
        orient->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: orientFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = orient->GetOutput();

    return true;
}











} //namespace filter
