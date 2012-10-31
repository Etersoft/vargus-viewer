#ifndef VIDEOWIDGETLOWLEVELPAINTING_H
#define VIDEOWIDGETLOWLEVELPAINTING_H

#include <QWidget>
#include <QMutex>
#include <QPainter>
#include <QTimer>
#include <vlc/vlc.h>
#include "painttextproperties.h"

class VideoWidgetLowLevelPainting: public QWidget
{
    Q_OBJECT

public:
    VideoWidgetLowLevelPainting();
    ~VideoWidgetLowLevelPainting();

    uchar* getFrameBits();
    void mutexLock();
    void mutexUnlock();

    virtual void drawImage() = 0;

    void activateLowLevelPainting();
    void deactivateLowLevelPainting();

    void setAspectComply(bool value);
    int getXDisplacement();

    void paintNoSignal();
    void paintNothing();
    void paintblank();
    static void setTextProperties(PaintTextProperties* _textProperties);

private:
    QMutex mutex;
    QImage *frame;

    QPainter painter;
    void paintEvent(QPaintEvent*);

    QTimer *repaintTimer;
    static const int repaintTime;

    int videosourceheight;
    int videosourcewidth;

    int widgetheight;
    int widgetwidth;

    int videoheight;
    int videowidth;

    int xdisplacement;

    float getAspectRatio();
    bool aspectComply;

    enum PaintType{NOTHING, VIDEO, NOSIGNAL, BLANK};
    PaintType type;

    bool isActiveLowLevelPainting;

    void checkChangeVideoSize();
    void printVideoFrame();
    void printNoSignal();
    void printblank();

    virtual libvlc_media_player_t *getvlcPlayer() = 0;
    virtual void startvlcPlayer() = 0;
    virtual void stoptvlcPlayer() = 0;

    static PaintTextProperties* textProperties;

protected:
    QString printedTitle;
    void restartPaintVideo();
    void updateGeometry();
    virtual bool isneedPrintTextEvents() = 0;
    virtual QString getTextEvent() = 0;

public slots:
    void animate();
};

#endif // VIDEOWIDGETLOWLEVELPAINTING_H
