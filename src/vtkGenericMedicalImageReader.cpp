//
// C++ Implementation: vtkGenericMedicalImageReader
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#define DEBUG


#include "vtkGenericMedicalImageReader.h"

#include <vtkObjectFactory.h>
#include <vtkDataObject.h>
#include <vtkImageData.h>
#include <vtkImageReader2.h>
#include <vtkMedicalImageProperties.h>
#include <vtkImagePermute.h>

#include <vtkDICOMImageReader.h>
#include <vtkMetaImageReader.h>
#include "vtkNiftiImageReader.h"

void ciccio2(int* orient);

vtkCxxRevisionMacro(vtkGenericMedicalImageReader, "$Revision: 1.7 $");
vtkStandardNewMacro(vtkGenericMedicalImageReader);


static const char* vtkGenericMedicalImageReaderImageTypeString[] = {
  "UNKNOWN_IMAGE (Unsupported)",
  "DICOM_IMAGE",
  "DICOM_GDCM_IMAGE (Unsupported)",
  "ANALYZE_IMAGE (Unsupported)",
  "NIFTI_IMAGE",
  "RAW_IMAGE (Unsupported)",
  "META_IMAGE",
  "PHILIPS_PAR_REC_IMAGE (Unsupported)",
  NULL
};


vtkGenericMedicalImageReader::vtkGenericMedicalImageReader() : vtkMedicalImageReader2(),
                                                               FileName(NULL),
                                                               DirectoryName(NULL),
                                                               ImageType(UNKNOWN_IMAGE),
                                                               m_Reader(NULL),
                                                               m_Orient2RAS(NULL)
{
}


vtkGenericMedicalImageReader::~vtkGenericMedicalImageReader()
{
  if(this->FileName)
    this->SetFileName(0);
  if(this->DirectoryName)
    this->SetDirectoryName(0);
  if(this->m_Orient2RAS)
    this->m_Orient2RAS->Delete();
  if(this->m_Reader)
    this->m_Reader->Delete();
}


void vtkGenericMedicalImageReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "FileType: " << GetDescriptiveName() << std::endl;
  os << indent << "FileName: " << (this->FileName ? this->FileName : "(none)") << std::endl;
  os << indent << "Directory: " << (this->DirectoryName ? this->DirectoryName : "(none)") << std::endl;
}



const char* vtkGenericMedicalImageReader::GetDescriptiveName()
{
  return vtkGenericMedicalImageReaderImageTypeString[ImageType];
}


