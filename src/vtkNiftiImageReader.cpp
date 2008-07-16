//filename: vtkNiftiImageReader.cpp
//author: Daniele Domenichelli

#include "vtkNiftiImageReader.h"

#include <vtkObjectFactory.h>

#include <nifti1_io.h>
#include <string>


#define LSB_FIRST 1
#define MSB_FIRST 2






vtkCxxRevisionMacro(vtkNiftiImageReader, "$Revision: 1.9 $");
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

void vtkNiftiImageReader::SetFileName(const char* fn)
{
    nifti_image *nim;


    //Controlla che l'header sia valido
    if (is_nifti_file(fn) < 0)
        std::cerr << "ERROR: void vtkNiftiImageReader::SetFileName(const char* fn) -> is_nifti_file(fn) < 0" << std::endl;

    //Controlla se il file è gzippato
    //TODO supportare anche i nifti compressi senza scompattarli
    if (nifti_is_gzfile(fn))
    {
        std::cerr << "WARNING: void vtkNiftiImageReader::SetFileName(const char* fn) -> nifti_is_gzfile(fn)" << std::endl;
        std::string command("gunzip -f -c ");
        command += fn;
        command += " > /tmp/tmp.nii";
        std::cout << command << std::endl;
        system(command.c_str());
        nim = nifti_image_read("/tmp/tmp.nii", 0); //0 legge solo l'header 1 legge anche i dati
    }
    else
    {
        nim = nifti_image_read(fn, 0); //0 legge solo l'header 1 legge anche i dati
    }
//    nifti_image_infodump(nim);

    //Controlla che il numero delle dimensioni sia 3
    //TODO supportare anche un numero diverso di dimensioni
    if (nim->ndim != 3 && !(nim->ndim == 4 && nim->nt == 1))
        std::cerr << "WARNING: void vtkNiftiImageReader::SetFileName(const char* fn) -> nim->ndim != 3 && !(nim->ndim == 4 && nim->nt == 1)" << std::endl;
    this->SetFileDimensionality(3);

/*
    CONVENZIONE RADIOLOGICA - NEUROLOGICA
    TODO Informarsi meglio a riguardo

    NON FUNZIONA CON vtkImageReader2 (con vtkImageReader funziona!)

    //Controlla le direzioni del file (convenzione radiologica/neurologica?)
    //TODO Controllare se è giusto!
    int i,j,k;
    nifti_mat44_to_orientation( nim->sto_xyz , &i,&j,&k ) ;
    if( i <= 0 || j <= 0 || k <= 0 )
        return false;
    double dir[3] = {1,1,1};
    if(i==NIFTI_L2R)    //Left-to-Right
        dir[0]=-1;
    else if(i==NIFTI_R2L)   //Right-to-Left
        dir[0]=1;
    if(j==NIFTI_P2A)    //Posterior-to-Anterior
        dir[1]=1;
    else if(j==NIFTI_A2P)   //Anterior-to-Posterior
        dir[1]=-1;
    if(k==NIFTI_I2S)    //Inferior-to-Superior
        dir[2]=1;
    if(k==NIFTI_S2I)    //Superior-to-Inferior
        dir[2]=-1;
    vtkTransform *orientation = vtkTransform::New(); //TODO si può fare in un altro modo?
    orientation->Scale(dir);
    this->SetTransform(orientation); //TODO controllare se si possono poi impostare più trasformazioni
    orientation->Delete();
*/
/*
for(int i=0;i<4;i++)
{
    for (int j=0; j<4; j++)
        std::cerr << nim->sto_xyz.m[i][j] << "\t";
    std::cerr << std::endl;
}
*/

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

    //Imposta FileLowerLeft
    this->SetFileLowerLeft(true);

    //Imposta l'offset nel file
    this->SetHeaderSize(nim->iname_offset); // size of the header

    //Imposta l'immagine in scala di grigi

    //Imposta posizione, voxel spacing e origine
    this->SetDataExtent(0,(nim->nx)-1, 0,(nim->ny)-1, 0,(nim->nz)-1);
    this->SetDataSpacing(nim->dx, nim->dy, nim->dz);       // voxel spacing 
    this->SetDataOrigin(nim->dx/2.0, nim->dy/2.0, nim->dy/2.0); //TODO Probabilmente è meglio impostare l'origine al centro
    //this->SetDataOrigin(0.0, 0.0, 0.0);
    //Imposta il numero di byte per voxel
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
            std::cerr << "WARNING: dataset = DT_RGB non testato" << std::endl;
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
            std::cerr << "ERROR: dataset non supportato dalle VTK" << std::endl;
            break;
        //case DT_NONE:
        case DT_UNKNOWN:
        default:
            std::cerr << "ERROR: dataset unknown" << std::endl;
            break;
    } //switch (nim->datatype)


    // Controlla se deve invertire l'ordine dei bytes
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
            std::cerr << "WARNING: unknown data byte order" << std::endl;
    }


    //Imposta il nome del file da leggere (sceglie in modo automatico sia che il file sia unico sia che siano 2)
    this->vtkImageReader2::SetFileName(nim->iname);
    nifti_image_free(nim);
}



