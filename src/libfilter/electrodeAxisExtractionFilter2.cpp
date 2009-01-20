//
// C++ Implementation: electrodeAxisExtractionFilter2
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "electrodeAxisExtractionFilter2.h"

#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkHessian3DToVesselnessMeasureImageFilter.h>


namespace filter
{

electrodeAxisExtractionFilter2::electrodeAxisExtractionFilter2() : abstractFilter()
{
    setName("electrodeAxisExtractionFilter2");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<double>(m_Sigma,  "Sigma",  1.0));
    addParameter(new filterParameter<double>(m_Alpha1, "Alpha1", 0.5));
    addParameter(new filterParameter<double>(m_Alpha2, "Alpha2", 2.0));

}


electrodeAxisExtractionFilter2::~electrodeAxisExtractionFilter2()
{
}

bool electrodeAxisExtractionFilter2::Execute( void )
{
    Update();


    typedef itk::HessianRecursiveGaussianImageFilter< ImageType > hrgType;
    typedef itk::Hessian3DToVesselnessMeasureImageFilter< ImageType::PixelType > hvmType;

    hrgType::Pointer hessian = hrgType::New();
    hessian->SetInput(m_InputPointer);

    hessian->SetSigma(m_Sigma);

    hvmType::Pointer vesselFilter = hvmType::New();
    vesselFilter->SetInput(hessian->GetOutput());
    vesselFilter->SetAlpha1(m_Alpha1);
    vesselFilter->SetAlpha2(m_Alpha2);



// Esecuzione
    try
    {
        std::cout << "ElectrodeAxisExtractionFilter: Hessian" << std::endl;
        hessian->Update();
        std::cout << "ElectrodeAxisExtractionFilter: VesselFilter" << std::endl;
        vesselFilter->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: ElectrodeAxisExtractionFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = vesselFilter->GetOutput();

    return true;

}

} //namespace filter
