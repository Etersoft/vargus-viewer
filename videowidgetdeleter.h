#ifndef VIDEOWIDGETDELETER_H
#define VIDEOWIDGETDELETER_H

#include <QThread>
#include <videowidget.h>
#include <QMutex>

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

    QMutex mutex;
    volatile bool stop;
    Container *videoContainer;

public:
    explicit VideoWidgetDeleter(Container *_videoContainer,QObject *parent = 0);
    void run();
    void sendstop();
};

#endif // VIDEOWIDGETDELETER_H
