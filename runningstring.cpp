#include "runningstring.h"
#include <cstdio>
#include <QRegExp>

RunningString::RunningString(const QString &_server, int _port)
{
    setServerAddres(_server, _port/*"192.168.5.23",7714*/);
    sconnect();
}

void RunningString::addPrintMethod(QString cam, PrintRunningString* printClass)
{
    printmethodlist.insert(cam, printClass);
}

void RunningString::print2Cam(QString cam, QString rString)
{    
    PrintRunningString* needPrintClass = printmethodlist[cam];
    if(needPrintClass)
    {
        needPrintClass -> printString(rString);
    }
}

void RunningString::receiveDataProcessing(QString inputData)
{
    QRegExp regexpcam("<cam(\\d*)>");
    regexpcam.indexIn(inputData);
    QString cam = "cam" + regexpcam.cap(1);
    QString rString = inputData.right( inputData.length() - cam.length()-3);    
    print2Cam(cam,rString);
    //print2Cam("cam1",inputData);
}

void RunningString::changeConnection(const QString &_server, int _port)
{
    disconnect();
    setServerAddres(_server, _port);
    sconnect();
}
