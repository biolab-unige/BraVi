//
// C++ Interface: electrodeAxisExtractionFilter3
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __ELECTRODEAXISEXTRACTIONFILTER3__
#define __ELECTRODEAXISEXTRACTIONFILTER3__

#include "abstractFilter.h"

#include <vector> //TODO Togliere sto vector
#include <map>

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class electrodeAxisExtractionFilter3 : public abstractFilter
{
public:
    electrodeAxisExtractionFilter3();
    ~electrodeAxisExtractionFilter3();
    bool Execute(void);

private:
// Typedef
    typedef ImageType::RegionType RegionType;
    typedef ImageType::IndexType IndexType;
    typedef ImageType::SizeValueType SizeValueType;
    typedef ImageType::ValueType ValueType;
    typedef std::vector< ImageType::IndexType > VectorType;
    typedef std::vector< float > VectorValueType;


    ImageType::Pointer m_InputPointer;
//    ImageType::Pointer m_OutputPointer;
    ImageType::Pointer m_Eigen0OutputPointer;
//    ImageType::Pointer m_Eigen1OutputPointer;
//    ImageType::Pointer m_Eigen2OutputPointer;

// Parametri
    float m_Sigma;


//Funzioni private
    ImageType::Pointer m_InternalPointer;

};

} //namespace filter

#endif // __ELECTRODEAXISEXTRACTIONFILTER3__
