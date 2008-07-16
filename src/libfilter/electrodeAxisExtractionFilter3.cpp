//
// C++ Implementation: electrodeAxisExtractionFilter3
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "electrodeAxisExtractionFilter3.h"
#include "itkSymmetricEigenVectorAnalysisImageFilter.h"
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkSymmetricEigenAnalysisImageFilter.h>
#include <itkNthElementImageAdaptor.h>
#include <itkVectorImageToImageAdaptor.h>
#include <itkVectorIndexSelectionCastImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkThresholdImageFilter.h>
#include <itkStreamingImageFilter.h>
#include <itkDerivativeImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkVectorCastImageFilter.h>
#include <itkAdaptImageFilter.h>
#include <itkImageFileWriter.h>


namespace filter
{

electrodeAxisExtractionFilter3::electrodeAxisExtractionFilter3() : abstractFilter()
{
    setName("electrodeAxisExtractionFilter3");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
//    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
    addOutput(new filterOutput<ImageType::Pointer>(m_Eigen0OutputPointer, "EigenValue 0"));
//    addOutput(new filterOutput<ImageType::Pointer>(m_Eigen1OutputPointer, "EigenValue 1"));
//    addOutput(new filterOutput<ImageType::Pointer>(m_Eigen2OutputPointer, "EigenValue 2"));

//Parameters
    addParameter(new filterParameter<float>(m_Sigma,  "Sigma",  1.0));
}


electrodeAxisExtractionFilter3::~electrodeAxisExtractionFilter3()
{
}

void DisplayITKEventCallback(itk::Object *caller, const itk::EventObject& event, void* clientData)
{
    itk::ProcessObject *processObject = (itk::ProcessObject*)caller;
    if(typeid(event) == typeid(itk::ProgressEvent))
    {
        std::cout << caller->GetNameOfClass() << " -> " << 100.0 * processObject->GetProgress() << "%" << std::endl;
    }
    else if (typeid(event) == typeid(itk::StartEvent))
    {
        std::cout << caller->GetNameOfClass() << " -> Starting" << std::endl;
    }
    else if (typeid(event) == typeid(itk::EndEvent))
    {
        std::cout << caller->GetNameOfClass() << " -> End" << std::endl;
    }
}



/*
template< class TPixel >
class EigenValueAccessor
{
public:
  typedef TPixel                     InternalType;
  typedef float                     ExternalType;
  
  inline ExternalType Get( const InternalType & input ) const 
    {
      return static_cast<ExternalType>( input[m_EigenIdx] );
    }

  void SetEigenIdx( unsigned int i )
    {
    this->m_EigenIdx = i;
    }
  
private:
  unsigned int m_EigenIdx;
};

*/

template< class TPixel >
class VectorAccessor
{
public:
    typedef TPixel                     InternalType;
    typedef itk::Vector< float, Dimension > ExternalType;

    enum Direction{ DirectionRow = 0, DirectionColumn};

    inline ExternalType Get( const InternalType & input ) const 
    {
        ExternalType vector;
        if( m_Direction == DirectionRow )
            for( unsigned int i=0; i<3; i++ )
                vector[i] = input[m_Index][i];
        else
            for( unsigned int i=0; i<3; i++ )
                vector[i] = input[i][m_Index];

    return vector;
    }

    inline bool operator!=( const VectorAccessor & other ) const
    {
        if( m_Index != other.m_Index || m_Direction != other.m_Direction )
            return true;
        return false;
    }

