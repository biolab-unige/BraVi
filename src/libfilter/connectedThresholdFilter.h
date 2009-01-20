//
// C++ Interface: connectedThresholdFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __CONNECTEDTHRESHOLDFILTER_H__
#define __CONNECTEDTHRESHOLDFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class connectedThresholdFilter : public abstractFilter
{
public:
    connectedThresholdFilter();
    ~connectedThresholdFilter();
    bool Execute(void);

private:
// Parametri
    ImageType::ValueType m_Lower;
    ImageType::ValueType m_Upper;
    ImageType::ValueType m_ReplaceValue;

    ImageType::IndexType m_Seed;

    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __CONNECTEDTHRESHOLDFILTER_H__
