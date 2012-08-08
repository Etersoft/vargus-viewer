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

VideoWidgetLowLevelPainting::VideoWidgetLowLevelPainting() :
    QWidget(), frame(0), widgetheight(0),
    widgetwidth(0), videosourceheight(0), videosourcewidth(0),
    aspectComply(false), xdisplacement(0),
    videoheight(0), videowidth(0),type(NOTHING), isActiveLowLevelPainting(false), printedTitle("")
{

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
    if(type == NOTHING)
        return;
    vargusLog.writeToFile("Restart paint video height:"+  QString::number(this->height())  + " width:"+  QString::number(this->width()) );

    if(libvlc_video_get_height(getvlcPlayer()) > 0)
    {

        videosourceheight = libvlc_video_get_height(getvlcPlayer());
    }
    if(libvlc_video_get_width(getvlcPlayer()) > 0)
    {
        videosourcewidth = libvlc_video_get_width(getvlcPlayer());
    }

    widgetheight = this->height();
    widgetwidth = this->width();

    videoheight = this->width()/getAspectRatio();
    videowidth = this->width();

    xdisplacement = getXDisplacement();

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

void VideoWidgetLowLevelPainting::setAspectComply(bool value)
{
    aspectComply = value;
}

float VideoWidgetLowLevelPainting::getAspectRatio()
{
    if(aspectComply)
    {
        if(videosourcewidth>0 && videosourceheight>0)
        {
            return (float)videosourcewidth/(float)videosourceheight;
        }
    }
    return 1.0;
}

int VideoWidgetLowLevelPainting::getXDisplacement()
{
    if(aspectComply)
    {
        if( videowidth>0 && videoheight>0 &&
            widgetheight>0 && widgetwidth>0)
        {
            return ((widgetheight - videoheight)/2);
        }
    }
    return 0;
}

void VideoWidgetLowLevelPainting::updateGeometry()
{
    restartPaintVideo();
}

void VideoWidgetLowLevelPainting::activateLowLevelPainting()
{
    isActiveLowLevelPainting = true;
    type = VIDEO;
    restartPaintVideo();
}

void VideoWidgetLowLevelPainting::deactivateLowLevelPainting()
{
    isActiveLowLevelPainting = false;
    type = NOTHING;
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
    if(!isActiveLowLevelPainting)
        return;

    if(type == NOSIGNAL)
    {
        printNoSignal();
    }

    if (type == VIDEO)
    {
        checkChangeVideoSize();
        printVideoFrame();
    }
}

void VideoWidgetLowLevelPainting::checkChangeVideoSize()
{
    if( widgetheight != this->height() ||
        widgetwidth != this->width() ||
        (
            (
                videosourceheight != libvlc_video_get_height(getvlcPlayer()) ||
                videosourcewidth != libvlc_video_get_width(getvlcPlayer())
            ) &&
            (
                libvlc_video_get_width(getvlcPlayer()) > 0 &&
                libvlc_video_get_height(getvlcPlayer())
            )
        )
      )
    {
        restartPaintVideo();
        return;
    }
}

void VideoWidgetLowLevelPainting::printVideoFrame()
{
    painter.begin(this);
    painter.drawImage(QPoint(0, xdisplacement),*frame);
    painter.setFont(QFont("Arial", 10));
    painter.setPen(Qt::white);
    painter.drawText(rect(),Qt::AlignBottom,printedTitle);
    painter.end();
}

void VideoWidgetLowLevelPainting::printNoSignal()
{
    painter.begin(this);
    painter.fillRect(0,0,this->width(), this->height(),Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.setPen(Qt::white);
    painter.drawText(rect(),Qt::AlignCenter,tr("NO SIGNAL"));
    painter.end();
}



void VideoWidgetLowLevelPainting::paintNoSignal()
{
    type = NOSIGNAL;
    repaint();
}

void VideoWidgetLowLevelPainting::paintNothing()
{
    type = VIDEO;
    repaint();
}

void VideoWidgetLowLevelPainting::animate()
{
    repaint();
}
