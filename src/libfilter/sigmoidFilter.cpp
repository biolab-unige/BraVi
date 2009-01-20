//
// C++ Implementation: sigmoidFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sigmoidFilter.h"
#include <itkSigmoidImageFilter.h>

namespace filter
{

sigmoidFilter::sigmoidFilter() : abstractFilter()
{
    setName("sigmoidFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    addParameter(new filterParameter<char>(m_Alpha, "Alpha", 3));
    addParameter(new filterParameter<unsigned char>(m_Beta, "Beta", 30));
    addParameter(new filterParameter<unsigned char>(m_OutputMinimum, "Output Minimum", 0));
    addParameter(new filterParameter<unsigned char>(m_OutputMaximum, "Output Maximum", 255));

}


sigmoidFilter::~sigmoidFilter()
{
}

bool sigmoidFilter::Execute( void )
{
    Update();

    typedef itk::SigmoidImageFilter< ImageType, ImageType > SigmoidFilterType;

    SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();
    sigmoid->SetOutputMinimum(m_OutputMinimum);
    sigmoid->SetOutputMaximum(m_OutputMaximum);
    sigmoid->SetAlpha(m_Alpha);
    sigmoid->SetBeta(m_Beta);

    sigmoid->SetInput( m_InputPointer );

// Esecuzione
    try
    {
        sigmoid->Update();
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

    m_OutputPointer = sigmoid->GetOutput();

    return true;
}











} //namespace filter
