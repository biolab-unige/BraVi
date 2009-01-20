//
// C++ Implementation: convertFilter
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "convertFilter.h"

namespace filter
{

convertFilter::convertFilter() : abstractFilter()
{
    setName("convertFilter");

// Input
    addInput(new filterInput<ImageType::Pointer>(m_InputPointer, "Input File"));

// Output
    addOutput(new filterOutput<ImageType::Pointer>(m_OutputPointer, "Output File"));
}


convertFilter::~convertFilter()
{
}

bool convertFilter::Execute( void )
{
    Update();

    m_OutputPointer = m_InputPointer;

    return true;
}











} //namespace filter
