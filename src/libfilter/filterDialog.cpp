//
// C++ Implementation: filterDialog
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filterDialog.h"

#include <limits>
#include <cfloat>

#include <QBoxLayout>

#include <QLabel>
#include <QString>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExpValidator>
#include <QScrollArea>
#include <QFont>
#include <QPushButton>

#include <QMessageBox>
#include <QFileDialog>

#include <QFile>
#include <QTextStream>

#include <iostream>

#define DEBUG

namespace filter
{

filterDialog::filterDialog(abstractFilter* filter, QWidget *parent) :
                            QDialog( parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint ),
                            m_AbstractFilter(filter)
{
    setWindowTitle(m_AbstractFilter->getName().c_str());
    this->setFixedSize(525,350);
    this->setContentsMargins(3,3,3,3);

    m_FilterLayout = new QGridLayout();
    m_FilterLayout->setAlignment ( Qt::AlignTop);
    m_FilterLayout->setOriginCorner ( Qt::TopLeftCorner );


    m_Count = 0;
    importInputs();
    importOutputs();
    importParameters();

    QWidget* filterWidget = new QWidget;
    filterWidget->setLayout(m_FilterLayout);

    QScrollArea* parametersScrollArea = new QScrollArea;
    parametersScrollArea->setBackgroundRole(QPalette::Dark);
    parametersScrollArea->setWidget(filterWidget);

    parametersScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget( parametersScrollArea );

    m_ButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults, Qt::Horizontal);
    mainLayout->addWidget( m_ButtonBox );


    this->setLayout(mainLayout);

    resetParameters();

    connect( m_ButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)) );
}

filterDialog::~filterDialog()
{
}

bool filterDialog::checkParameters()
{
    std::vector<QLineEdit*>::iterator iter;
    int i;
#ifdef DEBUG
        std::cout << "QValidator::Acceptable = " << (int)QValidator::Acceptable << std::endl;
        std::cout << "QValidator::Intermediate = " << (int)QValidator::Intermediate << std::endl;
        std::cout << "QValidator::Invalid = " << (int)QValidator::Invalid << std::endl;
#endif
    for (i = 0,  iter = m_ParameterValues.begin(); iter < m_ParameterValues.end(); i++, iter++)
    {
        abstractParameter* curr = m_AbstractFilter->getParameter(i);

        int pos = 0;
        QString txt = m_ParameterValues[i]->text();

        int check = (m_ParameterValues[i]->validator()->validate(txt, pos) );

#ifdef DEBUG
        std::cout << "txt = " << qPrintable(txt) << " - check = " << check << std::endl;
#endif
        if (check != QValidator::Acceptable)
        {
#ifdef DEBUG
        m_ParameterValues[i]->validator()->fixup(txt);
        std::cout << "pos = " << pos << std::endl;
        std::cout << "fixup = " << qPrintable(txt) << " - compare = " << QString::compare(txt, m_ParameterValues[i]->text()) << std::endl;
        pos = 0;
        check = (m_ParameterValues[i]->validator()->validate(txt, pos) );
        std::cout << "txt = " << qPrintable(txt) << " - check = " << check << std::endl;
//        QChar* data = txt.constData();

        std::cout << "txt.size() = " << txt.size() << std::endl;
        for (int bb = 0 ; bb < txt.size(); bb++)
          std::cout << "txt[" << bb << "] = " << (int)(txt[bb].toLatin1()) << std::endl;
#endif
                    QMessageBox::critical(  this,
                                            QString(m_AbstractFilter->getName().c_str()),
                                            tr("Errore nell'impostazione del parametro:\n\n\"")
                                                  + QString(curr->getName().c_str()) + "\" = " + "\"" + txt + "\"",
                                            QMessageBox::Ok);

            m_ParameterValues[i]->setFocus();
            return false;
        }
        if (CheckParameterTypeMacro(std::string, curr))
        {
            typedef filterParameter<std::string> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue(txt.toStdString());
        }
        else if (CheckParameterTypeMacro(double, curr))
        {
            typedef filterParameter<double> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue(txt.toDouble());
        }
        else if (CheckParameterTypeMacro(float, curr))
        {
            typedef filterParameter<float> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue(txt.toFloat());
        }
        else if (CheckParameterTypeMacro(int, curr))
        {
            typedef filterParameter<int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue(txt.toInt());
        }
        else if (CheckParameterTypeMacro(unsigned int, curr))
        {
            typedef filterParameter<unsigned int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue(txt.toInt());
        }
        else if (CheckParameterTypeMacro(long int, curr))
        {
            typedef filterParameter<long int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue(txt.toInt());
        }
        else if (CheckParameterTypeMacro(char, curr))
        {
            typedef filterParameter<char> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue((char)(txt.toInt()));
        }
        else if (CheckParameterTypeMacro(unsigned char, curr))
        {
            typedef filterParameter<unsigned char> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                par->setValue((unsigned char)(txt.toInt()));
        }
    }
    return true;
}


