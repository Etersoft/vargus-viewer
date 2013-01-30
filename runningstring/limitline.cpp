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

#include "limitline.h"

RunningTextSettings::RunningTextSettings()
{
    color = 0xFFFFFF;
    opacity = 255;
    position = -1;
    refresh = 1000;
    size = -1;
    timeout = 0;
    x = 0;
    y = 0;
    limitLine = 1;
}

LimitLine::LimitLine(int _numLimitLine) : numLimitLine(_numLimitLine)
{
    strings = new QStringList();
}

LimitLine::~LimitLine()
{
    delete strings;
}

void LimitLine::AddString(QString string)
{
    strings->append(string);
    if(strings->count() > numLimitLine)
    {
        strings->pop_front();
    }
}

QString LimitLine::getLimitLine()
{
    QString finalResult = QString("");
    foreach(const QString &sringElement, *strings)
    {
        finalResult += sringElement + "";
    }
    return finalResult;
}

QString LimitLine::AddStringGetLine(QString* string)
{
    if(string)
        AddString(*string);
    return getLimitLine();
}
