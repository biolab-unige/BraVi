//
// C++ Interface: convertSeriesFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __CONVERTSERIESFILTER_H__
#define __CONVERTSERIESFILTER_H__

#include "abstractFilter.h"

namespace filter
{
/**
	@author Daniele Domenichelli
*/
class convertSeriesFilter : public abstractFilter
{
public:
    convertSeriesFilter();
    ~convertSeriesFilter();
    bool Execute(void);

private:

    std::string m_DirectoryName;
    unsigned char m_ReverseOrder;
    std::string m_OutputFileName;
};

} //namespace filter

#endif // __CONVERTSERIESFILTER_H__
