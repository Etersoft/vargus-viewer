#include "videowidgetlowlevelpainting.h"
#include"logger.h"
#include "vlc/vlc.h"
#include "runningstring.h"
extern Logger &vargusLog;

const int VideoWidgetLowLevelPainting::repaintTime = 50;
PaintTextProperties* VideoWidgetLowLevelPainting::textProperties = 0;

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
    QWidget(), frame(0),
    videosourceheight(0), videosourcewidth(0),
    widgetheight(0), widgetwidth(0),
    videoheight(0), videowidth(0),
    xdisplacement(0),
    aspectComply(false),
    type(NOTHING), isActiveLowLevelPainting(false), printedTitle("")
{
    if(!textProperties)
    {
        textProperties = new PaintTextProperties();
    }
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
    if(type == NOTHING || type == BLANK)
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

    if(aspectComply)
    {
        if(((float)this->width()/(float)this->height()) > getAspectRatio() )
        {
            videowidth = videoheight * getAspectRatio();
            videoheight = this->height();
        }
        else
        {
            videowidth = this->width();
            videoheight = this->width()/getAspectRatio();
        }
    }
    else
    {
        videowidth = this->width();
        videoheight = this->height();
    }


    xdisplacement = getXDisplacement();
    ydisplacement = getYDisplacement();
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
    restartPaintVideo();
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

int VideoWidgetLowLevelPainting::getYDisplacement()
{
    if(aspectComply)
    {
        if( videowidth>0 && videoheight>0 &&
            widgetheight>0 && widgetwidth>0 && widgetheight > videoheight)
        {
            return ((widgetheight - videoheight)/2);
        }
    }
    return 0;
}

int VideoWidgetLowLevelPainting::getXDisplacement()
{
    if(aspectComply)
    {
        if( videowidth>0 && videoheight>0 &&
            widgetheight>0 && widgetwidth>0 && widgetwidth > videowidth)
        {
            return ((widgetwidth - videowidth)/2);
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

    if (type == BLANK)
    {
        printblank();
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

void VideoWidgetLowLevelPainting::setTextProperties(PaintTextProperties* _textProperties)
{
    textProperties = _textProperties;
}




void VideoWidgetLowLevelPainting::printVideoFrame()
{
    painter.begin(this);
    painter.drawImage(QPoint(xdisplacement, ydisplacement), *frame);
    painter.setFont(QFont("Arial", 10));
    painter.setPen(Qt::white);
    painter.drawText(xdisplacement, ydisplacement, this->width() - 2*xdisplacement,this->height() - 2*ydisplacement, Qt::AlignBottom, printedTitle);
    if(isneedPrintTextEvents() && getTextEvent().length()>0)
    {
        int size = textProperties->size + (this->height()-textProperties->heightForNoCoef)*textProperties->coefficient;
        QFont* font = new QFont(textProperties->font);
        QString printString = getTextEvent();

        painter.fillRect(0, 0, this->width(), this->height(),QColor(0,0,0,100));
        font->setPixelSize(size);
        painter.setFont(*font);
        painter.drawText(rect(), Qt::AlignTop | Qt::TextDontClip, printString);
    }

    painter.end();
}

void VideoWidgetLowLevelPainting::printNoSignal()
{
    painter.begin(this);
    painter.fillRect(0, 0, this->width(), this->height(), Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, tr("NO SIGNAL"));
    painter.end();
}

void VideoWidgetLowLevelPainting::printblank()
{
    painter.begin(this);
    painter.end();
}


void VideoWidgetLowLevelPainting::paintNoSignal()
{
    type = NOSIGNAL;
    repaint();
}

void VideoWidgetLowLevelPainting::paintblank()
{
    type = BLANK;
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
