#include "logger.h"
#include<QDateTime>
Logger *Logger::myInstance = 0;
Logger &Logger::instance()
{
    if(myInstance == 0)
        myInstance = new Logger();
    return *myInstance;
}

bool Logger::openLogFile(const QString &filename)
{
    file = new QFile(filename);
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    return true;
}

bool Logger::writeToFile(const QString &text)
{
    QDateTime d = QDateTime::currentDateTime();
    QString s;
    s=d.toString(Qt::ISODate)+" "+text+"\n";
    file->write(s.toUtf8());
    return true;
}

void Logger::closeFile()
{
    if(file->isOpen())
        file->close();
    delete file;
}
