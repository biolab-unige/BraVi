//
// C++ Interface: skelInsertFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __SKELINSERTFILTER_H__
#define __SKELINSERTFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class skelInsertFilter : public abstractFilter
{
public:
    skelInsertFilter();
    ~skelInsertFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
    std::string m_Filename;
};

} //namespace filter

#endif // __CONVERTFILTER_H__