void vtkGenericMedicalImageReader::ExecuteInformation()
{
  this->vtkMedicalImageReader2::ExecuteInformation();

  // Checks if a filename or a directory was specified
  if(!this->FileName && !this->DirectoryName)
  {
    vtkErrorMacro(<< this->GetClassName() << ": A filename or a directory must be specified.");
    return;
  }

  // Checks if an image type was specified
  if(!this->ImageType)
  {
    vtkErrorMacro(<< this->GetClassName() << ": An image type must be specified.");
    return;
  }

  // Creates reader
  switch (this->ImageType)
  {
    case DICOM_IMAGE:
      vtkDebugMacro(<< this->GetClassName() << ": Using vtkDICOMImageReader");
      this->m_Reader = vtkDICOMImageReader::New();
      break;
    case NIFTI_IMAGE:
      m_Reader = vtkNiftiImageReader::New();
      vtkDebugMacro(<< this->GetClassName() << ": Using vtkNiftiImageReader");
      break;
    case META_IMAGE:
      vtkDebugMacro(<< this->GetClassName() << ": Using vtkMetaImageReader");
      m_Reader = vtkMetaImageReader::New();
      break;
    case ANALYZE_IMAGE:
    case DICOM_GDCM_IMAGE:
    case RAW_IMAGE:
    case PHILIPS_PAR_REC_IMAGE:
      vtkErrorMacro(<< this->GetClassName() << ": Not yet supported.");
      return;
    case UNKNOWN_IMAGE:
    default:
      vtkErrorMacro(<< this->GetClassName() << ": Unknown error.");
      return;
  }

  // Checks FileName and DirectoryName
  switch (this->ImageType)
  {
    case DICOM_IMAGE:
      if(this->DirectoryName)
      {
        if (this->FileName)
          vtkGenericWarningMacro(<< this->GetClassName() << ": Both Filename and Directory set. Using Directory.");
        vtkDebugMacro(<< this->GetClassName() << ": Setting Directory to " << (this->DirectoryName ? this->DirectoryName : "(none)") );
        static_cast<vtkDICOMImageReader*>(m_Reader)->SetDirectoryName(DirectoryName);
      }
      else
      {
        vtkDebugMacro(<< this->GetClassName() << ": Setting FileName to " << (this->FileName ? this->FileName : "(none)") );
        this->m_Reader->SetFileName ( FileName );
      }
      break;
    case NIFTI_IMAGE:
    case META_IMAGE:
      if(this->DirectoryName)
      {
        if (this->FileName)
        {
          vtkGenericWarningMacro(<< this->GetClassName() << ": Both Filename and Directory set. Using FileName.");
        }
        else
        {
          vtkErrorMacro(<< this->GetClassName() << ": Using Directory is not supported for this file type. Use SetFileName.");
          return;
        }
      }
      vtkDebugMacro(<< this->GetClassName() << ": Setting FileName to " << (this->FileName ? this->FileName : "(none)") );
      this->m_Reader->SetFileName ( FileName );
      break;
    case ANALYZE_IMAGE:
    case DICOM_GDCM_IMAGE:
    case RAW_IMAGE:
    case PHILIPS_PAR_REC_IMAGE:
    case UNKNOWN_IMAGE:
    default:
      vtkErrorMacro(<< this->GetClassName() << ": Unknown error.");
      return;
  }

  vtkDebugMacro(<< this->GetClassName() << ": Setting reader release data flag on." );
  this->m_Reader->ReleaseDataFlagOn();

  vtkDebugMacro(<< this->GetClassName() << ": Updating reader informations." );
  this->m_Reader->UpdateInformation();

  vtkAcquisitionOrientation orient;

  // Orienting image
  switch (this->ImageType)
  {
    case DICOM_IMAGE:
    {
      vtkDICOMImageReader* dicomReader = vtkDICOMImageReader::SafeDownCast(m_Reader);

      float vec[3];
      float* orientation = dicomReader->GetImageOrientationPatient();

      vec[0] = fabs( orientation[1]*orientation[5] - orientation[2]*orientation[4] );
      vec[1] = fabs( orientation[0]*orientation[5] - orientation[2]*orientation[3] );
      vec[2] = fabs( orientation[0]*orientation[4] - orientation[3]*orientation[1] );

      if      (vec[0] > vec[1] && vec[0] > vec[2]) orient = SAGITTAL;
      else if (vec[1] > vec[0] && vec[1] > vec[2]) orient = CORONAL;
      else if (vec[2] > vec[0] && vec[2] > vec[1]) orient = AXIAL;
      else                                         orient = UNKNOWN;
    }
      break;

    case NIFTI_IMAGE:
    {
      vtkNiftiImageReader* niftiReader = vtkNiftiImageReader::SafeDownCast(m_Reader);
      int orientation[3];
      niftiReader->GetOrientation(orientation[0], orientation[1], orientation[2]);

      ciccio2(orientation);

      if      (orientation[0]==VTK_L2R && orientation[1]==VTK_P2A && orientation[2]==VTK_I2S) orient = RAS;          //RAS
      else if (orientation[0]==VTK_R2L && orientation[1]==VTK_P2A && orientation[2]==VTK_I2S) orient = LAS;          //LAS
      else if (orientation[0]==VTK_R2L && orientation[1]==VTK_S2I && orientation[2]==VTK_P2A) orient = LIA;          //LIA
      else if (orientation[0]==VTK_A2P && orientation[1]==VTK_S2I && orientation[2]==VTK_R2L) orient = PIL;          //PIL
      else if (orientation[0]==VTK_R2L && orientation[1]==VTK_A2P && orientation[2]==VTK_S2I) orient = LPI;          //LPI
      else if (orientation[0]==VTK_R2L && orientation[1]==VTK_A2P && orientation[2]==VTK_I2S) orient = LPS;          //LPS
      else
      {
        std::cerr << "WARNING, orientation could be wrong (" << std::flush;
        for(int i=0; i<3; i++)
        {
          switch (orientation[i])
          {
            case VTK_L2R:
              std::cerr << "R" << std::flush;
              break;
            case VTK_R2L:
              std::cerr << "L" << std::flush;
              break;
            case VTK_P2A:
              std::cerr << "A" << std::flush;
              break;
            case VTK_A2P:
              std::cerr << "P" << std::flush;
              break;
            case VTK_I2S:
              std::cerr << "S" << std::flush;
              break;
            case VTK_S2I:
              std::cerr << "I" << std::flush;
              break;
            default:
              std::cerr << "?" << std::flush;
              break;
          }
        }
        std::cerr << ")" << std::endl;
        orient = UNKNOWN;
      }
    }
      break;

    case META_IMAGE:
    {
//      vtkMetaImageReader* metaReader = vtkMetaImageReader::SafeDownCast(m_Reader);
      std::cerr << "WARNING, orientation could be wrong" << std::flush;
      orient = UNKNOWN;
    }

    default:
      orient = UNKNOWN;
      break;
  }

  m_Orient2RAS = vtkImagePermute::New();
  m_Orient2RAS->SetInputConnection(m_Reader->GetOutputPort());

  if(orient == RAS)
  {
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to RAS." );
    m_Orient2RAS->SetResliceAxesDirectionCosines(  1,  0,  0,
                                                   0,  1,  0,
                                                   0,  0,  1 );
  }
  if(orient == LAS)
  {
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to LAS." );
    m_Orient2RAS->SetResliceAxesDirectionCosines( -1,  0,  0,
                                                   0,  1,  0,
                                                   0,  0,  1 );
  }
  // Axial Acquisition
  else if(orient == AXIAL)
  {
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to AXIAL." );
    m_Orient2RAS->SetResliceAxesDirectionCosines(  1,  0,  0,
                                                   0,  1,  0,
                                                   0,  0, -1 );
  }
  else if(orient == LPI)
  {
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to LPI." );
    m_Orient2RAS->SetResliceAxesDirectionCosines( -1,  0,  0,
                                                   0, -1,  0,
                                                   0,  0, -1 );
  }
/*  else if(orient == LPS)
  {
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to LPS." );
    std::cerr << "WARNING, orientation could be wrong (LPS)" << std::endl;
    m_Orient2RAS->SetResliceAxesDirectionCosines( -1,  0,  0,
                                                   0,  0,  1,
                                                   0, -1,  0 );
  }*/
// Sagittal Acquisition
  else if(orient == SAGITTAL)
  {
    m_Orient2RAS->SetResliceAxesDirectionCosines(  0,  0, -1,
                                                  -1,  0,  0,
                                                   0,  1,  0 );
  vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to SAGITTAL." );
  }
// Sagittal Acquisition
  else if(orient == PIL)
  {
    m_Orient2RAS->SetResliceAxesDirectionCosines(  0,  0, -1,
                                                  -1,  0,  0,
                                                   0, -1,  0 );
  vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to PIL." ); //teoricamente (0,-1,0,-1,0,0,0,1,0)
  }
// Coronal Acquisition
  else if(orient == CORONAL)
  {
    m_Orient2RAS->SetResliceAxesDirectionCosines( -1,  0,  0,
                                                   0,  0, -1,
                                                   0,  1,  0 );
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to CORONAL." );
  }
  else if(orient == LIA)
  {
    m_Orient2RAS->SetResliceAxesDirectionCosines( -1,  0,  0,
                                                   0,  0,  1,
                                                   0, -1,  0 );
    vtkDebugMacro(<< this->GetClassName() << ": Setting Orient to LIA." );
  }
// Unknown Acquisition
  else if(orient == UNKNOWN)
  {
    vtkDebugMacro(<< this->GetClassName() << ": Orientation UNKNOWN! Doing nothing." );
    m_Orient2RAS->SetResliceAxesDirectionCosines(  1,  0,  0,
                                                   0,  1,  0,
                                                   0,  0,  1 );
    vtkDebugMacro(<< this->GetClassName() << ": Orientation UNKNOWN! Doing nothing." );
  }

  vtkDebugMacro(<< this->GetClassName() << ": Updating orientation informations." );
  this->m_Orient2RAS->Update();

  this->SetDataExtent(this->m_Orient2RAS->GetOutput()->GetExtent());
  this->SetFileDimensionality(*(this->m_Orient2RAS->GetOutput()->GetDimensions()));
  this->SetDataScalarType(this->m_Orient2RAS->GetOutput()->GetScalarType());
  this->SetDataSpacing(this->m_Orient2RAS->GetOutput()->GetSpacing());
  this->SetNumberOfScalarComponents(this->m_Orient2RAS->GetOutput()->GetNumberOfScalarComponents());
  this->SetDataByteOrder(this->m_Reader->GetDataByteOrder());
  this->SetFileLowerLeft(this->m_Reader->GetFileLowerLeft());
  this->SetSwapBytes(this->m_Reader->GetSwapBytes());
  this->SetDataOrigin(this->m_Orient2RAS->GetOutput()->GetOrigin());
}

