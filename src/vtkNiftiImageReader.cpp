//filename: vtkNiftiImageReader.cpp
//author: Daniele Domenichelli

#include "vtkNiftiImageReader.h"

#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>

#include <nifti1_io.h>
#include <string>


#define LSB_FIRST 1
#define MSB_FIRST 2


//#define DEBUG_ALL


vtkCxxRevisionMacro(vtkNiftiImageReader, "$Revision: 2.0 $");
vtkStandardNewMacro(vtkNiftiImageReader);

vtkNiftiImageReader::vtkNiftiImageReader()
{
}

vtkNiftiImageReader::~vtkNiftiImageReader()
{
}

void vtkNiftiImageReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

int vtkNiftiImageReader::CanReadFile(const char* fname)
{
  if (is_nifti_file(fname) < 0)
    return 0;
  return 3;
}




void vtkNiftiImageReader::ExecuteInformation()
{
  this->ComputeInternalFileName(this->DataExtent[4]);
  if (this->InternalFileName == NULL)
  {
    return;
  }

  nifti_image *nim;

#ifdef DEBUG_ALL
  nifti_image_infodump(nim);
#endif //DEBUG_ALL


  nim = nifti_image_read(this->GetInternalFileName(), 0); //0 legge solo l'header 1 legge anche i dati

  if (!nim)
  {
    vtkErrorWithObjectMacro(this,
                            "niftilib could not read file: "
                            << this->InternalFileName);
    return;
  }

  //TODO support images of all dimensions
  if (nim->ndim != 3 && !(nim->ndim == 4 && nim->nt == 1))
  {
    vtkErrorWithObjectMacro(this,
                            "Supported only 3D images: "
                            << this->InternalFileName);
    return;
  }

//Set dimension, position, voxel spacing and origin

  this->SetFileDimensionality(3);
  this->SetDataExtent(0,(nim->nx)-1, 0,(nim->ny)-1, 0,(nim->nz)-1);
  this->SetDataSpacing(nim->dx, nim->dy, nim->dz);       // voxel spacing 
  //TODO Check if origin should be in the center of voxel or in [0,0,0] ( this->SetDataOrigin(0.0, 0.0, 0.0); )
  //TODO Check if position in space could be found in nifti header
  this->SetDataOrigin(nim->dx/2.0, nim->dy/2.0, nim->dy/2.0); // Set origin in the center of the first voxel

//Set datatype
  switch (nim->datatype)
  {
    //case DT_UNSIGNED_CHAR:                          // unsigned char (8 bits/voxel)
    case DT_UINT8:                                  // unsigned char (8 bits/voxel)
      this->SetDataScalarTypeToUnsignedChar();
      this->SetNumberOfScalarComponents(1);
      break;
    //case DT_SIGNED_SHORT:                           // signed short (16 bits/voxel)
    case DT_INT16:                                  // signed short (16 bits/voxel)
      this->SetDataScalarTypeToShort();
      this->SetNumberOfScalarComponents(1);
      break;
    case DT_UINT16:                                 // unsigned short (16 bits)
      this->SetDataScalarTypeToUnsignedShort();
      this->SetNumberOfScalarComponents(1);
      break;
    //case DT_RGB:                                    // RGB triple (24 bits/voxel)
    case DT_RGB24:                                  // RGB triple (24 bits/voxel)
      vtkWarningWithObjectMacro( this,
                                 "dataset = DT_RGB not tested yet: "
                                 << this->InternalFileName );
      this->SetDataScalarTypeToUnsignedChar();
      this->SetNumberOfScalarComponents(3);
      break;
    //case DT_SIGNED_INT:                             // signed int (32 bits/voxel)
    case DT_INT32:                                  // signed int (32 bits/voxel)
      this->SetDataScalarTypeToInt();
      this->SetNumberOfScalarComponents(1);
      break;
    //case DT_FLOAT:                                  // float (32 bits/voxel)
    case DT_FLOAT32:                                // float (32 bits/voxel)
      this->SetDataScalarTypeToFloat();
      this->SetNumberOfScalarComponents(1);
      break;
    //case DT_DOUBLE:                                 // double (64 bits/voxel)
    case DT_FLOAT64:                                // double (64 bits/voxel)
      this->SetDataScalarTypeToDouble();
      this->SetNumberOfScalarComponents(1);
      break;
    case DT_ALL:
    case DT_BINARY:                                 // binary (1 bit/voxel)
    case DT_INT8:                                   // signed char (8 bits)
    case DT_UINT32:                                 // unsigned int (32 bits)
    //case DT_COMPLEX:                                // complex (64 bits/voxel)
    case DT_COMPLEX64:                              // complex (64 bits/voxel)
    case DT_INT64:                                  // long long (64 bits)
    case DT_UINT64:                                 // unsigned long long (64 bits)
    case DT_FLOAT128:                               // long double (128 bits)
    case DT_COMPLEX128:                             // double pair (128 bits)
    case DT_COMPLEX256:                             // long double pair (256 bits)
      vtkErrorWithObjectMacro( this,
                               "datatype not supported: "
                               << this->InternalFileName );
      return;
    //case DT_NONE:
    case DT_UNKNOWN:
    default:
      vtkErrorWithObjectMacro( this,
                               "datatype unknown: "
                               << this->InternalFileName );
      return;
    } //switch (nim->datatype)


// Check if bytes should be swapped
//    BigEndian => most significant byte first
//    LittleEndian => less significant byte first
  switch (nim->byteorder)
  {
    case MSB_FIRST:
      this->SetDataByteOrderToBigEndian();
      break;
    case LSB_FIRST:
      this->SetDataByteOrderToLittleEndian();
      break;
    default:
      vtkWarningWithObjectMacro( this,
                                 "unknown data byte order: "
                                 << this->InternalFileName);
      break;
  }


//TODO Tutto quello che c'è qua sotto dovrà un giorno essere eliminato
//BEGIN CUT
  nifti_mat44_to_orientation( nim->sto_xyz , &m_Orientation[0],&m_Orientation[1],&m_Orientation[2] ) ;

#ifdef DEBUG
  std::cout << "Orientation = " << std::flush;
  for (int i=0; i<3; i++)
  {
    switch (m_Orientation[i])
    {
      case NIFTI_L2R:
        std::cout << "R" << std::flush;
        break;
      case NIFTI_R2L:
        std::cout << "L" << std::flush;
        break;
      case NIFTI_P2A:
        std::cout << "A" << std::flush;
        break;
      case NIFTI_A2P:
        std::cout << "P" << std::flush;
        break;
      case NIFTI_I2S:
        std::cout << "S" << std::flush;
        break;
      case NIFTI_S2I:
        std::cout << "I" << std::flush;
        break;
      default:
        std::cout << "?" << std::flush;
        break;
    }
  }
  std::cout << std::endl;
#endif //DEBUG
//END CUT


  this->vtkImageReader2::ExecuteInformation();

  //Free memory
  nifti_image_free(nim);


}



