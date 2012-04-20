#include "logger.h"
#include<QDateTime>
#include<QDir>
Logger *Logger::myInstance = 0;
Logger &Logger::instance()
{
    if(myInstance == 0)
        myInstance = new Logger();
    return *myInstance;
}

bool Logger::makeLogFile()
{
    if(!enabled) return true;
    QString path = WORKDIR;
    path += "logs";
    QDir dir;
    dir.mkpath(path);
    dir.cd(path);
    QDateTime d = QDateTime::currentDateTime();
    return openLogFile(dir.absolutePath() + "/log " + d.toString(Qt::ISODate) + ".txt");
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
    if(!enabled) return true;
    if(!file) return false;
    QDateTime d = QDateTime::currentDateTime();
    QString s;
    s = d.toString(Qt::ISODate) + " " + text + "\n";
    file -> write(s.toUtf8());
    file -> flush();
    return true;
}

void Logger::closeFile()
{
    if(!file) return;
    if(file -> isOpen())
        file -> close();
    delete file;
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
    QString s = WORKDIR;
    s += "logs";
    QDir d;
    if(!d.cd(s)) return false;
    QStringList l;
    l << "*.txt";
    l = d.entryList(l);
    QStringList::Iterator it = l.begin();
    QStringList::Iterator end = l.end();
    QString fname = getFileName();
    while(it != end)
    {
        QString tmp = d.absoluteFilePath(*it);
        if(tmp != fname)
            QFile::remove(tmp);
        it++;
    }
    return true;
}
