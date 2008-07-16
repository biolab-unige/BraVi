//
// C++ Interface: derivativeFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __DERIVATIVEFILTER_H__
#define __DERIVATIVEFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class derivativeFilter : public abstractFilter
{
public:
    derivativeFilter();
    ~derivativeFilter();
    bool Execute(void);

private:

    typedef float InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension> InternalImageType;

//parameters
    unsigned int m_Order;
    unsigned int m_Direction;
    InternalPixelType  m_RescaleInternalOutputMinimum;
    InternalPixelType  m_RescaleInternalOutputMaximum;
    unsigned char m_RescaleOutputMinimum;
    unsigned char m_RescaleOutputMaximum;
//input
    ImageType::Pointer m_InputPointer;
//output
    ImageType::Pointer m_OutputPointer;
};

} //namespace filter

#endif // __DERIVATIVEFILTER_H__
