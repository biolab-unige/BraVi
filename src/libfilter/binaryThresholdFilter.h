//
// C++ Interface: binaryThresholdFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BINARYTHRESHOLDFILTER_H__
#define __BINARYTHRESHOLDFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class binaryThresholdFilter : public abstractFilter
{
public:
    binaryThresholdFilter();
    ~binaryThresholdFilter();
    bool Execute(void);

private:
    ImageType::ValueType m_LowerThreshold;
    ImageType::ValueType m_UpperThreshold;
    ImageType::ValueType m_OutsideValue;
    ImageType::ValueType m_InsideValue;

    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __BINARYTHRESHOLDFILTER_H__
