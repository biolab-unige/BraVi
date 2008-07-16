//
// C++ Interface: fileIOFilterWidget
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FILEFILTERIOWIDGET__
#define __FILEFILTERIOWIDGET__


#include <QWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFont>

#include "filterIOWidget.h"
#include "imageDefs.h"


class fileFilterIOWidget : public filterIOWidget
{
Q_OBJECT

public:

    enum IOMode {INPUT_MODE, OUTPUT_MODE};

    fileFilterIOWidget(IOMode mode = OUTPUT_MODE);
    ~fileFilterIOWidget();

    inline void setFont(QFont font) {m_LineEdit->setFont(font);}
    inline void setAcceptMode(QFileDialog::AcceptMode mode) {m_FileDialog->setAcceptMode(mode);}
    inline void setFileMode(QFileDialog::FileMode mode ) {m_FileDialog->setFileMode(mode);}
    inline void setText(QString text) {m_LineEdit->setText(text);}
    inline QString text(void) {return m_LineEdit->text();}

    inline void setFocus() {m_LineEdit->setFocus();}

    inline bool check(void) {return !FileExists();}

    bool readNifti(ImageType::Pointer& image);
    bool writeNifti(ImageType::Pointer& image);

private:
    QHBoxLayout* m_Layout;
    QLineEdit* m_LineEdit;
    QPushButton* m_Button;
    QFileDialog* m_FileDialog;

    bool FileExists();

private slots:
    void setFile(void);



};




#endif // __FILEIOFILTERWIDGET__
