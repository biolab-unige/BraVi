//
// C++ Interface: $name$
//
// Description: 
//
//
// Author: Daniele Domenichelli <daniele.domenichelli@fastwebnet.it>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __FILTERVTKFILTERIOWIDGET_H__
#define __FILTERVTKFILTERIOWIDGET_H__

#include "filterIOWidget.h"

namespace filter
{


class vtkFilterIOWidget : public filterIOWidget
{
Q_OBJECT
public:
    vtkFilterIOWidget();
    ~vtkFilterIOWidget();

    inline bool check(void) {return true;}
};

} //namespace filter

#endif // __FILTERVTKFILTERIOWIDGET_H__
