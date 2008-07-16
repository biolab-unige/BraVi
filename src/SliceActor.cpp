//
// C++ Implementation: SliceActor
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifdef WIN32
#define M_PI 3.1415926535
#endif

#include "SliceActor.h"

#include <vtkObjectFactory.h>

#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>


#include <cmath>


vtkCxxRevisionMacro(SliceActor, "$Revision: 1.5 $");
vtkStandardNewMacro(SliceActor);


#if defined(VTK_USE_MANGLED_MESA)
SliceActor::SliceActor() : vtkMesaImageActor(),
#else
SliceActor::SliceActor() : vtkOpenGLImageActor(),
#endif
                            m_ResliceAxes(NULL),
                            m_ImageReslice(NULL),
                            m_LookupTable(NULL),
                            m_Alpha(NULL),
                            m_Beta(NULL),
                            m_Gamma(NULL)
{
    m_ResliceAxes = vtkMatrix4x4::New();
    m_ImageReslice = vtkImageReslice::New();
    m_LookupTable = vtkLookupTable::New();

    m_Alpha = vtkMatrix4x4::New();
    m_Beta = vtkMatrix4x4::New();
    m_Gamma = vtkMatrix4x4::New();
}

SliceActor::~SliceActor()
{
    if(m_ResliceAxes)
        m_ResliceAxes->Delete();
    if(m_LookupTable)
        m_LookupTable->Delete();
    if(m_ImageReslice)
        m_ImageReslice->Delete();
    if(m_Alpha)
        m_Alpha->Delete();
    if(m_Beta)
        m_Beta->Delete();
    if(m_Gamma)
        m_Gamma->Delete();
}


void SliceActor::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os,indent);
    os << "MinX = " << m_extent[0] << std::endl;
    os << "MaxX = " << m_extent[1] << std::endl;
    os << "MinY = " << m_extent[2] << std::endl;
    os << "MaxY = " << m_extent[3] << std::endl;
    os << "MinZ = " << m_extent[4] << std::endl;
    os << "MaxZ = " << m_extent[5] << std::endl;
    m_ResliceAxes->PrintSelf(os, indent);
}



void SliceActor::SetInput(vtkImageData* input)
{
    input->UpdateInformation();
    input->GetWholeExtent(m_extent);
    input->GetSpacing(m_spacing);
    input->GetOrigin(m_origin);


    m_limit[0] = m_origin[0] + m_spacing[0]*m_extent[0];
    m_limit[1] = m_origin[0] + m_spacing[0]*m_extent[1];
    m_limit[2] = m_origin[1] + m_spacing[1]*m_extent[2];
    m_limit[3] = m_origin[1] + m_spacing[1]*m_extent[3];
    m_limit[4] = m_origin[2] + m_spacing[2]*m_extent[4];
    m_limit[5] = m_origin[2] + m_spacing[2]*m_extent[5];

    m_center[0] = (m_extent[0] + m_extent[1])/2;
    m_center[1] = (m_extent[2] + m_extent[3])/2;
    m_center[2] = (m_extent[4] + m_extent[5])/2;

    m_ResliceAxes->DeepCopy(axialElements);
    m_Alpha->DeepCopy(axialElements);
    m_Beta->DeepCopy(axialElements);
    m_Gamma->DeepCopy(axialElements);

    // Set the point through which to slice
/*
    m_ResliceAxes->SetElement(0, 3, center[0]);
    m_ResliceAxes->SetElement(1, 3, center[1]);
    m_ResliceAxes->SetElement(2, 3, center[2]);
*/

    m_ImageReslice->SetInput(input);
    m_ImageReslice->SetOutputDimensionality(2);
    m_ImageReslice->SetResliceAxes(m_ResliceAxes);
    m_ImageReslice->AutoCropOutputOn();
//    m_ImageReslice->WrapOn();
//    m_ImageReslice->BorderOff();

    //m_ImageReslice->SetInterpolationModeToNearestNeighbor();
    //m_ImageReslice->SetInterpolationModeToLinear();
    m_ImageReslice->SetInterpolationModeToCubic();

    // Create a greyscale lookup table
    m_LookupTable->SetValueRange(0.0, 1.0); // from black to white
    m_LookupTable->SetSaturationRange(0.0, 0.0); // no color saturation
    //m_LookupTable->SetAlphaRange(0.5,0.7);
    //m_LookupTable->SetHueRange(0.5,0.5);

    m_LookupTable->SetRampToLinear();
    //m_LookupTable->SetRampToSCurve();

    // Map the image through the lookup table
    vtkImageMapToColors *ImageMapToColors = vtkImageMapToColors::New();
    ImageMapToColors->SetLookupTable(m_LookupTable);
    ImageMapToColors->SetInputConnection(m_ImageReslice->GetOutputPort());

    vtkImageActor::SetInput(ImageMapToColors->GetOutput());


    ImageMapToColors->Delete();
}

