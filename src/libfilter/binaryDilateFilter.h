//
// C++ Interface: binaryDilateFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BINARYDILATEFILTER__
#define __BINARYDILATEFILTER__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class binaryDilateFilter : public abstractFilter
{
public:
    binaryDilateFilter();
    ~binaryDilateFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;

// Parametri
    ImageType::ValueType m_Foreground;
    ImageType::ValueType m_Background;

    int m_DilateRadius;
};

} //namespace filter

#endif // __BINARYDILATEFILTER__
