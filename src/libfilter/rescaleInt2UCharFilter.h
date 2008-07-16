//
// C++ Interface: rescaleInt2UCharFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __RESCALEINT2UCHARFILTER_H__
#define __RESCALEINT2UCHARFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class rescaleInt2UCharFilter : public abstractFilter
{
public:
    rescaleInt2UCharFilter();
    ~rescaleInt2UCharFilter();
    bool Execute(void);

private:
    unsigned char m_OutputMinimum;
    unsigned char m_OutputMaximum;

    std::string m_InputFileName;

    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __RESCALEINT2UCHARFILTER_H__
