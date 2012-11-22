#include "worksock.h"
#include <QtCore>
#include <QApplication>
#include <QWidget>
#include "logger.h"

extern Logger &vargusLog;

worksock::worksock():nextBlockSize(0)
{
    createSocket();
    createTimer();
}

worksock::worksock(const QString &_host, int _port):nextBlockSize(0)
{
    host = _host;
    port = _port;
    createSocket();
    createTimer();
}

void worksock::setServerAddres(const QString &_host, int _port)
{
    host = _host;
    port = _port;
}

void worksock::createSocket()
{
    wsocket = new QTcpSocket(this);
    connect(wsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(wsocket, SIGNAL(connected()), this, SLOT(connectProcessing()));
    connect(wsocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(errorProcessing(QAbstractSocket::SocketError)));
}

void worksock::createTimer()
{
    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(tryConnect()));
}

void worksock::sconnect()
{
    vargusLog.writeToFile("connect");
    wsocket->connectToHost(host, port);
    vargusLog.writeToFile("status connect to server " + QString::number(wsocket->state()));
}

void worksock::sdisconnect()
{
    if(wsocket)
        wsocket->disconnect();
}

void worksock::connectProcessing()
{
}

void worksock::receiveData()
{

    QByteArray sendedString;
    sendedString = wsocket->readAll().data();
    str = str.append(sendedString);



    const int sendedlenlenght = 8;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    int i = 0;
    int partlen = 0;

    vargusLog.writeToFile(QString("get data |") + codec->toUnicode(sendedString) + "|");
    vargusLog.writeToFile(QString("all data |") + codec->toUnicode(str) + "|");

    while(str.length() > 0 && (str.length() >=  str.left(sendedlenlenght).toInt()) )
    {

        partlen = str.left(sendedlenlenght).toInt();
        if(!partlen && (i++ < 5))
        {
            str.clear();
            vargusLog.writeToFile(QString("May error at string [") + codec->toUnicode(str) + "]");
            return;
        }

        QString toString = codec->toUnicode(str.left(partlen - 1).right(partlen - 2 - sendedlenlenght ) );
        receiveDataProcessing(toString);
        str = str.right(str.length() - partlen);
    }

}

void worksock::errorProcessing (QAbstractSocket::SocketError error)
{
    vargusLog.writeToFile("errorProcessing " + QString::number(error));
    //QtConcurrent::run(&VideoWidget::clearVlc, vlcPlayer, vlcMedia);
    /*while(status != CONNECTED)
    {
        sconnect();
        sleep (10000);
    }
    */
    reconnectTimer->start(5000);
}

void worksock::tryConnect()
{
    vargusLog.writeToFile("try connect state " + QString::number(wsocket->state()));
    if(wsocket->state() ==  QTcpSocket::ConnectedState && wsocket->state() == QTcpSocket::ConnectingState)
    {
        reconnectTimer->stop();
    }
    else
    {
        sconnect();
    }
}




