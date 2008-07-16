#include "ReadImageDialog.h"


ReadImageDialog::ReadImageDialog(QWidget *parent, QString title) :
  QFileDialog(parent),
  m_ImageType(UNKNOWN_IMAGE)
{
  // Imposta il filtro
  m_Filters << "All Supported Files (*.dcm *.hdr *.nii *.nii.gz *.mhd)"
            << "Dicom Files (*.dcm)"
            << "Analyze Files (*.hdr)"
            << "Nifti Files (*.nii *.nii.gz *.hdr)"
            << "MetaImage Files (*.mhd)"
            << "Any Files (*)";
  this->setFilters(m_Filters);

  this->setWindowTitle( title );

  this->setFileMode( QFileDialog::ExistingFile );
  this->setDirectory( QDir::current() );

  connect ( this, SIGNAL(directoryEntered( const QString& )),  this, SLOT (onDirectoryEntered( const QString& )) );
  connect ( this, SIGNAL(filesSelected( const QStringList& )), this, SLOT (onFilesSelected( const QStringList& )) );

}

ReadImageDialog::~ReadImageDialog()
{
}

bool ReadImageDialog::useDirectory() const
{
  switch (m_ImageType)
  {
    case NIFTI_IMAGE:
    case META_IMAGE:
      return false;
    case DICOM_IMAGE:
      return true;
    default:
      return true;
  }
}


void ReadImageDialog::onDirectoryEntered( const QString& directory )
{
  m_Directory = this->directory().canonicalPath().toStdString();
  QDir::setCurrent(m_Directory.c_str());
}

void ReadImageDialog::onFilesSelected( const QStringList& selected )
{
  QString fileName = selected.at(0);
  m_Filename = fileName.toStdString();

  if ( fileName.endsWith(".nii", Qt::CaseInsensitive) || fileName.endsWith(".nii.gz", Qt::CaseInsensitive) || fileName.endsWith(".hdr", Qt::CaseInsensitive) )
    m_ImageType = NIFTI_IMAGE;
  else if ( fileName.endsWith(".mhd", Qt::CaseInsensitive) )
    m_ImageType = META_IMAGE;
  else if ( fileName.endsWith(".dcm", Qt::CaseInsensitive) )
    m_ImageType = DICOM_IMAGE;
  /// \todo eseguire qualche controllo in più
  else
    m_ImageType = DICOM_IMAGE;
}
