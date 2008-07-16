//
// C++ Interface: geodesicActiveContourFullFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __GEODESICACTIVECONTOURFULLFILTER_H__
#define __GEODESICACTIVECONTOURFULLFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class geodesicActiveContourFullFilter : public abstractFilter
{
public:
    geodesicActiveContourFullFilter();
    ~geodesicActiveContourFullFilter();
    bool Execute(void);

private:
    typedef float InternalPixelType;
    typedef itk::Image<InternalPixelType, Dimension> InternalImageType;



// Input
    ImageType::Pointer m_InputPointer;

//Output
    ImageType::Pointer m_OutputPointer;

// Parametri
    //Gradient Magnitude
    float m_Sigma;

    //Sigmoid
    float m_Alpha;
    float m_Beta;

    //Fast Marching
    ImageType::IndexType m_Seed;
    double m_InitialDistance;

    //Geodesic Active Contour
    float m_PropagationScaling;
    float m_CurvatureScaling;
    float m_AdvectionScaling;


};

} //namespace filter

#endif // __GEODESICACTIVECONTOURFULLFILTER_H__
