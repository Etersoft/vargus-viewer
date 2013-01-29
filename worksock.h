/*


Copyright (C) 2012-2013 Etersoft <info@etersoft.ru>

This file is part of VargusViewer.

VargusViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

VargusViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef WORKSOCK_H
#define WORKSOCK_H
#include <QString>
#include <QTcpSocket>
#include <QtCore>
#include <QApplication>
#include <QTimer>

class worksock: public QObject
{
    Q_OBJECT

private:
    QString host;
    int port;

    void createSocket();
    quint16 nextBlockSize;

    QTcpSocket *wsocket;
    QTimer *reconnectTimer;
    void createTimer();
    QByteArray str;
protected:
    virtual void receiveDataProcessing(QString inputData) = 0;

public:
    worksock();
    worksock(const QString &_host, int _port);

    void sconnect();
    void sdisconnect();

    void setServerAddres(const QString &_host, int _port);

public slots:

    void receiveData();
    void errorProcessing (QAbstractSocket::SocketError error);
    void connectProcessing ();
    void tryConnect();
};

#endif // WORKSOCK_H
