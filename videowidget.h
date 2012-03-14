#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QUrl>

#include <vlc/vlc.h>

class QFrame;

class VideoWidget: public QWidget
{
    Q_OBJECT
    QFrame *frame;

    QTimer *poller;
    bool _isPlaying;

    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;
    libvlc_media_t *vlcMedia;

    QUrl urlBigVideoStream;
    QUrl urlSmallVideoStream;

public:
    VideoWidget();
    ~VideoWidget();

    enum sizeVideo {BIGVIDEO, SMALLVIDEO};
    void setUrlVideoStream(QUrl urlStream, sizeVideo size);

    void startPlay(sizeVideo size);
    void stopPlay();

public slots:
    void updateInterface();
};

#endif // VIDEOVIDGET_H
