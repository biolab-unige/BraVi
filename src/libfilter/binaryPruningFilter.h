//
// C++ Interface: binaryPruningFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BINARYPRUNINGFILTER__
#define __BINARYPRUNINGFILTER__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class binaryPruningFilter : public abstractFilter
{
public:
    binaryPruningFilter();
    ~binaryPruningFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __BINARYPRUNINGFILTER__
