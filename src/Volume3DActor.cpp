//
// C++ Implementation: Volume3DActor
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Volume3DActor.h"

#include <vtkObjectFactory.h>
#include <vtkVolumeTextureMapper3D.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
//#include <vtkKWEGPUVolumeRayCastMapper.h>
#include <iostream>

vtkCxxRevisionMacro(Volume3DActor, "$Revision: 1.7 $");
vtkStandardNewMacro(Volume3DActor);

Volume3DActor::Volume3DActor() : vtkVolume(),
                                 m_Color(NULL),
                                 m_Opacity(NULL),
                                 m_VolumeProperty(NULL)
{
  m_Opacity = vtkPiecewiseFunction::New();
  m_Color = vtkColorTransferFunction::New();
  m_VolumeProperty = vtkVolumeProperty::New();

  m_VolumeProperty->SetColor(m_Color);
  m_VolumeProperty->SetScalarOpacity(m_Opacity);

  this->SetProperty(m_VolumeProperty);
}

Volume3DActor::~Volume3DActor()
{
  if(m_VolumeProperty)
    m_VolumeProperty->Delete();
  if(m_Color)
    m_Color->Delete();
  if(m_Opacity)
    m_Opacity->Delete();
}


void Volume3DActor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

// NB Per usare "IsRenderSupported" è necessario che l' OpenGL context sia inizializzato (deve essere chiamato Render() prima di SetInput)
void Volume3DActor::SetInput(vtkImageData* input)
{
//Il volume viene riscalato per avere una visualizzazione migliore (se i valori non sono uniformemente distribuiti) e per risparmiare memoria video
  vtkImageShiftScale* rescaler = vtkImageShiftScale::New();

  double range[2];
  input->GetScalarRange(range);
  double scale = 255.0/(range[1] - range[0]);
  double shift = -range[0];

  rescaler->SetInput( input ); 
  rescaler->SetShift( shift );
  rescaler->SetScale( scale );
  rescaler->SetOutputScalarTypeToUnsignedChar();
  rescaler->Update();


  vtkVolumeMapper* mapper;

  // Prova prima il rendering hardware
  mapper = vtkVolumeTextureMapper3D::New();
  vtkVolumeTextureMapper3D::SafeDownCast(mapper)->SetPreferredMethodToNVidia();

  mapper->SetInputConnection(rescaler->GetOutputPort());
  this->SetMapper(mapper);

#if 0
  if( !(vtkVolumeTextureMapper3D::SafeDownCast(mapper)->IsRenderSupported(this->GetProperty())) )
  {
      vtkDebugMacro("Texture3D not supported. Switching to Software Ray Tracing");
      mapper->Delete();

      vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
      mapper = vtkVolumeRayCastMapper::New();
      vtkVolumeRayCastMapper::SafeDownCast(mapper)->SetVolumeRayCastFunction(compositeFunction);

      mapper->SetInputConnection(rescaler->GetOutputPort());
      this->SetMapper(mapper);

      compositeFunction->Delete();
  
// TODO VERSIONE CHE USA VTKEdge, per ora commentata
/*
    vtkDebugMacro("Texture3D not supported. Switching to GPU Ray Tracing");
    mapper->Delete();

    vtkKWEGPUVolumeRayCastMapper *mapper = vtkKWEGPUVolumeRayCastMapper::New();
    mapper->SetInputConnection(rescaler->GetOutputPort());
    vtkKWEGPUVolumeRayCastMapper::SafeDownCast(mapper)->SetBlendModeToComposite();

    this->SetMapper(mapper);

    // Check for support
    if( ! (vtkKWEGPUVolumeRayCastMapper::SafeDownCast(mapper)->IsRenderSupported(renwin, property)) )
    {
      vtkDebugMacro("GPU Ray Tracing not supported. Switching to Software Ray Tracing");
      mapper->Delete();

      vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
      mapper = vtkVolumeRayCastMapper::New();
      vtkVolumeRayCastMapper::SafeDownCast(mapper)->SetVolumeRayCastFunction(compositeFunction);

      mapper->SetInputConnection(rescaler->GetOutputPort());
      this->SetMapper(mapper);

      compositeFunction->Delete();
*/
  }
#endif //0
  mapper->Delete();
  rescaler->Delete();
}


void Volume3DActor::SetFloat()
{
  m_Color->RemoveAllPoints();
  m_Color->AddRGBPoint(0.0,   0.0, 0.0, 0.0);
  m_Color->AddRGBPoint(0.25,  0.5, 0.5, 0.5);
  m_Color->AddRGBPoint(0.5,   0.7, 0.7, 0.7);
  m_Color->AddRGBPoint(0.999, 1.0, 1.0, 1.0);
  m_Color->AddRGBPoint(1.0,   0.0, 0.0, 0.0);

  m_Opacity->RemoveAllPoints();
  m_Opacity->AddPoint(0.0,   0.0);
  m_Opacity->AddPoint(0.25,  0.15);
  m_Opacity->AddPoint(0.999, 0.02);
  m_Opacity->AddPoint(1.0,   0.0);
}

void Volume3DActor::SetUnsignedChar()
{
  m_Color->RemoveAllPoints();
  m_Color->AddRGBPoint(0u,    0.0, 0.0, 0.0);
  m_Color->AddRGBPoint(0x3fu, 0.5, 0.5, 0.5);
  m_Color->AddRGBPoint(0x7fu, 0.7, 0.7, 0.7);
  m_Color->AddRGBPoint(0xfeu, 1.0, 1.0, 1.0);
  m_Color->AddRGBPoint(0xffu, 0.0, 0.0, 0.0);

  m_Opacity->RemoveAllPoints();
  m_Opacity->AddPoint(0u,    0.0);
  m_Opacity->AddPoint(0x3fu, 0.15);
  m_Opacity->AddPoint(0xfeu, 0.02);
  m_Opacity->AddPoint(0xffu, 0.0);
}
void Volume3DActor::SetUnsigned12Bit()
{
  m_Color->RemoveAllPoints();
  m_Color->AddRGBPoint(0u,      0.0, 0.0, 0.0);
  m_Color->AddRGBPoint(0x03ffu, 0.5, 0.5, 0.5);
  m_Color->AddRGBPoint(0x07ffu, 0.7, 0.7, 0.7);
  m_Color->AddRGBPoint(0x0ffeu, 1.0, 1.0, 1.0);
  m_Color->AddRGBPoint(0x0fffu, 0.0, 0.0, 0.0);

  m_Opacity->RemoveAllPoints();
  m_Opacity->AddPoint(0u,       0.0);
  m_Opacity->AddPoint(0x03ffu,  0.15);
  m_Opacity->AddPoint(0x0ffeu,  0.02);
  m_Opacity->AddPoint(0x0fffu,  0.0);
}

void Volume3DActor::SetUnsignedShort()
{
  m_Color->RemoveAllPoints();
  m_Color->AddRGBPoint(0u,      0.0, 0.0, 0.0);
  m_Color->AddRGBPoint(0x3fffu, 0.5, 0.5, 0.5);
  m_Color->AddRGBPoint(0x7fffu, 0.7, 0.7, 0.7);
  m_Color->AddRGBPoint(0xfffeu, 1.0, 1.0, 1.0);
  m_Color->AddRGBPoint(0xffffu, 0.0, 0.0, 0.0);

  m_Opacity->RemoveAllPoints();
  m_Opacity->AddPoint(0u,       0.0);
  m_Opacity->AddPoint(0x3fffu,  0.15);
  m_Opacity->AddPoint(0xfffeu,  0.02);
  m_Opacity->AddPoint(0xffffu,  0.0);
}


