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

#include "logger.h"
#include<QDateTime>

Logger *Logger::myInstance = 0;
Logger &Logger::instance()
{
    if(myInstance == 0)
        myInstance = new Logger();
    return *myInstance;
}

Logger::Logger()
{
   file = NULL;
   enabled = true;
   currentDir = NULL;
}

bool Logger::makeLogFile()
{
    if(!enabled) return true;
    if(currentDir == NULL)
    {
        QString path = WORKDIR;
        path += "logs";
        currentDir = new QDir();
        currentDir->mkpath(path);
        currentDir->cd(path);
    }
    QDateTime d = QDateTime::currentDateTime();
    return openLogFile(currentDir->absolutePath() + "/logs " + d.toString("yyyy_MM_ddThh_mm_ss") + ".txt");
}

bool Logger::openLogFile(const QString &filename)
{
    if(!enabled) return true;
    file = new QFile(filename);
    if(file->open(QIODevice::WriteOnly | QIODevice::Append))
        return true;
    return false;
}

bool Logger::writeToFile(const QString &text)
{
    locker.lock();
    if(!enabled)
    {
        locker.unlock();
        return true;
    }
    if(!file)
    {
        locker.unlock();
        return false;
    }
    QDateTime d = QDateTime::currentDateTime();
    QString s;
    s = d.toString(Qt::ISODate) + " " + text + "\r\n";
    file->write(s.toUtf8());
    file->flush();
    locker.unlock();
    return true;
}

Logger::~Logger()
{
    if( file && file->isOpen() )
        file->close();
    if(file)
        delete file;
    if(currentDir)
        delete currentDir;
}

QString Logger::getFileName()
{
    if(!file)
        return QDateTime::currentDateTime().toString(Qt::ISODate);
    return file->fileName();
}

void Logger::setActive(bool isEnabled)
{
    enabled = isEnabled;
    if(isEnabled && (file == NULL))
            makeLogFile();
}

bool Logger::deleteLogFiles()
{
    if(!currentDir) return false;
    QStringList l;
    l << "*.txt";
    l = currentDir->entryList(l);
    QString fname = getFileName();
    for(QStringList::Iterator it = l.begin(); it != l.end(); it++)
    {
        QString tmp = currentDir -> absoluteFilePath(*it);
        if(tmp != fname)
            QFile::remove(tmp);
    }
    return true;
}

bool Logger::changeDirectory(const QString &_path)
{
    if(!currentDir)
    {
        currentDir = new QDir();
        if( ! (currentDir->mkpath(_path)) )
            return false;
        currentDir->cd(_path);
        return true;
    }
    if(currentDir->absolutePath() == _path)
        return true;
    if( ! (currentDir->mkpath(_path)) )
        return false;
    if(file)
    {
        if( file->isOpen() )
        {
            file->close();
        }
        QString fname = file->fileName();
        QString newFname = _path +
                fname.right( fname.length() -
                             currentDir->absolutePath().length());

        file->rename( newFname);
        currentDir->cd(_path);
        if(file->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            return true;
        }
        return false;
    }
    currentDir->cd(_path);
    return true;


}
