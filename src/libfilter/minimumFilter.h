//
// C++ Interface: minimumFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __MINIMUMFILTER_H__
#define __MINIMUMFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class minimumFilter : public abstractFilter
{
public:
    minimumFilter();
    ~minimumFilter();
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

#endif // __MINIMUMFILTER_H__