bool filterDialog::readInput()
{
    std::vector<filterIOWidget*>::iterator iter;
    int i;
    for (i = 0,  iter = m_InputWidgets.begin(); iter < m_InputWidgets.end(); i++, iter++)
    {
        abstractInput* curr = m_AbstractFilter->getInput(i);

        if ((*iter)->check())
        {
            QMessageBox::critical(  this,
                                    QString(m_AbstractFilter->getName().c_str()),
                                    tr("Errore nell'impostazione dell'input:\n\n\"")
                                        + QString(curr->getName().c_str()) + "\"",
                                    QMessageBox::Ok);
            m_InputWidgets[i]->setFocus();
            return false;
        }
        if (CheckInputTypeMacro(ImageType::Pointer, curr))
        {
            typedef filterInput<ImageType::Pointer> filterInputType;
            filterInputType* input = dynamic_cast<filterInputType*>(curr);

            fileFilterIOWidget* reader =  dynamic_cast<fileFilterIOWidget*>(m_InputWidgets[i]);
            if(input != NULL && reader != NULL)
            {
                ImageType::Pointer inputImg;
                if (!reader->readNifti(inputImg))
                {
                    QMessageBox::critical(  this,
                                            QString(m_AbstractFilter->getName().c_str()),
                                            tr("impossibile leggere l'input:\n\n\"")
                                                + QString(curr->getName().c_str()) + "\"",
                                            QMessageBox::Ok);
                    m_InputWidgets[i]->setFocus();
                    return false;
                }
                input->Set(inputImg);
            }
        }
        else if (CheckInputTypeMacro(std::string, curr))
        {
            typedef filterInput<std::string> filterInputType;
            filterInputType* input = dynamic_cast<filterInputType*>(curr);

            fileFilterIOWidget* reader =  dynamic_cast<fileFilterIOWidget*>(m_InputWidgets[i]);
            if(input != NULL && reader != NULL)
            {
                std::string inputString = reader->text().toStdString();
                input->Set(inputString);
            }
        }
        else
        {
            QMessageBox::critical(  this, QString(m_AbstractFilter->getName().c_str()), tr("Not working yet 2"), QMessageBox::Ok);
            return false;
        }
    }
    return true;
}


