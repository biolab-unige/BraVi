//
// C++ Interface: orientFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __ORIENTFILTER_H__
#define __ORIENTFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class orientFilter : public abstractFilter
{
public:
    orientFilter();
    ~orientFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __ORIENTFILTER_H__
