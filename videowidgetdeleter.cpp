#include "videowidgetdeleter.h"
#include <logger.h>

extern Logger &vargusLog;

void Container::addVideoWidgets(QList<VideoWidget *> _vList)
{
    mutex.lock();
    vargusLog.writeToFile(QString("Add widgets to delete %1").arg(_vList.size()));
    buffer.append(_vList);
    mutex.unlock();
}
QList<VideoWidget *> Container::getVideoWidgets()
{
    QMutexLocker locker(&mutex);
    vargusLog.writeToFile(QString("Try to get widgets"));
    QList<VideoWidget *> bufferToReturn = buffer;
    buffer.clear();
    return bufferToReturn;
}

VideoWidgetDeleter::VideoWidgetDeleter(Container *_videoContainer, QObject *parent) :
    QThread(parent)
{
    stop = false;
    videoContainer = _videoContainer;
}

void VideoWidgetDeleter::run()
{
    QThread::msleep(10000);
    QList<VideoWidget *> deleterBuffer;
    for(;;)
    {
        mutex.lock();
        if(stop)
        {
            mutex.unlock();
            break;
        }
        deleterBuffer = videoContainer->getVideoWidgets();
        if(deleterBuffer.size() == 0)
        {
            mutex.unlock();
            this->sleep(1);
        }
        else
        {
            mutex.unlock();
            VideoWidget *vw;
            foreach(vw, deleterBuffer)
                delete vw;
            deleterBuffer.clear();
        }
    }
    deleterBuffer.append(videoContainer->getVideoWidgets());
    if(deleterBuffer.isEmpty() == false)
    {
        VideoWidget *vw;
        foreach(vw, deleterBuffer)
            delete vw;
    }
}


void VideoWidgetDeleter::sendstop()
{
    mutex.lock();
    stop = true;
    mutex.unlock();
}
