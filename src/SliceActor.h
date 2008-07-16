//
// C++ Interface: SliceActor
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SLICEACTOR_H
#define SLICEACTOR_H


#include "config.h"

#if defined(VTK_USE_MANGLED_MESA)
    #include <vtkMesaImageActor.h>
#else
    #include <vtkOpenGLImageActor.h>
#endif

class vtkImageData;
class vtkMatrix4x4;
class vtkImageReslice;
class vtkLookupTable;


#ifdef SV_NEUROLOGICALCONVENTION
//Neurological Convention
static const double axialElements[16] =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1 
};
#else
//Radiological Convention
static const double axialElements[16] =
{
   -1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1 
};
#endif // SV_NEUROLOGICALCONVENTION

static const double sagittalElements[16] =
{
    0, 0,-1, 0,
   -1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 1
};

#ifdef SV_NEUROLOGICALCONVENTION
//Neurologic Convention
static const double coronalElements[16] =
{
    1, 0, 0, 0,
    0, 0,-1, 0,
    0, 1, 0, 0,
    0, 0, 0, 1 
};
#else
static const double coronalElements[16] =
{
   -1, 0, 0, 0,
    0, 0,-1, 0,
    0, 1, 0, 0,
    0, 0, 0, 1 
};
#endif // SV_NEUROLOGICALCONVENTION



#if defined(VTK_USE_MANGLED_MESA)
class VTK_RENDERING_EXPORT SliceActor : public vtkMesaImageActor
#else
class VTK_RENDERING_EXPORT SliceActor : public vtkOpenGLImageActor
#endif
{
public:
#if defined(VTK_USE_MANGLED_MESA)
    vtkTypeRevisionMacro(SliceActor,vtkMesaImageActor);
#else
    vtkTypeRevisionMacro(SliceActor,vtkOpenGLImageActor);
#endif

    void PrintSelf(ostream& os, vtkIndent indent);
    static SliceActor* New(void);

    void SetInput(vtkImageData* input);
    void setMatrix(vtkMatrix4x4* matr);

    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setAngleAlpha(int alpha);
    void setAngleBeta(int beta);
    void setAngleGamma(int gamma);


    vtkMatrix4x4* getResliceAxes(void) {return m_ResliceAxes;}
    vtkImageReslice* getImageReslice(void) {return m_ImageReslice;}

    int getMinX(void) {return m_extent[0];}
    int getMaxX(void) {return m_extent[1];}
    int getMinY(void) {return m_extent[2];}
    int getMaxY(void) {return m_extent[3];}
    int getMinZ(void) {return m_extent[4];}
    int getMaxZ(void) {return m_extent[5];}

    int getCenterX(void) {return m_center[0];}
    int getCenterY(void) {return m_center[1];}
    int getCenterZ(void) {return m_center[2];}

    double getX(void) {return m_X;}
    double getY(void) {return m_Y;}
    double getZ(void) {return m_Z;}

    void SetRange(double min, double max);
    void SetConventionRadiological();
    void SetConventionNeurological();

protected:
    SliceActor();
    ~SliceActor();

private:
    vtkMatrix4x4* m_ResliceAxes; ///direzione degli assi e centro
    vtkLookupTable* m_LookupTable;
    vtkImageReslice* m_ImageReslice;

    double m_limit[6];
    double m_spacing[3];
    double m_origin[3];

    int m_extent[6];
    int m_center[3];

    double m_X;
    double m_Y;
    double m_Z;

    vtkMatrix4x4* m_Alpha;
    vtkMatrix4x4* m_Beta;
    vtkMatrix4x4* m_Gamma;

    void UpdateRotation();
};



#endif //SLICEACTOR_H
