/*
Class for logs.

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

#ifndef LOGGER_H
#define LOGGER_H
#include<QFile>
#include <QDir>
#include <QMutex>

class Logger
{
public:
    static Logger& instance();
    bool makeLogFile();
    bool openLogFile(const QString &filename);
    bool writeToFile(const QString &text);
    QString getFileName();
    void setActive(bool isEnabled);
    bool deleteLogFiles();
    bool changeDirectory(const QString &_path);
    ~Logger();

private:
    static Logger* myInstance;
    Logger();
    Logger& operator=(Logger const&);
    Logger(Logger const&);
    QFile *file;
    bool enabled;
    QDir *currentDir;
    mutable QMutex locker;
};

#endif // LOGGER_H
