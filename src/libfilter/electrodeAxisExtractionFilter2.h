//
// C++ Interface: electrodeAxisExtractionFilter2
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __ELECTRODEAXISEXTRACTIONFILTER2__
#define __ELECTRODEAXISEXTRACTIONFILTER2__

#include "abstractFilter.h"

#include <vector> //TODO Togliere sto vector
#include <map>

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class electrodeAxisExtractionFilter2 : public abstractFilter
{
public:
    electrodeAxisExtractionFilter2();
    ~electrodeAxisExtractionFilter2();
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
    ImageType::Pointer m_InternalPointer;

};

} //namespace filter

#endif // __ELECTRODEAXISEXTRACTIONFILTER2__
