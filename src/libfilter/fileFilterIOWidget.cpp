//
// C++ Implementation: fileFilterIOWidget
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fileFilterIOWidget.h"

#include <QBoxLayout>
#include <QDir>

#include <fstream>

fileFilterIOWidget::fileFilterIOWidget(IOMode mode)
{
        m_LineEdit = new QLineEdit();
        m_LineEdit->setText( QDir::currentPath() );
        m_LineEdit->setFixedSize(200,20);

        m_Button = new QPushButton("...");
        m_Button->setFixedSize(20,20);

        m_Layout = new QHBoxLayout();
        m_Layout->addWidget (m_LineEdit);
        m_Layout->addWidget (m_Button);

        this->setLayout(m_Layout);

        connect (m_Button, SIGNAL ( clicked(void) ), this, SLOT(setFile(void)));

        m_FileDialog = new QFileDialog(this);

        QStringList filters;
        filters << tr( "All Supported Files (*.dcm *.hdr *.nii *.nii.gz)" )
                << tr( "Dicom Files (*.dcm)" )
                << tr( "Analyze Files (*.hdr)" )
                << tr( "Nifti Files (*.nii *.nii.gz *.hdr)" )
                << tr( "Any Files (*)" );
        m_FileDialog->setFilters( filters );
        m_FileDialog->setViewMode(QFileDialog::Detail);

        m_FileDialog->setDirectory( QDir::current() );

        if (mode == INPUT_MODE)
        {
            m_FileDialog->setFileMode( QFileDialog::ExistingFile );
            m_FileDialog->setAcceptMode( QFileDialog::AcceptOpen );
            m_FileDialog->setWindowTitle(tr( "Choose Input File" ));
        }
        else
        {
            m_FileDialog->setFileMode(QFileDialog::AnyFile);
            m_FileDialog->setAcceptMode( QFileDialog::AcceptSave );
            m_FileDialog->setConfirmOverwrite(true); /// \todo non funziona
            m_FileDialog->setDefaultSuffix("nii");
            m_FileDialog->setWindowTitle(tr( "Choose Output File" ));
        }

}


fileFilterIOWidget::~fileFilterIOWidget()
{
}


void fileFilterIOWidget::setFile(void)
{
    if (m_FileDialog->exec())
    {
        QDir::setCurrent(m_FileDialog->directory().canonicalPath());
        m_LineEdit->setText(m_FileDialog->selectedFiles().at(0));
    }
}

bool fileFilterIOWidget::FileExists()
{
    std::string fileName = m_LineEdit->text().toStdString();
    std::cout << fileName << std::endl;
    std::ifstream inp;
    inp.open(fileName.c_str(), std::ifstream::in);
    inp.close();
    if(inp.fail())
        return false;
    return true;
}

#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImageIOFactory.h>
#include <itkNiftiImageIOFactory.h>
#include <itkNiftiImageIO.h>

bool fileFilterIOWidget::readNifti(ImageType::Pointer& image)
{

    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    ImageReaderType::Pointer imageReaderPointer = ImageReaderType::New();


    std::string filename = m_LineEdit->text().toStdString();
    imageReaderPointer->SetFileName(filename.c_str());

    try
    {
        imageReaderPointer->Update();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: reader ---" << std::endl << message << std::endl;
        return false;
    }

    image = imageReaderPointer->GetOutput();

    return true;
}

bool fileFilterIOWidget::writeNifti(ImageType::Pointer& image)
{
    typedef itk::ImageFileWriter<ImageType> ImageWriterType;
    ImageWriterType::Pointer imageWriterPointer = ImageWriterType::New();

    std::string filename = m_LineEdit->text().toStdString();
    imageWriterPointer->SetFileName(filename.c_str());

    imageWriterPointer->SetInput( image );

    try
    {
        imageWriterPointer->Update();
        imageWriterPointer->Write();
    }
    catch ( itk::ExceptionObject & ex )
    {
        std::string message;
        message = ex.GetLocation();
        message += "\n";
        message += ex.GetDescription();
        std::cerr << "--- ERROR: writer ---" << std::endl << message << std::endl;
        return false;
    }
    return true;
}

