//
// C++ Interface: betFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __BETFILTER_H__
#define __BETFILTER_H__

#include "abstractFilter.h"

#define _FSL_CONF_ "FSLDIR=\"/usr/local/src/FSL/fsl\" FSLOUTPUTTYPE=\"NIFTI\""
#define _BET_ "/usr/local/src/FSL/fsl/bin/bet"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class betFilter : public abstractFilter
{
public:
    betFilter();
    ~betFilter();
    bool Execute(void);

private:
    bool m_Mask;
    double m_FractionaryThreshold;
    std::string m_InputFile;
    std::string m_OutputFile;
};

} //namespace filter

#endif // __BETFILTER_H__
