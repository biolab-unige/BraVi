//
// C++ Implementation: convertFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2008
//         Gabriele Arnulfo <gabriele.arnulfo@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "applyRigidTransformFilter.h"

#include "itkResampleImageFilter.h"
#include "itkTransformFileReader.h"
#include "itkCenteredAffineTransform.h"



namespace filter
{

applyRigidTransformFilter::applyRigidTransformFilter() : abstractFilter()
{
    setName("applyRigidTransformFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<std::string>(m_TransformFilename, "Transform file", "/home/biolab/tmp/transform/transform.file"));
}


applyRigidTransformFilter::~applyRigidTransformFilter()
{
}

bool applyRigidTransformFilter::Execute( void )
{
    Update();

    typedef itk::TransformFileReader TransformReader;
    typedef itk::ResampleImageFilter< ImageType, ImageType > ResampleFilterType;
    typedef itk::CenteredAffineTransform<double, Dimension> TransformType;

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    TransformReader::Pointer transformreader = TransformReader::New();
    TransformType::Pointer transform = TransformType::New();

    transformreader->SetFileName(m_TransformFilename.c_str());

    try
    {
        std::cout << "Reading Transform File: " << m_TransformFilename << std::flush;
        transformreader->Update();
        std::cout << "OK" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: applyRigidTransformFilter ---" << std::endl << message << std::endl;
        return false;
    }

    TransformReader::TransformListType * transforms = transformreader->GetTransformList();
    transform->SetParameters((*transforms->begin())->GetParameters());

    resampler->SetTransform(transform);

    resampler->SetOutputParametersFromImage(m_InputPointer);
    resampler->SetDefaultPixelValue(0);
    resampler->SetInput(m_InputPointer);

    try
    {

        std::cout << "Resample: " << std::flush;
        resampler->Update();
        std::cout << "OK" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: applyRigidTransformFilter ---" << std::endl << message << std::endl;
        return false;
    }

    m_OutputPointer = resampler->GetOutput();

    return true;
}











} //namespace filter