bool filterDialog::writeOutput(void)
{
    std::vector<filterIOWidget*>::iterator iter;
    int i;
    for (i = 0,  iter = m_OutputWidgets.begin(); iter < m_OutputWidgets.end(); i++, iter++)
    {
        abstractOutput* curr = m_AbstractFilter->getOutput(i);

        if (CheckOutputTypeMacro(ImageType::Pointer, curr))
        {
            typedef filterOutput<ImageType::Pointer> filterOutputType;
            filterOutputType* output = dynamic_cast<filterOutputType*>(curr);
            fileFilterIOWidget* writer =  dynamic_cast<fileFilterIOWidget*>(m_OutputWidgets[i]);

            if(output != NULL && writer != NULL)
            {
                ImageType::Pointer outputImg = output->Get();
                if (!writer->writeNifti(outputImg))
                {
                    QMessageBox::critical(  this,
                                            QString(m_AbstractFilter->getName().c_str()),
                                            tr("impossibile scrivere l'output:\n\n\"")
                                                + QString(curr->getName().c_str()) + "\"",
                                            QMessageBox::Ok);
                    m_OutputWidgets[i]->setFocus();
                    return false;
                }

                QString txtName( (dynamic_cast<fileFilterIOWidget*>(*iter))->text() + ".txt" );

                QFile file(txtName);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                  std::cout << "WARNING: Impossibile scrivere il file " << qPrintable(txtName) << std::endl;
                  return true;
                }
                QTextStream fout(&file);


                fout << m_AbstractFilter->getName().c_str() << "\n"; 

                std::vector<filterIOWidget*>::iterator inIter;
                for (unsigned int j = 0; j < m_InputWidgets.size(); j++)
                {
                  abstractInput* currIn = m_AbstractFilter->getInput(i);
                  fileFilterIOWidget* currFF = dynamic_cast<fileFilterIOWidget*>(m_InputWidgets[i]);
                  fout << currIn->getName().c_str() << ": " << currFF->text() << "\n";
                }

                std::vector<QLineEdit*>::iterator parIter;
          //      for (i = 0,  parIter = m_ParameterValues.begin(); parIter < m_ParameterValues.end(); i++, iter++)
                for (unsigned int k = 0;  k < m_ParameterValues.size(); k++)
                {
                  abstractParameter* currPar = m_AbstractFilter->getParameter(k);
                  fout << currPar->getName().c_str() << ": " << qPrintable(m_ParameterValues[k]->text()) << "\n";
                }
            }
        }
        else
        {
            QMessageBox::critical(  this, QString(m_AbstractFilter->getName().c_str()), tr("Not working yet 1"), QMessageBox::Ok);
            return false;
        }
    }
    return true;
}





