//
// C++ Interface: QVTKSquareWidget
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@unige.it>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QVTKSQUAREWIDGET_H
#define QVTKSQUAREWIDGET_H


#include <QVTKWidget.h>
#include <QSize>
#include <QSizePolicy>



class QVTKSquareWidget : public QVTKWidget
{
    Q_OBJECT

public:
    QVTKSquareWidget();
    ~QVTKSquareWidget();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual QSize sizeHint();
    virtual QSize minimumSizeHint();
    virtual QSizePolicy sizePolicy();
    virtual inline int heightForWidth(int width) {return width;}
private:
    static const int minimum_size = 120;
};



#endif // QVTKSQUAREWIDGET_H
