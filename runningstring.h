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
};

#endif // RUNNINGSTRING_H
