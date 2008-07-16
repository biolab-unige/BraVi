//
// C++ Implementation: testFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "testFilter.h"

namespace filter
{

testFilter::testFilter() : abstractFilter()
{
    setName("testFilter");
    addParameter(new filterParameter<std::string>(a, "Prova std::string", "Default"));
    addParameter(new filterParameter<double>(b, "Prova double", 3.0));
    addParameter(new filterParameter<float>(c, "Prova float", 3.0));
    addParameter(new filterParameter<int>(d, "Prova int",6,5,10));
    addParameter(new filterParameter<char>(e, "Prova char", 9,-55,32));
    addParameter(new filterParameter<unsigned char>(f, "Prova unsigned char", 255));

    addParameter(new filterParameter<short int>(g, "Prova unknown", 255));

}


testFilter::~testFilter()
{
}

bool testFilter::Execute( void )
{
    Update();
    std::cout << "testFilter::Execute()" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "e = " << e << std::endl;
    std::cout << "f = " << f << std::endl;
    std::cout << "g = " << g << std::endl;
	return true;
}

} //namespace filter
