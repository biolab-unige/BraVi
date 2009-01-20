//
// C++ Interface: padFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __PADFILTER_H__
#define __PADFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class padFilter : public abstractFilter
{
public:
    padFilter();
    ~padFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
    unsigned int m_PadSize;
    unsigned char m_PadReplaceValue;
};

} //namespace filter

#endif // __PADFILTER_H__
