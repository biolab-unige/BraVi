//
// C++ Implementation: geodesicActiveContourFullFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "geodesicActiveContourFullFilter.h"

#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkFastMarchingImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkGeodesicActiveContourLevelSetImageFilter.h>

#include <itkImageFileWriter.h>


namespace filter
{

geodesicActiveContourFullFilter::geodesicActiveContourFullFilter() : abstractFilter()
{
    setName("geodesicActiveContourFullFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri
    //Gradient Magnitude
    addParameter(new filterParameter<float>(m_Sigma, "Sigma", 1.0));

    //Sigmoid
    addParameter(new filterParameter<float>(m_Alpha, "Alpha", -0.3));
    addParameter(new filterParameter<float>(m_Beta, "Beta",    3.0));

    //Fast Marching
    addParameter(new filterParameter<ImageType::IndexValueType>(m_Seed[0], "Seed [0]", (ImageType::IndexValueType)120));
    addParameter(new filterParameter<ImageType::IndexValueType>(m_Seed[1], "Seed [1]", (ImageType::IndexValueType)173));
    addParameter(new filterParameter<ImageType::IndexValueType>(m_Seed[2], "Seed [2]", (ImageType::IndexValueType)255));
    addParameter(new filterParameter<double>(m_InitialDistance, "Initial Distance", 0.0));

    //Geodesic Active Contour
    addParameter(new filterParameter<float>(m_PropagationScaling, "Propagation Scaling", 1.0));
    addParameter(new filterParameter<float>(m_CurvatureScaling,   "Curvature Scaling",   1.0));
    addParameter(new filterParameter<float>(m_AdvectionScaling,   "Advection Scaling",   1.0));
}


geodesicActiveContourFullFilter::~geodesicActiveContourFullFilter()
{
}

bool geodesicActiveContourFullFilter::Execute( void )
{
    Update();


    typedef itk::ImageFileWriter< ImageType > WriterType;
    typedef itk::RescaleIntensityImageFilter< InternalImageType, ImageType >   CastFilterType;

    CastFilterType::Pointer caster1 = CastFilterType::New();
    CastFilterType::Pointer caster2 = CastFilterType::New();
    CastFilterType::Pointer caster3 = CastFilterType::New();
    CastFilterType::Pointer caster4 = CastFilterType::New();
    CastFilterType::Pointer caster5 = CastFilterType::New();

    caster1->SetOutputMinimum(   0 );
    caster1->SetOutputMaximum( 255 );
    caster2->SetOutputMinimum(   0 );
    caster2->SetOutputMaximum( 255 );
    caster3->SetOutputMinimum(   0 );
    caster3->SetOutputMaximum( 255 );
    caster4->SetOutputMinimum(   0 );
    caster4->SetOutputMaximum( 255 );
    caster5->SetOutputMinimum(   0 );
    caster5->SetOutputMaximum( 255 );

    WriterType::Pointer writer1 = WriterType::New();
    WriterType::Pointer writer2 = WriterType::New();
    WriterType::Pointer writer3 = WriterType::New();
    WriterType::Pointer writer4 = WriterType::New();
    WriterType::Pointer writer5 = WriterType::New();

    writer1->SetInput( caster1->GetOutput() );
    writer1->SetFileName("/home/biolab/tmp/GeoFull/Smoothing.nii");

    writer2->SetInput( caster2->GetOutput() );
    writer2->SetFileName("/home/biolab/tmp/GeoFull/GradientMagnitude.nii");

    writer3->SetInput( caster3->GetOutput() );
    writer3->SetFileName("/home/biolab/tmp/GeoFull/Sigmoid.nii");

    writer4->SetInput( caster4->GetOutput() );
    writer4->SetFileName("/home/biolab/tmp/GeoFull/FastMarching.nii");

    writer5->SetInput( caster5->GetOutput() );
    writer5->SetFileName("/home/biolab/tmp/GeoFull/GeodesicNoThreshold.nii");




/*************************

  RESCALE INTERNAL

*************************/

    typedef itk::RescaleIntensityImageFilter<ImageType, InternalImageType> RescaleInternalIntensityFilterType;

    RescaleInternalIntensityFilterType::Pointer rescaleInternal = RescaleInternalIntensityFilterType::New();
    rescaleInternal->SetOutputMinimum( 0.0);
    rescaleInternal->SetOutputMaximum( 1.0 );
    rescaleInternal->SetInput( m_InputPointer );

    try
    {
        std::cout << "Geodesic Active Contour - Rescale internal: " << std::flush;
        rescaleInternal->Update();
        std::cout << "OK" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }



/*************************

  SMOOTHING

*************************/

    typedef itk::CurvatureAnisotropicDiffusionImageFilter<InternalImageType, InternalImageType> SmoothingFilterType;

    SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();

    smoothing->SetTimeStep( 0.0625 );
    smoothing->SetNumberOfIterations(  5 );
    smoothing->SetConductanceParameter( 9.0 );

    smoothing->SetInput( rescaleInternal->GetOutput() );


    try
    {
        std::cout << "Geodesic Active Contour - Smoothing: " << std::flush;
        smoothing->Update();
        std::cout << "OK" << std::endl;
        caster1->SetInput( smoothing->GetOutput() );
        writer1->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }




/*************************

  GRADIENT MAGNITUDE

*************************/

    typedef itk::GradientMagnitudeRecursiveGaussianImageFilter< InternalImageType, InternalImageType > GradientMagnitudeFilterType;

    GradientMagnitudeFilterType::Pointer gradientMagnitude = GradientMagnitudeFilterType::New();
    gradientMagnitude->SetSigma(m_Sigma);

    gradientMagnitude->SetInput( smoothing->GetOutput() );

    try
    {
        std::cout << "Geodesic Active Contour - Gradient Magnitude: " << std::flush;
        gradientMagnitude->Update();
        std::cout << "OK" << std::endl;
        caster2->SetInput( gradientMagnitude->GetOutput() );
        writer2->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }


/*************************

  SIGMOID

*************************/

    typedef itk::SigmoidImageFilter< InternalImageType, InternalImageType > SigmoidFilterType;

    SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();
    sigmoid->SetOutputMinimum( 0.0 );
    sigmoid->SetOutputMaximum( 1.0 );
    sigmoid->SetAlpha(m_Alpha);
    sigmoid->SetBeta(m_Beta);

    sigmoid->SetInput( gradientMagnitude->GetOutput() );

    try
    {
        std::cout << "Geodesic Active Contour - Sigmoid: " << std::flush;
        sigmoid->Update();
        std::cout << "OK" << std::endl;
        caster3->SetInput( sigmoid->GetOutput() );
        writer3->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }



/*************************

  FAST MARCHING

*************************/


    typedef itk::FastMarchingImageFilter< InternalImageType, InternalImageType > FastMarchingFilterType;
    FastMarchingFilterType::Pointer fastMarching = FastMarchingFilterType::New();
    fastMarching->SetInput( sigmoid->GetOutput() );

    typedef FastMarchingFilterType::NodeContainer NodeContainer;
    typedef FastMarchingFilterType::NodeType NodeType;
    NodeContainer::Pointer seeds = NodeContainer::New();

    NodeType node;
    node.SetValue( - m_InitialDistance );
    node.SetIndex( m_Seed );
    seeds->Initialize();
    seeds->InsertElement(0, node);

    fastMarching->SetTrialPoints(seeds);
    //fastMarching->SetOutputSize( rescaleInternal->GetOutput()->GetBufferedRegion().GetSize() );

    fastMarching->SetSpeedConstant( 1.0 );


    try
    {
        std::cout << "Geodesic Active Contour - Fast Marching: " << std::flush;
        fastMarching->Update();
        std::cout << "OK" << std::endl;
        caster4->SetInput( fastMarching->GetOutput() );
        writer4->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }


/*************************

  GEODESIC ACTIVE CONTOUR

*************************/




    typedef itk::GeodesicActiveContourLevelSetImageFilter< InternalImageType, InternalImageType > GeodesicActiveContourFullFilterType;
    GeodesicActiveContourFullFilterType::Pointer geodesicActiveContour = GeodesicActiveContourFullFilterType::New();

    geodesicActiveContour->SetPropagationScaling( m_PropagationScaling );
    geodesicActiveContour->SetCurvatureScaling  ( m_CurvatureScaling   );
    geodesicActiveContour->SetAdvectionScaling  ( m_AdvectionScaling   );

    geodesicActiveContour->SetMaximumRMSError( 0.02 );
    geodesicActiveContour->SetNumberOfIterations( 800 );

    geodesicActiveContour->SetInput( fastMarching->GetOutput() ); //contorno di partenza
    geodesicActiveContour->SetFeatureImage( sigmoid->GetOutput() ); //Immagine da cui estrarre il contorno

    try
    {
        std::cout << "Geodesic Active Contour - Geodesic Active Contour: " << std::flush;
        geodesicActiveContour->Update();
        std::cout << "OK" << std::endl;
        caster5->SetInput( geodesicActiveContour->GetOutput() );
        writer5->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }


/*************************

  THRESHOLDER

*************************/

    typedef itk::BinaryThresholdImageFilter<InternalImageType, ImageType>    ThresholdingFilterType;
    ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();

    thresholder->SetLowerThreshold( -1000.0 );
    thresholder->SetUpperThreshold(     0.0 );

    thresholder->SetOutsideValue(  0  );
    thresholder->SetInsideValue(  255 );

    thresholder->SetInput( geodesicActiveContour->GetOutput() );

    try
    {
        std::cout << "Geodesic Active Contour - Thresholder: " << std::flush;
        thresholder->Update();
        std::cout << "OK" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::cout << "FAIL" << std::endl;
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "ERROR" << std::endl << message << std::endl;
        return false;
    }


    m_OutputPointer = thresholder->GetOutput();

    return true;
}







} //namespace filter