void filterDialog::resetParameters()
{
    for (int i=0; i<m_AbstractFilter->m_filterParameterList.getSize(); i++)
    {
        abstractParameter* curr = m_AbstractFilter->getParameter(i);

        if (CheckParameterTypeMacro(std::string, curr))
        {
            typedef filterParameter<std::string> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString(par->getDefaultValue().c_str()));
        }
        else if (CheckParameterTypeMacro(double, curr))
        {
            typedef filterParameter<double> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else if (CheckParameterTypeMacro(float, curr))
        {
            typedef filterParameter<float> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else if (CheckParameterTypeMacro(int, curr))
        {
            typedef filterParameter<int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else if (CheckParameterTypeMacro(unsigned int, curr))
        {
            typedef filterParameter<unsigned int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else if (CheckParameterTypeMacro(long int, curr))
        {
            typedef filterParameter<long int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else if (CheckParameterTypeMacro(char, curr))
        {
            typedef filterParameter<char> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else if (CheckParameterTypeMacro(unsigned char, curr))
        {
            typedef filterParameter<unsigned char> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
                m_ParameterValues[i]->setText(QString::number(par->getDefaultValue()));
        }
        else
        {
            m_ParameterValues[i]->setText("**************");
        }
    }
}

void filterDialog::buttonClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton but = m_ButtonBox->standardButton ( button );
    switch (but)
    {
        case QDialogButtonBox::Cancel :
            reject();
            break;
        case QDialogButtonBox::Ok :
            if(Execute())
                accept();
            break;
        case QDialogButtonBox::RestoreDefaults:
            resetParameters();
            break;
        default:
            break;
    }
}




void filterDialog::importParameters(void)
{
    QFont parametersLabelFont("Helvetica", 14, QFont::StyleItalic);
    QFont nameLabelFont("Helvetica", 10, QFont::Bold);
    QFont valueLineEditFont("Courier", 10);
    QFont valueLabelFont("Helvetica", 6, QFont::Bold);

    if(m_AbstractFilter->m_filterParameterList.getSize())
    {
        QLabel* parametersLabel = new QLabel( tr("Parameters") );
        parametersLabel->setFont(parametersLabelFont);
        m_FilterLayout->addWidget(parametersLabel, m_Count++, 0, 1, 3, Qt::AlignCenter);
    }

    for (int i=0; i<m_AbstractFilter->m_filterParameterList.getSize(); i++, m_Count++)
    {
        abstractParameter* curr = m_AbstractFilter->getParameter(i);

        QLabel* nameLabel = new QLabel(curr->getName().c_str());
        nameLabel->setFixedSize(200,20);
        nameLabel->setFont(nameLabelFont);

        QLineEdit* valueLineEdit = new QLineEdit();
        valueLineEdit->setAlignment(Qt::AlignRight);
        valueLineEdit->setFixedSize(200,20);
        m_ParameterValues.push_back(valueLineEdit);
        valueLineEdit->setFont(valueLineEditFont);


        QLabel* valueLabel = new QLabel();
        valueLabel->setFont(valueLabelFont);

        nameLabel->setBuddy(valueLineEdit);
        valueLabel->setBuddy(valueLineEdit);

        QValidator* validator;

        if (CheckParameterTypeMacro(std::string, curr))
        {
            typedef filterParameter<std::string> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                validator = new QRegExpValidator(QRegExp(".+"), this);
                valueLabel->setText("STRING");
            }
        }
        else if (CheckParameterTypeMacro(double, curr))
        {
            typedef filterParameter<double> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                double max = (par->hasMax()) ? par->getMaxValue() : DBL_MAX;
                double min = (par->hasMin()) ? par->getMinValue() : -DBL_MAX;
                validator = new QDoubleValidator(min, max, 10, this);
                valueLabel->setText("DOUBLE");
            }
        }
        else if (CheckParameterTypeMacro(float, curr))
        {
            typedef filterParameter<float> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                float max = (par->hasMax()) ? par->getMaxValue() : FLT_MAX;
                float min = (par->hasMin()) ? par->getMinValue() : -FLT_MAX;
                validator = new QDoubleValidator(min, max, 10, this);
                valueLabel->setText("FLOAT");
            }
        }
        else if (CheckParameterTypeMacro(int, curr))
        {
            typedef filterParameter<int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                int max = (par->hasMax()) ? par->getMaxValue() : INT_MAX;
                int min = (par->hasMin()) ? par->getMinValue() : INT_MIN;
                validator = new QIntValidator(min, max, this);
                valueLabel->setText("INT");
            }
        }
        else if (CheckParameterTypeMacro(unsigned int, curr))
        {
            typedef filterParameter<unsigned int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                int max = (par->hasMax()) ? par->getMaxValue() : INT_MAX;
                int min = (par->hasMin()) ? par->getMinValue() : 0;
                validator = new QIntValidator(min, max, this);
                valueLabel->setText("UINT");
            }
        }
        else if (CheckParameterTypeMacro(long int, curr))
        {
            typedef filterParameter<long int> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                long int max = (par->hasMax()) ? par->getMaxValue() : INT_MAX;//LONG_MAX;
                long int min = (par->hasMin()) ? par->getMinValue() : INT_MIN;//LONG_MIN;
                validator = new QIntValidator(min, max, this);
#ifdef DEBUG
                std::cout << "[LONG INT] - max = " << max << " - min = " << min << std::endl;
#endif
                valueLabel->setText("LONG INT");
            }
        }
        else if (CheckParameterTypeMacro(char, curr))
        {
            typedef filterParameter<char> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                char max = (par->hasMax()) ? par->getMaxValue() : CHAR_MAX;
                char min = (par->hasMin()) ? par->getMinValue() : CHAR_MIN;
                validator = new QIntValidator(min, max, this);
                valueLabel->setText("CHAR");
            }
        }
        else if (CheckParameterTypeMacro(unsigned char, curr))
        {
            typedef filterParameter<unsigned char> filterParameterType;
            filterParameterType* par = dynamic_cast<filterParameterType*>(curr);
            if(par != NULL)
            {
                unsigned char max = (par->hasMax()) ? par->getMaxValue() : UCHAR_MAX;
                unsigned char min = (par->hasMin()) ? par->getMinValue() : 0;
                validator = new QIntValidator(min, max, this);
                valueLabel->setText("UCHAR");
            }
        }
        else
        {
            validator = new QRegExpValidator(QRegExp(".+"), this);
            valueLineEdit->setEnabled(false);
            valueLabel->setText("UNKNOWN");
        }


        valueLineEdit->setValidator(validator);

        m_FilterLayout->addWidget(nameLabel,     m_Count, 0, 1, 1, Qt::AlignLeft);
        m_FilterLayout->addWidget(valueLineEdit, m_Count, 1, 1, 1, Qt::AlignLeft);
        m_FilterLayout->addWidget(valueLabel,    m_Count, 2, 1, 1, Qt::AlignRight);
    }
}


