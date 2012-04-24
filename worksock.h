#ifndef WORKSOCK_H
#define WORKSOCK_H
#include <QString>
#include <QTcpSocket>
#include <QtCore>
#include <QApplication>

class worksock: public QObject
{
    Q_OBJECT
private:
    QString host;
    int port;

    void createSocket();
    quint16 nextBlockSize;

    QTcpSocket *wsocket;
    static void tryConnectAlwaysThis(worksock* ws);
protected:
    virtual void receiveDataProcessing(QString inputData) = 0;
public:
    worksock();
    worksock(const QString &_host, int _port);

    void sconnect();
    void sdisconnect();

    void setServerAddres(const QString &_host, int _port);
    void tryConnectAlways();

public slots:
    void receiveData ();
    void errorProcessing (QAbstractSocket::SocketError error);
    void connectProcessing ();
};

#endif // WORKSOCK_H
