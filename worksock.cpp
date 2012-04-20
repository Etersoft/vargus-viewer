#include "worksock.h"
#include <QtCore>
#include <QApplication>
#include <QWidget>

worksock::worksock():nextBlockSize(0)
{
    createSocket();
}

worksock::worksock(QString _host, int _port):nextBlockSize(0)
{
    host = _host;
    port = _port;
    createSocket();
}

void worksock::setServerAddres(QString _host, int _port)
{
    host = _host;
    port = _port;
}

void worksock::createSocket()
{
    wsocket = new QTcpSocket(this);
    connect(wsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(wsocket, SIGNAL(connected()), this, SLOT(connectProcessing()));
    connect(wsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorProcessing(QAbstractSocket::SocketError)));
}

void worksock::sconnect()
{
    wsocket->connectToHost(host, port);
}

void worksock::connectProcessing()
{
}

void worksock::receiveData()
{
    printf ("receiveData\n");
    QDataStream in;
    QString str;
    str = wsocket->readAll().data();
    receiveDataProcessing(str);
}

void worksock::errorProcessing (QAbstractSocket::SocketError error)
{

    //QtConcurrent::run(&VideoWidget::clearVlc, vlcPlayer, vlcMedia);
    /*while(status != CONNECTED)
    {
        sconnect();
        sleep (10000);
    }
    */
    QtConcurrent::run(&tryConnectAlwaysThis, this);
}

void worksock::tryConnectAlways()
{

    while(wsocket->state() !=  QTcpSocket::ConnectedState || wsocket->state() != QTcpSocket::ConnectedState)
    {
        sconnect();
        sleep (10000);
    }
}

void worksock::tryConnectAlwaysThis(worksock* ws)
{
    ws->tryConnectAlways();
}