void SliceActor::setX(int x)
{
    if (x < m_extent[0])
        m_X = m_limit[0];
    else if (x > m_extent[1])
        m_X = m_limit[1];
    else
        m_X = m_origin[0] + x*m_spacing[0];

    m_ResliceAxes->SetElement(0, 3, m_X);
    PokeMatrix ( m_ResliceAxes );
}

void SliceActor::setY(int y)
{
    if (y < m_extent[2])
        m_Y = m_limit[2];
    else if (y > m_extent[3])
        m_Y = m_limit[3];
    else
        m_Y = m_origin[1] + y*m_spacing[1];

    m_ResliceAxes->SetElement(1, 3, m_Y);
    PokeMatrix ( m_ResliceAxes );
}


void SliceActor::setZ(int z)
{
    if (z < m_extent[4])
        m_Z = m_limit[4];
    else if (z > m_extent[5])
        m_Z = m_limit[5];
    else
        m_Z = m_origin[2] + z*m_spacing[2];

    m_ResliceAxes->SetElement(2, 3, m_Z);
    PokeMatrix ( m_ResliceAxes );
}



void SliceActor::setMatrix(vtkMatrix4x4* matr)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m_ResliceAxes->SetElement( i, j, matr->GetElement(i, j) );
}

void SliceActor::setAngleAlpha(int alpha)
{
    double alpha_rad = alpha/180.0*M_PI;
    m_ResliceAxes->SetElement(1, 1, cos(alpha_rad));
    m_ResliceAxes->SetElement(2, 2, cos(alpha_rad));
    m_ResliceAxes->SetElement(1, 2, sin(alpha_rad));
    m_ResliceAxes->SetElement(2, 1, -sin(alpha_rad));
}

void SliceActor::setAngleBeta(int beta)
{
    double beta_rad = beta/180.0*M_PI;
    m_ResliceAxes->SetElement(0, 0, cos(beta_rad));
    m_ResliceAxes->SetElement(2, 2, cos(beta_rad));
    m_ResliceAxes->SetElement(2, 0, sin(beta_rad));
    m_ResliceAxes->SetElement(0, 2, -sin(beta_rad));
}

void SliceActor::setAngleGamma(int gamma)
{
    double gamma_rad = gamma/180.0*M_PI;
    m_ResliceAxes->SetElement(0, 0, cos(gamma_rad));
    m_ResliceAxes->SetElement(1, 1, cos(gamma_rad));
    m_ResliceAxes->SetElement(0, 1, sin(gamma_rad));
    m_ResliceAxes->SetElement(1, 0, -sin(gamma_rad));
}

void SliceActor::UpdateRotation()
{
    vtkMatrix4x4* temp1 = vtkMatrix4x4::New();
    vtkMatrix4x4* temp2 = vtkMatrix4x4::New();
    vtkMatrix4x4::Multiply4x4(m_Alpha, m_Beta, temp1);
    vtkMatrix4x4::Multiply4x4(temp1, m_Gamma, temp2);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m_ResliceAxes->SetElement( i, j, temp2->GetElement(i, j) );
    this->SetDisplayExtent(-1,0,0,0,0,0);
    PokeMatrix ( m_ResliceAxes );
}

void SliceActor::SetRange(double min, double max)
{
    m_LookupTable->SetRange(min, max);
    m_LookupTable->Build();
}

void SliceActor::SetConventionRadiological()
{

}

void SliceActor::SetConventionNeurological()
{
}

