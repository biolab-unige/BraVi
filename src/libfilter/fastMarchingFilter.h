//
// C++ Interface: fastMarchingFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FASTMARCHINGFILTER_H__
#define __FASTMARCHINGFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class fastMarchingFilter : public abstractFilter
{
public:
    fastMarchingFilter();
    ~fastMarchingFilter();
    bool Execute(void);

private:
    typedef float InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension> InternalImageType;

// Input
    ImageType::Pointer m_InputPointer;

//Output
    ImageType::Pointer m_OutputPointer;

// Parametri
    InternalPixelType  m_RescaleInternalOutputMinimum;
    InternalPixelType  m_RescaleInternalOutputMaximum;

    PixelType  m_RescaleOutputMinimum;
    PixelType  m_RescaleOutputMaximum;

    ImageType::IndexType m_Seed;

    double  m_StoppingTime;
    double  m_SpeedConstant;
    double  m_SeedValue;

};

} //namespace filter

#endif // __FASTMARCHINGFILTER_H__
