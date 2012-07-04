#include "videowidgetdeleter.h"
#include<logger.h>
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
    mutex.lock();
    vargusLog.writeToFile(QString("Try to get widgets"));
    QList<VideoWidget *> bufferToReturn = buffer;
    buffer.clear();
    mutex.unlock();
    return bufferToReturn;
}

VideoWidgetDeleter::VideoWidgetDeleter(Container *_videoContainer, QObject *parent) :
    QThread(parent)
{
    /*log.setFileName("DeleterThreadLogger");
    log.open(QFile::WriteOnly);
    log.write(QString("Constructor of widgetdeleter\n").toUtf8());
    log.flush();*/
    stop = false;
    finished = false;
    videoContainer = _videoContainer;
    /*log.write(QString("Constructor of widgetdeleter success\n").toUtf8());
    log.flush();*/
}
/*
void VideoWidgetDeleter::addVideoWidgets(QList<VideoWidget *> _vList)
{
    mutex.lock();
    vargusLog.writeToFile(QString("Add widgets to delete %1").arg(_vList.size()));
    deleterBuffer.append(_vList);
    vargusLog.writeToFile("Successful adding");
    mutex.unlock();
}
*/
void VideoWidgetDeleter::run()
{
    QList<VideoWidget *> deleterBuffer;
    for(;;)
    {
        mutex.lock();
        if(stop)
        {
            /*log.write(QString("Stop = true\n").toUtf8());
            log.flush();*/
            mutex.unlock();
            break;
        }
        deleterBuffer = videoContainer->getVideoWidgets();
        /*log.write(QString("Stop = false\n").toUtf8());
        log.flush();*/
        if(deleterBuffer.size() == 0)
        {
            /*log.write(QString("Buffer size for deleter thread = 0, sleep\n").toUtf8());
            log.flush();*/
            mutex.unlock();
            this->sleep(1);
        }
        else
        {
            /*log.write(QString("Started delete vidgets\n").toUtf8());
            log.flush();
            log.write(QString("There is %1 widgets to delete\n").arg(deleterBuffer.size()).toUtf8());
            log.flush();*/
            mutex.unlock();
            QList<VideoWidget*>::iterator it = deleterBuffer.begin();
            QList<VideoWidget*>::iterator end = deleterBuffer.end();
            while(it != end)
            {
                /*log.write(QString("Delete videoWidget from deleter thread\n").toUtf8());
                log.flush();*/
                delete *it;
                it++;
            }
            deleterBuffer.clear();
            /*log.write(QString("Delete end\n").toUtf8());
            log.flush();*/
        }
    }
    if(deleterBuffer.isEmpty() == false)
    {
        /*log.write(QString("Stopped was recived,but deleterBuffer is not empty. There are %1 elements\n").arg(deleterBuffer.size()).toUtf8());
        log.flush();*/
        QList<VideoWidget*>::iterator it = deleterBuffer.begin();
        QList<VideoWidget*>::iterator end = deleterBuffer.end();
        while(it != end)
        {
            /*log.write(QString("Delete videoWidget from deleter thread, stop was recived\n").toUtf8());
            log.flush();*/
            delete *it;
            it++;
        }
    }
    finished = true;
}


void VideoWidgetDeleter::sendstop()
{
    mutex.lock();
    /*log.write(QString("Recive stop\n").toUtf8());
    log.flush();*/
    stop = true;
    /*log.write(QString("Made stop := true\n").toUtf8());
    log.flush();*/
    mutex.unlock();
}
