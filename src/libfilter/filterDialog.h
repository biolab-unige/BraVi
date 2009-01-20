//
// C++ Interface: filterDialog
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FILTERDIALOG_H__
#define __FILTERDIALOG_H__

#include <vector>

#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QAbstractButton>

#include <QGridLayout>


#include "filterParameter.h"
#include "abstractFilter.h"

#include "fileFilterIOWidget.h"
#include "vtkFilterIOWidget.h"




namespace filter
{

class filterDialog : public QDialog
{

    Q_OBJECT

public:
    filterDialog(abstractFilter* filter, QWidget *parent = 0);
    ~filterDialog();

private:
    abstractFilter* m_AbstractFilter;
    QDialogButtonBox* m_ButtonBox;


    std::vector<QLineEdit*> m_ParameterValues;
    std::vector<filterIOWidget*> m_InputWidgets;
    std::vector<filterIOWidget*> m_OutputWidgets;


    QGridLayout* m_FilterLayout;

    bool Execute(void);

    bool checkParameters();
    bool readInput();
    bool writeOutput();

    void resetParameters();

    void importInputs(void);
    void importParameters(void);
    void importOutputs(void);
    int m_Count;  //Usato per creare il layout

private slots:
    void buttonClicked(QAbstractButton* button);

};

} //namespace filter

#endif //__FILTERDIALOG_H__

