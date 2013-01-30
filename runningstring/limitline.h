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

#ifndef LIMITLINE_H
#define LIMITLINE_H
#include <QStringList>

struct RunningTextSettings
{
    RunningTextSettings();
    int color;
    int opacity;
    int position;
    int refresh;
    int size;
    int timeout;
    int x;
    int y;
    int limitLine;
};

class LimitLine
{
private:
    QStringList* strings;
    int numLimitLine;

public:
    LimitLine(int _numLimitLine);
    ~LimitLine();
    void setNumLimitLine(int num);
    QString getLimitLine();
    void AddString(QString string);
    QString AddStringGetLine(QString* string);
};

#endif // LIMITLINE_H
