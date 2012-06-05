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
    vargusLog.writeToFile("add print method for " + cam);
    printmethodlist[cam].append(printClass);
}

void RunningString::addAppendMethod(QString cam, AppendRunningString* appendClass)
{
    vargusLog.writeToFile("add append string method for " + cam);
    appendmethodlist[cam].append(appendClass);
}

void RunningString::dropPrintMethod(QString cam)
{
    vargusLog.writeToFile("dropPrintMethod " + cam);
    printmethodlist.remove(cam);
}

void RunningString::dropAppendMethod(QString cam)
{
    appendmethodlist.remove(cam);
}

void RunningString::print2Cam(QString cam, QString rString)
{
     foreach (AppendRunningString* needAppendClass, appendmethodlist[cam])
     {
         vargusLog.writeToFile("appendString");
         needAppendClass -> appendString(rString);
     }

    foreach (PrintRunningString* needPrintClass, printmethodlist[cam])
    {
        vargusLog.writeToFile("printString");
        if(needPrintClass)
            needPrintClass -> printString();
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
