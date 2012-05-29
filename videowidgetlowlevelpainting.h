#ifndef VIDEOWIDGETLOWLEVELPAINTING_H
#define VIDEOWIDGETLOWLEVELPAINTING_H

#include <QWidget>
#include <QMutex>
#include <QPainter>
#include <QTimer>
#include <vlc/vlc.h>

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
private:
    QMutex mutex;
    QImage *frame;

    QPainter painter;
    void paintEvent(QPaintEvent*);

    QTimer *repaintTimer;
    static const int repaintTime;

    int videoheight;
    int videowidth;
    bool active;

    virtual libvlc_media_player_t *getvlcPlayer() = 0;
    virtual void startvlcPlayer() = 0;
    virtual void stoptvlcPlayer() = 0;
protected:
    void restartPaintVideo();
    void updateGeometry();
public slots:
    void animate();
};

#endif // VIDEOWIDGETLOWLEVELPAINTING_H
