#ifndef RUNNINGSTRING_H
#define RUNNINGSTRING_H

#include <QMap>
#include <QString>
#include "worksock.h"

class PrintRunningString
{
public:
    virtual void printString(QString str) = 0;
};

class RunningString: public worksock
{
private:
    QMap<QString, PrintRunningString*> printmethodlist;
protected:
    virtual void receiveDataProcessing(QString inputData);

public:
    RunningString();
    void addPrintMethod(QString cam, PrintRunningString* classPrint);
    void print2Cam(QString cam, QString rString);
};

#endif // RUNNINGSTRING_H
