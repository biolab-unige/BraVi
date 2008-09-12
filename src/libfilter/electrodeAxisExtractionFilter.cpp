//
// C++ Implementation: electrodeAxisExtractionFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "electrodeAxisExtractionFilter.h"

#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkHessian3DToVesselnessMeasureImageFilter.h>

#include <itkMeanImageFilter.h>

#include <vector>

#include <ctime>




namespace filter
{

electrodeAxisExtractionFilter::electrodeAxisExtractionFilter() : abstractFilter()
{
    setName("electrodeAxisExtractionFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));

//Parameters
    addParameter(new filterParameter<double>(m_Sigma,  "Sigma",  1.0));
    addParameter(new filterParameter<double>(m_Alpha1, "Alpha1", 0.5));
    addParameter(new filterParameter<double>(m_Alpha2, "Alpha2", 2.0));

}


electrodeAxisExtractionFilter::~electrodeAxisExtractionFilter()
{
}

bool electrodeAxisExtractionFilter::Execute( void )
{
    Update();


//Allocazione immagini


/*
    ImageType::SizeType size = m_InputPointer->GetBufferedRegion().GetSize();
    ImageType::IndexType start = {0,0,0};
    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    std::cout << "Allocazione Immagine Interna:" << std::endl;
    ImageType::Pointer internalPointer = ImageType::New();
    internalPointer->SetRegions(region);
    internalPointer->SetSpacing(m_InputPointer->GetSpacing());
    internalPointer->Allocate();
    std::cout << "Allocazione Immagine Output:" << std::endl;
    ImageType::Pointer outputPointer = ImageType::New();
    outputPointer->SetRegions(region);
    outputPointer->SetSpacing(m_InputPointer->GetSpacing());
    outputPointer->Allocate();
*/

    std::cout << "Allocazione Immagine Output:" << std::endl;
    ImageType::Pointer outputPointer = ImageType::New();
    std::cout << "Allocazione Immagine Output: SetRegions" << std::endl;
    outputPointer->SetRegions( m_InputPointer->GetRequestedRegion() );
    std::cout << "Allocazione Immagine Output: CopyInformation" << std::endl;
    outputPointer->CopyInformation( m_InputPointer );
    std::cout << "Allocazione Immagine Output: Allocate" << std::endl;
    outputPointer->Allocate();



// Calcolo delle medie
/*    typedef itk::MeanImageFilter<ImageType, ImageType >  MeanType;

    MeanType::Pointer mean_small = MeanType::New();
    ImageType::SizeType indexRadius_small;
    indexRadius_small[0] = 2; // radius along x
    indexRadius_small[1] = 2; // radius along y
    indexRadius_small[2] = 2; // radius along z
    mean_small->SetRadius( indexRadius_small );
    mean_small->SetInput( m_InputPointer );

    MeanType::Pointer mean_big = MeanType::New();
    ImageType::SizeType indexRadius_big;
    indexRadius_big[0] = 20; // radius along x
    indexRadius_big[1] = 20; // radius along y
    indexRadius_big[2] = 20; // radius along z

    mean_big->SetRadius( indexRadius_big );
    mean_big->SetInput( m_InputPointer );


    try
    {
        std::cout << "Mean Small:" << std::endl;
        time_t  start_time_small = clock();
        mean_small->Update();
        time_t  end_time_small = clock();
        float time_small = (float) (end_time_small - start_time_small) / CLOCKS_PER_SEC;
        std::cout << "Tempo di esecuzione: " << time_small << " secondi" << std::endl;
        std::cout << "Mean Big:" << std::endl;
        time_t  start_time_big = clock();
        mean_big->Update();
        time_t  end_time_big = clock();
        float time_big = (float) (end_time_big - start_time_big) / CLOCKS_PER_SEC;
        std::cout << "Tempo di esecuzione: " << time_big << " secondi" << std::endl;
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: ElectrodeAxisExtractionFilter ---" << std::endl << message << std::endl;
    }

*/


// Ricerca del punto di partenza

    time_t  start_time_small = clock();
    VectorType max = findMax(m_InputPointer->GetRequestedRegion());

    time_t  end_time_small = clock();
    float time_small = (float) (end_time_small - start_time_small) / CLOCKS_PER_SEC;
    std::cerr << "Tempo di esecuzione: " << time_small << " secondi" << std::endl;
    VectorType::iterator iter;
    for( iter = max.begin(); iter < max.end(); iter++)
    {
        outputPointer->SetPixel(*iter, 100);
    }


    if (max.size() > 1)
    {
        std::cerr << "Media" << std::endl;
        VectorValueType small = meanVOI(max, 2);
        VectorValueType big = meanVOI(max, 10);


        VectorValueType small_sort = small;
#ifdef WIN32
		std::cout << "ERRORE!!!!" << std::endl; /// @todo WIN32 non ha sort!
#else
		sort(small_sort.begin(),small_sort.end());
#endif
        double d_s = small_sort[small_sort.size()-10];
        std::cout << "d_s = " << d_s << std::endl;

        VectorValueType big_sort = big;
#ifdef WIN32
		std::cout << "ERRORE!!!!" << std::endl; /// @todo WIN32 non ha sort!
#else
        sort(big_sort.begin(),big_sort.end());
#endif
        double d_b = big_sort[10];
        std::cout << "d_b = " << d_s << std::endl;

        VectorValueType::iterator iter_small;
        VectorValueType::iterator iter_big;
        int count = 0;
        for ( iter=max.begin(), iter_small=small.begin(), iter_big=big.begin(); iter < max.end(); iter++, iter_small++, iter_big++)
        {
            if (*iter_small<=d_b && *iter_big <= d_b)
            {
                std::cout << *iter << " " << *iter_small << " " << *iter_big << std::endl;
                count++;
            }
        }
        if (count == 0)
            std::cout << "Nulla di fatto" << std::endl;

        //VectorValueType big_sort = sort(big.begin(),big.end());


/*
        VectorValueType::iterator iter_small;
        VectorValueType::iterator iter_big;
        int pippo = 0;
        for ( iter=max.begin(), iter_small=small.begin(), iter_big=big.begin(); iter < max.end(); iter++, iter_small++, iter_big++)
            std::cerr << pippo << " " << *iter << " " << (double)(*iter_small) << " " << (double)(*iter_big) << std::endl;
*/
/*
        ImageType::IndexType start01Index = findMaxVOI(max, 1, 20);
        ImageType::IndexType start02Index = findMinVOI(max, 1, 20);
        ImageType::IndexType start03Index = findMaxVOI(max, 1, 20, VOI_SPHERE);
        ImageType::IndexType start04Index = findMinVOI(max, 1, 20, VOI_SPHERE);
        ImageType::IndexType start05Index = findMaxVOI(max, 3, 20, VOI_SPHERE);
        ImageType::IndexType start06Index = findMinVOI(max, 3, 20, VOI_SPHERE);
        ImageType::IndexType start07Index = findMaxVOI(max, 5, 20, VOI_SPHERE);
        ImageType::IndexType start08Index = findMinVOI(max, 5, 20, VOI_SPHERE);
        ImageType::IndexType start09Index = findMaxVOI(max, 9, 20, VOI_SPHERE);
        ImageType::IndexType start10Index = findMinVOI(max, 9, 20, VOI_SPHERE);
        ImageType::IndexType start17Index = findMaxVOI(max, 20, 20, VOI_SPHERE);
        ImageType::IndexType start18Index = findMinVOI(max, 20, 20, VOI_SPHERE);
        ImageType::IndexType start11Index = findMaxVOI(max, 3, 20);
        ImageType::IndexType start12Index = findMinVOI(max, 3, 20);
        ImageType::IndexType start13Index = findMaxVOI(max, 5, 20);
        ImageType::IndexType start14Index = findMinVOI(max, 5, 20);
        ImageType::IndexType start15Index = findMaxVOI(max, 9, 20);
        ImageType::IndexType start16Index = findMinVOI(max, 9, 20);
        ImageType::IndexType start19Index = findMaxVOI(max, 20, 20);
        ImageType::IndexType start20Index = findMinVOI(max, 20, 20);

        std::cerr << "-----------------  RISULTATI:  -------------" << std::endl;
        std::cerr << start01Index << ": Massimo"              << std::endl;
        std::cerr << start03Index << ": Massimo sphere"       << std::endl;
        std::cerr << start05Index << ": Massimo sphere voi_3" << std::endl;
        std::cerr << start07Index << ": Massimo sphere voi_5" << std::endl;
        std::cerr << start09Index << ": Massimo sphere voi_9" << std::endl;
        std::cerr << start17Index << ": Massimo sphere voi_20"<< std::endl;
        std::cerr << start11Index << ": Massimo voi_3"        << std::endl;
        std::cerr << start13Index << ": Massimo voi_5"        << std::endl;
        std::cerr << start15Index << ": Massimo voi_9"        << std::endl;
        std::cerr << start19Index << ": Massimo voi_20"       << std::endl;
        std::cerr << start02Index << ": Minimo"               << std::endl;
        std::cerr << start04Index << ": Minimo sphere"        << std::endl;
        std::cerr << start06Index << ": Minimo sphere voi_3"  << std::endl;
        std::cerr << start08Index << ": Minimo sphere voi_5"  << std::endl;
        std::cerr << start10Index << ": Minimo sphere voi_9"  << std::endl;
        std::cerr << start18Index << ": Minimo sphere voi_20" << std::endl;
        std::cerr << start12Index << ": Minimo voi_3"         << std::endl;
        std::cerr << start14Index << ": Minimo voi_5"         << std::endl;
        std::cerr << start16Index << ": Minimo voi_9"         << std::endl;
        std::cerr << start20Index << ": Minimo voi_20"         << std::endl;



        bool out_min = true;
        bool out_max = true;

        if (out_max)
        {
            outputPointer->SetPixel(start01Index, 100);
            outputPointer->SetPixel(start03Index, 100);
            outputPointer->SetPixel(start05Index, 100);
            outputPointer->SetPixel(start07Index, 100);
            outputPointer->SetPixel(start09Index, 100);
            outputPointer->SetPixel(start11Index, 100);
            outputPointer->SetPixel(start13Index, 100);
            outputPointer->SetPixel(start15Index, 100);
        }
        if (out_min)
        {
            outputPointer->SetPixel(start02Index, 100);
            outputPointer->SetPixel(start04Index, 100);
            outputPointer->SetPixel(start06Index, 100);
            outputPointer->SetPixel(start08Index, 100);
            outputPointer->SetPixel(start10Index, 100);
            outputPointer->SetPixel(start12Index, 100);
            outputPointer->SetPixel(start14Index, 100);
            outputPointer->SetPixel(start16Index, 100);
        }

        for (int i=0; i<size[2]; i++) for (int j=0; j<size[1]; j++) for (int k=0; k<size[0]; k++)
        {
            int ct = 0;
            ct += i==0;
            ct += j==0;
            ct += k==0;
            ct += i==size[2]-1;
            ct += j==size[1]-1;
            ct += k==size[0]-1;
            if (ct >= 2)
            {
                ImageType::IndexType curr;
                curr[0] = k;
                curr[1] = j;
                curr[2] = i;
                outputPointer->SetPixel(curr, 100);
                curr[2] = 1;
                outputPointer->SetPixel(curr, 100);
            }
        }

*/    }
    else std::cout << "CICCIA" << std::endl;

    m_OutputPointer = outputPointer;

    std::cerr << "FINE " << max.size() << std::endl;
    return true;




}


long double electrodeAxisExtractionFilter::VOISum(ImageType::IndexType index, int VOISize, voi_mode mode)
{
/*    long double sum = 0;
    ImageType::IndexType currIndex;
    int VOISizeSquare = VOISize*VOISize;

    switch (mode)
    {
        case VOI_CUBE:
            for (int i = -VOISize; i <= +VOISize; i++) for (int j = -VOISize; j <= +VOISize; j++) for (int k = -VOISize; k <= +VOISize; k++)
            {
                currIndex[0] = index[0]+i;
                currIndex[1] = index[1]+j;
                currIndex[2] = index[2]+k;
                if (isInRegion(currIndex))
                    sum += m_InputPointer->GetPixel(currIndex);
            }
            break;
        case VOI_SPHERE:
            for (int i = -VOISize; i <= +VOISize; i++) for (int j = -VOISize; j <= +VOISize; j++) for (int k = -VOISize; k <= +VOISize; k++)
            {
                if ((i*i+j*j+k*k) <= VOISizeSquare)
                {
                    currIndex[0] = index[0]+i;
                    currIndex[1] = index[1]+j;
                    currIndex[2] = index[2]+k;
                    if (isInRegion(currIndex))
                        sum += m_InputPointer->GetPixel(currIndex);
                }
            }
            break;
        case VOI_ITKMEAN:
            
            break;
        default:
            break;
    }
    return sum;
*/
	return 0;
}


#include "itkImageRegionConstIteratorWithIndex.h"

/// Trova i massimi sull'intero volume
electrodeAxisExtractionFilter::VectorType electrodeAxisExtractionFilter::findMax(RegionType region)
{
    std::cerr << "Ricerca del massimo:" << std::flush;
    IndexType currIndex;
    VectorType maxIndexVector;
    ImageType::ValueType currValue;
    ImageType::ValueType maxValue = 0;
    //int countMax = 0;


    typedef itk::ImageRegionConstIteratorWithIndex< ImageType > IteratorType;
    IteratorType inputIt( m_InputPointer, region );

    for ( inputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt)
    {
        currValue = inputIt.Get();
        if (currValue)
        {
            currIndex = inputIt.GetIndex();
            if (currValue > maxValue)
            {
                maxIndexVector.clear();
                maxValue = currValue;
                maxIndexVector.push_back(currIndex);
            }
            else if (currValue == maxValue)
                maxIndexVector.push_back(currIndex);
        }
    }
    return maxIndexVector;

}



#include <itkConstNeighborhoodIterator.h>

//ImageType::IndexType electrodeAxisExtractionFilter::findMaxVOI(std::vector<ImageType::IndexType> indexVector, int startVOISize, int maxSteps, voi_mode mode)
electrodeAxisExtractionFilter::VectorValueType electrodeAxisExtractionFilter::meanVOI(VectorType indexVector, SizeValueType VOISize)
{

    typedef itk::NumericTraits< ValueType >::AccumulateType SumType;
    typedef itk::NumericTraits< SumType >::RealType MeanType;

    typedef itk::ConstNeighborhoodIterator< ImageType > NeighborhoodIteratorType;
    NeighborhoodIteratorType::RadiusType radius;
    radius.Fill(VOISize);

    int VoxelPerVOI = (VOISize*2+1)*(VOISize*2+1)*(VOISize*2-1);


    ImageType::SizeType size;
    size[0] = 1;
    size[1] = 1;
    size[2] = 1;
    ImageType::RegionType region;
    region.SetSize(size);

    VectorValueType resultVector;

    NeighborhoodIteratorType it;
    VectorType::iterator iter;
    for( iter = indexVector.begin(); iter < indexVector.end(); iter++)
    {
        region.SetIndex(*iter);
        it = NeighborhoodIteratorType( radius, m_InputPointer, region );
        unsigned int neighborhoodSize = it.Size();
        SumType sum = itk::NumericTraits< SumType >::Zero;

        for (it.GoToBegin(); !it.IsAtEnd(); ++it)
        {
            for (unsigned int i = 0; i < neighborhoodSize; ++i)
            {
                sum += static_cast<ValueType>( it.GetPixel(i) );
            }
        }
        MeanType mean = static_cast< MeanType >( sum ) / static_cast< MeanType >( VoxelPerVOI );
        std::cout << "SUM = " << sum << " VoxelPerVOI = " << VoxelPerVOI << " mean = " << mean << std::endl;
        resultVector.push_back(mean);
    }
    return resultVector;



/*




    long double max;
    int steps;

    std::vector<ImageType::IndexType> newIndexVector;

    for (steps = 0; steps <= maxSteps; steps++)
    {
        max = 0;
        std::cerr << "-------------------> step " << steps << " size = " << indexVector.size() << std::endl;
        std::vector<ImageType::IndexType>::iterator iter;
        for( iter = indexVector.begin(); iter < indexVector.end(); iter++)
        {
            long double curr = VOISum((*iter), startVOISize+steps, mode);
            std::cerr << (*iter) << " " << curr << std::endl;
            if (curr > max)
            {
                max = curr;
                newIndexVector.clear();
                newIndexVector.push_back(*iter);
            }
            else if (curr == max)
                newIndexVector.push_back(*iter);
        }
        //Se ho un solo elemento esco
        if (newIndexVector.size()==1)
            break;
        //Altrimenti ripeto il ciclo sul nuovo vettore, con un VOI più largo
        indexVector.clear();
        for ( iter = newIndexVector.begin(); iter < newIndexVector.end(); iter++)
            indexVector.push_back(*iter);
        newIndexVector.clear();
    }
    std::cerr << "Massimo = " << newIndexVector[0] << " " << (int)m_InputPointer->GetPixel(newIndexVector[0]) << std::endl;
    std::cerr << "steps eseguiti = " << steps+1 << std::endl;

    if (newIndexVector.size() > 1)
        std::cerr << "attenzione: individuato più di un valore dopo " << maxSteps << "cicli. Seleziono il primo." << std::endl;
    return newIndexVector[0];
*/
}

#include <cfloat>

ImageType::IndexType electrodeAxisExtractionFilter::findMinVOI(std::vector<ImageType::IndexType> indexVector, int startVOISize, int maxSteps, voi_mode mode)
{
    long double min;
    int steps;

    std::vector<ImageType::IndexType> newIndexVector;

    for (steps = 0; steps <= maxSteps; steps++)
    {
        min = LDBL_MAX;
        std::cerr << "-------------------> step " << steps << " size = " << indexVector.size() << std::endl;
        std::vector<ImageType::IndexType>::iterator iter;
        for( iter = indexVector.begin(); iter < indexVector.end(); iter++)
        {
            long double curr = VOISum((*iter), startVOISize+steps, mode);
            std::cerr << (*iter) << " " << curr << std::endl;
            if (curr < min)
            {
                min = curr;
                newIndexVector.clear();
                newIndexVector.push_back(*iter);
            }
            else if (curr == min)
                newIndexVector.push_back(*iter);
        }
        //Se ho un solo elemento esco
        if (newIndexVector.size()==1)
            break;
        //Altrimenti ripeto il ciclo sul nuovo vettore, con un VOI più largo
        indexVector.clear();
        for ( iter = newIndexVector.begin(); iter < newIndexVector.end(); iter++)
            indexVector.push_back(*iter);
        newIndexVector.clear();
    }
    std::cerr << "Minimo = " << newIndexVector[0] << " " << (int)m_InputPointer->GetPixel(newIndexVector[0]) << std::endl;
    std::cerr << "steps eseguiti = " << steps+1 << std::endl;

    if (newIndexVector.size() > 1)
        std::cerr << "attenzione: individuato più di un valore dopo " << maxSteps << "cicli. Seleziono il primo." << std::endl;
    return newIndexVector[0];
}










} //namespace filter









/*
typedef itk::HessianRecursiveGaussianImageFilter< ImageType > hrgType;
typedef itk::Hessian3DToVesselnessMeasureImageFilter< ImageType::PixelType > hvmType;

hrgType::Pointer hessian = hrgType::New();
hessian->SetInput(m_InputPointer);

hessian->SetSigma(m_Sigma);// Will find vessels of radius 5

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
*/
