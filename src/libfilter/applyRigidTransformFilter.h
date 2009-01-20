//
// C++ Interface: applyRigidTransformFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __APPLYRIGIDTRANSFORMFILTER_H__
#define __APPLYRIGIDTRANSFORMFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class applyRigidTransformFilter : public abstractFilter
{
public:
    applyRigidTransformFilter();
    ~applyRigidTransformFilter();
    bool Execute(void);

private:
    ImageType::Pointer m_InputPointer;
    ImageType::Pointer m_OutputPointer;
    std::string m_TransformFilename;
};

} //namespace filter

#endif // __APPLYRIGIDTRANSFORMFILTER_H__
