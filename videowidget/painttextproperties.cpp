/*


Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include <QString>
#include "painttextproperties.h"

const int PaintTextProperties::heightForNoCoef = 480;
PaintTextProperties::PaintTextProperties()
{
    font = "Courier New";
    size = 20;
    coefficient = 0;
}

PaintTextProperties::PaintTextProperties(QString _font, int _size, qreal _coefficient):
    font(_font), size(_size), coefficient(_coefficient)
{

}
