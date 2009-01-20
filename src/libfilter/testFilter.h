//
// C++ Interface: testFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __TESTFILTER_H__
#define __TESTFILTER_H__

#include "abstractFilter.h"
#include <iostream>

namespace filter
{
/**
	@author 
*/
class testFilter : public abstractFilter
{
public:
    testFilter();
    ~testFilter();
    bool Execute(void);

private:
    std::string a;
    double b;
    float c;
    int d;
    char e;
    unsigned char f;
    short int g;
};

} //namespace filter

#endif // __TESTFILTER_H__
