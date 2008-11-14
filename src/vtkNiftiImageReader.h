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
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  /*!
   * \brief Is the given file a NIFTI1 file?
   */
  int CanReadFile(const char* fname);

  /*! Get the file extensions for this format.
   * \brief Returns a string with a space separated list of extensions in the format .extension
   */
  virtual const char* GetFileExtensions()
  {
    return ".nii .hdr .nii.gz";
  }

  /*!
   * Return a descriptive name for the file format that might be useful in a GUI.
   */
  virtual const char* GetDescriptiveName()
    {
      return "NIFTI1";
    }

protected:
  vtkNiftiImageReader();
  ~vtkNiftiImageReader();

  virtual void ExecuteInformation();
  virtual void ExecuteData(vtkDataObject *out);

private:
  vtkNiftiImageReader(const vtkNiftiImageReader&);  // Not implemented.
  void operator=(const vtkNiftiImageReader&);  // Not implemented.


//TODO Tutto quello che c'è qua sotto dovrà un giorno essere eliminato
//BEGIN CUT
public:
//    void SetFileName(const char* fn);
    inline void GetOrientation(int& orient0, int& orient1, int& orient2) {orient0 = m_Orientation[0];
                                                                          orient1 = m_Orientation[1];
                                                                          orient2 = m_Orientation[2];}
private:
    int m_Orientation[3];
//END CUT

}; // class vtkNiftiImageReader


#endif // VTKNIFTIIMAGEREADER_H
