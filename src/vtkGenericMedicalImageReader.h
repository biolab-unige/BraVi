//
// C++ Interface: vtkGenericMedicalImageReader
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef VTKGENERICMEDICALIMAGEREADER_H
#define VTKGENERICMEDICALIMAGEREADER_H



#include <vtkMedicalImageReader2.h>

#include "config.h"

class vtkImageReader2;
class vtkImagePermute;

enum vtkImageType
{
  UNKNOWN_IMAGE = 0,
  DICOM_IMAGE,
  DICOM_GDCM_IMAGE,
  ANALYZE_IMAGE,
  NIFTI_IMAGE,
  RAW_IMAGE,
  META_IMAGE,
  PHILIPS_PAR_REC_IMAGE,
};

/// @todo Inserirlo in un header importato da vtkNiftiImageReader e confrontarli con orientazioni nifti
enum vtkOrientation
{
  VTK_UNKNOWN_ORIENTATION = 0,
  VTK_L2R = 1,
  VTK_R2L = 2,
  VTK_P2A = 3,
  VTK_A2P = 4,
  VTK_I2S = 5,
  VTK_S2I = 6
};

enum vtkAcquisitionOrientation
{
  UNKNOWN = 0,
  RAS,
  LAS,
  LPI,
  PIL,
  LIA,
  LPS,
  AXIAL,
  SAGITTAL,
  CORONAL
};

/**
 * \brief Read any (supported) file.
 * Output image is always RAS
 */
class VTK_IO_EXPORT vtkGenericMedicalImageReader : public vtkMedicalImageReader2
{
public:
  static vtkGenericMedicalImageReader* New();
  vtkTypeRevisionMacro(vtkGenericMedicalImageReader, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

/// \brief Set/Get the type of the image to read
  vtkSetMacro(ImageType, vtkImageType);
  vtkGetMacro(ImageType, vtkImageType);

/// \brief Set/Get the name of the file to read
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

/// \brief Set/Get the name of the directory to read
  vtkSetStringMacro(DirectoryName);
  vtkGetStringMacro(DirectoryName);

/// \brief Returns a string with a space separated list of extensions in the format .extension
  virtual const char* GetFileExtensions()
  {
    return ".dcm .hdr .nii .mhd";
  }

/// \brief Return a descriptive name for the file format that might be useful in a GUI.
  virtual const char* GetDescriptiveName();

/**
 * \brief Return non zero if the reader can read the given file name.
 * For non zero return values the following values are to be used
 * 1 - I think I can read the file but I cannot prove it
 * 2 - I definitely can read the file
 * 3 - I can read the file and I have validated that I am the correct reader for this file
 * \bug Per ora ritorna sempre 3
 * \todo scrivere questa funzione
*/
  virtual inline int CanReadFile(const char* fname) {return 3;}

protected:
  vtkGenericMedicalImageReader();
  ~vtkGenericMedicalImageReader();

  char* FileName;
  char* DirectoryName;
  vtkImageType ImageType;

/// \brief Updates extents, etc.
  virtual void ExecuteInformation();

/// \brief Reads data
  virtual void ExecuteData(vtkDataObject* output);

private:
  vtkGenericMedicalImageReader(const vtkGenericMedicalImageReader&); // Not implemented
  void operator=(const vtkGenericMedicalImageReader); // Not Implemented

  vtkImageReader2* m_Reader;
  vtkImagePermute* m_Orient2RAS;

  void SetInformationDICOM(vtkImageReader2* reader);


};







#endif // VTKGENERICMEDICALIMAGEREADER_H
