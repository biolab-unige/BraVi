//
// C++ Interface: sigmoidFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __SIGMOIDFILTER_H__
#define __SIGMOIDFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class sigmoidFilter : public abstractFilter
{
public:
    sigmoidFilter();
    ~sigmoidFilter();
    bool Execute(void);

private:
    char m_Alpha;
    unsigned char m_Beta;
    unsigned char m_OutputMinimum;
    unsigned char m_OutputMaximum;

    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __SIGMOIDFILTER_H__
