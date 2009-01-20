//
// C++ Interface: maximumFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __MAXIMUMFILTER_H__
#define __MAXIMUMFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class maximumFilter : public abstractFilter
{
public:
    maximumFilter();
    ~maximumFilter();
    bool Execute(void);

private:
// Parametri

// Input
    ImageType::Pointer m_InputPointer1;
    ImageType::Pointer m_InputPointer2;

//Output
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __MAXIMUMFILTER_H__