    inline void SetIndex( unsigned int i ) { this->m_Index = i; }
    inline void SetDirection( Direction dir ) { this->m_Direction = dir; }


private:
    unsigned int m_Index;
    Direction m_Direction;
};




bool electrodeAxisExtractionFilter3::Execute( void )
{
    Update();

    typedef itk::Image<float, Dimension>                   floatImageType;


/////////////////////////
// Observer
/////////////////////////
    typedef itk::CStyleCommand commandType;
    typedef commandType::Pointer commandPointer;
    commandPointer printCallback = commandType::New();
    printCallback->SetCallback(DisplayITKEventCallback);


/////////////////////////
// Hessiana
/////////////////////////

    typedef itk::HessianRecursiveGaussianImageFilter< ImageType > HessianRecursiveGaussianType;

    HessianRecursiveGaussianType::Pointer hessian = HessianRecursiveGaussianType::New();
    hessian->ReleaseDataFlagOn();

    hessian->SetSigma(m_Sigma);

    hessian->AddObserver(itk::ProgressEvent(), printCallback);
    hessian->AddObserver(itk::StartEvent(), printCallback);
    hessian->AddObserver(itk::EndEvent(), printCallback);

    hessian->SetInput(m_InputPointer);

    try
    {
        std::cout << "ElectrodeAxisExtractionFilter: Hessian" << std::endl;
        hessian->UpdateLargestPossibleRegion();
        std::cout << " OK" << std::endl;
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

    HessianRecursiveGaussianType::OutputImageType* hessianImage = hessian->GetOutput();
    hessianImage->DisconnectPipeline();


/////////////////////////
// Gradiente
/////////////////////////

    typedef itk::GradientRecursiveGaussianImageFilter< ImageType > GradientRecursiveGaussianType;

    GradientRecursiveGaussianType::Pointer gradient = GradientRecursiveGaussianType::New();
    gradient->ReleaseDataFlagOn();

    gradient->SetSigma(m_Sigma);

    gradient->AddObserver(itk::ProgressEvent(), printCallback);
    gradient->AddObserver(itk::StartEvent(), printCallback);
    gradient->AddObserver(itk::EndEvent(), printCallback);

    gradient->SetInput(m_InputPointer);


    try
    {
        std::cout << "ElectrodeAxisExtractionFilter: Gradient" << std::endl;
        gradient->UpdateLargestPossibleRegion();
        std::cout << " OK" << std::endl;
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

    GradientRecursiveGaussianType::OutputImageType* gradientImage = gradient->GetOutput();
    gradientImage->DisconnectPipeline();


/////////////////////////
// Autovettori
/////////////////////////

    typedef itk::Matrix< float, Dimension, Dimension > EigenVectorType;
    typedef itk::Image< EigenVectorType, Dimension > EigenVectorImageType;

    typedef itk::SymmetricEigenVectorAnalysisImageFilter< 
                    HessianRecursiveGaussianType::OutputImageType,
                    EigenVectorImageType >
                                                       EigenVectorAnalysisType;

    EigenVectorAnalysisType::Pointer eigenVectorAnalysis = EigenVectorAnalysisType::New();

    eigenVectorAnalysis->ReleaseDataFlagOn();

    eigenVectorAnalysis->SetDimension(Dimension);
    eigenVectorAnalysis->OrderEigenValuesBy(EigenVectorAnalysisType::FunctorType::OrderByValue);

    eigenVectorAnalysis->SetInput(hessianImage);

    try
    {
        std::cout << "ElectrodeAxisExtractionFilter: Eigen Vectors" << std::endl;
        hessian->UpdateLargestPossibleRegion();
        std::cout << " OK" << std::endl;
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

    EigenVectorAnalysisType::OutputImageType* eigenVectorImage = eigenVectorAnalysis->GetOutput();
    eigenVectorImage->DisconnectPipeline();

/////////////////////////
// Autovettore principale
/////////////////////////
/*
    typedef itk::ImageAdaptor< EigenVectorImageType, VectorAccessor< EigenVectorType > > ImageAdaptorType;
    ImageAdaptorType* EigenVector2 = ImageAdaptorType::New();
    typedef VectorAccessor<EigenVectorType> AccessorType;
    AccessorType accessor2;
    accessor2.SetIndex(2);
    accessor2.SetDirection(AccessorType::DirectionRow);
    EigenVector2->SetPixelAccessor(accessor2);
    EigenVector2->SetImage(eigenVectorAnalysis->GetOutput());

    typedef itk::VectorCastImageFilter< ImageAdaptorType, itk::Vector< float, Dimension >  >  CastImageFilterType;
    CastImageFilterType eigenCastFilter2 = CastImageFilterType::New();
    eigenCastFilter2->SetInput( EigenVector2 );
*/

    typedef itk::Image< itk::Vector< float, Dimension >, Dimension> VectorFloatImageType;
    typedef VectorAccessor<EigenVectorType> AccessorType;
    typedef itk::Functor::AccessorFunctor<EigenVectorType, AccessorType > FunctorType;
    typedef itk::AdaptImageFilter< EigenVectorImageType, VectorFloatImageType, AccessorType > ImageAdaptorType;

    ImageAdaptorType* EigenVector2 = ImageAdaptorType::New();
    EigenVector2->ReleaseDataFlagOn();


    FunctorType accessor2;
    accessor2.GetAccessor().SetIndex(2);
    accessor2.GetAccessor().SetDirection(AccessorType::DirectionRow);

    EigenVector2->SetFunctor(accessor2);
    EigenVector2->SetInput(eigenVectorImage);

    ImageAdaptorType::OutputImageType* eigenVector2Image =  EigenVector2->GetOutput();
    eigenVector2Image->DisconnectPipeline();



/////////////////////////
// Prodotto Scalare
/////////////////////////

    floatImageType::Pointer scalarProd = floatImageType::New();
    scalarProd->SetRegions( eigenVector2Image->GetRequestedRegion() );
    scalarProd->CopyInformation( eigenVector2Image );
    try
    {
        scalarProd->Allocate();
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

    typedef itk::ImageRegionIterator< ImageAdaptorType::OutputImageType> EigenVectorIteratorType;
    typedef itk::ImageRegionIterator< GradientRecursiveGaussianType::OutputImageType> GradientIteratorType;
    typedef itk::ImageRegionIterator<floatImageType> floatIterator;

    EigenVectorIteratorType eigen( eigenVector2Image, eigenVector2Image->GetRequestedRegion() );
    GradientIteratorType grad( gradientImage, eigenVector2Image->GetRequestedRegion() );
    floatIterator fl( scalarProd, eigenVector2Image->GetRequestedRegion() );

    for ( eigen.GoToBegin(), grad.GoToBegin(), fl.GoToBegin(); !eigen.IsAtEnd(); ++eigen, ++grad, ++fl)
    {
        float prod = eigen.Get()[0]*grad.Get()[0] + eigen.Get()[1]*grad.Get()[1] + eigen.Get()[2]*grad.Get()[2];
        fl.Set(prod);
    }




/////////////////////////
// Autovalori
/////////////////////////


/*


    typedef itk::Vector< float, Dimension > EigenValueArrayType;
    typedef itk::Image< EigenValueArrayType, Dimension > EigenValueImageType;

    typedef itk::SymmetricEigenAnalysisImageFilter< 
                    HessianRecursiveGaussianType::OutputImageType,
                    EigenValueImageType >
                                                       EigenAnalysisType;

    EigenAnalysisType::Pointer eigenAnalysis = EigenAnalysisType::New();
    eigenAnalysis->ReleaseDataFlagOn();

    eigenAnalysis->SetDimension(Dimension);
    eigenAnalysis->OrderEigenValuesBy(EigenAnalysisType::FunctorType::OrderByValue);

    eigenAnalysis->SetInput(hessianImage);


*/
/////////////////////////
// Estrazione componenti (ricerca autovalore principale)
/////////////////////////

/*
    typedef itk::VectorImageToImageAdaptor <float, Dimension> adaptorType;
//    typedef itk::NthElementImageAdaptor <
//                    EigenAnalysisType::OutputImageType,
//                    floatImageType >
//                                                        adaptorType;
//    typedef itk::VectorIndexSelectionCastImageFilter <
//                    EigenAnalysisType::OutputImageType,
//                    floatImageType >
//                                                        indexSelectionType;

    adaptorType::Pointer eigenMax = adaptorType::New();
//    eigenMax->ReleaseDataFlagOn();

    eigenMax->SetExtractComponentIndex(2);

    eigenMax->SetImage(eigenAnalysis->GetOutput());

/////////////////////////
// Ricerca punti autovalore principale < 0
/////////////////////////

    typedef itk::ThresholdImageFilter<adaptorType> thresholdType;

    thresholdType::Pointer threshold = thresholdType::New();
    threshold->ReleaseDataFlagOn();

    threshold->SetUpper(0.1);
    threshold->SetOutsideValue(1.0);

    threshold->SetInput(eigenMax);

*/

/////////////////////////
// Rescale
/////////////////////////
    typedef itk::RescaleIntensityImageFilter<
//                    thresholdType::OutputImageType,
                    floatImageType,
                    ImageType >
                                                        rescaleType;

    rescaleType::Pointer rescale = rescaleType::New();
    rescale->ReleaseDataFlagOn();

    //rescale->SetInput(threshold->GetOutput());
    rescale->SetInput(scalarProd);

/////////////////////////
// Streaming
/////////////////////////


    typedef itk::StreamingImageFilter<rescaleType::OutputImageType, rescaleType::OutputImageType> streamingType;
    streamingType::Pointer streaming = streamingType::New();
    streaming->SetNumberOfStreamDivisions(256);
//    streaming->SetNumberOfThreads(4);

    streaming->AddObserver(itk::ProgressEvent(), printCallback);
    streaming->AddObserver(itk::StartEvent(), printCallback);
    streaming->AddObserver(itk::EndEvent(), printCallback);

    streaming->SetInput(rescale->GetOutput());



    try
    {
        streaming->Update();
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


/////////////////////////
// Output
/////////////////////////

//    m_OutputPointer = vesselFilter->GetOutput();
    m_Eigen0OutputPointer = streaming->GetOutput();


    return true;

}

} //namespace filter
