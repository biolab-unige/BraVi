//
// C++ Implementation: QVTKSquareWidget
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "QVTKSquareWidget.h"

QVTKSquareWidget::QVTKSquareWidget() : QVTKWidget()
{
    setSizePolicy(sizePolicy());
    setMinimumSize(minimumSizeHint());
    updateGeometry();
}

QVTKSquareWidget::~QVTKSquareWidget() {}


void QVTKSquareWidget::resizeEvent(QResizeEvent *event)
{
    int newWidth = width();
    int newHeight = height();

    //Check if dimensions are bigger then minimum size
    if (newWidth < minimum_size)
        newWidth = minimum_size;
    if (newHeight < minimum_size)
        newHeight = minimum_size;

    if (newWidth > newHeight)
         newWidth = newHeight;
    else if (newWidth < newHeight)
         newHeight = newWidth;
    resize(newWidth, newHeight);
    repaint();
    QVTKWidget::resizeEvent(event);
}

QSize QVTKSquareWidget::sizeHint()
{
    return QSize(minimum_size,minimum_size);
}

QSize QVTKSquareWidget::minimumSizeHint()
{
    return QSize(minimum_size,minimum_size);
}

QSizePolicy QVTKSquareWidget::sizePolicy()
{
    QSizePolicy policy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding, QSizePolicy::DefaultType );
    policy.setHeightForWidth(true);
    return policy;
 }