//----------------------------------------------------------------------------
// This function reads a data from a file.  The datas extent/axes
// are assumed to be the same as the file extent/order.
void vtkNiftiImageReader::ExecuteData(vtkDataObject *output)
{
  vtkImageData *data = this->AllocateOutputData(output);

  if (this->InternalFileName == NULL)
  {
    vtkErrorMacro("A FileName must be specified.");
    return;
  }

  this->ComputeDataIncrements();
  data->GetPointData()->GetScalars()->SetName("NIFTI1Image");

  void* outPtr = data->GetScalarPointer();

  nifti_image *nim;

  nim = nifti_image_read(this->GetInternalFileName(), 1); //0 legge solo l'header 1 legge anche i dati

  if (!nim)
  {
    vtkErrorWithObjectMacro(this,
                            "niftilib could not read file: "
                            << this->InternalFileName);
    return;
  }

  if( (data->GetNumberOfPoints() * data->GetScalarSize()) != (nim->nbyper * nim->nvox))
  {
    vtkWarningWithObjectMacro(this,
                              "(data->GetNumberOfPoints() * data->GetScalarSize()) != (nim->nbyper * nim->nvox): "
                              << this->InternalFileName);
  }

  //void * memcpy ( void * destination, const void * source, size_t num );
  memcpy (outPtr, nim->data, nim->nbyper*nim->nvox);

  //Free memory
  nifti_image_free(nim);
}










