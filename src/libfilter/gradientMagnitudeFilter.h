//
// C++ Interface: gradientMagnitudeFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __GRADIENTMAGNITUDEFILTER_H__
#define __GRADIENTMAGNITUDEFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class gradientMagnitudeFilter : public abstractFilter
{
public:
    gradientMagnitudeFilter();
    ~gradientMagnitudeFilter();
    bool Execute(void);

private:

    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __GRADIENTMAGNITUDEFILTER_H__
