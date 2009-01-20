//
// C++ Interface: imageDefs.h
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __IMAGEDEFS__
#define __IMAGEDEFS__

#include <itkImage.h>

const unsigned int Dimension = 3;
typedef unsigned char PixelType;
typedef itk::Image<PixelType, Dimension> ImageType;

#endif // __IMAGEDEFS__
