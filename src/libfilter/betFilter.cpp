//
// C++ Implementation: betFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "betFilter.h"

#include <cstdlib>
#include <sstream>

namespace filter
{

betFilter::betFilter() : abstractFilter()
{
    setName("betFilter");

// Parametri
    addInput(new filterInput<std::string>(m_InputFile, "Input File")); //, "/home/biolab/DADO/risonanze_elettrodi/Valdirosa_SEEG.hdr"));
    addParameter(new filterParameter<std::string>(m_OutputFile, "Output File", "/home/biolab/tmp/bet"));
    addParameter(new filterParameter<bool>(m_Mask, "Mask", true));
    addParameter(new filterParameter<double>(m_FractionaryThreshold, "Fractionary Threshold", 0.5));

}


betFilter::~betFilter()
{
}

bool betFilter::Execute( void )
{
    Update();

    std::ostringstream command;

    //Fsl Config
    command << _FSL_CONF_ << " ";

    //Program Name
    command << _BET_ << " ";

    //Input File
    command << m_InputFile << " ";

    //Output File
    command << m_OutputFile << " ";

    //Options
    if (m_Mask)
        command << "-m ";

    if (m_FractionaryThreshold != 0.5)
        command << "-f " << m_FractionaryThreshold << " ";

    std::cout << command.str() << std::endl;
    int i = system(command.str().c_str());
    return i ? false : true;
}




} //namespace filter
