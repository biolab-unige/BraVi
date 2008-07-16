//Filename: vtkNiftiImageReader.h
//Author: Daniele Domenichelli

#ifndef VTKNIFTIIMAGEREADER_H
#define VTKNIFTIIMAGEREADER_H


#include <vtkImageReader2.h>

#include "config.h"


class VTK_IO_EXPORT vtkNiftiImageReader : public vtkImageReader2
{
public:
    static vtkNiftiImageReader *New();
    vtkTypeRevisionMacro(vtkNiftiImageReader,vtkImageReader2);
    void PrintSelf(ostream& os, vtkIndent indent);

    inline int CanReadFile(const char* fname) {return 3;} /// \todo controllare se effettivamente può leggere il file

    void SetFileName(const char* fn);

    inline void GetOrientation(int& orient0, int& orient1, int& orient2) {orient0 = m_Orientation[0];
                                                                          orient1 = m_Orientation[1];
                                                                          orient2 = m_Orientation[2];}

protected:
    vtkNiftiImageReader();
    ~vtkNiftiImageReader();


private:

    int m_Orientation[3];
}; // class vtkNiftiImageReader


#endif // VTKNIFTIIMAGEREADER_H
