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

#ifndef RUNNINGSTRING_H
#define RUNNINGSTRING_H

#include <QMap>
#include <QString>
#include <QVector>
#include "worksock.h"

class PrintRunningString
{
public:
    virtual void printString() = 0;
};

class AppendRunningString
{
public:
    virtual void appendString(QString str) = 0;
};

class RunningString: public worksock
{
private:
    QMap<QString, QList<PrintRunningString*> > printmethodlist;
    QMap<QString, QList<AppendRunningString*> > appendmethodlist;

protected:
    virtual void receiveDataProcessing(QString inputData);

public:
    RunningString(const QString &_server, int _port);
    void addPrintMethod(QString cam, PrintRunningString* printClass);
    void dropPrintMethod(QString cam);
    void addAppendMethod(QString cam, AppendRunningString* appendClass);
    void dropAppendMethod(QString cam);
    void print2Cam(QString cam, QString rString);
    void changeConnection(const QString &_server, int _port);
    static int getNumberLine(QString line, int numberMaxSymbol);
    static QString cutFirstString(QString inputstring, int numberMaxSymbol, int numSimbol);
};

#endif // RUNNINGSTRING_H
