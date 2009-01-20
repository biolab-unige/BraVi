//
// C++ Interface: abstractFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __ABSTRACTFILTER_H__
#define __ABSTRACTFILTER_H__

#include <itkImage.h>

#include "imageDefs.h"

#include "filterParameter.h"
#include "filterInput.h"
#include "filterOutput.h"


namespace filter
{

/**
	@author Daniele Domenichelli
*/
class abstractFilter
{
friend class filterDialog;
public:
    abstractFilter(std::string name = "Filter") {}
    ~abstractFilter() {}

    virtual bool Execute() = 0;

    inline std::string getName(void) {return m_Name;}
    inline void setName(std::string name) {m_Name = name;}


protected:
    inline bool addParameter(abstractParameter* par) {return m_filterParameterList.addParameter(par);}
    inline abstractParameter* getParameter(std::string name) {return m_filterParameterList.getParameter(name);}
    inline abstractParameter* getParameter(int index) {return m_filterParameterList.getParameter(index);}

    inline bool addInput(abstractInput* input) {return m_filterInputList.addInput(input);}
    inline abstractInput* getInput(std::string name) {return m_filterInputList.getInput(name);}
    inline abstractInput* getInput(int index) {return m_filterInputList.getInput(index);}

    inline bool addOutput(abstractOutput* output) {return m_filterOutputList.addOutput(output);}
    inline abstractOutput* getOutput(std::string name) {return m_filterOutputList.getOutput(name);}
    inline abstractOutput* getOutput(int index) {return m_filterOutputList.getOutput(index);}

    inline void Update(void) {m_filterParameterList.Update(); m_filterInputList.Update(); m_filterOutputList.Update();}

private:
    filterParameterList m_filterParameterList;
    filterInputList m_filterInputList;
    filterOutputList m_filterOutputList;

    std::string m_Name;

};

} //namespace filter

#endif // __ABSTRACTFILTER_H__
