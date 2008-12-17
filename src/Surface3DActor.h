//
// C++ Interface: Surface3DActor
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SURFACE3DACTOR_H
#define SURFACE3DACTOR_H


#if defined(VTK_USE_MANGLED_MESA)
    #include <vtkMesaActor.h>
#else
    #include <vtkOpenGLActor.h>
#endif



#include <vtkImageData.h>
#include <vtkMarchingCubes.h>




#if defined(VTK_USE_MANGLED_MESA)
class VTK_RENDERING_EXPORT Surface3DActor : public vtkMesaActor
#else
class VTK_RENDERING_EXPORT Surface3DActor : public vtkOpenGLActor
#endif
{
public:
#if defined(VTK_USE_MANGLED_MESA)
    vtkTypeRevisionMacro(Surface3DActor,vtkMesaActor);
#else
    vtkTypeRevisionMacro(Surface3DActor,vtkOpenGLActor);
#endif

    void PrintSelf(ostream& os, vtkIndent indent);
    static Surface3DActor* New(void);

    void SetInput(vtkImageData* input);

    inline double getMinSurfaceValue() const {return m_Range[0];}
    inline double getMaxSurfaceValue() const {return m_Range[1];}
    inline double getSurfaceValue() const {return m_MarchingCubes->GetValue(0);}

    void setSurface(int surf);

    enum ColorType {COLOR_RED = 0, COLOR_GREEN, COLOR_BLUE, COLOR_METAL, COLOR_SKIN, COLOR_GLASS};
    void setColor(ColorType cType);
    void setCustomColor(float color0, float color1, float color2, float specular, float specularPower);




private:
    Surface3DActor();
    ~Surface3DActor();

    vtkMarchingCubes* m_MarchingCubes;
    double m_Range[2];
};

#endif //SURFACE3DACTOR_H
