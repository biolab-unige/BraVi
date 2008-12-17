//
// C++ Implementation: Surface3DActor
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


//#define __DECIMATE__
//#define __WIREFRAME__



#include "Surface3DActor.h"

#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>


#ifdef __DECIMATE__
  #include <vtkDecimatePro.h>
  #include <vtkSmoothPolyDataFilter.h>
  #include <vtkPolyDataNormals.h>
#endif




vtkCxxRevisionMacro(Surface3DActor, "$Revision: 1.3 $");
vtkStandardNewMacro(Surface3DActor);


Surface3DActor::Surface3DActor() : vtkOpenGLActor()
{
    m_MarchingCubes = NULL;
}

Surface3DActor::~Surface3DActor()
{
    if(m_MarchingCubes)
        m_MarchingCubes->Delete();
}

void Surface3DActor::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os,indent);
}

void Surface3DActor::SetInput(vtkImageData* input)
{
    input->GetScalarRange(m_Range);

    m_MarchingCubes = vtkMarchingCubes::New();
    m_MarchingCubes->SetInput(input);
    m_MarchingCubes->SetValue(0, m_Range[1]); //Il secondo valore setta la superficie
    m_MarchingCubes->ComputeGradientsOff();
    m_MarchingCubes->ComputeNormalsOff();
    m_MarchingCubes->ComputeScalarsOn();

    vtkPolyDataMapper* SurfMapper = vtkPolyDataMapper::New();

#ifdef __DECIMATE__
    vtkDecimatePro* decimate = vtkDecimatePro::New();
    decimate->SetInputConnection(m_MarchingCubes->GetOutputPort());
    decimate->SetTargetReduction(0.4);
    decimate->PreserveTopologyOn();

    vtkSmoothPolyDataFilter* smoother = vtkSmoothPolyDataFilter::New();
    smoother->SetInputConnection(decimate->GetOutputPort());
    smoother->SetNumberOfIterations(50);

    vtkPolyDataNormals* normals = vtkPolyDataNormals::New();
    normals->SetInputConnection(smoother->GetOutputPort());
    normals->FlipNormalsOn();

    SurfMapper->SetInputConnection(normals->GetOutputPort());

    decimate->Delete();
    smoother->Delete();
    normals->Delete();
#else
    SurfMapper->SetInputConnection(m_MarchingCubes->GetOutputPort());
#endif

    SurfMapper->ScalarVisibilityOff();

    SetMapper(SurfMapper);
    SurfMapper->Delete();

    setColor(COLOR_RED);

#ifdef __WIREFRAME__
//  WIREFRAME
    GetProperty()->SetRepresentationToWireframe();
#endif
}

void Surface3DActor::setSurface(int surf)
{
    m_MarchingCubes->SetValue(0, surf);
}

void Surface3DActor::setColor(ColorType cType)
{
    float color[3];
    float specular = 0.5;
    float specularPower = 2;
    float opacity = 1.0;
    switch (cType)
    {
        case COLOR_RED:
            color[0] = 1.0;
            color[1] = 0.0;
            color[2] = 0.0;
            break;
        case COLOR_GREEN:
            color[0] = 0.0;
            color[1] = 1.0;
            color[2] = 0.0;
            break;
        case COLOR_BLUE:
            color[0] = 0.0;
            color[1] = 0.0;
            color[2] = 1.0;
            break;
        case COLOR_SKIN:
            color[0] = 0.9;
            color[1] = 0.7;
            color[2] = 0.5;
            specular = 0.0;
            specularPower = 5;
            break;
        case COLOR_METAL:
            color[0] = 1.0;
            color[1] = 1.0;
            color[2] = 0.9142;
            specular = 0.3;
            specularPower = 20;
            break;
        case COLOR_GLASS:
            color[0] = 0.8;
            color[1] = 1.0;
            color[2] = 1.0;
            specular = 0.1;
            opacity = 0.2;
GetProperty()->BackfaceCullingOn();
GetProperty()->FrontfaceCullingOn();

            break;
        default:
            break;
    }
    GetProperty()->SetDiffuseColor(color[0], color[1], color[2]);
    GetProperty()->SetSpecular(specular);
    GetProperty()->SetSpecularPower(specularPower);
    GetProperty()->SetOpacity(opacity);
}

void Surface3DActor::setCustomColor(float color0, float color1, float color2, float specular, float specularPower)

{
    GetProperty()->SetDiffuseColor(color0, color1, color2);
    GetProperty()->SetSpecular(specular);
    GetProperty()->SetSpecularPower(specularPower);
}
