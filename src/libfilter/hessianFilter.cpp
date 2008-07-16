//
// C++ Implementation: hessianFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "hessianFilter.h"

#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkNiftiImageIO.h>


namespace filter
{

hessianFilter::hessianFilter() : abstractFilter()
{
    setName("hessianFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

//Parameters
    addParameter(new filterParameter<double>(m_Sigma,  "Sigma",  1.0));
    addParameter(new filterParameter<std::string>(m_OutputFileName, "Output File", "~"));

}


hessianFilter::~hessianFilter()
{
}

bool hessianFilter::Execute( void )
{
    Update();


    typedef itk::HessianRecursiveGaussianImageFilter< ImageType > hrgType;

    hrgType::Pointer hessian = hrgType::New();
    hessian->SetInput(m_InputPointer);

    hessian->SetSigma(m_Sigma);





// Esecuzione
    try
    {
        std::cout << "hessianFilter: Hessian" << std::endl;
        hessian->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: HessianFilter ---" << std::endl << message << std::endl;
        return false;
    }

    typedef itk::ImageFileWriter<hrgType::OutputImageType> ImageWriterType;
    ImageWriterType::Pointer imageWriterPointer = ImageWriterType::New();

    imageWriterPointer->SetFileName(m_OutputFileName.c_str());

    imageWriterPointer->SetInput( hessian->GetOutput() );

    try
    {
        std::cout << "hessianFilter: Write" << std::endl;
        imageWriterPointer->Update();
        imageWriterPointer->Write();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: writer ---" << std::endl << message << std::endl;
        return false;
    }
    return true;

}

} //namespace filter
