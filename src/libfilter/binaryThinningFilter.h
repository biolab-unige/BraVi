//
// C++ Interface: binaryThinningFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BINARYTHINNINGFILTER__
#define __BINARYTHINNINGFILTER__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class binaryThinningFilter : public abstractFilter
{
public:
    binaryThinningFilter();
    ~binaryThinningFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __BINARYTHINNINGFILTER__
