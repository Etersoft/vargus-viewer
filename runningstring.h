#ifndef RUNNINGSTRING_H
#define RUNNINGSTRING_H

#include <QMap>
#include <QString>
#include <QVector>
#include "worksock.h"

class PrintRunningString
{
public:
    virtual void printString(QString str) = 0;
};

class RunningString: public worksock
{
private:
    QMap<QString, QList<PrintRunningString*> > printmethodlist;
protected:
    virtual void receiveDataProcessing(QString inputData);

public:
    RunningString(const QString &_server, int _port);
    void addPrintMethod(QString cam, PrintRunningString* classPrint);
    void print2Cam(QString cam, QString rString);
    void changeConnection(const QString &_server, int _port);
};

#endif // RUNNINGSTRING_H