/** \todo CanReadFile
  if (!m_Reader->CanReadFile(filename.c_str()))
  {
    vtkErrorMacro(<< this->GetClassName() << ": Can't Read File.");
    m_Reader->Delete();
    return;
  }
*/


void vtkGenericMedicalImageReader::ExecuteData(vtkDataObject* output)
{
  if (!this->m_Reader)
    return;

  vtkDebugMacro(<< this->GetClassName() << ": Updating reader" );
  this->m_Reader->Update();

  m_Reader->Update();
  m_Orient2RAS->Update();
  vtkImageData *outputImg = vtkImageData::SafeDownCast(output);
  outputImg->ShallowCopy(m_Orient2RAS->GetOutput());

#ifdef DEBUG
//stampa le informazioni sull'immagine
  outputImg->Print(std::cout);
#endif
}

//UpdateProgress
//        self->UpdateProgress(count/(50.0*target));



void vtkGenericMedicalImageReader::SetInformationDICOM(vtkImageReader2* reader)
{
  vtkDebugMacro(<< this->GetClassName() << ": Setting DICOM Information" );
  vtkDICOMImageReader* dicomReader = vtkDICOMImageReader::SafeDownCast(reader);
  if (!dicomReader)
  {
    vtkErrorMacro(<< this->GetClassName() << ": Using SetInformationDicom without a vtkDICOMImageReader.");
    return;
  }

  vtkMedicalImageProperties *properties = this->GetMedicalImageProperties();

  // Patient Name
  properties->SetPatientName(dicomReader->GetPatientName());

  // Patient ID
  //properties->SetPatientID();

  // Patient Age
  //properties->SetPatientAge();

  // Patient Sex
  //properties->SetPatientSex();

  // Patient Birth Date
  //properties->SetPatientBirthDate();

  // Acquisition Date
  //properties->SetAcquisitionDate();

  // Acquisition Time
  //properties->SetAcquisitionTime();

  // Image Date
  //properties->SetImageDate();

  // Image Time
  //properties->SetImageTime();

  // Image Number
  //properties->SetImageNumber();

  // Series Number
  //properties->SetSeriesNumber();

  // Series Description
  //properties->SetSeriesDescription();

  // Study ID
  properties->SetStudyID(dicomReader->GetStudyID());

  // Study Description
  //properties->SetStudyDescription();

  // Modality
  //properties->SetModality();

  // Manufacturer
  //properties->SetManufacturer();

  // Manufacturer Model Name
  //properties->SetManufacturerModelName();

  // Station Name
  //properties->SetStationName();

  // Istitution Name
  //properties->SetIstitutionName();

  // Convolution Kernel
  //properties->SetConvolutionKernel();

  // Slice Thickness
  //properties->SetSliceThickness();

  // KVP (Peak kilo voltage otuput of the (x-ray) generator used
  //properties->SetFVP();

  //Gantry/Detector Tilt
  //properties->SetGantryTilt();

  //Echo time
  //properties->GetEchoTime();

  //Echo Train Length
  //properties->GetEchoTrainLength();

  //Repetition Time
  //properties->GetRepetitionTime();

  //Exposure Time
  //properties->GetExposureTime();

  //X-ray Tube Current
  //properties->XRayTubeCurrent();

  //Exposure
  //properties->GetExposure();

/*
  if(this->ImageType ==DICOM_IMAGE)
  {
    vtkDICOMImageReader* ciccio = vtkDICOMImageReader::SafeDownCast(m_Reader);

    const char* name = ciccio->GetPatientName();
    std::cerr << "0010,0010,Patient Name = " << name << std::endl;

    float* pippo=ciccio->GetImagePositionPatient();
    std::cerr << "0020,0032,Image Position Patient = "; 
    for (int i=0; i<3;i++)
      std::cerr << pippo[i] << " ";
    std::cerr << std::endl;

    float* poldo=ciccio->GetImageOrientationPatient();
    std::cerr << "0020,0037,Image Orientation (Patient) = "; 
    for (int i=0; i<6;i++)
      std::cerr << poldo[i] << " ";
    std::cerr << std::endl;

    double* gigi=ciccio->GetPixelSpacing();
    for (int i=0; i<3;i++)
      std::cerr << gigi[i] << " ";
    std::cerr << std::endl;
    }
*/
}

