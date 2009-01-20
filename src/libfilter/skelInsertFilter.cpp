//
// C++ Implementation: convertFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "skelInsertFilter.h"

#include <fstream>
#include <cmath>

#include <itkImageRegionIterator.h>

#define OUT_VALUE 0
#define ARTIFACT_VALUE 100
#define AXIS_VALUE 255


namespace filter
{

skelInsertFilter::skelInsertFilter() : abstractFilter()
{
    setName("skelInsertFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<std::string>(m_Filename, "Skeleton file", "/home/biolab/tmp/08/skel.img-hd1.00.skel"));
}


skelInsertFilter::~skelInsertFilter()
{
}

bool skelInsertFilter::Execute( void )
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


    typedef itk::ImageRegionIterator<ImageType> IteratorType;

    IteratorType input( m_InputPointer, m_InputPointer->GetRequestedRegion() );
    IteratorType output( m_OutputPointer, m_OutputPointer->GetRequestedRegion() );

    std::cout << "Start" << std::endl;
    for ( input.GoToBegin(), input.GoToBegin(); !input.IsAtEnd(); ++input, ++output)
    {
        if(input.Get() == 0)
            output.Set(OUT_VALUE);
        else
            output.Set(ARTIFACT_VALUE);
    }
    std::cout << "End" << std::endl;

    std::ifstream is;
    is.open (m_Filename.c_str(), std::ios::binary );


    while (is.good())
    {


        float in[3];
        int l;
        float p;

        double d[3];
        float f[3];
        ImageType::IndexType pos;
        // Read a line
        is >> in[0] >> in[1] >> in[2] >> l >> p;

        for (int i=0; i<3; i++)
        {
            f[i] = modf (in[i], &d[i]);
            if (f[i]>0.5) ++d[i];
        }

        pos[0] = (ImageType::IndexValueType)d[0];
        pos[1] = (ImageType::IndexValueType)d[1];
        pos[2] = (ImageType::IndexValueType)d[2];

        m_OutputPointer->SetPixel(pos, AXIS_VALUE);
        std::cout << pos << std::endl;
    }
    return true;
}











} //namespace filter
