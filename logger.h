#ifndef LOGGER_H
#define LOGGER_H
#include<QFile>

class Logger
{
public:
    static Logger& instance();
    bool openLogFile(const QString &filename);
    bool writeToFile(const QString &text);
    void closeFile();
private:
    static Logger* myInstance;
    Logger(){}
    Logger& operator=(Logger const&);
    Logger(Logger const&);
    QFile *file;

};

#endif // LOGGER_H