void filterDialog::importInputs(void)
{
    QFont inputLabelFont("Helvetica", 14, QFont::StyleItalic);
    QFont nameLabelFont("Helvetica", 10, QFont::Bold);
    QFont valueLineEditFont("Courier", 10);
    QFont valueLabelFont("Helvetica", 6, QFont::Bold);

    if (m_AbstractFilter->m_filterInputList.getSize())
    {
        QLabel* inputLabel = new QLabel( tr("Input") );
        inputLabel->setFont(inputLabelFont);
        m_FilterLayout->addWidget(inputLabel, m_Count++, 0, 1, 3, Qt::AlignCenter);
    }

    for (int i=0; i<m_AbstractFilter->m_filterInputList.getSize(); i++, m_Count++)
    {
        abstractInput* curr = m_AbstractFilter->getInput(i);

        QLabel* nameLabel = new QLabel(curr->getName().c_str());
        nameLabel->setFixedSize(200,20);
        nameLabel->setFont(nameLabelFont);

        fileFilterIOWidget* fileWidget = new fileFilterIOWidget(fileFilterIOWidget::INPUT_MODE);
        m_InputWidgets.push_back(fileWidget);
        fileWidget->setFont(valueLineEditFont);

        m_FilterLayout->addWidget(nameLabel,  m_Count, 0, 1, 1, Qt::AlignLeft);
        m_FilterLayout->addWidget(fileWidget, m_Count, 1, 1, 2, Qt::AlignLeft);

    }
}



void filterDialog::importOutputs(void)
{
    QFont outputLabelFont("Helvetica", 14, QFont::StyleItalic);
    QFont nameLabelFont("Helvetica", 10, QFont::Bold);
    QFont valueLineEditFont("Courier", 10);
    QFont valueLabelFont("Helvetica", 6, QFont::Bold);

    if(m_AbstractFilter->m_filterOutputList.getSize())
    {
        QLabel* outputLabel = new QLabel( tr("Output") );
        outputLabel->setFont(outputLabelFont);
        m_FilterLayout->addWidget(outputLabel, m_Count++, 0, 1, 3, Qt::AlignCenter);
    }

    for (int i=0; i<m_AbstractFilter->m_filterOutputList.getSize(); i++, m_Count++)
    {
        abstractOutput* curr = m_AbstractFilter->getOutput(i);

        QLabel* nameLabel = new QLabel(curr->getName().c_str());
        nameLabel->setFixedSize(200,20);
        nameLabel->setFont(nameLabelFont);

        fileFilterIOWidget* fileWidget = new fileFilterIOWidget(fileFilterIOWidget::OUTPUT_MODE);
        m_OutputWidgets.push_back(fileWidget);
        fileWidget->setFont(valueLineEditFont);
        fileWidget->setAcceptMode(QFileDialog::AcceptOpen);

        m_FilterLayout->addWidget(nameLabel,  m_Count, 0, 1, 1, Qt::AlignLeft);
        m_FilterLayout->addWidget(fileWidget, m_Count, 1, 1, 2, Qt::AlignLeft);
    }
}


bool filterDialog::Execute(void)
{
    if (!checkParameters())
        return false;
    if (!readInput())
        return false;
    if (!m_AbstractFilter->Execute())
    {
        QMessageBox::critical(  this, QString(m_AbstractFilter->getName().c_str()), tr("Errore durante l'esecuzione del filtro"), QMessageBox::Ok);
        return false;
    }
    if (!writeOutput())
        return false;
    return true;
}


} // namespace filter
