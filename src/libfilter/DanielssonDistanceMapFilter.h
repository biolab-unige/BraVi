//
// C++ Interface: DanielssonDistanceMapFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __DANIELSSONDISTANCEMAPFILTER__
#define __DANIELSSONDISTANCEMAPFILTER__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class DanielssonDistanceMapFilter : public abstractFilter
{
public:
    DanielssonDistanceMapFilter();
    ~DanielssonDistanceMapFilter();
    bool Execute(void);

private:
//    typedef int InternalPixelType;
//    typedef itk::Image<InternalPixelType, Dimension> InternalImageType;

    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;

// Parametri
    PixelType  m_RescaleOutputMinimum;
    PixelType  m_RescaleOutputMaximum;
};

} //namespace filter

#endif // __DANIELSSONDISTANCEMAPFILTER__
