//
// C++ Implementation: geodesicActiveContourFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "geodesicActiveContourFilter.h"




//definire uno solo di questi:
//#define __THRESHOLDER__
#define __RESCALE__


#include <itkGeodesicActiveContourLevelSetImageFilter.h>
  #include <itkRescaleIntensityImageFilter.h>

#ifdef __THRESHOLDER__
  #include <itkBinaryThresholdImageFilter.h>
#endif


namespace filter
{

geodesicActiveContourFilter::geodesicActiveContourFilter() : abstractFilter()
{
    setName("geodesicActiveContourFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer1, "Input File"));
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer2, "Level Set Zero Function"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

// Parametri

    addParameter(new filterParameter<float>(m_PropagationScaling, "Propagation Scaling", 1.0));
    addParameter(new filterParameter<float>(m_CurvatureScaling, "Curvature Scaling", 1.0));
    addParameter(new filterParameter<float>(m_AdvectionScaling, "Advection Scaling", 1.0));


    addParameter(new filterParameter<InternalPixelType>(m_RescaleInternalOutputMinimum, "Internal Minimum", 0.0));
    addParameter(new filterParameter<InternalPixelType>(m_RescaleInternalOutputMaximum, "Internal Maximum", 1.0));

    addParameter(new filterParameter<PixelType>(m_RescaleOutputMinimum, "Minimum", 0));
    addParameter(new filterParameter<PixelType>(m_RescaleOutputMaximum, "Maximum", 255));
}


geodesicActiveContourFilter::~geodesicActiveContourFilter()
{
}

bool geodesicActiveContourFilter::Execute( void )
{
    Update();


    typedef itk::RescaleIntensityImageFilter<ImageType, InternalImageType> RescaleInternalIntensityFilterType;

    RescaleInternalIntensityFilterType::Pointer rescaleInternal1 = RescaleInternalIntensityFilterType::New();
    rescaleInternal1->SetOutputMinimum(m_RescaleInternalOutputMinimum);
    rescaleInternal1->SetOutputMaximum(m_RescaleInternalOutputMaximum);
    rescaleInternal1->SetInput( m_InputPointer1 );

    RescaleInternalIntensityFilterType::Pointer rescaleInternal2 = RescaleInternalIntensityFilterType::New();
    rescaleInternal2->SetOutputMinimum(m_RescaleInternalOutputMinimum);
    rescaleInternal2->SetOutputMaximum(m_RescaleInternalOutputMaximum);
    rescaleInternal2->SetInput( m_InputPointer2 );



    typedef itk::GeodesicActiveContourLevelSetImageFilter< InternalImageType, InternalImageType > GeodesicActiveContourFilterType;
    GeodesicActiveContourFilterType::Pointer geodesicActiveContour = GeodesicActiveContourFilterType::New();

    geodesicActiveContour->SetPropagationScaling( m_PropagationScaling );
    geodesicActiveContour->SetCurvatureScaling (  m_CurvatureScaling );
    geodesicActiveContour->SetAdvectionScaling (  m_AdvectionScaling );

    geodesicActiveContour->SetFeatureImage( rescaleInternal1->GetOutput() ); //Immagine da cui estrarre il contorno
    geodesicActiveContour->SetInput( rescaleInternal2->GetOutput() ); //contorno di partenza




#ifdef __THRESHOLDER__
    typedef itk::BinaryThresholdImageFilter<InternalImageType, ImageType> ThresholdingFilterType;
    ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();
    thresholder->SetLowerThreshold( -1000.0 );
    thresholder->SetUpperThreshold(     0.0 );
    thresholder->SetOutsideValue(  0  );
    thresholder->SetInsideValue(  255 );
    thresholder->SetInput( geodesicActiveContour->GetOutput() );
#endif



#ifdef __RESCALE__
    typedef itk::RescaleIntensityImageFilter<InternalImageType, ImageType> RescaleIntensityFilterType;
    RescaleIntensityFilterType::Pointer rescale = RescaleIntensityFilterType::New();
    rescale->SetOutputMinimum(m_RescaleOutputMinimum);
    rescale->SetOutputMaximum(m_RescaleOutputMaximum);
    rescale->SetInput( geodesicActiveContour->GetOutput() );
#endif

// Esecuzione
    try
    {
        std::cout << "Geodesic Active Contour:" << std::endl;
        std::cout << "- Rescale internal 1" << std::endl;
        rescaleInternal1->Update();
        std::cout << "- Rescale internal 2" << std::endl;
        rescaleInternal2->Update();
        std::cout << "- Geodesic Active Contour" << std::endl;
        geodesicActiveContour->Update();
#ifdef __THRESHOLDER__
        std::cout << "- Thresholder" << std::flush;
        thresholder->Update();
#endif
#ifdef __RESCALE__
        std::cout << "- Rescale" << std::endl;
        rescale->Update();
#endif

    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: geodesicActiveContourFilter ---" << std::endl << message << std::endl;
        return false;
    }

#ifdef __THRESHOLDER__
    m_OutputPointer = thresholder->GetOutput();
#endif
#ifdef __RESCALE__
    m_OutputPointer = rescale->GetOutput();
#endif
    return true;
}











} //namespace filter
