//
// C++ Interface: filterIOWidget
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FILTERIOWIDGET__
#define __FILTERIOWIDGET__

#include <QWidget>

class filterIOWidget : public QWidget
{
    Q_OBJECT
public:
    filterIOWidget() {}
    ~filterIOWidget() {}

    virtual bool check(void) = 0;
};


#endif // __FILTERIOWIDGET__
