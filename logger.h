#ifndef LOGGER_H
#define LOGGER_H
#include<QFile>
#include<QDir>

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

};

#endif // LOGGER_H
