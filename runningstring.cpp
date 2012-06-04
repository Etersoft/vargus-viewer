#include "runningstring.h"
#include <cstdio>
#include <QRegExp>
#include"logger.h"
extern Logger &vargusLog;

RunningString::RunningString(const QString &_server, int _port)
{
    setServerAddres(_server, _port);
    sconnect();
}

void RunningString::addPrintMethod(QString cam, PrintRunningString* printClass)
{
    printmethodlist[cam].append(printClass);
}

void RunningString::print2Cam(QString cam, QString rString)
{
    foreach (PrintRunningString* needPrintClass, printmethodlist[cam])
    {
        needPrintClass -> printString(rString);
    }
}

void RunningString::receiveDataProcessing(QString inputData)
{
    vargusLog.writeToFile("receive data " + inputData);
    QRegExp regexpcam("cam(\\d*)");
    regexpcam.indexIn(inputData);
    QString cam = "cam" + regexpcam.cap(1);
    QString rString = inputData.right( inputData.length() - cam.length()-1);
    vargusLog.writeToFile("receive data cam |" + cam + "| inputData |" + inputData + "|");
    print2Cam(cam,rString);
    //print2Cam("cam1",inputData);
}

void RunningString::changeConnection(const QString &_server, int _port)
{
    disconnect();
    setServerAddres(_server, _port);
    sconnect();
}
