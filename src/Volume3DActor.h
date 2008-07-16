//
// C++ Interface: Volume3DActor
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOLUME3DACTOR_H
#define VOLUME3DACTOR_H

#include <vtkVolume.h>

class vtkColorTransferFunction;
class vtkPiecewiseFunction;
class vtkVolumeProperty;
class vtkImageData;

class VTK_RENDERING_EXPORT Volume3DActor : public vtkVolume
{
public:
    vtkTypeRevisionMacro(Volume3DActor,vtkVolume);
    void PrintSelf(ostream& os, vtkIndent indent);
    static Volume3DActor* New(void);

    void SetInput(vtkImageData* input);

    void SetFloat();
    void SetUnsignedChar();
    void SetUnsigned12Bit();
    void SetUnsignedShort();

protected:
    Volume3DActor();
    ~Volume3DActor();

private:
    vtkColorTransferFunction* m_Color;
    vtkPiecewiseFunction* m_Opacity;
    vtkVolumeProperty* m_VolumeProperty;

};


#endif //VOLUME3DACTOR_H
