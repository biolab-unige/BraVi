//
// C++ Interface: rescaleFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __RESCALEFILTER_H__
#define __RESCALEFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class rescaleFilter : public abstractFilter
{
public:
    rescaleFilter();
    ~rescaleFilter();
    bool Execute(void);

private:
    unsigned char m_OutputMinimum;
    unsigned char m_OutputMaximum;

    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __RESCALEFILTER_H__
