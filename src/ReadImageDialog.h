#ifndef READIMAGEDIALOG_H
#define READIMAGEDIALOG_H

#include "vtkGenericMedicalImageReader.h"

#include <QFileDialog>
#include <QStringList>
#include <string>


class ReadImageDialog : public QFileDialog
{
  Q_OBJECT

public:
  ReadImageDialog(QWidget *parent, QString title = "Open Image");
  ~ReadImageDialog();

  bool useDirectory() const;
  inline vtkImageType getImageType() const { return m_ImageType; }
  inline std::string getFilename() const { return m_Filename; }
  inline std::string getDirectory() const { return m_Directory; }


private:
  QStringList m_Filters;
  vtkImageType m_ImageType;

  std::string m_Filename;
  std::string m_Directory;

private slots:
    void onDirectoryEntered( const QString& directory );
    void onFilesSelected( const QStringList& selected );

};
#endif // READIMAGEDIALOG_H
