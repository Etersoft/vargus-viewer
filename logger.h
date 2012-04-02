#ifndef LOGGER_H
#define LOGGER_H
#include<QFile>

class Logger
{
public:
    static Logger& instance();
    bool makeLogFile();
    bool openLogFile(const QString &filename);
    bool writeToFile(const QString &text);
    void closeFile();
    QString getFileName();
    void setActive(bool isEnabled);
private:
    static Logger* myInstance;
    Logger(){ file = NULL; enabled = true;}
    Logger& operator=(Logger const&);
    Logger(Logger const&);
    QFile *file;
    bool enabled;

};

#endif // LOGGER_H
