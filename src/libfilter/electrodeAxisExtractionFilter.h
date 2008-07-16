//
// C++ Interface: electrodeAxisExtractionFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __ELECTRODEAXISEXTRACTIONFILTER__
#define __ELECTRODEAXISEXTRACTIONFILTER__

#include "abstractFilter.h"

#include <vector> //TODO Togliere sto vector
#include <map>

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class electrodeAxisExtractionFilter : public abstractFilter
{
public:
    electrodeAxisExtractionFilter();
    ~electrodeAxisExtractionFilter();
    bool Execute(void);

private:
// Typedef
    typedef ImageType::RegionType RegionType;
    typedef ImageType::IndexType IndexType;
    typedef ImageType::SizeValueType SizeValueType;
    typedef ImageType::ValueType ValueType;
    typedef std::vector< ImageType::IndexType > VectorType;
    typedef std::vector< double > VectorValueType;


    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;

// Parametri
    double m_Sigma;
    double m_Alpha1;
    double m_Alpha2;

//Funzioni private

    enum voi_mode {VOI_CUBE = 0, VOI_SPHERE, VOI_ITKMEAN};

    ImageType::Pointer m_InternalPointer;

    long double VOISum(ImageType::IndexType index, int VOISize, voi_mode mode);
    VectorType findMax(RegionType region);
    VectorValueType meanVOI(VectorType indexVector, SizeValueType VOISize);

//    ImageType::IndexType findMaxVOI(VectorType indexVector, int startVOISize = 1, int maxSteps=5, voi_mode mode = VOI_CUBE);
    ImageType::IndexType findMinVOI(VectorType indexVector, int startVOISize = 1, int maxSteps=5, voi_mode mode = VOI_CUBE);
};

} //namespace filter

#endif // __ELECTRODEAXISEXTRACTIONFILTER__
