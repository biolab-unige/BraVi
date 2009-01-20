//
// C++ Interface: geodesicActiveContourFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __GEODESICACTIVECONTOURFILTER_H__
#define __GEODESICACTIVECONTOURFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class geodesicActiveContourFilter : public abstractFilter
{
public:
    geodesicActiveContourFilter();
    ~geodesicActiveContourFilter();
    bool Execute(void);

private:
    typedef float InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension> InternalImageType;



// Input
    ImageType::Pointer m_InputPointer1;
    ImageType::Pointer m_InputPointer2;

//Output
    ImageType::Pointer m_OutputPointer;

// Parametri
    InternalPixelType  m_RescaleInternalOutputMinimum;
    InternalPixelType  m_RescaleInternalOutputMaximum;

    PixelType  m_RescaleOutputMinimum;
    PixelType  m_RescaleOutputMaximum;

    float m_PropagationScaling;
    float m_CurvatureScaling;
    float m_AdvectionScaling;



};

} //namespace filter

#endif // __GEODESICACTIVECONTOURFILTER_H__
