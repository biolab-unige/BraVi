//
// C++ Interface: hessianFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __HESSIANFILTER__
#define __HESSIANFILTER__

#include "abstractFilter.h"

#include <vector> //TODO Togliere sto vector
#include <map>

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class hessianFilter : public abstractFilter
{
public:
    hessianFilter();
    ~hessianFilter();
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
    std::string m_OutputFileName;

//Funzioni private
    ImageType::Pointer m_InternalPointer;

};

} //namespace filter

#endif // __HESSIANFILTER__