void ciccio2(int* orient)
{
  int matr[9];

  for (int i=0; i<3; i++)
  {
    switch (orient[i])
    {
      case VTK_L2R:
        matr[i*3]     =  1;
        matr[i*3 + 1] =  0;
        matr[i*3 + 2] =  0;
        break;
      case VTK_R2L:
        matr[i*3]     = -1;
        matr[i*3 + 1] =  0;
        matr[i*3 + 2] =  0;
        break;
      case VTK_P2A:
        matr[i*3]     =  0;
        matr[i*3 + 1] =  1;
        matr[i*3 + 2] =  0;
        break;
      case VTK_A2P:
        matr[i*3]     =  0;
        matr[i*3 + 1] = -1;
        matr[i*3 + 2] =  0;
        break;
      case VTK_I2S:
        matr[i*3]     =  0;
        matr[i*3 + 1] =  0;
        matr[i*3 + 2] =  1;
        break;
      case VTK_S2I:
        matr[i*3]     =  0;
        matr[i*3 + 1] =  0;
        matr[i*3 + 2] = -1;
        break;
      default:
        break;
    }
  }
  for (int i=0; i<9; i++)
  {
    std::cout << matr[i] << " " << std::flush;
    if (!((i+1)%3)) std::cout << std::endl;
  }
  std::cout << std::endl;
}
