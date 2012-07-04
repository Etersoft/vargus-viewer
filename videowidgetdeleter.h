#ifndef VIDEOWIDGETDELETER_H
#define VIDEOWIDGETDELETER_H

#include <QThread>
#include<videowidget.h>
#include<QMutex>

//для логов из этого потока
#include<QFile>


class Container
{
    QList<VideoWidget *> buffer;
    QMutex mutex;
public:
    void addVideoWidgets(QList<VideoWidget *> _vList);
    QList<VideoWidget *> getVideoWidgets();

};


class VideoWidgetDeleter : public QThread
{
    Q_OBJECT

    //QList<VideoWidget *> deleterBuffer;
   // QFile log; uncomment to log
    QMutex mutex;
    volatile bool stop;
    volatile bool finished;
    Container *videoContainer;
public:
    explicit VideoWidgetDeleter(Container *_videoContainer,QObject *parent = 0);
    void run();
    //void addVideoWidgets(QList<VideoWidget *> _vList);
    void sendstop();
    
signals:
    
public slots:
    
};

#endif // VIDEOWIDGETDELETER_H
