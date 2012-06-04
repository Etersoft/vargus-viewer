#include "videowidgetlowlevelpainting.h"
#include"logger.h"
#include "vlc/vlc.h"
extern Logger &vargusLog;

const int VideoWidgetLowLevelPainting::repaintTime = 50;


static void display(void *core, void *picture)
{
    Q_UNUSED(picture);
    VideoWidgetLowLevelPainting *player = (VideoWidgetLowLevelPainting *)core;
    player->drawImage();
    Q_ASSERT(picture == NULL);
}

static void *lock(void *core, void **planes)
{
    VideoWidgetLowLevelPainting *player = (VideoWidgetLowLevelPainting *)core;
    player->mutexLock();
    *planes = player->getFrameBits();
    return NULL;
}

static void unlock(void *core, void *picture, void *const *planes)
{
    Q_UNUSED(planes);
    Q_UNUSED(picture);

    VideoWidgetLowLevelPainting *player = (VideoWidgetLowLevelPainting *)core;
    player->mutexUnlock();
    Q_ASSERT(picture == NULL);
}

uchar* VideoWidgetLowLevelPainting::getFrameBits()
{
    return frame->bits();
}
void VideoWidgetLowLevelPainting::mutexLock()
{
    mutex.lock();
}

void VideoWidgetLowLevelPainting::mutexUnlock()
{
    mutex.unlock();
}


VideoWidgetLowLevelPainting::VideoWidgetLowLevelPainting() : QWidget(), frame(0), videoheight(0), videowidth(0)
{
    active = false;
    repaintTimer = new QTimer(this);
    connect(repaintTimer, SIGNAL(timeout()), this, SLOT(animate()));
}

VideoWidgetLowLevelPainting::~VideoWidgetLowLevelPainting()
{  
    vargusLog.writeToFile("destroy VideoWidgetLowLevelPainting");
    deactivateLowLevelPainting();
}

void VideoWidgetLowLevelPainting::restartPaintVideo()
{
    if(!active)
        return;
    vargusLog.writeToFile("Restart paint video height:"+  QString::number(this->height())  + " width:"+  QString::number(this->width()) );

    videoheight = this->height();
    videowidth = this->width();

    repaintTimer->stop();
    stoptvlcPlayer();
    if(frame)
        delete frame;
    frame = new QImage(videowidth, videoheight, QImage::Format_ARGB32_Premultiplied);
    libvlc_video_set_format(getvlcPlayer(), "RV32", frame->width(), frame->height(), frame->width() * 4);
    libvlc_video_set_callbacks(getvlcPlayer(), lock, unlock, display, this);
    startvlcPlayer();
    repaintTimer->start(repaintTime);
}

void VideoWidgetLowLevelPainting::updateGeometry()
{
    restartPaintVideo();
}

void VideoWidgetLowLevelPainting::activateLowLevelPainting()
{
    active = true;
    restartPaintVideo();
}

void VideoWidgetLowLevelPainting::deactivateLowLevelPainting()
{
    active = false;
    repaintTimer->stop();
    mutexLock();
    if(frame)
    {
        delete frame;
        frame = 0;
    }
    mutexUnlock();
}

void VideoWidgetLowLevelPainting::paintEvent(QPaintEvent*)
{
    if( videoheight != this->height() ||  videowidth != this->width())
    {
        restartPaintVideo();
        return;
    }
    if (active)
    {
        painter.begin(this);
        painter.drawImage(QPoint(0, 0),*frame);
        painter.end();
    }
}

void VideoWidgetLowLevelPainting::animate()
{
    repaint();
}
