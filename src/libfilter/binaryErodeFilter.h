//
// C++ Interface: binaryErodeFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BINARYERODEFILTER__
#define __BINARYERODEFILTER__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class binaryErodeFilter : public abstractFilter
{
public:
    binaryErodeFilter();
    ~binaryErodeFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;

// Parametri
    ImageType::ValueType m_Foreground;
    ImageType::ValueType m_Background;

    int m_ErodeRadius;
};

} //namespace filter

#endif // __BINARYERODEFILTER__
